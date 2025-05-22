// Application.cc
#include <omnetpp.h>
#include "messages/WSNMessages_m.h"

using namespace omnetpp;

namespace wsn_simulation {

class Application : public cSimpleModule
{
private:
    // 参数
    std::string nodeType;
    int nodeId;
    int clusterId;

    // 状态
    double currentTime;
    double positionX;
    double positionY;
    double positionZ;
    int frequency;
    double batteryLevel;
    int status;  // 0:初始化, 1:运行, 2:低电量, 3:故障

    // 统计
    int sentMessages;
    int receivedMessages;

    // 时钟相关
    double localClock; // 本地时钟值
    double clockDrift; // 时钟漂移
    double lastSyncTime; // 上次同步时间

    // 定时器
    cMessage *statusTimer; // 态势消息定时器
    cMessage *syncTimer;   // 同步定时器

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 生成各种类型的消息
    TimeMessage* generateTimeMessage();
    TimeCalibration* generateCalibrationMessage();
    StatusMessage* generateStatusMessage();
    StatusAggregation* generateStatusAggregationMessage();

    // 处理各种类型的消息
    void handleTimeMessage(TimeMessage *msg);
    void handleCalibrationMessage(TimeCalibration *msg);
    void handleStatusMessage(StatusMessage *msg);
    void handleStatusAggregationMessage(StatusAggregation *msg);
    void handleManagementCommand(ManagementCommand *msg);
    void handleAddressingMessage(cMessage *msg);
    void handleWaveformConfigMessage(WaveformConfig *msg);

    // 定时器处理
    void handleStatusTimer();
    void handleSyncTimer();

    // 辅助函数
    void sendToComm(cMessage *msg);
    void updateBatteryStatus(double consumption);
    void updateClock();
    void calculatePosition();
};

Define_Module(Application);

void Application::initialize()
{
    // 获取参数
    nodeType = par("nodeType").stringValue();
    nodeId = par("nodeId");

    // 初始化状态
    currentTime = 0.0;
    sentMessages = 0;
    receivedMessages = 0;

    // 根据节点类型确定簇ID和频率
    if (nodeType == "control") {
        clusterId = 0;
        frequency = -1; // 控制节点支持所有频段
        status = 1; // 运行状态
    } else if (nodeType == "clusterHead") {
        // 簇头ID为1-5，对应频段也为1-5
        clusterId = nodeId % 5 + 1;
        frequency = clusterId;
        status = 1; // 运行状态
    } else { // ordinary
        // 普通节点根据ID分配到对应的簇
        clusterId = (nodeId / 200) % 5 + 1;
        frequency = clusterId;
        status = 1; // 运行状态
    }

    // 获取位置信息
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");
    positionX = mobilityModule->par("initialX");
    positionY = mobilityModule->par("initialY");
    positionZ = mobilityModule->par("initialZ");

    // 获取电池信息
    cModule *batteryModule = getParentModule()->getSubmodule("battery");
    batteryLevel = batteryModule->par("initialCapacity").doubleValue();

    // 初始化时钟
    localClock = simTime().dbl();
    clockDrift = uniform(0.99, 1.01); // 1% 漂移
    lastSyncTime = simTime().dbl();

    // 设置定时器
    double statusInterval = 1.0; // 1秒发送一次态势消息
    double syncInterval = 60.0;  // 60秒同步一次时钟

    statusTimer = new cMessage("statusTimer");
    scheduleAt(simTime() + statusInterval, statusTimer);

    syncTimer = new cMessage("syncTimer");
    if (nodeType != "control") { // 非控制节点需要同步
        scheduleAt(simTime() + syncInterval, syncTimer);
    }

    EV << "Node " << nodeId << " (" << nodeType << ") initialized with frequency " << frequency << " in cluster " << clusterId << endl;
}

void Application::handleMessage(cMessage *msg)
{
    // 更新本地时钟
    updateClock();

    // 处理定时器消息
    if (msg == statusTimer) {
        handleStatusTimer();
        return;
    } else if (msg == syncTimer) {
        handleSyncTimer();
        return;
    }

    // 处理接收到的消息
    receivedMessages++;

    // 检查消息类型并分发到相应的处理函数
    if (dynamic_cast<TimeMessage *>(msg)) {
        handleTimeMessage(check_and_cast<TimeMessage *>(msg));
    } else if (dynamic_cast<TimeCalibration *>(msg)) {
        handleCalibrationMessage(check_and_cast<TimeCalibration *>(msg));
    } else if (dynamic_cast<StatusMessage *>(msg)) {
        handleStatusMessage(check_and_cast<StatusMessage *>(msg));
    } else if (dynamic_cast<StatusAggregation *>(msg)) {
        handleStatusAggregationMessage(check_and_cast<StatusAggregation *>(msg));
    } else if (dynamic_cast<ManagementCommand *>(msg)) {
        handleManagementCommand(check_and_cast<ManagementCommand *>(msg));
    } else if (dynamic_cast<ClusterAddressing *>(msg) || dynamic_cast<InterClusterAddressing *>(msg)) {
        handleAddressingMessage(msg);
    } else if (dynamic_cast<WaveformConfig *>(msg)) {
        handleWaveformConfigMessage(check_and_cast<WaveformConfig *>(msg));
    } else {
        EV << "Received unknown message type" << endl;
    }

    // 释放消息
    delete msg;
}

void Application::finish()
{
    // 清理定时器
    cancelAndDelete(statusTimer);
    cancelAndDelete(syncTimer);

    // 输出统计信息
    recordScalar("sentMessages", sentMessages);
    recordScalar("receivedMessages", receivedMessages);

    EV << "Node " << nodeId << " (" << nodeType << ") finished with stats:" << endl;
    EV << "  Sent messages: " << sentMessages << endl;
    EV << "  Received messages: " << receivedMessages << endl;
}

TimeMessage* Application::generateTimeMessage()
{
    TimeMessage *msg = new TimeMessage("TimeSync");
    msg->setSrcId(nodeId);
    msg->setDestId(-1); // 广播给所有节点
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages++);
    msg->setTimestamp(localClock);
    msg->setMessageType(1); // 时间消息类型

