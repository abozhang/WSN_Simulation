// ResultCollector.cc
#include "ResultCollector.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <filesystem>

using namespace wsn_simulation;

// 初始化静态实例
ResultCollector* ResultCollector::instance = nullptr;

ResultCollector::ResultCollector()
    : resultPath("results/")
{
}

ResultCollector::~ResultCollector()
{
    // 关闭所有文件
    if (nodeStateFile.is_open()) nodeStateFile.close();
    if (clusterHeadChangeFile.is_open()) clusterHeadChangeFile.close();
    if (communicationFile.is_open()) communicationFile.close();
    if (energyFile.is_open()) energyFile.close();
}

ResultCollector* ResultCollector::getInstance()
{
    if (instance == nullptr) {
        instance = new ResultCollector();
    }
    return instance;
}

void ResultCollector::initialize(const std::string& path)
{
    resultPath = path;

    // 创建目录（如果不存在）
    std::filesystem::create_directories(resultPath);
    std::filesystem::create_directories(resultPath + "trajectories");
    std::filesystem::create_directories(resultPath + "battery");
    std::filesystem::create_directories(resultPath + "communication");
    std::filesystem::create_directories(resultPath + "clusters");

    // 打开输出文件
    nodeStateFile.open(resultPath + "node_states.csv");
    nodeStateFile << "time,nodeId,nodeType,status,batteryLevel,positionX,positionY,positionZ" << std::endl;

    clusterHeadChangeFile.open(resultPath + "cluster_head_changes.csv");
    clusterHeadChangeFile << "time,clusterId,oldHeadId,newHeadId,reason" << std::endl;

    communicationFile.open(resultPath + "communication.csv");
    communicationFile << "time,sourceId,destinationId,delay,success,signalStrength,packetLossRate" << std::endl;

    energyFile.open(resultPath + "energy.csv");
    energyFile << "time,nodeId,remainingEnergy,consumptionRate" << std::endl;
}

void ResultCollector::recordNodeState(simtime_t time, int nodeId, const std::string& nodeType, int status,
                                    double batteryLevel, double positionX, double positionY, double positionZ)
{
    // 添加到记录
    NodeStateRecord record;
    record.time = time;
    record.nodeId = nodeId;
    record.nodeType = nodeType;
    record.status = status;
    record.batteryLevel = batteryLevel;
    record.positionX = positionX;
    record.positionY = positionY;
    record.positionZ = positionZ;

    nodeStateRecords.push_back(record);

    // 写入文件
    if (nodeStateFile.is_open()) {
        nodeStateFile << time << "," << nodeId << "," << nodeType << "," << status << ","
                    << batteryLevel << "," << positionX << "," << positionY << "," << positionZ << std::endl;
    }
}

void ResultCollector::recordClusterHeadChange(simtime_t time, int clusterId, int oldHeadId, int newHeadId,
                                            const std::string& reason)
{
    // 添加到记录
    ClusterHeadChangeRecord record;
    record.time = time;
    record.clusterId = clusterId;
    record.oldHeadId = oldHeadId;
    record.newHeadId = newHeadId;
    record.reason = reason;

    clusterHeadChanges.push_back(record);

    // 写入文件
    if (clusterHeadChangeFile.is_open()) {
        clusterHeadChangeFile << time << "," << clusterId << "," << oldHeadId << ","
                            << newHeadId << "," << reason << std::endl;
    }
}

void ResultCollector::recordCommunication(simtime_t time, int sourceId, int destinationId, double delay,
                                        bool success, double signalStrength, double packetLossRate)
{
    // 添加到记录
    CommunicationRecord record;
    record.time = time;
    record.sourceId = sourceId;
    record.destinationId = destinationId;
    record.delay = delay;
    record.success = success;
    record.signalStrength = signalStrength;
    record.packetLossRate = packetLossRate;

    communicationRecords.push_back(record);

    // 写入文件
    if (communicationFile.is_open()) {
        communicationFile << time << "," << sourceId << "," << destinationId << ","
                        << delay << "," << (success ? "1" : "0") << ","
                        << signalStrength << "," << packetLossRate << std::endl;
    }
}

