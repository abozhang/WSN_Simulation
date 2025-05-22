// NetworkManager.cc
#include "NetworkManager.h"
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace wsn_simulation;

NetworkManager::NetworkManager(cSimpleModule* owner) : ownerModule(owner) {
    // 初始化簇头轮换策略参数
    batteryWeightFactor = 0.4;      // 电池电量占40%权重
    lossRateWeightFactor = 0.2;     // 丢包率占20%权重
    delayWeightFactor = 0.2;        // 延迟占20%权重
    signalWeightFactor = 0.2;       // 信号强度占20%权重

    // 设置簇头选择阈值
    minBatteryForClusterHead = 30.0;  // 电池至少30%电量
    maxLossRateAllowed = 0.1;         // 丢包率不超过10%
    maxDelayAllowed = 100.0;          // 延迟不超过100ms

    // 设置簇头超时时间
    clusterHeadTimeout = 10.0;        // 簇头10秒无响应认为失联
}

void NetworkManager::initializeNetwork(int numClusters, int nodesPerCluster) {
    // 先创建簇
    for (int i = 1; i <= numClusters; i++) {
        createCluster(i, i); // 簇ID和频段一一对应
    }

    // 添加控制节点
    addNode(0, "control", 0, -1); // 控制节点ID为0，支持所有频段

    // 添加簇头节点
    for (int i = 1; i <= numClusters; i++) {
        int headId = i; // 簇头ID与簇ID相同
        addNode(headId, "clusterHead", i, i);
        setClusterHead(i, headId);
    }

    // 添加普通节点
    int nodeId = numClusters + 1;
    for (int i = 1; i <= numClusters; i++) {
        for (int j = 0; j < nodesPerCluster; j++) {
            addNode(nodeId, "ordinary", i, i);
            addNodeToCluster(nodeId, i);
            nodeId++;
        }
    }

    // 记录初始网络状态
    logNetworkStatus();
}

void NetworkManager::addNode(int nodeId, const std::string& nodeType, int clusterId, int frequency) {
    // 创建新节点信息
    NodeInfo node;
    node.nodeId = nodeId;
    node.nodeType = nodeType;
    node.clusterId = clusterId;
    node.status = 1; // 初始为运行状态
    node.frequency = frequency;
    node.batteryLevel = (nodeType == "control") ? 5000.0 :
                        (nodeType == "clusterHead") ? 3000.0 : 1000.0;

    // 设置初始位置 - 使用全局RNG (index 0)
    cRNG *globalRNG = cSimulation::getActiveSimulation()->getContext()->getRNG(0);

    node.positionX = uniform(globalRNG, 0.0, 1000.0);
    node.positionY = uniform(globalRNG, 0.0, 1000.0);
    node.positionZ = (nodeType == "control") ? 30.0 :
                     (nodeType == "clusterHead") ? 20.0 : uniform(globalRNG, 1.0, 5.0);

    node.lastUpdateTime = simTime().dbl();

    // 将节点添加到映射表
    nodes[nodeId] = node;

    EV_INFO << "Added node: ID=" << nodeId << ", Type=" << nodeType
            << ", Cluster=" << clusterId << ", Frequency=" << frequency << endl;
}

void NetworkManager::updateNodeStatus(int nodeId, int status) {
    if (nodes.find(nodeId) != nodes.end()) {
        nodes[nodeId].status = status;
        nodes[nodeId].lastUpdateTime = simTime().dbl();

        EV_INFO << "Updated node " << nodeId << " status to " << status << endl;

        // 如果是簇头且状态异常，考虑轮换簇头
        if (nodes[nodeId].nodeType == "clusterHead" && (status == 3 || status == 4)) {
            int clusterId = nodes[nodeId].clusterId;
            EV_WARN << "Cluster head " << nodeId << " of cluster " << clusterId
                    << " is in abnormal state " << status << ". Considering rotation." << endl;
            rotateClusterHead(clusterId, true); // 强制轮换簇头
        }
    }
}