    msg->setReferenceTime(localClock);
    msg->setAccuracy(0.001); // 1ms精度
    msg->setIdentityInfo(nodeType.c_str());

    return msg;
}

TimeCalibration* Application::generateCalibrationMessage()
{
    TimeCalibration *msg = new TimeCalibration("TimeCalibration");
    msg->setSrcId(nodeId);
    msg->setDestId(-1); // 广播给所有节点
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages++);
    msg->setTimestamp(localClock);
    msg->setMessageType(2); // 校时消息类型

    msg->setSendTime(localClock);
    msg->setReceiveTime(0.0); // 接收端会填写
    msg->setAccuracy(0.000001); // 1µs精度
    msg->setCalibrationType(1); // 标准校准

    return msg;
}

StatusMessage* Application::generateStatusMessage()
{
    StatusMessage *msg = new StatusMessage("StatusMessage");
    msg->setSrcId(nodeId);

    // 根据节点类型设置目标ID
    if (nodeType == "ordinary") {
        // 普通节点发送给其簇头
        msg->setDestId(clusterId); // 簇头ID等于簇ID
    } else if (nodeType == "clusterHead") {
        // 簇头发送给控制节点
        msg->setDestId(0); // 控制节点ID为0
    } else {
        // 控制节点广播状态
        msg->setDestId(-1);
    }

    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages++);
    msg->setTimestamp(localClock);
    msg->setMessageType(3); // 态势消息类型

    msg->setPositionX(positionX);
    msg->setPositionY(positionY);
    msg->setPositionZ(positionZ);

    int nodeTypeCode;
    if (nodeType == "control") nodeTypeCode = 1;
    else if (nodeType == "clusterHead") nodeTypeCode = 2;
    else nodeTypeCode = 3; // ordinary

    msg->setNodeType(nodeTypeCode);
    msg->setStatus(status);
    msg->setFrequency(frequency);
    msg->setBatteryLevel(batteryLevel);

    return msg;
}

