// ControlNodeApplication.h
#ifndef __WSN_SIMULATION_CONTROL_NODE_APPLICATION_H
#define __WSN_SIMULATION_CONTROL_NODE_APPLICATION_H

#include <omnetpp.h>
#include "NetworkManager.h"
#include "messages/WSNMessages_m.h"
#include <map>
#include <vector>

using namespace omnetpp;

namespace wsn_simulation {

class ControlNodeApplication : public cSimpleModule
{
private:
    // 网络管理器
    NetworkManager *networkManager;

    // 参数
    int nodeId;
    double clusterHeadCheckInterval;
    double networkStatusLogInterval;
    double waveformConfigInterval;
    double managementCommandInterval;

    // 定时器
    cMessage *statusTimer;
    cMessage *clusterHeadCheckTimer;
    cMessage *networkStatusTimer;
    cMessage *waveformConfigTimer;
    cMessage *managementCommandTimer;

    // 统计
    int sentMessages;
    int receivedMessages;
    std::map<int, int> receivedStatusByCluster;
    std::map<int, std::vector<double>> clusterDelays;

    // 性能监控
    std::map<int, double> nodePerformance;
    std::map<int, double> lastPacketTimes;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 处理各类消息
    void handleStatusMessage(StatusMessage *msg);
    void handleStatusAggregationMessage(StatusAggregation *msg);
    void handleManagementCommandResponse(ManagementCommand *msg);
    void handleAddressingMessage(cMessage *msg);

    // 定时器处理
    void handleStatusTimer();
    void handleClusterHeadCheckTimer();
    void handleNetworkStatusTimer();
    void handleWaveformConfigTimer();
    void handleManagementCommandTimer();

    // 簇头管理
    void checkClusterHeads();
    void rotateClusterHead(int clusterId, int newHeadId);

    // 辅助函数
    void sendToComm(cMessage *msg);
    void updateNodePerformance(int nodeId, double delay, bool success);
    void sendNetworkCommand(int commandType, int targetId, const std::string& data);
    void distributeWaveformConfig();

    // 消息生成
    StatusMessage* generateStatusMessage();
    ManagementCommand* generateManagementCommand(int commandType, int targetId, const std::string& data);
    WaveformConfig* generateWaveformConfigMessage(int targetId, int waveformType, double value);

public:
    ControlNodeApplication();
    virtual ~ControlNodeApplication();
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_CONTROL_NODE_APPLICATION_H
