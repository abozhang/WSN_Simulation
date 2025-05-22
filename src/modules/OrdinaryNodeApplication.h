// OrdinaryNodeApplication.h
#ifndef __WSN_SIMULATION_ORDINARY_NODE_APPLICATION_H
#define __WSN_SIMULATION_ORDINARY_NODE_APPLICATION_H

#include <omnetpp.h>
#include "messages/WSNMessages_m.h"

using namespace omnetpp;

namespace wsn_simulation {

class OrdinaryNodeApplication : public cSimpleModule
{
private:
    // 参数
    int nodeId;
    int clusterId;
    int frequency;

    // 状态
    double currentTime;
    double positionX;
    double positionY;
    double positionZ;
    double batteryLevel;
    int status;  // 0:初始化, 1:运行, 2:低电量, 3:故障

    // 同步时钟
    double localClock;
    double clockDrift;
    double lastSyncTime;

    // 簇头管理
    int clusterHeadId;
    bool becameClusterHead;

    // 定时器
    cMessage *statusTimer;
    cMessage *syncTimer;

    // 统计
    int sentMessages;
    int receivedMessages;
    double avgDelayToClusterHead;
    int delayMeasurements;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 处理各类消息
    void handleStatusMessage(StatusMessage *msg);
    void handleTimeMessage(TimeMessage *msg);
    void handleTimeCalibrationMessage(TimeCalibration *msg);
    void handleManagementCommand(ManagementCommand *msg);
    void handleAddressingMessage(cMessage *msg);
    void handleWaveformConfigMessage(WaveformConfig *msg);

    // 定时器处理
    void handleStatusTimer();
    void handleSyncTimer();

    // 管理命令处理
    void handleClusterHeadChange(ManagementCommand *msg);

    // 簇头相关
    void becomeClusterHead();

    // 生成消息
    StatusMessage* generateStatusMessage();
    TimeCalibration* generateTimeCalibrationMessage();

    // 辅助函数
    void sendToComm(cMessage *msg);
    void updateBatteryStatus(double consumption);
    void updatePosition();
    void updateClock();

public:
    OrdinaryNodeApplication();
    virtual ~OrdinaryNodeApplication();
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_ORDINARY_NODE_APPLICATION_H