void ResultCollector::recordEnergy(simtime_t time, int nodeId, double remainingEnergy, double consumptionRate)
{
    // 添加到记录
    EnergyRecord record;
    record.time = time;
    record.nodeId = nodeId;
    record.remainingEnergy = remainingEnergy;
    record.consumptionRate = consumptionRate;

    energyRecords.push_back(record);

    // 写入文件
    if (energyFile.is_open()) {
        energyFile << time << "," << nodeId << "," << remainingEnergy << "," << consumptionRate << std::endl;
    }
}

void ResultCollector::recordMobility(simtime_t time, int nodeId, double x, double y, double z, double speed, int state)
{
    // 添加到记录
    MobilityRecord record;
    record.time = time;
    record.nodeId = nodeId;
    record.x = x;
    record.y = y;
    record.z = z;
    record.speed = speed;
    record.state = state;

    mobilityRecords[nodeId].push_back(record);
}

void ResultCollector::updateClusterStatistics(int clusterId, int memberCount, int activeMembers,
                                            double avgBatteryLevel, double avgDelay, double avgLossRate,
                                            const std::vector<int>& memberIds)
{
    ClusterStatistics& stats = clusterStats[clusterId];
    stats.clusterId = clusterId;
    stats.memberCount = memberCount;
    stats.activeMembers = activeMembers;
    stats.avgBatteryLevel = avgBatteryLevel;
    stats.avgDelay = avgDelay;
    stats.avgLossRate = avgLossRate;
    stats.memberIds = memberIds;
}

void ResultCollector::saveResults()
{
    // 生成各种报告
    generateTrajectoryFiles();
    generateEnergyReport();
    generateCommunicationReport();
    generateClusterReport();

    // 生成几个时间点的网络图
    generateNetworkGraph(0);                // 起始时刻
    generateNetworkGraph(1800);             // 中间时刻
    generateNetworkGraph(3600);             // 结束时刻
}

void ResultCollector::generateTrajectoryFiles()
{
    for (const auto& pair : mobilityRecords) {
        int nodeId = pair.first;
        const auto& records = pair.second;

        // 跳过没有移动记录的节点
        if (records.empty()) continue;

        // 创建节点轨迹文件
        std::stringstream filename;
        filename << resultPath << "trajectories/trajectory_" << nodeId << ".csv";

        std::ofstream file(filename.str());
        if (file.is_open()) {
            // 写入标题行
            file << "time,x,y,z,speed,state" << std::endl;

            // 写入数据行
            for (const auto& record : records) {
                file << record.time << "," << record.x << "," << record.y << ","
                    << record.z << "," << record.speed << "," << record.state << std::endl;
            }

            file.close();
        }
    }
}

