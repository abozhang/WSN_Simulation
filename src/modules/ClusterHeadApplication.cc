// ClusterHeadApplication.cc
#include "ClusterHeadApplication.h"
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace wsn_simulation;

Define_Module(ClusterHeadApplication);

ClusterHeadApplication::ClusterHeadApplication()
{
}

ClusterHeadApplication::~ClusterHeadApplication()
{
}

void ClusterHeadApplication::initialize()
{
    // 获取参数
    nodeId = getParentModule()->par("nodeId");
    clusterId = nodeId; // 簇头ID等于簇ID
    frequency = clusterId; // 频段与簇ID对应

    // 初始化状态
    currentTime = 0.0;
    batteryLevel = 3000.0; // 初始电池电量
    status = 1; // 运行状态
    roleReplaced = false;

    // 获取位置信息
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");
    positionX = mobilityModule->par("initialX");
    positionY = mobilityModule->par("initialY");
    positionZ = mobilityModule->par("initialZ");

    // 创建定时器
    statusTimer = new cMessage("statusTimer");
    aggregationTimer = new cMessage("aggregationTimer");
    memberCheckTimer = new cMessage("memberCheckTimer");

    // 初始化统计
    sentMessages = 0;
    receivedMessages = 0;
    aggregatedStatusCount = 0;

    // 调度定时器
    scheduleAt(simTime() + 1.0, statusTimer);
    scheduleAt(simTime() + 10.0, aggregationTimer);
    scheduleAt(simTime() + 30.0, memberCheckTimer);

    EV_INFO << "ClusterHeadApplication initialized for cluster " << clusterId << endl;
}

