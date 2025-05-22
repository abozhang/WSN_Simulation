// TimeSync.cc
#include <omnetpp.h>
#include "messages/TimeMessage_m.h"
#include "messages/TimeCalibration_m.h"

using namespace omnetpp;

namespace wsn_simulation {

class TimeSync : public cSimpleModule
{
private:
    // 参数
    std::string nodeType;
    bool isTimeBase;
    double clockDrift;
    double syncInterval;

    // 状态
    double localClock;
    double lastSyncTime;

    // 统计
    int syncAttempts;
    double maxClockOffset;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 辅助函数
    void updateBatteryStatus(double consumption);
};

Define_Module(TimeSync);

void TimeSync::initialize()
{
    // 获取参数
    nodeType = par("nodeType").stringValue();
    isTimeBase = par("isTimeBase");
    clockDrift = par("clockDrift").doubleValue();
    syncInterval = par("syncInterval").doubleValue();

    // 初始化状态
    localClock = simTime().dbl();
    lastSyncTime = simTime().dbl();

    // 初始化统计
    syncAttempts = 0;
    maxClockOffset = 0.0;

    EV << "TimeSync module initialized with isTimeBase=" << isTimeBase << ", clockDrift=" << clockDrift << endl;
}

void TimeSync::handleMessage(cMessage *msg)
{
    // 来自应用层的消息
    if (TimeMessage *timeMsg = dynamic_cast<TimeMessage *>(msg)) {
        // 处理时间同步消息
        EV << "Received time sync message with reference time " << timeMsg->getReferenceTime() << endl;

        if (!isTimeBase) {
            // 非时间基准节点需要同步时钟
            double clockOffset = timeMsg->getReferenceTime() - localClock;

            // 记录最大时钟偏差
            if (fabs(clockOffset) > maxClockOffset) {
                maxClockOffset = fabs(clockOffset);
            }

            // 调整本地时钟
            localClock = timeMsg->getReferenceTime();
            lastSyncTime = simTime().dbl();

            // 更新统计
            syncAttempts++;

            EV << "Synchronized clock, offset was: " << clockOffset << endl;
        }

        // 将消息转发到应用层
        send(msg, "toApp");
    } else if (TimeCalibration *calibMsg = dynamic_cast<TimeCalibration *>(msg)) {
        // 处理校时消息
        EV << "Received time calibration message" << endl;

        // 将消息转发到应用层
        send(msg, "toApp");
    } else {
        // 未知消息类型，丢弃
        EV << "Received unknown message type" << endl;
        delete msg;
    }

    // 更新能耗
    updateBatteryStatus(0.05); // 假设每次处理同步消息消耗0.05单位电量
}

void TimeSync::finish()
{
    // 记录统计信息
    recordScalar("syncAttempts", syncAttempts);
    recordScalar("maxClockOffset", maxClockOffset);

    EV << "TimeSync module finished with stats:" << endl;
    EV << "  Sync attempts: " << syncAttempts << endl;
    EV << "  Max clock offset: " << maxClockOffset << endl;
}

void TimeSync::updateBatteryStatus(double consumption)
{
    // 向电池模块报告能耗
    cPacket *batteryMsg = new cPacket("energy");
    batteryMsg->setByteLength(sizeof(double));
    batteryMsg->setContextPointer(new double(consumption));
    send(batteryMsg, "toBattery");
}

} // namespace wsn_simulation
