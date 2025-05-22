// ControlNodeApplication.cc
#include "ControlNodeApplication.h"
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace wsn_simulation;

Define_Module(ControlNodeApplication);

ControlNodeApplication::ControlNodeApplication() : networkManager(nullptr)
{
}

ControlNodeApplication::~ControlNodeApplication()
{
    delete networkManager;
}

void ControlNodeApplication::initialize()
{
    // 获取参数
    nodeId = getParentModule()->par("nodeId");
    clusterHeadCheckInterval = 30.0; // 30秒检查一次簇头状态
    networkStatusLogInterval = 120.0; // 120秒记录一次网络状态
    waveformConfigInterval = 300.0; // 300秒配置一次波形参数
    managementCommandInterval = 180.0; // 180秒执行一次管理命令

    // 初始化统计
    sentMessages = 0;
    receivedMessages = 0;

    // 创建网络管理器
    networkManager = new NetworkManager(this);

    // 获取网络参数
    cModule *networkModule = getParentModule()->getParentModule();
    int numClusters = networkModule->par("numClusters");
    int nodesPerCluster = networkModule->par("nodesPerCluster");

    // 初始化网络
    networkManager->initializeNetwork(numClusters, nodesPerCluster);

    // 创建定时器
    statusTimer = new cMessage("statusTimer");
    clusterHeadCheckTimer = new cMessage("clusterHeadCheckTimer");
    networkStatusTimer = new cMessage("networkStatusTimer");
    waveformConfigTimer = new cMessage("waveformConfigTimer");
    managementCommandTimer = new cMessage("managementCommandTimer");

    // 调度定时器
    scheduleAt(simTime() + 1.0, statusTimer);  // 每1秒发送状态
    scheduleAt(simTime() + clusterHeadCheckInterval, clusterHeadCheckTimer);
    scheduleAt(simTime() + networkStatusLogInterval, networkStatusTimer);
    scheduleAt(simTime() + waveformConfigInterval, waveformConfigTimer);
    scheduleAt(simTime() + managementCommandInterval, managementCommandTimer);

    EV_INFO << "ControlNodeApplication initialized with nodeId=" << nodeId << endl;
}

void ControlNodeApplication::handleMessage(cMessage *msg)
{
    if (msg == statusTimer) {
        handleStatusTimer();
    } else if (msg == clusterHeadCheckTimer) {
        handleClusterHeadCheckTimer();
    } else if (msg == networkStatusTimer) {
        handleNetworkStatusTimer();
    } else if (msg == waveformConfigTimer) {
        handleWaveformConfigTimer();
    } else if (msg == managementCommandTimer) {
        handleManagementCommandTimer();
    } else if (msg->arrivedOn("fromComm")) {
        // 处理接收到的消息
        receivedMessages++;

        if (StatusMessage *statusMsg = dynamic_cast<StatusMessage *>(msg)) {
            handleStatusMessage(statusMsg);
        } else if (StatusAggregation *aggMsg = dynamic_cast<StatusAggregation *>(msg)) {
            handleStatusAggregationMessage(aggMsg);
        } else if (ManagementCommand *cmdMsg = dynamic_cast<ManagementCommand *>(msg)) {
            handleManagementCommandResponse(cmdMsg);
        } else if (dynamic_cast<ClusterAddressing *>(msg) || dynamic_cast<InterClusterAddressing *>(msg)) {
            handleAddressingMessage(msg);
        } else {
            EV_WARN << "Received unknown message type" << endl;
            delete msg;
        }
    } else {
        // 未知消息
        EV_WARN << "Received unknown message" << endl;
        delete msg;
    }
}

void ControlNodeApplication::finish()
{
    // 清理定时器
    cancelAndDelete(statusTimer);
    cancelAndDelete(clusterHeadCheckTimer);
    cancelAndDelete(networkStatusTimer);
    cancelAndDelete(waveformConfigTimer);
    cancelAndDelete(managementCommandTimer);

    // 输出统计信息
    recordScalar("sentMessages", sentMessages);
    recordScalar("receivedMessages", receivedMessages);

    EV_INFO << "ControlNodeApplication finished with stats:" << endl;
    EV_INFO << "  Sent messages: " << sentMessages << endl;
    EV_INFO << "  Received messages: " << receivedMessages << endl;

    // 记录每个簇的状态收集情况
    for (const auto& pair : receivedStatusByCluster) {
        std::string scalarName = "receivedStatusFromCluster_" + std::to_string(pair.first);
        recordScalar(scalarName.c_str(), pair.second);
    }
}