StatusAggregation* Application::generateStatusAggregationMessage()
{
    StatusAggregation *msg = new StatusAggregation("StatusAggregation");
    msg->setSrcId(nodeId);
    msg->setDestId(0); // 发送给控制节点
    msg->setFrequency(frequency);
    msg->setSequenceNumber(sentMessages++);
    msg->setTimestamp(localClock);
    msg->setMessageType(4); // 态势汇聚消息类型

    // 假设每个簇头管理200个节点，这里我们简化处理
    msg->setNodeCount(200);
    msg->setActiveNodes(180 + intuniform(-10, 10)); // 模拟有部分节点不活跃

    std::string usage = "CPU:30%,MEM:45%,BW:20%";
    msg->setResourceUsage(usage.c_str());

    return msg;
}

void Application::handleTimeMessage(TimeMessage *msg)
{
    EV << "Node " << nodeId << " received time message from " << msg->getSrcId() << endl;

    // 只有非控制节点需要处理时间同步消息
    if (nodeType != "control") {
        // 更新本地时钟
        double timeDiff = msg->getReferenceTime() - localClock;
        EV << "Time difference: " << timeDiff << " seconds" << endl;

        // 如果时间差超过阈值，则同步时钟
        if (fabs(timeDiff) > 0.1) { // 100ms阈值
            localClock = msg->getReferenceTime();
            lastSyncTime = simTime().dbl();
            EV << "Clock synchronized to: " << localClock << endl;
        }
    }
}

void Application::handleCalibrationMessage(TimeCalibration *msg)
{
    EV << "Node " << nodeId << " received calibration message from " << msg->getSrcId() << endl;

    // 控制节点和簇头节点响应校时请求
    if (nodeType == "control" || nodeType == "clusterHead") {
        // 创建响应消息
        TimeCalibration *response = new TimeCalibration("CalibrationResponse");
        response->setSrcId(nodeId);
        response->setDestId(msg->getSrcId());
        response->setFrequency(msg->getFrequency());
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(localClock);
        response->setMessageType(2); // 校时消息类型

        response->setSendTime(msg->getSendTime());
        response->setReceiveTime(localClock);
        response->setAccuracy(0.000001); // 1µs精度
        response->setCalibrationType(2); // 响应校准

        // 发送响应
        sendToComm(response);
    }
    // 普通节点接收校时响应
    else if (nodeType == "ordinary" && msg->getCalibrationType() == 2) {
        // 计算往返时间和时钟偏差
        double roundTripTime = localClock - msg->getSendTime();
        double clockOffset = (msg->getReceiveTime() - msg->getSendTime() - roundTripTime/2);

        EV << "Round trip time: " << roundTripTime << " seconds" << endl;
        EV << "Clock offset: " << clockOffset << " seconds" << endl;

        // 调整本地时钟
        localClock -= clockOffset;
        lastSyncTime = simTime().dbl();

        EV << "Clock calibrated to: " << localClock << endl;
    }
}

void Application::handleStatusMessage(StatusMessage *msg)
{
    EV << "Node " << nodeId << " received status message from " << msg->getSrcId() << endl;

    // 簇头收集簇内普通节点的状态
    if (nodeType == "clusterHead" && msg->getNodeType() == 3) {
        // 这里可以实现状态聚合逻辑
        // 在实际应用中，簇头会收集多个普通节点的状态，然后汇总
        // 简化处理，每接收10个状态消息，生成一个汇聚消息
        if (receivedMessages % 10 == 0) {
            StatusAggregation *aggMsg = generateStatusAggregationMessage();
            sendToComm(aggMsg);
        }
    }
    // 控制节点处理簇头的状态
    else if (nodeType == "control" && msg->getNodeType() == 2) {
        // 控制节点处理簇头状态逻辑
        // 简化处理
    }
}