void NetworkManager::updateNodeBattery(int nodeId, double batteryLevel) {
    if (nodes.find(nodeId) != nodes.end()) {
        nodes[nodeId].batteryLevel = batteryLevel;
        nodes[nodeId].lastUpdateTime = simTime().dbl();

        // 如果是簇头且电池电量过低，考虑轮换
        if (nodes[nodeId].nodeType == "clusterHead" && batteryLevel < minBatteryForClusterHead) {
            int clusterId = nodes[nodeId].clusterId;
            EV_WARN << "Cluster head " << nodeId << " of cluster " << clusterId
                    << " has low battery: " << batteryLevel << ". Considering rotation." << endl;
            rotateClusterHead(clusterId);
        }
    }
}

void NetworkManager::updateNodePosition(int nodeId, double x, double y, double z) {
    if (nodes.find(nodeId) != nodes.end()) {
        nodes[nodeId].positionX = x;
        nodes[nodeId].positionY = y;
        nodes[nodeId].positionZ = z;
        nodes[nodeId].lastUpdateTime = simTime().dbl();
    }
}

void NetworkManager::updateNodePerformance(int nodeId, double lossRate, double delay, double signalStrength) {
    if (nodes.find(nodeId) != nodes.end()) {
        NodeInfo& node = nodes[nodeId];
        node.packetLossRate = lossRate;
        node.averageDelay = delay;
        node.signalStrength = signalStrength;
        node.lastUpdateTime = simTime().dbl();

        // 更新综合得分
        node.clusterHeadScore = calculateClusterHeadScore(node);

        // 如果是簇头且性能不佳，考虑轮换
        if (node.nodeType == "clusterHead" &&
            (lossRate > maxLossRateAllowed || delay > maxDelayAllowed)) {
            int clusterId = node.clusterId;
            EV_WARN << "Cluster head " << nodeId << " of cluster " << clusterId
                    << " has performance issues. Loss rate: " << lossRate
                    << ", Delay: " << delay << ". Considering rotation." << endl;
            rotateClusterHead(clusterId);
        }
    }
}

void NetworkManager::createCluster(int clusterId, int frequency) {
    ClusterInfo cluster;
    cluster.clusterId = clusterId;
    cluster.frequency = frequency;
    cluster.clusterHeadId = -1; // 初始无簇头

    clusters[clusterId] = cluster;

    // 创建该簇的簇头候选评估队列
    clusterHeadCandidates[clusterId] = std::priority_queue<std::pair<double, int>>();

    EV_INFO << "Created cluster: ID=" << clusterId << ", Frequency=" << frequency << endl;
}

void NetworkManager::addNodeToCluster(int nodeId, int clusterId) {
    if (nodes.find(nodeId) != nodes.end() && clusters.find(clusterId) != clusters.end()) {
        // 更新节点所属簇
        nodes[nodeId].clusterId = clusterId;

        // 将节点添加到簇成员列表
        clusters[clusterId].memberIds.push_back(nodeId);

        EV_INFO << "Added node " << nodeId << " to cluster " << clusterId << endl;
    }
}

void NetworkManager::removeNodeFromCluster(int nodeId, int clusterId) {
    if (clusters.find(clusterId) != clusters.end()) {
        // 从簇成员列表中移除节点
        auto& members = clusters[clusterId].memberIds;
        members.erase(std::remove(members.begin(), members.end(), nodeId), members.end());

        // 如果节点是簇头，需要选择新的簇头
        if (clusters[clusterId].clusterHeadId == nodeId) {
            EV_WARN << "Removing current cluster head " << nodeId << " from cluster " << clusterId << endl;
            rotateClusterHead(clusterId, true);
        }

        EV_INFO << "Removed node " << nodeId << " from cluster " << clusterId << endl;
    }
}

void NetworkManager::setClusterHead(int clusterId, int nodeId) {
    if (clusters.find(clusterId) != clusters.end() && nodes.find(nodeId) != nodes.end()) {
        // 检查节点是否适合作为簇头
        if (!isNodeSuitableForClusterHead(nodes[nodeId])) {
            EV_ERROR << "Node " << nodeId << " is not suitable for cluster head role." << endl;
            return;
        }

        // 先将原簇头恢复为普通节点
        int oldHeadId = clusters[clusterId].clusterHeadId;
        if (oldHeadId != -1 && nodes.find(oldHeadId) != nodes.end()) {
            nodes[oldHeadId].nodeType = "ordinary";
            EV_INFO << "Changed node " << oldHeadId << " from cluster head to ordinary node." << endl;
        }

        // 设置新簇头
        clusters[clusterId].clusterHeadId = nodeId;
        nodes[nodeId].nodeType = "clusterHead";
        nodes[nodeId].clusterId = clusterId;

        // 更新簇头最后更新时间
        clusterHeadLastUpdate[clusterId] = simTime().dbl();

        EV_INFO << "Set node " << nodeId << " as cluster head of cluster " << clusterId << endl;
    }
}