void ClusterHeadApplication::handleMessage(cMessage *msg)
{
    if (roleReplaced) {
        // 如果已经不再是簇头，转发消息给新簇头
        if (msg->isSelfMessage()) {
            delete msg; // 删除自己的定时器消息
            return;
        } else if (msg->arrivedOn("fromComm")) {
            BaseMessage *baseMsg = check_and_cast<BaseMessage *>(msg);
            // 检查是否是发给自己的消息
            if (baseMsg->getDestId() == nodeId || baseMsg->getDestId() == -1) {
                // 处理本地，因为这条消息可能是为这个节点准备的
                // 进一步处理在下面的if-else结构中
            } else {
                // 不是发给自己的，删除
                delete msg;
                return;
            }
        }
    }

    // 常规消息处理
    if (msg == statusTimer) {
        handleStatusTimer();
    } else if (msg == aggregationTimer) {
        handleAggregationTimer();
    } else if (msg == memberCheckTimer) {
        handleMemberCheckTimer();
    } else if (msg->arrivedOn("fromComm")) {
        // 处理接收到的消息
        receivedMessages++;

        if (StatusMessage *statusMsg = dynamic_cast<StatusMessage *>(msg)) {
            handleStatusMessage(statusMsg);
        } else if (TimeMessage *timeMsg = dynamic_cast<TimeMessage *>(msg)) {
            handleTimeMessage(timeMsg);
        } else if (TimeCalibration *calibMsg = dynamic_cast<TimeCalibration *>(msg)) {
            handleTimeCalibrationMessage(calibMsg);
        } else if (ManagementCommand *cmdMsg = dynamic_cast<ManagementCommand *>(msg)) {
            handleManagementCommand(cmdMsg);
        } else if (dynamic_cast<ClusterAddressing *>(msg) || dynamic_cast<InterClusterAddressing *>(msg)) {
            handleAddressingMessage(msg);
        } else if (WaveformConfig *configMsg = dynamic_cast<WaveformConfig *>(msg)) {
            handleWaveformConfigMessage(configMsg);
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

void ClusterHeadApplication::finish()
{
    // 清理定时器
    cancelAndDelete(statusTimer);
    cancelAndDelete(aggregationTimer);
    cancelAndDelete(memberCheckTimer);

    // 输出统计信息
    recordScalar("sentMessages", sentMessages);
    recordScalar("receivedMessages", receivedMessages);
    recordScalar("aggregatedStatusCount", aggregatedStatusCount);

    EV_INFO << "ClusterHeadApplication finished with stats:" << endl;
    EV_INFO << "  Sent messages: " << sentMessages << endl;
    EV_INFO << "  Received messages: " << receivedMessages << endl;
    EV_INFO << "  Aggregated status count: " << aggregatedStatusCount << endl;

    // 记录簇内成员状态
    EV_INFO << "  Cluster members at finish: " << clusterMembers.size() << endl;
    for (const auto& pair : clusterMembers) {
        int memberId = pair.first;
        const NodeInfo& info = pair.second;
        EV_INFO << "    Node " << memberId << ": status=" << info.status
                << ", battery=" << info.batteryLevel
                << ", loss rate=" << info.getPacketLossRate()
                << ", avg delay=" << info.getAverageDelay() << endl;
    }
}

void ClusterHeadApplication::handleStatusMessage(StatusMessage *msg)
{
    int srcId = msg->getSrcId();
    int nodeType = msg->getNodeType();
    double delay = simTime().dbl() - msg->getTimestamp();

    // 只处理来自本簇普通节点的状态消息
    if (nodeType == 3) { // 普通节点
        // 检查是否在簇内
        int senderClusterId = (srcId - 6) / 200 + 1; // 根据节点ID计算簇ID

        if (senderClusterId == clusterId) {
            // 更新或添加节点信息
            if (clusterMembers.find(srcId) == clusterMembers.end()) {
                // 新节点
                NodeInfo info;
                info.nodeId = srcId;
                info.batteryLevel = msg->getBatteryLevel();
                info.positionX = msg->getPositionX();
                info.positionY = msg->getPositionY();
                info.positionZ = msg->getPositionZ();
                info.status = msg->getStatus();
                info.lastUpdateTime = simTime().dbl();
                info.receivedPackets = 1;
                info.lostPackets = 0;
                info.delays.push_back(delay);

                clusterMembers[srcId] = info;

                // 添加到普通节点列表
                ordinaryNodeIds.push_back(srcId);
            } else {
                // 更新已有节点信息
                NodeInfo& info = clusterMembers[srcId];

                // 检查是否丢失了中间的包
                double expectedInterval = 1.0; // 每秒一个状态消息
                double actualInterval = simTime().dbl() - info.lastUpdateTime;
                int expectedPackets = std::max(1, static_cast<int>(actualInterval / expectedInterval));
                int lostPackets = std::max(0, expectedPackets - 1);

                info.batteryLevel = msg->getBatteryLevel();
                info.positionX = msg->getPositionX();
                info.positionY = msg->getPositionY();
                info.positionZ = msg->getPositionZ();
                info.status = msg->getStatus();
                info.lastUpdateTime = simTime().dbl();
                info.receivedPackets++;
                info.lostPackets += lostPackets;

                // 记录延迟
                info.delays.push_back(delay);
                if (info.delays.size() > 10) {
                    info.delays.erase(info.delays.begin());
                }
            }

            // 更新簇内节点性能监控
            nodeBatteryLevels[srcId] = msg->getBatteryLevel();
            nodeDelays[srcId] = delay;
            nodePacketCounts[srcId]++;
            nodeLastUpdateTimes[srcId] = simTime().dbl();

            EV_INFO << "Updated status for node " << srcId << ", battery=" << msg->getBatteryLevel()
                    << ", status=" << msg->getStatus() << ", delay=" << delay << endl;
        }
    }

    // 处理完毕，删除消息
    delete msg;
}

void ClusterHeadApplication::handleTimeMessage(TimeMessage *msg)
{
    int srcId = msg->getSrcId();
    double referenceTime = msg->getReferenceTime();

    EV_INFO << "Received time message from node " << srcId
            << ", reference time=" << referenceTime << endl;

    // 处理完毕，删除消息
    delete msg;
}

void ClusterHeadApplication::handleTimeCalibrationMessage(TimeCalibration *msg)
{
    int srcId = msg->getSrcId();
    double sendTime = msg->getSendTime();

    EV_INFO << "Received time calibration from node " << srcId
            << ", send time=" << sendTime << endl;

    // 处理完毕，删除消息
    delete msg;
}

void ClusterHeadApplication::handleManagementCommand(ManagementCommand *msg)
{
    int srcId = msg->getSrcId();
    int commandType = msg->getCommandType();
    int targetId = msg->getTargetId();
    std::string commandData = msg->getCommandData();

    EV_INFO << "Received management command from node " << srcId
            << ", command type=" << commandType
            << ", target=" << targetId
            << ", data=" << commandData << endl;

    // 检查是否是簇头变更命令
    if (commandType == 1 && (targetId == nodeId || targetId == -1)) {
        // 簇头变更命令
        if (commandData.find("CH_CHANGE:") == 0) {
            handleClusterHeadChange(msg);
            return; // handleClusterHeadChange会删除消息
        }
    }

    // 创建响应消息
    if (msg->getResponseFlag() == 1) {
        ManagementCommand *response = new ManagementCommand("CommandResponse");
        response->setSrcId(nodeId);
        response->setDestId(srcId);
        response->setFrequency(frequency);
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(simTime().dbl());
        response->setMessageType(5); // 管理响应类型

        response->setTargetId(targetId);
        response->setCommandType(commandType);
        response->setExtensionParam(0.0);

        std::string responseData = "ACK_FROM_CH";
        response->setCommandData(responseData.c_str());
        response->setResponseFlag(0); // 不需要再次响应

        sendToComm(response);
    }

    // 处理完毕，删除消息
    delete msg;
}

void ClusterHeadApplication::handleAddressingMessage(cMessage *msg)
{
    // 处理寻址消息
    if (ClusterAddressing *clusterMsg = dynamic_cast<ClusterAddressing *>(msg)) {
        int srcId = clusterMsg->getSrcId();
        std::string targetId = clusterMsg->getTargetIdentity();

        EV_INFO << "Received cluster addressing from node " << srcId
                << ", looking for " << targetId << endl;

        // 创建响应
        ClusterAddressing *response = new ClusterAddressing("AddressingResponse");
        response->setSrcId(nodeId);
        response->setDestId(srcId);
        response->setFrequency(frequency);
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(simTime().dbl());
        response->setMessageType(6); // 寻址响应类型

        response->setSourceIdentity(std::to_string(nodeId).c_str());
        response->setTargetIdentity(clusterMsg->getSourceIdentity());

        // 检查目标是否在簇内
        bool found = false;
        for (int memberId : ordinaryNodeIds) {
            if (std::to_string(memberId) == targetId) {
                found = true;
                break;
            }
        }

        std::string responseQuery = found ? "FOUND" : "NOT_FOUND";
        response->setAddressingQuery(responseQuery.c_str());

        sendToComm(response);
    } else if (InterClusterAddressing *interMsg = dynamic_cast<InterClusterAddressing *>(msg)) {
        int srcId = interMsg->getSrcId();
        std::string targetCluster = interMsg->getTargetCluster();

        EV_INFO << "Received inter-cluster addressing from node " << srcId
                << ", looking for cluster " << targetCluster << endl;

        // 创建响应
        InterClusterAddressing *response = new InterClusterAddressing("InterAddressingResponse");
        response->setSrcId(nodeId);
        response->setDestId(srcId);
        response->setFrequency(frequency);
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(simTime().dbl());
        response->setMessageType(7); // 簇间寻址响应类型

        response->setSourceCluster(std::to_string(clusterId).c_str());
        response->setTargetCluster(interMsg->getSourceCluster());

        // 检查是否是查询本簇
        bool isOwnCluster = (targetCluster == std::to_string(clusterId));
        std::string responseQuery = isOwnCluster ? "CLUSTER_FOUND" : "CLUSTER_NOT_FOUND";
        response->setAddressingQuery(responseQuery.c_str());

        sendToComm(response);
    }

    // 处理完毕，删除消息
    delete msg;
}

void ClusterHeadApplication::handleWaveformConfigMessage(WaveformConfig *msg)
{
    int srcId = msg->getSrcId();
    int waveformType = msg->getWaveformType();
    double value = msg->getParameterValue();

    EV_INFO << "Received waveform config from node " << srcId
            << ", type=" << waveformType
            << ", value=" << value << endl;

    // 在实际应用中，这里会根据波形类型调整相应的参数
    // 比如调整传输速率、功率等

    // 发送响应
    if (msg->getConfigResponse() == 1) {
        WaveformConfig *response = new WaveformConfig("WaveformConfigResponse");
        response->setSrcId(nodeId);
        response->setDestId(srcId);
        response->setFrequency(frequency);
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(simTime().dbl());
        response->setMessageType(8); // 波形参数响应类型

        response->setWaveformType(waveformType);
        response->setParameterValue(value);
        response->setConfigResponse(0); // 不需要响应

        sendToComm(response);
    }

    // 处理完毕，删除消息
    delete msg;
}

void ClusterHeadApplication::handleStatusTimer()
{
    // 发送状态消息
    StatusMessage *statusMsg = generateStatusMessage();
    sendToComm(statusMsg);

    // 更新位置
    updatePosition();

    // 消耗电池电量
    updateBatteryStatus(0.2); // 簇头消耗更多电量

    // 重新调度定时器
    scheduleAt(simTime() + 1.0, statusTimer);
}

void ClusterHeadApplication::handleAggregationTimer()
{
    // 聚合并发送簇内状态
    aggregateClusterStatus();

    // 重新调度定时器
    scheduleAt(simTime() + 10.0, aggregationTimer);
}

void ClusterHeadApplication::handleMemberCheckTimer()
{
    // 检查簇内成员状态
    checkClusterMembers();

    // 重新调度定时器
    scheduleAt(simTime() + 30.0, memberCheckTimer);
}

void ClusterHeadApplication::aggregateClusterStatus()
{
    // 统计簇内活跃节点数量
    int activeNodes = 0;
    for (const auto& pair : clusterMembers) {
        if (isNodeActive(pair.first)) {
            activeNodes++;
        }
    }

    // 生成并发送簇状态聚合消息
    StatusAggregation *aggMsg = generateStatusAggregationMessage();
    aggMsg->setNodeCount(clusterMembers.size());
    aggMsg->setActiveNodes(activeNodes);

    // 资源使用情况信息
    std::ostringstream resourceInfo;
    double avgBattery = 0.0;
    double avgDelay = 0.0;
    double avgLossRate = 0.0;

    // 计算簇内平均指标
    if (!clusterMembers.empty()) {
        for (const auto& pair : clusterMembers) {
            const NodeInfo& info = pair.second;
            avgBattery += info.batteryLevel;
            avgDelay += info.getAverageDelay();
            avgLossRate += info.getPacketLossRate();
        }

        avgBattery /= clusterMembers.size();
        avgDelay /= clusterMembers.size();
        avgLossRate /= clusterMembers.size();
    }

    resourceInfo << "BAT:" << avgBattery << ",DLY:" << avgDelay << ",LOSS:" << avgLossRate * 100 << "%";

    aggMsg->setResourceUsage(resourceInfo.str().c_str());

    sendToComm(aggMsg);
    aggregatedStatusCount++;

    EV_INFO << "Sent aggregated status: members=" << clusterMembers.size()
            << ", active=" << activeNodes
            << ", resources=" << resourceInfo.str() << endl;
}

void ClusterHeadApplication::checkClusterMembers()
{
    // 检查簇内成员状态
    double currentTime = simTime().dbl();
    std::vector<int> inactiveNodes;

    for (auto& pair : clusterMembers) {
        int memberId = pair.first;
        NodeInfo& info = pair.second;

        // 如果节点长时间未更新，标记为不活跃
        if (currentTime - info.lastUpdateTime > 10.0) {
            info.status = 4; // 离线状态
            inactiveNodes.push_back(memberId);

            EV_WARN << "Node " << memberId << " is inactive. Last update: "
                    << info.lastUpdateTime << ", now: " << currentTime << endl;
        }
    }

    // 从普通节点列表中移除不活跃节点
    for (int nodeId : inactiveNodes) {
        auto it = std::find(ordinaryNodeIds.begin(), ordinaryNodeIds.end(), nodeId);
        if (it != ordinaryNodeIds.end()) {
            ordinaryNodeIds.erase(it);
        }
    }

    EV_INFO << "Cluster member check completed. "
            << inactiveNodes.size() << " inactive nodes found." << endl;
}

bool ClusterHeadApplication::isNodeActive(int nodeId)
{
    auto it = clusterMembers.find(nodeId);
    if (it != clusterMembers.end()) {
        const NodeInfo& info = it->second;
        return (info.status == 1 || info.status == 2) && // 运行或低电量状态
               (simTime().dbl() - info.lastUpdateTime <= 5.0); // 最近5秒内有更新
    }
    return false;
}

void ClusterHeadApplication::handleClusterHeadChange(ManagementCommand *msg)
{
    int srcId = msg->getSrcId();
    int targetId = msg->getTargetId();
    std::string commandData = msg->getCommandData();

    // 解析命令数据，获取簇ID
    int targetClusterId = -1;
    size_t pos = commandData.find(":");
    if (pos != std::string::npos) {
        targetClusterId = std::stoi(commandData.substr(pos + 1));
    }

    // 检查是否是本簇的簇头变更
    if (targetClusterId == clusterId) {
        // 检查目标节点是否是自己
        if (targetId == nodeId) {
            // 自己被指定为新簇头，应该已经是簇头，无需处理
            EV_INFO << "I am already the cluster head of cluster " << clusterId << endl;
        } else {
            // 自己被替换，转变为普通节点
            EV_INFO << "I am being replaced as cluster head by node " << targetId << endl;
            becomeOrdinaryNode();
        }
    }

    // 发送响应
    if (msg->getResponseFlag() == 1) {
        ManagementCommand *response = new ManagementCommand("ChChangeResponse");
        response->setSrcId(nodeId);
        response->setDestId(srcId);
        response->setFrequency(frequency);
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(simTime().dbl());
        response->setMessageType(5); // 管理响应类型

        response->setTargetId(targetId);
        response->setCommandType(1); // 簇头变更
        response->setExtensionParam(0.0);

        std::string responseData = roleReplaced ? "CH_REPLACED" : "CH_ACTIVE";
        response->setCommandData(responseData.c_str());
        response->setResponseFlag(0); // 不需要再次响应

        sendToComm(response);
    }

    // 处理完毕，删除消息
    delete msg;
}

StatusMessage* ClusterHeadApplication::generateStatusMessage()
{
    StatusMessage *msg = new StatusMessage("ClusterHeadStatus");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(0); // 发送给控制节点
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(simTime().dbl());
    msg->setMessageType(3); // 态势消息类型

    // 簇头节点特有信息
    msg->setPositionX(positionX);
    msg->setPositionY(positionY);
    msg->setPositionZ(positionZ);
    msg->setNodeType(2); // 簇头节点类型
    msg->setStatus(status);
    msg->setFrequency(frequency);
    msg->setBatteryLevel(batteryLevel);

    return msg;
}

StatusAggregation* ClusterHeadApplication::generateStatusAggregationMessage()
{
    StatusAggregation *msg = new StatusAggregation("ClusterAggregation");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(0); // 发送给控制节点
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(simTime().dbl());
    msg->setMessageType(4); // 态势汇聚消息类型

    return msg;
}

void ClusterHeadApplication::sendToComm(cMessage *msg)
{
    // 发送消息到通信模块
    send(msg, "toComm");
    sentMessages++;
}

void ClusterHeadApplication::updateBatteryStatus(double consumption)
{
    // 更新电池电量
    batteryLevel -= consumption;

    // 检查电池状态
    if (batteryLevel <= 0) {
        batteryLevel = 0;
        status = 3; // 故障状态
    } else if (batteryLevel < 500) {
        status = 2; // 低电量状态
    }
}

void ClusterHeadApplication::updatePosition()
{
    // 获取最新位置
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");

    if (mobilityModule->isSimple()) {
        // 直接获取参数
        positionX = mobilityModule->par("initialX").doubleValue();
        positionY = mobilityModule->par("initialY").doubleValue();
        positionZ = mobilityModule->par("initialZ").doubleValue();
    } else {
        // 通过接口获取位置
        cGate *gate = mobilityModule->gate("out");
        if (gate && gate->isConnected()) {
            cMessage *posReq = new cMessage("positionRequest");
            send(posReq, gate);
        }
    }
}

double ClusterHeadApplication::calculateAverageDelay(int nodeId)
{
    auto it = clusterMembers.find(nodeId);
    if (it != clusterMembers.end()) {
        return it->second.getAverageDelay();
    }
    return 0.0;
}

void ClusterHeadApplication::becomeOrdinaryNode()
{
    // 转变为普通节点
    roleReplaced = true;

    // 更改节点类型
    getParentModule()->par("nodeType").setStringValue("ordinary");

    // 停止簇头特有的功能
    cancelEvent(aggregationTimer);
    cancelEvent(memberCheckTimer);

    // 清空簇成员信息
    clusterMembers.clear();
    ordinaryNodeIds.clear();

    // 发送状态消息通知身份变更
    StatusMessage *statusMsg = generateStatusMessage();
    statusMsg->setNodeType(3); // 普通节点类型
    sendToComm(statusMsg);

    EV_INFO << "Node " << nodeId << " is now an ordinary node" << endl;
}