void ResultCollector::generateEnergyReport()
{
    // 按节点ID组织能耗记录
    std::map<int, std::vector<EnergyRecord>> nodeEnergyRecords;
    for (const auto& record : energyRecords) {
        nodeEnergyRecords[record.nodeId].push_back(record);
    }

    // 为每个节点生成能耗报告
    for (const auto& pair : nodeEnergyRecords) {
        int nodeId = pair.first;
        const auto& records = pair.second;

        // 创建节点能耗文件
        std::stringstream filename;
        filename << resultPath << "battery/energy_" << nodeId << ".csv";

        std::ofstream file(filename.str());
        if (file.is_open()) {
            // 写入标题行
            file << "time,remainingEnergy,consumptionRate" << std::endl;

            // 写入数据行
            for (const auto& record : records) {
                file << record.time << "," << record.remainingEnergy << "," << record.consumptionRate << std::endl;
            }

            file.close();
        }
    }

    // 创建节点类型平均能耗报告
    std::map<std::string, std::vector<EnergyRecord>> typeEnergyRecords;
    for (const auto& record : energyRecords) {
        // 查找节点类型
        std::string nodeType = "unknown";
        for (const auto& stateRecord : nodeStateRecords) {
            if (stateRecord.nodeId == record.nodeId) {
                nodeType = stateRecord.nodeType;
                break;
            }
        }

        typeEnergyRecords[nodeType].push_back(record);
    }

    // 为每种节点类型生成平均能耗报告
    for (const auto& pair : typeEnergyRecords) {
        const std::string& nodeType = pair.first;
        const auto& records = pair.second;

        // 创建节点类型能耗文件
        std::stringstream filename;
        filename << resultPath << "battery/energy_" << nodeType << "_avg.csv";

        std::ofstream file(filename.str());
        if (file.is_open()) {
            // 写入标题行
            file << "time,avgRemainingEnergy,avgConsumptionRate" << std::endl;

            // 按时间点计算平均值
            std::map<simtime_t, std::pair<double, double>> timeData;
            for (const auto& record : records) {
                auto& data = timeData[record.time];
                data.first += record.remainingEnergy;
                data.second += record.consumptionRate;
            }

            // 计算每个时间点的平均值并写入
            for (const auto& pair : timeData) {
                simtime_t time = pair.first;
                double avgEnergy = pair.second.first / records.size();
                double avgRate = pair.second.second / records.size();

                file << time << "," << avgEnergy << "," << avgRate << std::endl;
            }

            file.close();
        }
    }
}

void ResultCollector::generateCommunicationReport()
{
    // 创建通信延迟报告
    std::ofstream delayFile(resultPath + "communication/delays.csv");
    if (delayFile.is_open()) {
        // 写入标题行
        delayFile << "time,sourceType,destinationType,avgDelay" << std::endl;

        // 按时间点和节点类型对组织数据
        std::map<std::tuple<simtime_t, std::string, std::string>, std::vector<double>> typeDelays;

        for (const auto& record : communicationRecords) {
            if (!record.success) continue; // 跳过失败的通信

            // 查找源节点和目标节点的类型
            std::string sourceType = "unknown";
            std::string destType = "unknown";

            for (const auto& stateRecord : nodeStateRecords) {
                if (stateRecord.nodeId == record.sourceId) {
                    sourceType = stateRecord.nodeType;
                }
                if (stateRecord.nodeId == record.destinationId) {
                    destType = stateRecord.nodeType;
                }

                if (sourceType != "unknown" && destType != "unknown") {
                    break;
                }
            }

            // 将延迟添加到对应类型组合
            auto key = std::make_tuple(record.time, sourceType, destType);
            typeDelays[key].push_back(record.delay);
        }

        // 计算每个组合的平均延迟并写入
        for (const auto& pair : typeDelays) {
            simtime_t time = std::get<0>(pair.first);
            std::string sourceType = std::get<1>(pair.first);
            std::string destType = std::get<2>(pair.first);

            const auto& delays = pair.second;
            double avgDelay = 0.0;
            if (!delays.empty()) {
                double sum = 0.0;
                for (double delay : delays) {
                    sum += delay;
                }
                avgDelay = sum / delays.size();
            }

            delayFile << time << "," << sourceType << "," << destType << "," << avgDelay << std::endl;
        }

        delayFile.close();
    }

    // 创建丢包率报告
    std::ofstream lossFile(resultPath + "communication/packet_loss.csv");
    if (lossFile.is_open()) {
        // 写入标题行
        lossFile << "time,sourceType,destinationType,packetLossRate" << std::endl;

        // 按时间点和节点类型对组织数据
        std::map<std::tuple<simtime_t, std::string, std::string>, std::pair<int, int>> typeLosses;

        for (const auto& record : communicationRecords) {
            // 查找源节点和目标节点的类型
            std::string sourceType = "unknown";
            std::string destType = "unknown";

            for (const auto& stateRecord : nodeStateRecords) {
                if (stateRecord.nodeId == record.sourceId) {
                    sourceType = stateRecord.nodeType;
                }
                if (stateRecord.nodeId == record.destinationId) {
                    destType = stateRecord.nodeType;
                }

                if (sourceType != "unknown" && destType != "unknown") {
                    break;
                }
            }

            // 更新成功/失败计数
            auto key = std::make_tuple(record.time, sourceType, destType);
            auto& counts = typeLosses[key];
            counts.first++; // 总数
            if (!record.success) {
                counts.second++; // 失败数
            }
        }

        // 计算每个组合的丢包率并写入
        for (const auto& pair : typeLosses) {
            simtime_t time = std::get<0>(pair.first);
            std::string sourceType = std::get<1>(pair.first);
            std::string destType = std::get<2>(pair.first);

            const auto& counts = pair.second;
            double lossRate = 0.0;
            if (counts.first > 0) {
                lossRate = static_cast<double>(counts.second) / counts.first;
            }

            lossFile << time << "," << sourceType << "," << destType << "," << lossRate << std::endl;
        }

        lossFile.close();
    }
}

