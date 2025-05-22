// NetworkManager.h
#ifndef __WSN_SIMULATION_NETWORK_MANAGER_H
#define __WSN_SIMULATION_NETWORK_MANAGER_H

#include <omnetpp.h>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include "protocols/WSNMessages_m.h"

using namespace omnetpp;

namespace wsn_simulation {

// 网络节点信息结构
struct NodeInfo {
    int nodeId;                  // 节点ID
    std::string nodeType;        // 节点类型: "control", "clusterHead", "ordinary"
    int clusterId;               // 所属簇ID
    int status;                  // 节点状态: 0-初始化, 1-运行, 2-低电量, 3-故障, 4-离线
    double batteryLevel;         // 电池电量
    double positionX;            // X坐标
    double positionY;            // Y坐标
    double positionZ;            // Z坐标
    int frequency;               // 使用的频段
    double lastUpdateTime;       // 最后更新时间

    // 性能指标
    double packetLossRate;       // 丢包率
    double averageDelay;         // 平均延迟
    double signalStrength;       // 信号强度

    // 用于簇头评分的综合指标
    double clusterHeadScore;     // 综合得分

    // 统计数据
    int receivedPackets;         // 接收的数据包
    int sentPackets;             // 发送的数据包
    std::vector<double> delays;  // 记录延迟数据

    // 构造函数
    NodeInfo() :
        nodeId(-1),
        nodeType("unknown"),
        clusterId(-1),
        status(0),
        batteryLevel(0),
        positionX(0),
        positionY(0),
        positionZ(0),
        frequency(-1),
        lastUpdateTime(0),
        packetLossRate(0),
        averageDelay(0),
        signalStrength(0),
        clusterHeadScore(0),
        receivedPackets(0),
        sentPackets(0) {}
};

// 簇信息结构
struct ClusterInfo {
    int clusterId;                       // 簇ID
    int clusterHeadId;                   // 当前簇头ID
    std::vector<int> memberIds;          // 成员节点ID列表
    int frequency;                       // 簇使用的频段

    // 簇性能指标
    double avgBatteryLevel;              // 平均电池电量
    double avgPacketLossRate;            // 平均丢包率
    double avgDelay;                     // 平均延迟

    // 构造函数
    ClusterInfo() :
        clusterId(-1),
        clusterHeadId(-1),
        frequency(-1),
        avgBatteryLevel(0),
        avgPacketLossRate(0),
        avgDelay(0) {}
};

// 链路信息结构
struct LinkInfo {
    int sourceId;               // 源节点ID
    int destinationId;          // 目标节点ID
    double quality;             // 链路质量 (0-1)
    double delay;               // 链路延迟
    double lastUpdateTime;      // 最后更新时间

    // 构造函数
    LinkInfo() :
        sourceId(-1),
        destinationId(-1),
        quality(0),
        delay(0),
        lastUpdateTime(0) {}
};

// 网络管理类
class NetworkManager {
private:
    // 网络拓扑信息
    std::map<int, NodeInfo> nodes;              // 节点信息映射表 (nodeId -> NodeInfo)
    std::map<int, ClusterInfo> clusters;        // 簇信息映射表 (clusterId -> ClusterInfo)
    std::map<std::pair<int, int>, LinkInfo> links;  // 链路信息映射表 ((sourceId, destId) -> LinkInfo)

    // 簇头候选评估队列
    std::map<int, std::priority_queue<std::pair<double, int>>> clusterHeadCandidates;  // clusterId -> (score, nodeId)

    // 簇头失联监控
    std::map<int, double> clusterHeadLastUpdate;  // 簇头最后更新时间
    double clusterHeadTimeout;                    // 簇头超时时间阈值

    // 簇头轮换策略参数
    double batteryWeightFactor;     // 电池电量权重
    double lossRateWeightFactor;    // 丢包率权重
    double delayWeightFactor;       // 延迟权重
    double signalWeightFactor;      // 信号强度权重

    // 簇头选择阈值
    double minBatteryForClusterHead;  // 最低电池电量要求
    double maxLossRateAllowed;       // 最高允许丢包率
    double maxDelayAllowed;          // 最高允许延迟

    // 日志记录
    cSimpleModule* ownerModule;

public:
    // 构造函数
    NetworkManager(cSimpleModule* owner);

    // 初始化网络
    void initializeNetwork(int numClusters, int nodesPerCluster);

    // 节点管理
    void addNode(int nodeId, const std::string& nodeType, int clusterId, int frequency);
    void updateNodeStatus(int nodeId, int status);
    void updateNodeBattery(int nodeId, double batteryLevel);
    void updateNodePosition(int nodeId, double x, double y, double z);
    void updateNodePerformance(int nodeId, double lossRate, double delay, double signalStrength);

    // 簇管理
    void createCluster(int clusterId, int frequency);
    void addNodeToCluster(int nodeId, int clusterId);
    void removeNodeFromCluster(int nodeId, int clusterId);
    void setClusterHead(int clusterId, int nodeId);

    // 链路管理
    void updateLink(int sourceId, int destId, double quality, double delay);
    double getLinkQuality(int sourceId, int destId);

    // 簇头轮换策略
    void evaluateClusterHeadCandidates(int clusterId);
    int selectNewClusterHead(int clusterId);
    void rotateClusterHead(int clusterId, bool force = false);
    bool checkClusterHeadStatus();

    // 生成管理命令
    ManagementCommand* generateClusterHeadChangeCommand(int clusterId, int newHeadId);

    // 辅助函数
    double calculateClusterHeadScore(const NodeInfo& node);
    void updateClusterStatistics(int clusterId);
    void logNetworkStatus();
    bool isNodeSuitableForClusterHead(const NodeInfo& node);

    // 获取信息
    const NodeInfo& getNodeInfo(int nodeId) const;
    const ClusterInfo& getClusterInfo(int clusterId) const;
    std::vector<int> getNodesInCluster(int clusterId) const;
    int getCurrentClusterHead(int clusterId) const;

    // 更新节点信息
    void processStatusMessage(StatusMessage* msg);
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_NETWORK_MANAGER_H
