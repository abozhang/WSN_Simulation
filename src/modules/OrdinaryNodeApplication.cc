// OrdinaryNodeApplication.cc
#include "OrdinaryNodeApplication.h"
#include <cmath>

using namespace wsn_simulation;

Define_Module(OrdinaryNodeApplication);

OrdinaryNodeApplication::OrdinaryNodeApplication()
{
}

OrdinaryNodeApplication::~OrdinaryNodeApplication()
{
}

void OrdinaryNodeApplication::initialize()
{
    // 获取参数
    nodeId = getParentModule()->par("nodeId");

    // 计算簇ID和频段
    clusterId = (nodeId - 6) / 200 + 1; // 6是控制节点(0)和5个簇头节点(1-5)
    frequency = clusterId;

    // 初始化状态
    currentTime = 0.0;
    batteryLevel = 1000.0; // 初始电池电量
    status = 1; // 运行状态

    // 初始化时钟
    localClock = simTime().dbl();
    clockDrift = uniform(0.9995, 1.0005); // 随机漂移
    lastSyncTime = simTime().dbl();

    // 簇头管理
    clusterHeadId = clusterId; // 默认簇头ID等于簇ID
    becameClusterHead = false;

    // 获取位置信息
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");
    positionX = mobilityModule->par("initialX");
    positionY = mobilityModule->par("initialY");
    positionZ = mobilityModule->par("initialZ");

    // 创建定时器
    statusTimer = new cMessage("statusTimer");
    syncTimer = new cMessage("syncTimer");

    // 初始化统计
    sentMessages = 0;
    receivedMessages = 0;
    avgDelayToClusterHead = 0.0;
    delayMeasurements = 0;

    // 调度定时器
    scheduleAt(simTime() + uniform(0.1, 1.0), statusTimer); // 随机初始化，避免同步发送
    scheduleAt(simTime() + 60.0, syncTimer);

    EV_INFO << "OrdinaryNodeApplication initialized for node " << nodeId
            << " in cluster " << clusterId << endl;
}