void NetworkManager::updateLink(int sourceId, int destId, double quality, double delay) {
    // 创建或更新链路信息
    std::pair<int, int> link = std::make_pair(sourceId, destId);

    LinkInfo& linkInfo = links[link];
    linkInfo.sourceId = sourceId;
    linkInfo.destinationId = destId;
    linkInfo.quality = quality;
    linkInfo.delay = delay;
    linkInfo.lastUpdateTime = simTime().dbl();

    // 更新节点延迟统计
    if (nodes.find(sourceId) != nodes.end()) {
        nodes[sourceId].delays.push_back(delay);
        // 保持最多100个样本
        if (nodes[sourceId].delays.size() > 100) {
            nodes[sourceId].delays.erase(nodes[sourceId].delays.begin());
        }

        // 更新平均延迟
        double sum = 0;
        for (double d : nodes[sourceId].delays) {
            sum += d;
        }
        nodes[sourceId].averageDelay = sum / nodes[sourceId].delays.size();
    }
}

double NetworkManager::getLinkQuality(int sourceId, int destId) {
    std::pair<int, int> link = std::make_pair(sourceId, destId);

    if (links.find(link) != links.end()) {
        return links[link].quality;
    }

    return 0.0; // 默认链路质量为0
}

void NetworkManager::evaluateClusterHeadCandidates(int clusterId) {
    if (clusters.find(clusterId) == clusters.end()) {
        return;
    }

    // 清空候选队列
    while (!clusterHeadCandidates[clusterId].empty()) {
        clusterHeadCandidates[clusterId].pop();
    }

    // 遍历簇内所有节点，评估适合度
    for (int nodeId : clusters[clusterId].memberIds) {
        // 跳过当前簇头和不合适的节点
        if (nodeId == clusters[clusterId].clusterHeadId || !isNodeSuitableForClusterHead(nodes[nodeId])) {
            continue;
        }

        // 计算节点作为簇头的得分
        double score = calculateClusterHeadScore(nodes[nodeId]);

        // 加入候选队列
        clusterHeadCandidates[clusterId].push(std::make_pair(score, nodeId));

        EV_DEBUG << "Node " << nodeId << " evaluated for cluster head with score " << score << endl;
    }
}

int NetworkManager::selectNewClusterHead(int clusterId) {
    // 如果没有候选，先评估
    if (clusterHeadCandidates[clusterId].empty()) {
        evaluateClusterHeadCandidates(clusterId);
    }

    // 如果仍然没有候选，返回-1表示无合适节点
    if (clusterHeadCandidates[clusterId].empty()) {
        EV_WARN << "No suitable cluster head candidates found for cluster " << clusterId << endl;
        return -1;
    }

    // 选择得分最高的候选
    std::pair<double, int> best = clusterHeadCandidates[clusterId].top();

    EV_INFO << "Selected node " << best.second << " as new cluster head with score " << best.first << endl;

    return best.second;
}

void NetworkManager::rotateClusterHead(int clusterId, bool force) {
    if (clusters.find(clusterId) == clusters.end()) {
        return;
    }

    int currentHeadId = clusters[clusterId].clusterHeadId;

    // 如果非强制且当前簇头状态良好，仅周期性检查
    if (!force &&
        nodes[currentHeadId].status == 1 &&
        nodes[currentHeadId].batteryLevel >= minBatteryForClusterHead &&
        nodes[currentHeadId].packetLossRate <= maxLossRateAllowed &&
        nodes[currentHeadId].averageDelay <= maxDelayAllowed) {

        // 每隔一段时间仍评估一次，但不一定切换
        if (simTime().dbl() - clusterHeadLastUpdate[clusterId] < 600.0) { // 每10分钟
            return; // 不需要轮换
        }
    }

    // 选择新簇头
    int newHeadId = selectNewClusterHead(clusterId);

    // 如果找不到合适的新簇头且当前簇头仍然可用，则保持不变
    if (newHeadId == -1 &&
        currentHeadId != -1 &&
        nodes[currentHeadId].status != 3 &&
        nodes[currentHeadId].status != 4) {
        EV_WARN << "No better cluster head found. Keeping current head " << currentHeadId << endl;
        return;
    }

    // 如果找到合适的新簇头，执行轮换
    if (newHeadId != -1) {
        // 生成簇头变更命令
        ManagementCommand* cmd = generateClusterHeadChangeCommand(clusterId, newHeadId);

        // 在实际应用中，这里会将命令发送给控制节点处理
        // 现在我们直接调用设置函数
        setClusterHead(clusterId, newHeadId);

        // 清理命令对象
        delete cmd;
    } else {
        EV_ERROR << "Critical: Cluster " << clusterId << " has no suitable cluster head candidate." << endl;
    }
}