void Application::handleStatusAggregationMessage(StatusAggregation *msg)
{
    EV << "Node " << nodeId << " received status aggregation message from " << msg->getSrcId() << endl;

    // 只有控制节点处理态势汇聚消息
    if (nodeType == "control") {
        // 处理态势汇聚消息的逻辑
        EV << "Node count: " << msg->getNodeCount() << endl;
        EV << "Active nodes: " << msg->getActiveNodes() << endl;
        EV << "Resource usage: " << msg->getResourceUsage() << endl;
    }
}

void Application::handleManagementCommand(ManagementCommand *msg)
{
    EV << "Node " << nodeId << " received management command from " << msg->getSrcId() << endl;

    // 处理网络管理指令
    if (msg->getTargetId() == nodeId || msg->getTargetId() == -1) {
        // 根据指令类型执行相应操作
        switch (msg->getCommandType()) {
            case 1: // 改变频段
                if (nodeType != "control") { // 控制节点不改变频段
                    frequency = std::stoi(msg->getCommandData());
                    EV << "Changed frequency to: " << frequency << endl;
                }
                break;
            case 2: // 改变状态
                status = std::stoi(msg->getCommandData());
                EV << "Changed status to: " << status << endl;
                break;
            case 3: // 其他命令
                EV << "Executing command: " << msg->getCommandData() << endl;
                break;
        }

        // 如果需要响应，则发送响应消息
        if (msg->getResponseFlag() == 1) {
            ManagementCommand *response = new ManagementCommand("CommandResponse");
            response->setSrcId(nodeId);
            response->setDestId(msg->getSrcId());
            response->setFrequency(frequency);
            response->setSequenceNumber(sentMessages++);
            response->setTimestamp(localClock);
            response->setMessageType(5); // 管理响应类型

            response->setTargetId(msg->getTargetId());
            response->setCommandType(msg->getCommandType());
            response->setExtensionParam(0.0);

            std::string responseData = "ACK";
            response->setCommandData(responseData.c_str());
            response->setResponseFlag(0); // 不需要再次响应

            sendToComm(response);
        }
    }
}

void Application::handleAddressingMessage(cMessage *msg)
{
    // 处理簇内寻址消息
    if (ClusterAddressing *clusterMsg = dynamic_cast<ClusterAddressing *>(msg)) {
        EV << "Node " << nodeId << " received cluster addressing message from " << clusterMsg->getSrcId() << endl;

        // 检查是否是目标节点
        if (clusterMsg->getTargetIdentity() == std::to_string(nodeId) ||
            (nodeType == "clusterHead" && std::stoi(clusterMsg->getTargetIdentity()) / 200 + 1 == clusterId)) {

            // 创建响应消息
            ClusterAddressing *response = new ClusterAddressing("AddressingResponse");
            response->setSrcId(nodeId);
            response->setDestId(clusterMsg->getSrcId());
            response->setFrequency(frequency);
            response->setSequenceNumber(sentMessages++);
            response->setTimestamp(localClock);
            response->setMessageType(6); // 寻址响应类型

            response->setSourceIdentity(std::to_string(nodeId).c_str());
            response->setTargetIdentity(clusterMsg->getSourceIdentity());
            response->setAddressingQuery("RESPONSE");

            sendToComm(response);
        }
    }
    // 处理簇间寻址消息
    else if (InterClusterAddressing *interMsg = dynamic_cast<InterClusterAddressing *>(msg)) {
        EV << "Node " << nodeId << " received inter-cluster addressing message from " << interMsg->getSrcId() << endl;

        // 只有簇头处理簇间寻址
        if (nodeType == "clusterHead" && interMsg->getTargetCluster() == std::to_string(clusterId)) {
            // 创建响应消息
            InterClusterAddressing *response = new InterClusterAddressing("InterAddressingResponse");
            response->setSrcId(nodeId);
            response->setDestId(interMsg->getSrcId());
            response->setFrequency(frequency);
            response->setSequenceNumber(sentMessages++);
            response->setTimestamp(localClock);
            response->setMessageType(7); // 簇间寻址响应类型

            response->setSourceCluster(std::to_string(clusterId).c_str());
            response->setTargetCluster(interMsg->getSourceCluster());
            response->setAddressingQuery("RESPONSE");

            sendToComm(response);
        }
    }
}