void OrdinaryNodeApplication::handleMessage(cMessage *msg)
{
    if (becameClusterHead) {
        // 如果已经变成簇头，则不再按普通节点处理消息
        // 应该由簇头应用程序模块接管
        delete msg;
        return;
    }

    if (msg == statusTimer) {
        handleStatusTimer();
    } else if (msg == syncTimer) {
        handleSyncTimer();
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

void OrdinaryNodeApplication::finish()
{
    // 清理定时器
    cancelAndDelete(statusTimer);
    cancelAndDelete(syncTimer);

    // 输出统计信息
    recordScalar("sentMessages", sentMessages);
    recordScalar("receivedMessages", receivedMessages);

    if (delayMeasurements > 0) {
        recordScalar("averageDelayToClusterHead", avgDelayToClusterHead);
    }

    EV_INFO << "OrdinaryNodeApplication finished with stats:" << endl;
    EV_INFO << "  Sent messages: " << sentMessages << endl;
    EV_INFO << "  Received messages: " << receivedMessages << endl;

    if (delayMeasurements > 0) {
        EV_INFO << "  Average delay to cluster head: " << avgDelayToClusterHead << "s" << endl;
    }
}

void OrdinaryNodeApplication::handleStatusMessage(StatusMessage *msg)
{
    int srcId = msg->getSrcId();

    if (srcId == clusterHeadId) {
        // 来自簇头的状态消息
        EV_INFO << "Received status from cluster head " << srcId << endl;
    } else if (srcId == 0) {
        // 来自控制节点的状态消息
        EV_INFO << "Received status from control node" << endl;
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::handleTimeMessage(TimeMessage *msg)
{
    int srcId = msg->getSrcId();
    double referenceTime = msg->getReferenceTime();

    // 来自控制节点的时间同步消息
    if (srcId == 0) {
        // 更新本地时钟
        double timeDiff = referenceTime - localClock;

        EV_INFO << "Received time sync from control node. Diff: " << timeDiff << "s" << endl;

        // 如果时间差超过阈值，则同步
        if (fabs(timeDiff) > 0.1) {
            localClock = referenceTime;
            lastSyncTime = simTime().dbl();
            EV_INFO << "Clock synchronized to: " << localClock << endl;
        }
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::handleTimeCalibrationMessage(TimeCalibration *msg)
{
    int srcId = msg->getSrcId();
    int calibrationType = msg->getCalibrationType();

    // 处理校时响应
    if (calibrationType == 2) { // 响应校准
        double sendTime = msg->getSendTime();
        double receiveTime = msg->getReceiveTime();
        double responseTime = simTime().dbl();

        // 计算往返时间和时钟偏差
        double roundTripTime = responseTime - sendTime;
        double clockOffset = (receiveTime - sendTime - roundTripTime/2);

        EV_INFO << "Received calibration response. RTT: " << roundTripTime
                << "s, offset: " << clockOffset << "s" << endl;

        // 调整本地时钟
        localClock -= clockOffset;
        lastSyncTime = simTime().dbl();

        EV_INFO << "Clock calibrated to: " << localClock << endl;
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::handleManagementCommand(ManagementCommand *msg)
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
    if (msg->getResponseFlag() == 1 && targetId == nodeId) {
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

        std::string responseData = "ACK_FROM_ON";
        response->setCommandData(responseData.c_str());
        response->setResponseFlag(0); // 不需要再次响应

        sendToComm(response);
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::handleAddressingMessage(cMessage *msg)
{
    // 处理寻址消息
    if (ClusterAddressing *clusterMsg = dynamic_cast<ClusterAddressing *>(msg)) {
        int srcId = clusterMsg->getSrcId();
        std::string targetId = clusterMsg->getTargetIdentity();

        // 检查是否在寻找自己
        if (targetId == std::to_string(nodeId)) {
            EV_INFO << "Received addressing query looking for me." << endl;

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
            response->setAddressingQuery("RESPONSE");

            sendToComm(response);
        }
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::handleWaveformConfigMessage(WaveformConfig *msg)
{
    // 普通节点通常不直接处理波形配置消息，除非是广播配置
    if (msg->getDestId() == -1 || msg->getDestId() == nodeId) {
        int srcId = msg->getSrcId();
        int waveformType = msg->getWaveformType();
        double value = msg->getParameterValue();

        EV_INFO << "Received broadcast waveform config from node " << srcId
                << ", type=" << waveformType
                << ", value=" << value << endl;

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
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::handleStatusTimer()
{
    // 更新时钟
    updateClock();

    // 更新位置
    updatePosition();

    // 发送状态消息
    StatusMessage *statusMsg = generateStatusMessage();
    sendToComm(statusMsg);

    // 消耗电池电量
    updateBatteryStatus(0.1);

    // 测量发送到簇头的延迟
    double sendTime = simTime().dbl();

    // 更新平均延迟统计
    // 这里是一个简化的模拟，实际上需要收到簇头的响应才能测量
    // 这里我们假设延迟与距离成正比
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");
    double nodeX = mobilityModule->par("initialX");
    double nodeY = mobilityModule->par("initialY");
    double nodeZ = mobilityModule->par("initialZ");

    // 计算与簇头的距离
    cModule *network = getParentModule()->getParentModule();
    cModule *clusterHeadModule = network->getSubmodule("clusterHeads", clusterId - 1);
    cModule *clusterHeadMobility = clusterHeadModule->getSubmodule("mobility");

    double chX = clusterHeadMobility->par("initialX");
    double chY = clusterHeadMobility->par("initialY");
    double chZ = clusterHeadMobility->par("initialZ");

    double distance = sqrt(pow(nodeX - chX, 2) + pow(nodeY - chY, 2) + pow(nodeZ - chZ, 2));

    // 假设每100米增加1ms延迟
    double estimatedDelay = distance / 100000; // 转换为秒

    // 更新平均延迟
    avgDelayToClusterHead = (avgDelayToClusterHead * delayMeasurements + estimatedDelay) / (delayMeasurements + 1);
    delayMeasurements++;

    // 重新调度定时器
    scheduleAt(simTime() + 1.0, statusTimer);
}

void OrdinaryNodeApplication::handleSyncTimer()
{
    // 发送校时请求
    TimeCalibration *calibMsg = generateTimeCalibrationMessage();
    sendToComm(calibMsg);

    // 重新调度定时器
    scheduleAt(simTime() + 60.0, syncTimer);
}

void OrdinaryNodeApplication::handleClusterHeadChange(ManagementCommand *msg)
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
            EV_INFO << "I am being promoted to cluster head for cluster " << clusterId << endl;
            becomeClusterHead();
        } else {
            // 更新簇头ID
            clusterHeadId = targetId;
            EV_INFO << "Updated cluster head for cluster " << clusterId << " to node " << targetId << endl;
        }
    }

    // 发送响应
    if (msg->getResponseFlag() == 1 && (targetId == nodeId || targetId == -1)) {
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

        std::string responseData = becameClusterHead ? "PROMOTED_TO_CH" : "CH_UPDATED";
        response->setCommandData(responseData.c_str());
        response->setResponseFlag(0); // 不需要再次响应

        sendToComm(response);
    }

    // 处理完毕，删除消息
    delete msg;
}

void OrdinaryNodeApplication::becomeClusterHead()
{
    // 转变为簇头
    becameClusterHead = true;

    // 更改节点类型
    getParentModule()->par("nodeType").setStringValue("clusterHead");

    // 增加电池电量，簇头需要更多电量
    batteryLevel = std::max(batteryLevel, 3000.0);

    // 创建并初始化簇头应用层模块
    cModuleType *moduleType = cModuleType::get("wsn_simulation.ClusterHeadApplication");
    cModule *module = moduleType->create("clusterHeadApp", getParentModule());
    module->finalizeParameters();
    module->buildInside();
    module->scheduleStart(simTime());

    // 停止普通节点的定时器
    cancelEvent(statusTimer);
    cancelEvent(syncTimer);

    EV_INFO << "Node " << nodeId << " has been promoted to cluster head for cluster " << clusterId << endl;
}

StatusMessage* OrdinaryNodeApplication::generateStatusMessage()
{
    StatusMessage *msg = new StatusMessage("OrdinaryStatus");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(clusterHeadId); // 发送给簇头
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(localClock); // 使用本地时钟
    msg->setMessageType(3); // 态势消息类型

    // 普通节点特有信息
    msg->setPositionX(positionX);
    msg->setPositionY(positionY);
    msg->setPositionZ(positionZ);
    msg->setNodeType(3); // 普通节点类型
    msg->setStatus(status);
    msg->setFrequency(frequency);
    msg->setBatteryLevel(batteryLevel);

    return msg;
}

TimeCalibration* OrdinaryNodeApplication::generateTimeCalibrationMessage()
{
    TimeCalibration *msg = new TimeCalibration("TimeCalibration");

    // 基本信息
    msg->setSrcId(nodeId);
    msg->setDestId(clusterHeadId); // 发送给簇头
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages);
    msg->setTimestamp(localClock); // 使用本地时钟
    msg->setMessageType(2); // 校时消息类型

    // 校时特有信息
    msg->setSendTime(localClock);
    msg->setReceiveTime(0.0); // 接收端填写
    msg->setAccuracy(0.000001); // 精度1微秒
    msg->setCalibrationType(1); // 请求校准

    return msg;
}

void OrdinaryNodeApplication::sendToComm(cMessage *msg)
{
    // 发送消息到通信模块
    send(msg, "toComm");
    sentMessages++;
}

void OrdinaryNodeApplication::updateBatteryStatus(double consumption)
{
    // 更新电池电量
    batteryLevel -= consumption;

    // 检查电池状态
    if (batteryLevel <= 0) {
        batteryLevel = 0;
        status = 3; // 故障状态
    } else if (batteryLevel < 200) {
        status = 2; // 低电量状态
    }
}

void OrdinaryNodeApplication::updatePosition()
{
    // 获取最新位置
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");

    if (mobilityModule->isSimple()) {
        // 尝试直接获取位置信息
        if (mobilityModule->hasMethod("getX")) {
            cMethodCallContext ctx{mobilityModule->getClassName(), "getX", "", 0};
            positionX = mobilityModule->callMethod(&ctx, "getX").doubleValue();
            positionY = mobilityModule->callMethod(&ctx, "getY").doubleValue();
            positionZ = mobilityModule->callMethod(&ctx, "getZ").doubleValue();
        } else {
            // 尝试获取参数
            positionX = mobilityModule->par("initialX");
            positionY = mobilityModule->par("initialY");
            positionZ = mobilityModule->par("initialZ");
        }
    }
}

void OrdinaryNodeApplication::updateClock()
{
    // 更新本地时钟，考虑漂移
    double realTime = simTime().dbl();
    double elapsedTime = realTime - lastSyncTime;

    // 应用时钟漂移
    localClock += elapsedTime * clockDrift;
    lastSyncTime = realTime;
}
