// ClusterHeadApplication.h
#ifndef __WSN_SIMULATION_CLUSTER_HEAD_APPLICATION_H
#define __WSN_SIMULATION_CLUSTER_HEAD_APPLICATION_H

#include <omnetpp.h>
#include "messages/WSNMessages_m.h"
#include <map>
#include <vector>

using namespace omnetpp;

namespace wsn_simulation {

class ClusterHeadApplication : public cSimpleModule
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

    // 簇管理
    std::map<int, NodeInfo> clusterMembers; // 簇内成员信息
    std::vector<int> ordinaryNodeIds;       // 簇内普通节点ID列表

    // 簇头角色是否被取代
    bool roleReplaced;

    // 定时器
    cMessage *statusTimer;
    cMessage *aggregationTimer;
    cMessage *memberCheckTimer;

    // 统计
    int sentMessages;
    int receivedMessages;
    int aggregatedStatusCount;

    // 簇内性能
    std::map<int, double> nodeBatteryLevels;
    std::map<int, double> nodeDelays;
    std::map<int, int> nodePacketCounts;
    std::map<int, double> nodeLastUpdateTimes;

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
    void handleAggregationTimer();
    void handleMemberCheckTimer();

    // 簇管理
    void aggregateClusterStatus();
    void checkClusterMembers();
    bool isNodeActive(int nodeId);

    // 管理命令处理
    void handleClusterHeadChange(ManagementCommand *msg);

    // 生成消息
    StatusMessage* generateStatusMessage();
    StatusAggregation* generateStatusAggregationMessage();

    // 辅助函数
    void sendToComm(cMessage *msg);
    void updateBatteryStatus(double consumption);
    void updatePosition();
    double calculateAverageDelay(int nodeId);
    void becomeOrdinaryNode();

    // 簇内成员管理
    struct NodeInfo {
        int nodeId;
        double batteryLevel;
        double positionX;
        double positionY;
        double positionZ;
        int status;
        double lastUpdateTime;
        std::vector<double> delays;
        int receivedPackets;
        int lostPackets;

        // 计算丢包率
        double getPacketLossRate() const {
            if (receivedPackets + lostPackets == 0) return 0.0;
            return static_cast<double>(lostPackets) / (receivedPackets + lostPackets);
        }

        // 计算平均延迟
        double getAverageDelay() const {
            if (delays.empty()) return 0.0;
            double sum = 0.0;
            for (double d : delays) sum += d;
            return sum / delays.size();
        }
    };

public:
    ClusterHeadApplication();
    virtual ~ClusterHeadApplication();
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_CLUSTER_HEAD_APPLICATION_H