bool NetworkManager::checkClusterHeadStatus() {
    bool changes = false;
    double currentTime = simTime().dbl();

    // 检查所有簇头的状态
    for (auto& clusterPair : clusters) {
        int clusterId = clusterPair.first;
        int headId = clusterPair.second.clusterHeadId;

        // 跳过无簇头的簇
        if (headId == -1) {
            continue;
        }

        // 检查簇头是否长时间未更新（可能失联）
        if (currentTime - nodes[headId].lastUpdateTime > clusterHeadTimeout) {
            EV_WARN << "Cluster head " << headId << " of cluster " << clusterId
                    << " has not updated for " << (currentTime - nodes[headId].lastUpdateTime)
                    << " seconds. Considering lost." << endl;

            // 将节点标记为离线
            updateNodeStatus(headId, 4);

            // 强制轮换簇头
            rotateClusterHead(clusterId, true);
            changes = true;
        }
    }

    return changes;
}

ManagementCommand* NetworkManager::generateClusterHeadChangeCommand(int clusterId, int newHeadId) {
    // 创建管理命令
    ManagementCommand* cmd = new ManagementCommand("ClusterHeadChange");

    // 设置基本信息
    cmd->setSrcId(0); // 控制节点发送
    cmd->setDestId(-1); // 广播给所有节点
    cmd->setFrequency(-1); // 使用所有频段
    cmd->setSequenceNumber(0); // 序列号由发送方设置
    cmd->setTimestamp(simTime().dbl());
    cmd->setMessageType(5); // 管理命令类型

    // 设置命令内容
    cmd->setTargetId(newHeadId); // 新簇头ID
    cmd->setCommandType(1); // 类型1表示簇头变更
    cmd->setExtensionParam(0.0); // 无扩展参数

    // 命令数据包含簇ID
    std::string cmdData = "CH_CHANGE:" + std::to_string(clusterId);
    cmd->setCommandData(cmdData.c_str());

    // 需要回复
    cmd->setResponseFlag(1);

    return cmd;
}

double NetworkManager::calculateClusterHeadScore(const NodeInfo& node) {
    // 逆转丢包率和延迟，使其与电池电量和信号强度方向一致（越高越好）
    double lossRateScore = 1.0 - node.packetLossRate;
    double delayScore = 1.0 - std::min(1.0, node.averageDelay / maxDelayAllowed);

    // 归一化电池电量 (0-100%)
    double batteryScore = node.batteryLevel / 1000.0;

    // 归一化信号强度 (假设信号强度范围0-1)
    double signalScore = node.signalStrength;

    // 计算加权得分
    double score = batteryWeightFactor * batteryScore +
                  lossRateWeightFactor * lossRateScore +
                  delayWeightFactor * delayScore +
                  signalWeightFactor * signalScore;

    return score;
}

void NetworkManager::updateClusterStatistics(int clusterId) {
    if (clusters.find(clusterId) == clusters.end()) {
        return;
    }

    ClusterInfo& cluster = clusters[clusterId];
    double totalBattery = 0.0;
    double totalLossRate = 0.0;
    double totalDelay = 0.0;
    int activeNodes = 0;

    // 计算簇统计信息
    for (int nodeId : cluster.memberIds) {
        if (nodes.find(nodeId) != nodes.end() && nodes[nodeId].status == 1) {
            totalBattery += nodes[nodeId].batteryLevel;
            totalLossRate += nodes[nodeId].packetLossRate;
            totalDelay += nodes[nodeId].averageDelay;
            activeNodes++;
        }
    }

    // 更新簇平均统计数据
    if (activeNodes > 0) {
        cluster.avgBatteryLevel = totalBattery / activeNodes;
        cluster.avgPacketLossRate = totalLossRate / activeNodes;
        cluster.avgDelay = totalDelay / activeNodes;
    }
}