void ResultCollector::generateClusterReport()
{
    // 创建簇统计报告
    std::ofstream clusterFile(resultPath + "clusters/cluster_stats.csv");
    if (clusterFile.is_open()) {
        // 写入标题行
        clusterFile << "clusterId,memberCount,activeMembers,avgBatteryLevel,avgDelay,avgLossRate" << std::endl;

        // 写入簇统计数据
        for (const auto& pair : clusterStats) {
            const auto& stats = pair.second;
            clusterFile << stats.clusterId << "," << stats.memberCount << "," << stats.activeMembers << ","
                        << stats.avgBatteryLevel << "," << stats.avgDelay << "," << stats.avgLossRate << std::endl;
        }

        clusterFile.close();
    }

    // 创建簇成员分布报告
    std::ofstream memberFile(resultPath + "clusters/cluster_members.csv");
    if (memberFile.is_open()) {
        // 写入标题行
        memberFile << "clusterId,nodeId" << std::endl;

        // 写入簇成员数据
        for (const auto& pair : clusterStats) {
            int clusterId = pair.first;
            const auto& stats = pair.second;

            for (int nodeId : stats.memberIds) {
                memberFile << clusterId << "," << nodeId << std::endl;
            }
        }

        memberFile.close();
    }

    // 创建簇头变更报告（已通过记录时创建）
}

// 生成网络拓扑图的简单表示（CSV格式，用于外部绘图工具）
void ResultCollector::generateNetworkGraph(simtime_t time)
{
    std::stringstream filename;
    filename << resultPath << "network_graph_" << time << ".csv";

    std::ofstream file(filename.str());
    if (file.is_open()) {
        // 写入节点信息
        file << "# Nodes" << std::endl;
        file << "nodeId,nodeType,x,y,z,status,batteryLevel" << std::endl;

        // 查找最接近指定时间的节点状态记录
        std::map<int, NodeStateRecord> latestStates;
        for (const auto& record : nodeStateRecords) {
            if (record.time <= time) {
                latestStates[record.nodeId] = record;
            }
        }

        // 写入节点状态
        for (const auto& pair : latestStates) {
            const auto& record = pair.second;
            file << record.nodeId << "," << record.nodeType << ","
                << record.positionX << "," << record.positionY << "," << record.positionZ << ","
                << record.status << "," << record.batteryLevel << std::endl;
        }

        // 写入簇信息
        file << std::endl << "# Clusters" << std::endl;
        file << "clusterId,clusterHeadId" << std::endl;

        // 确定每个簇在指定时间的簇头
        std::map<int, int> currentHeads;
        for (int clusterId = 1; clusterId <= 5; clusterId++) { // 假设有5个簇
            int headId = clusterId; // 默认簇头ID等于簇ID

            // 查找最后一次在指定时间之前的簇头变更
            for (const auto& change : clusterHeadChanges) {
                if (change.time <= time && change.clusterId == clusterId) {
                    headId = change.newHeadId;
                }
            }

            currentHeads[clusterId] = headId;
        }

        // 写入当前簇头
        for (const auto& pair : currentHeads) {
            file << pair.first << "," << pair.second << std::endl;
        }

        // 写入簇成员关系
        file << std::endl << "# ClusterMembers" << std::endl;
        file << "clusterId,nodeId" << std::endl;

        for (const auto& pair : latestStates) {
            int nodeId = pair.first;
            const auto& record = pair.second;

            // 跳过簇头，已经在上面输出
            if (record.nodeType == "clusterHead" || record.nodeType == "control") {
                continue;
            }

            // 确定节点所属的簇
            int clusterId = 0;
            if (nodeId >= 1000) { // 普通节点ID从1000开始
                clusterId = (nodeId - 1000) / 200 + 1; // 根据ID计算簇ID
            }

            if (clusterId > 0) {
                file << clusterId << "," << nodeId << std::endl;
            }
        }

        file.close();
    }
}