void ControlNodeApplication::handleStatusMessage(StatusMessage *msg)
{
    int srcId = msg->getSrcId();
    int nodeType = msg->getNodeType();
    int clusterId = 0;

    // 确定发送方的簇ID
    if (nodeType == 2) { // 簇头
        clusterId = srcId; // 簇头ID等于簇ID
    } else if (nodeType == 3) { // 普通节点
        clusterId = (srcId - 6) / 200 + 1; // 根据节点ID计算簇ID
    }

    // 记录收到的状态消息数量
    receivedStatusByCluster[clusterId]++;

    // 更新节点性能信息
    double delay = simTime().dbl() - msg->getTimestamp();
    updateNodePerformance(srcId, delay, true);

    // 更新网络管理器中的节点信息
    networkManager->updateNodeStatus(srcId, msg->getStatus());
    networkManager->updateNodeBattery(srcId, msg->getBatteryLevel());
    networkManager->updateNodePosition(srcId, msg->getPositionX(), msg->getPositionY(), msg->getPositionZ());

    // 计算节点性能指标
    if (lastPacketTimes.find(srcId) != lastPacketTimes.end()) {
        double packetInterval = simTime().dbl() - lastPacketTimes[srcId];
        double expectedInterval = 1.0; // 预期每秒一个状态消息

        // 计算丢包率 (基于包间隔)
        double lossRate = std::max(0.0, (packetInterval - expectedInterval) / expectedInterval);
        lossRate = std::min(1.0, lossRate); // 确保在[0,1]范围内

        // 更新节点性能
        networkManager->updateNodePerformance(srcId, lossRate, delay, nodePerformance[srcId]);
    }

    // 更新最后收到数据包的时间
    lastPacketTimes[srcId] = simTime().dbl();

    // 将消息添加到簇延迟统计
    if (clusterId > 0) {
        clusterDelays[clusterId].push_back(delay);
        // 保持最多100个样本
        if (clusterDelays[clusterId].size() > 100) {
            clusterDelays[clusterId].erase(clusterDelays[clusterId].begin());
        }
    }

    EV_INFO << "Received status message from node " << srcId
            << " (type=" << nodeType << ", cluster=" << clusterId
            << "), status=" << msg->getStatus()
            << ", battery=" << msg->getBatteryLevel() << endl;

    // 处理完毕，删除消息
    delete msg;
}

void ControlNodeApplication::handleStatusAggregationMessage(StatusAggregation *msg)
{
    int srcId = msg->getSrcId();
    int clusterId = srcId; // 簇头ID等于簇ID

    EV_INFO << "Received status aggregation from cluster head " << srcId
            << ", node count=" << msg->getNodeCount()
            << ", active nodes=" << msg->getActiveNodes() << endl;

    // 处理完毕，删除消息
    delete msg;
}

void ControlNodeApplication::handleManagementCommandResponse(ManagementCommand *msg)
{
    int srcId = msg->getSrcId();
    int commandType = msg->getCommandType();
    std::string response = msg->getCommandData();

    EV_INFO << "Received management command response from node " << srcId
            << ", command type=" << commandType
            << ", response=" << response << endl;

    // 处理完毕，删除消息
    delete msg;
}

void ControlNodeApplication::handleAddressingMessage(cMessage *msg)
{
    // 处理寻址消息
    if (ClusterAddressing *clusterMsg = dynamic_cast<ClusterAddressing *>(msg)) {
        int srcId = clusterMsg->getSrcId();
        std::string targetId = clusterMsg->getTargetIdentity();

        EV_INFO << "Received cluster addressing from node " << srcId
                << ", looking for " << targetId << endl;
    } else if (InterClusterAddressing *interMsg = dynamic_cast<InterClusterAddressing *>(msg)) {
        int srcId = interMsg->getSrcId();
        std::string targetCluster = interMsg->getTargetCluster();

        EV_INFO << "Received inter-cluster addressing from node " << srcId
                << ", looking for cluster " << targetCluster << endl;
    }

    // 处理完毕，删除消息
    delete msg;
}

void ControlNodeApplication::handleStatusTimer()
{
    // 发送状态消息
    StatusMessage *statusMsg = generateStatusMessage();
    sendToComm(statusMsg);

    // 重新调度定时器
    scheduleAt(simTime() + 1.0, statusTimer);
}

void ControlNodeApplication::handleClusterHeadCheckTimer()
{
    // 检查簇头状态
    checkClusterHeads();

    // 重新调度定时器
    scheduleAt(simTime() + clusterHeadCheckInterval, clusterHeadCheckTimer);
}

void ControlNodeApplication::handleNetworkStatusTimer()
{
    // 记录网络状态
    networkManager->logNetworkStatus();

    // 重新调度定时器
    scheduleAt(simTime() + networkStatusLogInterval, networkStatusTimer);
}

void ControlNodeApplication::handleWaveformConfigTimer()
{
    // 分发波形配置
    distributeWaveformConfig();

    // 重新调度定时器
    scheduleAt(simTime() + waveformConfigInterval, waveformConfigTimer);
}

void ControlNodeApplication::handleManagementCommandTimer()
{
    // 随机选择一个簇进行簇头轮换
    cModule *networkModule = getParentModule()->getParentModule();
    int numClusters = networkModule->par("numClusters");
    int clusterId = intuniform(1, numClusters);

    // 尝试轮换簇头
    int currentHeadId = networkManager->getCurrentClusterHead(clusterId);
    const auto& clusterInfo = networkManager->getClusterInfo(clusterId);

    if (!clusterInfo.memberIds.empty()) {
        // 随机选择一个不是当前簇头的成员
        std::vector<int> candidates;
        for (int nodeId : clusterInfo.memberIds) {
            if (nodeId != currentHeadId) {
                candidates.push_back(nodeId);
            }
        }

        if (!candidates.empty()) {
            int newHeadId = candidates[intuniform(0, candidates.size() - 1)];

            // 发送簇头轮换命令
            EV_INFO << "Initiating periodic cluster head rotation for cluster "
                    << clusterId << ", from " << currentHeadId << " to " << newHeadId << endl;

            rotateClusterHead(clusterId, newHeadId);
        }
    }

    // 重新调度定时器
    scheduleAt(simTime() + managementCommandInterval, managementCommandTimer);
}

