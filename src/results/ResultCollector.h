// ResultCollector.h
#ifndef __WSN_SIMULATION_RESULT_COLLECTOR_H
#define __WSN_SIMULATION_RESULT_COLLECTOR_H

#include <omnetpp.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace omnetpp;

namespace wsn_simulation {

/**
 * 仿真结果收集器类，用于收集、处理和保存各种仿真指标
 */
class ResultCollector
{
private:
    // 静态实例 (单例模式)
    static ResultCollector* instance;

    // 文件输出路径
    std::string resultPath;

    // 节点状态记录
    struct NodeStateRecord {
        simtime_t time;
        int nodeId;
        std::string nodeType;
        int status;
        double batteryLevel;
        double positionX;
        double positionY;
        double positionZ;
    };
    std::vector<NodeStateRecord> nodeStateRecords;

    // 簇头变更记录
    struct ClusterHeadChangeRecord {
        simtime_t time;
        int clusterId;
        int oldHeadId;
        int newHeadId;
        std::string reason;
    };
    std::vector<ClusterHeadChangeRecord> clusterHeadChanges;

    // 通信性能记录
    struct CommunicationRecord {
        simtime_t time;
        int sourceId;
        int destinationId;
        double delay;
        bool success;
        double signalStrength;
        double packetLossRate;
    };
    std::vector<CommunicationRecord> communicationRecords;

    // 能耗记录
    struct EnergyRecord {
        simtime_t time;
        int nodeId;
        double remainingEnergy;
        double consumptionRate;
    };
    std::vector<EnergyRecord> energyRecords;

    // 移动轨迹记录
    struct MobilityRecord {
        simtime_t time;
        int nodeId;
        double x;
        double y;
        double z;
        double speed;
        int state;
    };
    std::map<int, std::vector<MobilityRecord>> mobilityRecords;

    // 簇统计信息
    struct ClusterStatistics {
        int clusterId;
        int memberCount;
        int activeMembers;
        double avgBatteryLevel;
        double avgDelay;
        double avgLossRate;
        std::vector<int> memberIds;
    };
    std::map<int, ClusterStatistics> clusterStats;

    // 输出文件流
    std::ofstream nodeStateFile;
    std::ofstream clusterHeadChangeFile;
    std::ofstream communicationFile;
    std::ofstream energyFile;

    // 私有构造函数和析构函数 (单例模式)
    ResultCollector();
    virtual ~ResultCollector();

public:
    // 获取单例实例
    static ResultCollector* getInstance();

    // 初始化
    void initialize(const std::string& path);

    // 添加记录
    void recordNodeState(simtime_t time, int nodeId, const std::string& nodeType, int status,
                        double batteryLevel, double positionX, double positionY, double positionZ);

    void recordClusterHeadChange(simtime_t time, int clusterId, int oldHeadId, int newHeadId,
                                const std::string& reason);

    void recordCommunication(simtime_t time, int sourceId, int destinationId, double delay,
                            bool success, double signalStrength, double packetLossRate);

    void recordEnergy(simtime_t time, int nodeId, double remainingEnergy, double consumptionRate);

    void recordMobility(simtime_t time, int nodeId, double x, double y, double z, double speed, int state);

    void updateClusterStatistics(int clusterId, int memberCount, int activeMembers,
                                double avgBatteryLevel, double avgDelay, double avgLossRate,
                                const std::vector<int>& memberIds);

    // 保存和处理结果
    void saveResults();

    void generateTrajectoryFiles();

    void generateEnergyReport();

    void generateCommunicationReport();

    void generateClusterReport();

    void generateNetworkGraph(simtime_t time);

    // 分析功能
    double getAverageNodeLifetime(const std::string& nodeType);

    double getAveragePacketDelay();

    double getAveragePacketLossRate();

    int getClusterHeadChangeCount(int clusterId);

    double getAverageInterNodeDistance(simtime_t time);
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_RESULT_COLLECTOR_H