double ResultCollector::getAverageNodeLifetime(const std::string& nodeType)
{
    // 查找所有指定类型节点的"死亡"时间（电量为0或状态为故障）
    std::map<int, simtime_t> nodeDeath;

    for (const auto& record : nodeStateRecords) {
        if (record.nodeType == nodeType) {
            if (record.status == 3 || record.batteryLevel <= 0) {
                // 如果节点之前没有记录死亡时间，则记录这次
                if (nodeDeath.find(record.nodeId) == nodeDeath.end()) {
                    nodeDeath[record.nodeId] = record.time;
                }
            }
        }
    }

    // 计算平均寿命
    double totalLifetime = 0.0;
    int count = 0;

    for (const auto& pair : nodeDeath) {
        totalLifetime += pair.second.dbl();
        count++;
    }

    if (count > 0) {
        return totalLifetime / count;
    } else {
        return 0.0; // 无死亡节点
    }
}

double ResultCollector::getAveragePacketDelay()
{
    double totalDelay = 0.0;
    int count = 0;

    for (const auto& record : communicationRecords) {
        if (record.success) {
            totalDelay += record.delay;
            count++;
        }
    }

    if (count > 0) {
        return totalDelay / count;
    } else {
        return 0.0;
    }
}

double ResultCollector::getAveragePacketLossRate()
{
    int totalPackets = communicationRecords.size();
    int failedPackets = 0;

    for (const auto& record : communicationRecords) {
        if (!record.success) {
            failedPackets++;
        }
    }

    if (totalPackets > 0) {
        return static_cast<double>(failedPackets) / totalPackets;
    } else {
        return 0.0;
    }
}

int ResultCollector::getClusterHeadChangeCount(int clusterId)
{
    int count = 0;

    for (const auto& change : clusterHeadChanges) {
        if (change.clusterId == clusterId) {
            count++;
        }
    }

    return count;
}

double ResultCollector::getAverageInterNodeDistance(simtime_t time)
{
    // 查找最接近指定时间的节点位置记录
    std::map<int, std::tuple<double, double, double>> nodePositions;

    for (const auto& record : nodeStateRecords) {
        if (record.time <= time) {
            nodePositions[record.nodeId] = std::make_tuple(record.positionX, record.positionY, record.positionZ);
        }
    }

    // 计算所有节点对之间的距离
    double totalDistance = 0.0;
    int totalPairs = 0;

    for (auto it1 = nodePositions.begin(); it1 != nodePositions.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != nodePositions.end(); ++it2) {
            double x1 = std::get<0>(it1->second);
            double y1 = std::get<1>(it1->second);
            double z1 = std::get<2>(it1->second);

            double x2 = std::get<0>(it2->second);
            double y2 = std::get<1>(it2->second);
            double z2 = std::get<2>(it2->second);

            double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2));

            totalDistance += distance;
            totalPairs++;
        }
    }

    if (totalPairs > 0) {
        return totalDistance / totalPairs;
    } else {
        return 0.0;
    }
}