void Application::handleWaveformConfigMessage(WaveformConfig *msg)
{
    EV << "Node " << nodeId << " received waveform config message from " << msg->getSrcId() << endl;

    // 处理波形参数设置消息
    if (msg->getWaveformType() == 1) { // 速率设置
        // 在实际应用中，这里会调整无线电参数
        EV << "Setting transmission rate to: " << msg->getParameterValue() << endl;
    }

    // 发送响应
    if (msg->getConfigResponse() == 1) {
        WaveformConfig *response = new WaveformConfig("WaveformConfigResponse");
        response->setSrcId(nodeId);
        response->setDestId(msg->getSrcId());
        response->setFrequency(frequency);
        response->setSequenceNumber(sentMessages++);
        response->setTimestamp(localClock);
        response->setMessageType(8); // 波形参数响应类型

        response->setWaveformType(msg->getWaveformType());
        response->setParameterValue(msg->getParameterValue());
        response->setConfigResponse(0); // 不需要响应

        sendToComm(response);
    }
}

void Application::handleStatusTimer()
{
    // 生成并发送态势消息
    StatusMessage *statusMsg = generateStatusMessage();
    sendToComm(statusMsg);

    // 如果是簇头，并且是每10个周期，则发送态势汇聚消息
    if (nodeType == "clusterHead" && simTime().inUnit(SIMTIME_S) % 10 == 0) {
        StatusAggregation *aggMsg = generateStatusAggregationMessage();
        sendToComm(aggMsg);
    }

    // 如果是控制节点，并且是每60个周期，则发送时间同步消息
    if (nodeType == "control" && simTime().inUnit(SIMTIME_S) % 60 == 0) {
        TimeMessage *timeMsg = generateTimeMessage();
        sendToComm(timeMsg);
    }

    // 重新调度定时器
    scheduleAt(simTime() + 1.0, statusTimer);
}

void Application::handleSyncTimer()
{
    // 非控制节点发送校时请求
    if (nodeType != "control") {
        TimeCalibration *calibMsg = generateCalibrationMessage();
        sendToComm(calibMsg);
    }

    // 重新调度定时器
    scheduleAt(simTime() + 60.0, syncTimer);
}

void Application::sendToComm(cMessage *msg)
{
    // 更新能耗
    updateBatteryStatus(0.1); // 假设每次发送消息消耗0.1单位电量

    // 发送消息到通信模块
    send(msg, "toComm");
    sentMessages++;
}

void Application::updateBatteryStatus(double consumption)
{
    // 更新电池电量
    batteryLevel -= consumption;

    // 检查电池状态
    if (batteryLevel < 0) {
        batteryLevel = 0;
        status = 3; // 故障状态
    } else if (batteryLevel < 200) {
        status = 2; // 低电量状态
    }

    // 向电池模块报告能耗
    cPacket *batteryMsg = new cPacket("energy");
    batteryMsg->setByteLength(sizeof(double));
    batteryMsg->setContextPointer(new double(consumption));
    send(batteryMsg, "toBattery");
}

void Application::updateClock()
{
    // 更新本地时钟，考虑漂移
    double realTime = simTime().dbl();
    double elapsedTime = realTime - lastSyncTime;

    // 应用时钟漂移
    localClock += elapsedTime * clockDrift;
    lastSyncTime = realTime;
}

void Application::calculatePosition()
{
    // 在实际应用中，这里会根据邻居节点的位置信息计算自己的位置
    // 简化处理，这里我们假设位置是固定的
}

} // namespace wsn_simulation