void ControlNodeApplication::checkClusterHeads()
{
    // 让网络管理器检查簇头状态
    bool changes = networkManager->checkClusterHeadStatus();

    if (changes) {
        EV_INFO << "Cluster head status check resulted in changes." << endl;
    } else {
        EV_INFO << "Cluster head status check completed. No changes needed." << endl;
    }
}

void ControlNodeApplication::rotateClusterHead(int clusterId, int newHeadId)
{
    // 创建管理命令
    std::string commandData = "CH_CHANGE:" + std::to_string(clusterId);
    ManagementCommand *cmd = generateManagementCommand(1, newHeadId, commandData);

    // 发送命令
    sendToComm(cmd);

    // 同时更新网络管理器
    networkManager->setClusterHead(clusterId, newHeadId);
}

void ControlNodeApplication::sendToComm(cMessage *msg)
{
    // 发送消息到通信模块
    send(msg, "toComm");
    sentMessages++;
}

void ControlNodeApplication::updateNodePerformance(int nodeId, double delay, bool success)
{
    // 简单的性能评分模型
    double currentScore = nodePerformance[nodeId];

    // 基于延迟和成功状态更新性能评分
    double delayFactor = 1.0 - std::min(1.0, delay / 0.5); // 500ms以内的延迟都认为很好
    double successFactor = success ? 1.0 : 0.0;

    // 加权组合
    double newScore = 0.6 * delayFactor + 0.4 * successFactor;

    // 平滑更新
    nodePerformance[nodeId] = 0.8 * currentScore + 0.2 * newScore;
}

void ControlNodeApplication::sendNetworkCommand(int commandType, int targetId, const std::string& data)
{
    // 创建并发送网络管理命令
    ManagementCommand *cmd = generateManagementCommand(commandType, targetId, data);
    sendToComm(cmd);
}

void ControlNodeApplication::distributeWaveformConfig()
{
    cModule *networkModule = getParentModule()->getParentModule();
    int numClusters = networkModule->par("numClusters");

    // 为每个簇头发送波形配置
    for (int i = 1; i <= numClusters; i++) {
        int clusterHeadId = networkManager->getCurrentClusterHead(i);

        if (clusterHeadId != -1) {
            // 随机选择一个波形参数 (例如传输速率)
            int waveformType = 1; // 类型1为速率设置
            double value = uniform(20, 100); // 20-100kbps范围内的随机值

            WaveformConfig *config = generateWaveformConfigMessage(clusterHeadId, waveformType, value);
            sendToComm(config);

            EV_INFO << "Sent waveform config to cluster head " << clusterHeadId
                    << ", type=" << waveformType << ", value=" << value << endl;
        }
    }
}

StatusMessage* ControlNodeApplication::generateStatusMessage()
{
    StatusMessage *msg = new StatusMessage("ControlStatus");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(-1); // 广播给所有节点
    msg->setFrequency(-1); // 使用所有频段
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(simTime().dbl());
    msg->setMessageType(3); // 态势消息类型

    // 控制节点特有信息
    msg->setPositionX(500); // 中央位置
    msg->setPositionY(500);
    msg->setPositionZ(30);
    msg->setNodeType(1); // 控制节点类型
    msg->setStatus(1); // 运行状态
    msg->setFrequency(-1); // 支持所有频段
    msg->setBatteryLevel(5000.0 - sentMessages * 0.01); // 电池电量逐渐减少

    return msg;
}

ManagementCommand* ControlNodeApplication::generateManagementCommand(int commandType, int targetId, const std::string& data)
{
    ManagementCommand *msg = new ManagementCommand("NetworkCommand");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(targetId);
    msg->setFrequency(-1); // 使用所有频段
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(simTime().dbl());
    msg->setMessageType(5); // 管理命令类型

    // 命令信息
    msg->setTargetId(targetId);
    msg->setCommandType(commandType);
    msg->setExtensionParam(0.0);
    msg->setCommandData(data.c_str());
    msg->setResponseFlag(1); // 需要响应

    return msg;
}

WaveformConfig* ControlNodeApplication::generateWaveformConfigMessage(int targetId, int waveformType, double value)
{
    WaveformConfig *msg = new WaveformConfig("WaveformConfig");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(targetId);
    msg->setFrequency(-1); // 使用所有频段
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(simTime().dbl());
    msg->setMessageType(8); // 波形参数消息类型

    // 波形配置信息
    msg->setWaveformType(waveformType);
    msg->setParameterValue(value);
    msg->setConfigResponse(1); // 需要响应

    return msg;
}