void NetworkManager::logNetworkStatus() {
    std::ostringstream log;
    log << "======= Network Status Report =======" << endl;
    log << "Time: " << simTime() << endl;
    log << "Total nodes: " << nodes.size() << endl;
    log << "Total clusters: " << clusters.size() << endl;

    // 记录簇信息
    for (auto& clusterPair : clusters) {
        int clusterId = clusterPair.first;
        ClusterInfo& cluster = clusterPair.second;

        log << "Cluster " << clusterId << ":" << endl;
        log << "  Head: " << cluster.clusterHeadId << endl;
        log << "  Members: " << cluster.memberIds.size() << endl;
        log << "  Frequency: " << cluster.frequency << endl;
        log << "  Avg Battery: " << cluster.avgBatteryLevel << "mAh" << endl;
        log << "  Avg Loss Rate: " << cluster.avgPacketLossRate * 100 << "%" << endl;
        log << "  Avg Delay: " << cluster.avgDelay << "ms" << endl;
    }

    // 记录簇头信息
    log << "Cluster Heads:" << endl;
    for (auto& clusterPair : clusters) {
        int headId = clusterPair.second.clusterHeadId;
        if (headId != -1 && nodes.find(headId) != nodes.end()) {
            NodeInfo& head = nodes[headId];
            log << "  Node " << headId << " (Cluster " << clusterPair.first << "):" << endl;
            log << "    Battery: " << head.batteryLevel << "mAh" << endl;
            log << "    Loss Rate: " << head.packetLossRate * 100 << "%" << endl;
            log << "    Delay: " << head.averageDelay << "ms" << endl;
            log << "    Signal: " << head.signalStrength << endl;
            log << "    Score: " << head.clusterHeadScore << endl;
        }
    }

    EV_INFO << log.str() << endl;
}

bool NetworkManager::isNodeSuitableForClusterHead(const NodeInfo& node) {
    // 检查基本条件
    if (node.status != 1) { // 必须在运行状态
        return false;
    }

    if (node.batteryLevel < minBatteryForClusterHead) {
        return false;
    }

    if (node.packetLossRate > maxLossRateAllowed) {
        return false;
    }

    if (node.averageDelay > maxDelayAllowed) {
        return false;
    }

    return true;
}

const NodeInfo& NetworkManager::getNodeInfo(int nodeId) const {
    static NodeInfo emptyNode; // 返回空节点信息的备选

    auto it = nodes.find(nodeId);
    if (it != nodes.end()) {
        return it->second;
    }

    return emptyNode;
}

const ClusterInfo& NetworkManager::getClusterInfo(int clusterId) const {
    static ClusterInfo emptyCluster; // 返回空簇信息的备选

    auto it = clusters.find(clusterId);
    if (it != clusters.end()) {
        return it->second;
    }

    return emptyCluster;
}

std::vector<int> NetworkManager::getNodesInCluster(int clusterId) const {
    auto it = clusters.find(clusterId);
    if (it != clusters.end()) {
        return it->second.memberIds;
    }

    return std::vector<int>(); // 返回空列表
}

int NetworkManager::getCurrentClusterHead(int clusterId) const {
    auto it = clusters.find(clusterId);
    if (it != clusters.end()) {
        return it->second.clusterHeadId;
    }

    return -1; // 无有效簇头
}

void NetworkManager::processStatusMessage(StatusMessage* msg) {
    int nodeId = msg->getSrcId();

    // 更新节点信息
    if (nodes.find(nodeId) != nodes.end()) {
        NodeInfo& node = nodes[nodeId];

        // 更新基本信息
        node.status = msg->getStatus();
        node.batteryLevel = msg->getBatteryLevel();
        node.positionX = msg->getPositionX();
        node.positionY = msg->getPositionY();
        node.positionZ = msg->getPositionZ();
        node.lastUpdateTime = simTime().dbl();

        // 如果是簇头，更新最后更新时间
        if (node.nodeType == "clusterHead") {
            clusterHeadLastUpdate[node.clusterId] = simTime().dbl();
        }

        EV_DEBUG << "Updated node " << nodeId << " info from status message." << endl;
    }
}
