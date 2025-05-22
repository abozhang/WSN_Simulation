// HumanMobility.cc
#include "HumanMobility.h"
#include <cmath>
#include <sstream>

using namespace wsn_simulation;

Define_Module(HumanMobility);

void HumanMobility::initialize()
{
    // 获取基本参数
    initialX = par("initialX").doubleValue();
    initialY = par("initialY").doubleValue();
    initialZ = par("initialZ").doubleValue();
    isMobile = par("isMobile").boolValue();
    mobilityType = par("mobilityType").stringValue();

    // 获取移动参数
    walkSpeed = par("walkSpeed").doubleValue();
    runSpeed = par("runSpeed").doubleValue();
    pauseTimeMean = par("pauseTimeMean").doubleValue();
    pauseTimeStdDev = par("pauseTimeStdDev").doubleValue();
    stateChangeProbability = par("stateChangeProbability").doubleValue();
    randomDestRadius = par("randomDestRadius").doubleValue();
    waypointProximity = par("waypointProximity").doubleValue();

    // 获取场景大小
    cModule *networkModule = getParentModule()->getParentModule();
    playgroundSizeX = networkModule->par("playgroundSizeX").doubleValue();
    playgroundSizeY = networkModule->par("playgroundSizeY").doubleValue();

    // 初始化状态
    currentX = initialX;
    currentY = initialY;
    currentZ = initialZ;
    speed = 0.0;
    state = STANDING;
    stateEntryTime = simTime();
    lastPositionUpdateTime = simTime();

    // 初始化方向
    directionX = 0.0;
    directionY = 0.0;
    directionZ = 0.0;

    // 初始化目标点
    targetX = currentX;
    targetY = currentY;
    targetZ = currentZ;

    // 创建定时器
    moveTimer = new cMessage("moveTimer");
    stateChangeTimer = new cMessage("stateChangeTimer");

    // 注册统计信号
    positionXSignal = registerSignal("positionX");
    positionYSignal = registerSignal("positionY");
    positionZSignal = registerSignal("positionZ");
    speedSignal = registerSignal("speed");
    stateSignal = registerSignal("state");

    // 记录初始位置
    emit(positionXSignal, currentX);
    emit(positionYSignal, currentY);
    emit(positionZSignal, currentZ);
    emit(speedSignal, speed);
    emit(stateSignal, static_cast<int>(state));

    // 记录轨迹
    trajectory.push_back(std::make_pair(simTime(), std::make_tuple(currentX, currentY, currentZ)));

    // 如果节点是移动的，则初始化移动
    if (isMobile) {
        // 初始进入步行状态
        enterWalkingState();

        // 生成第一个目标点
        generateNewTarget();

        // 安排下一次位置更新
        scheduleNextPositionUpdate();

        // 安排状态变化
        scheduleAt(simTime() + exponential(10.0), stateChangeTimer);
    }

    EV_INFO << "HumanMobility initialized at position (" << currentX << ", " << currentY << ", " << currentZ << ")" << endl;
}

void HumanMobility::handleMessage(cMessage *msg)
{
    if (msg == moveTimer) {
        // 更新位置
        updatePosition();

        // 检查是否到达目标点
        if (reachedTarget()) {
            // 如果有更多目标点
            if (!waypoints.empty()) {
                // 移动到下一个目标点
                Waypoint next = waypoints.front();
                waypoints.pop_front();

                targetX = next.x;
                targetY = next.y;
                targetZ = next.z;

                // 计算新方向
                double distance = sqrt(pow(targetX - currentX, 2) + pow(targetY - currentY, 2) + pow(targetZ - currentZ, 2));
                if (distance > 0) {
                    directionX = (targetX - currentX) / distance;
                    directionY = (targetY - currentY) / distance;
                    directionZ = (targetZ - currentZ) / distance;
                }
            } else {
                // 生成新的目标点
                generateNewTarget();
            }
        }

        // 安排下一次位置更新
        scheduleNextPositionUpdate();
    } else if (msg == stateChangeTimer) {
        // 尝试改变状态
        changeState();

        // 安排下一次状态变化
        scheduleAt(simTime() + exponential(20.0), stateChangeTimer);
    } else {
        // 未知消息，丢弃
        delete msg;
    }
}

void HumanMobility::finish()
{
    // 释放定时器
    cancelAndDelete(moveTimer);
    cancelAndDelete(stateChangeTimer);

    // 保存轨迹到文件
    std::string nodeId = getParentModule()->getFullName();
    std::ostringstream filename;
    filename << "results/trajectory_" << nodeId << ".csv";

    std::ofstream file(filename.str());
    if (file.is_open()) {
        file << "time,x,y,z" << std::endl;
        for (const auto& point : trajectory) {
            file << point.first << ","
                 << std::get<0>(point.second) << ","
                 << std::get<1>(point.second) << ","
                 << std::get<2>(point.second) << std::endl;
        }
        file.close();
        EV_INFO << "Saved mobility trajectory to " << filename.str() << endl;
    }
}

void HumanMobility::updatePosition()
{
    simtime_t currentTime = simTime();
    simtime_t timeDelta = currentTime - lastPositionUpdateTime;
    double delta = timeDelta.dbl();

    // 根据当前状态决定行为
    if (state == WALKING || state == RUNNING) {
        // 更新位置
        currentX += directionX * speed * delta;
        currentY += directionY * speed * delta;
        currentZ += directionZ * speed * delta;

        // 确保不超出场景边界
        currentX = std::max(0.0, std::min(playgroundSizeX, currentX));
        currentY = std::max(0.0, std::min(playgroundSizeY, currentY));
        currentZ = std::max(0.0, currentZ);

        // 记录新位置
        emit(positionXSignal, currentX);
        emit(positionYSignal, currentY);
        emit(positionZSignal, currentZ);
        emit(speedSignal, speed);

        // 记录轨迹
        trajectory.push_back(std::make_pair(currentTime, std::make_tuple(currentX, currentY, currentZ)));

        EV_DEBUG << "Position updated to (" << currentX << ", " << currentY << ", " << currentZ
                << "), speed: " << speed << " m/s" << endl;
    }

    lastPositionUpdateTime = currentTime;
}

void HumanMobility::changeState()
{
    // 随机决定是否改变状态
    if (uniform(0, 1) < stateChangeProbability) {
        double randomValue = uniform(0, 1);

        // 状态转换逻辑
        switch (state) {
            case STANDING:
                if (randomValue < 0.7) {
                    enterWalkingState();
                } else if (randomValue < 0.9) {
                    enterPausedState();
                } else {
                    enterRunningState();
                }
                break;

            case WALKING:
                if (randomValue < 0.3) {
                    enterStandingState();
                } else if (randomValue < 0.4) {
                    enterRunningState();
                } else if (randomValue < 0.5) {
                    enterPausedState();
                }
                break;

            case RUNNING:
                if (randomValue < 0.4) {
                    enterWalkingState();
                } else if (randomValue < 0.5) {
                    enterStandingState();
                }
                break;

            case PAUSED:
                if (randomValue < 0.8) {
                    enterWalkingState();
                } else {
                    enterStandingState();
                }
                break;
        }
    }
}

void HumanMobility::generateNewTarget()
{
    double angle, radius;

    // 根据移动模型类型生成目标点
    if (mobilityType == "randomWaypoint") {
        // 完全随机的目标点
        targetX = uniform(0, playgroundSizeX);
        targetY = uniform(0, playgroundSizeY);
        targetZ = currentZ; // 保持Z坐标不变

    } else if (mobilityType == "randomDirection") {
        // 随机方向，固定距离
        angle = uniform(0, 2 * M_PI);
        directionX = cos(angle);
        directionY = sin(angle);
        directionZ = 0;

        // 在当前位置的基础上向随机方向移动
        targetX = currentX + directionX * randomDestRadius;
        targetY = currentY + directionY * randomDestRadius;
        targetZ = currentZ;

        // 确保目标点在场景内
        targetX = std::max(0.0, std::min(playgroundSizeX, targetX));
        targetY = std::max(0.0, std::min(playgroundSizeY, targetY));

    } else if (mobilityType == "gaussMarkov") {
        // 高斯-马尔可夫模型，考虑历史方向
        // 使用自相关系数α来保持方向连续性
        double alpha = 0.8; // 方向自相关系数

        // 新方向 = α * 旧方向 + (1-α) * 随机方向
        angle = uniform(0, 2 * M_PI);
        double newDirX = alpha * directionX + (1 - alpha) * cos(angle);
        double newDirY = alpha * directionY + (1 - alpha) * sin(angle);

        // 归一化方向向量
        double norm = sqrt(newDirX * newDirX + newDirY * newDirY);
        if (norm > 0) {
            directionX = newDirX / norm;
            directionY = newDirY / norm;
        }

        // 在当前位置的基础上向新方向移动
        radius = randomDestRadius * uniform(0.5, 1.5);
        targetX = currentX + directionX * radius;
        targetY = currentY + directionY * radius;
        targetZ = currentZ;

        // 确保目标点在场景内，如果接近边界则反弹
        if (targetX < 10.0 || targetX > playgroundSizeX - 10.0) {
            directionX = -directionX;
            targetX = currentX + directionX * radius;
        }
        if (targetY < 10.0 || targetY > playgroundSizeY - 10.0) {
            directionY = -directionY;
            targetY = currentY + directionY * radius;
        }

        targetX = std::max(0.0, std::min(playgroundSizeX, targetX));
        targetY = std::max(0.0, std::min(playgroundSizeY, targetY));

    } else if (mobilityType == "humanMobility") {
        // 人类移动模型，考虑活动区域

        // 人类活动具有区域性特点，70%的移动在30米范围内
        double localProbability = 0.7;

        if (uniform(0, 1) < localProbability) {
            // 本地移动 (短距离)
            radius = uniform(5, 30);
        } else {
            // 远距离移动
            radius = uniform(50, 200);
        }

        // 选择一个随机方向
        angle = uniform(0, 2 * M_PI);
        directionX = cos(angle);
        directionY = sin(angle);

        // 计算目标点
        targetX = currentX + directionX * radius;
        targetY = currentY + directionY * radius;
        targetZ = currentZ;

        // 确保目标点在场景内
        targetX = std::max(0.0, std::min(playgroundSizeX, targetX));
        targetY = std::max(0.0, std::min(playgroundSizeY, targetY));

    } else {
        // 默认简单随机
        targetX = uniform(0, playgroundSizeX);
        targetY = uniform(0, playgroundSizeY);
        targetZ = currentZ;
    }

    // 更新方向
    double distance = sqrt(pow(targetX - currentX, 2) + pow(targetY - currentY, 2) + pow(targetZ - currentZ, 2));
    if (distance > 0) {
        directionX = (targetX - currentX) / distance;
        directionY = (targetY - currentY) / distance;
        directionZ = (targetZ - currentZ) / distance;
    }

    EV_INFO << "New target generated: (" << targetX << ", " << targetY << ", " << targetZ
            << "), distance: " << distance << "m" << endl;

    // 有时候会添加中间路径点，模拟人类不会直线行走的特点
    if (distance > 50 && uniform(0, 1) < 0.3) {
        // 添加一个中间点
        double midX = currentX + directionX * distance * uniform(0.3, 0.7);
        double midY = currentY + directionY * distance * uniform(0.3, 0.7);

        // 向中间点添加一些随机偏移，模拟避障或非直线行走
        double deviation = uniform(5, 20);
        double deviationAngle = uniform(0, 2 * M_PI);

        midX += deviation * cos(deviationAngle);
        midY += deviation * sin(deviationAngle);

        // 确保中间点在场景内
        midX = std::max(0.0, std::min(playgroundSizeX, midX));
        midY = std::max(0.0, std::min(playgroundSizeY, midY));

        // 先添加中间点，再添加最终目标点
        addWaypoint(midX, midY, currentZ);
    }

    // 添加最终目标点
    addWaypoint(targetX, targetY, targetZ);
}

void HumanMobility::addWaypoint(double x, double y, double z)
{
    Waypoint waypoint;
    waypoint.x = x;
    waypoint.y = y;
    waypoint.z = z;

    // 计算到达时间
    double distance = sqrt(pow(x - currentX, 2) + pow(y - currentY, 2) + pow(z - currentZ, 2));
    double travelTime = 0.0;

    if (speed > 0) {
        travelTime = distance / speed;
    } else if (state == WALKING) {
        travelTime = distance / walkSpeed;
    } else if (state == RUNNING) {
        travelTime = distance / runSpeed;
    } else {
        travelTime = distance / walkSpeed; // 默认使用步行速度
    }

    waypoint.arrivalTime = simTime() + travelTime;

    // 添加到队列
    waypoints.push_back(waypoint);

    EV_DEBUG << "Added waypoint: (" << x << ", " << y << ", " << z
            << "), expected arrival: " << waypoint.arrivalTime << endl;
}

bool HumanMobility::reachedTarget()
{
    // 检查是否到达目标点
    double distance = sqrt(pow(targetX - currentX, 2) + pow(targetY - currentY, 2) + pow(targetZ - currentZ, 2));
    return distance <= waypointProximity;
}

void HumanMobility::scheduleNextPositionUpdate()
{
    // 安排下一次位置更新
    simtime_t updateInterval = 0.1; // 默认每0.1秒更新一次

    if (!moveTimer->isScheduled()) {
        scheduleAt(simTime() + updateInterval, moveTimer);
    }
}

void HumanMobility::enterStandingState()
{
    state = STANDING;
    speed = 0.0;
    stateEntryTime = simTime();

    emit(stateSignal, static_cast<int>(state));
    emit(speedSignal, speed);

    EV_INFO << "Entered STANDING state at " << simTime() << endl;
}

void HumanMobility::enterWalkingState()
{
    state = WALKING;
    speed = walkSpeed;
    stateEntryTime = simTime();

    emit(stateSignal, static_cast<int>(state));
    emit(speedSignal, speed);

    EV_INFO << "Entered WALKING state at " << simTime() << ", speed: " << speed << " m/s" << endl;
}

void HumanMobility::enterRunningState()
{
    state = RUNNING;
    speed = runSpeed;
    stateEntryTime = simTime();

    emit(stateSignal, static_cast<int>(state));
    emit(speedSignal, speed);

    EV_INFO << "Entered RUNNING state at " << simTime() << ", speed: " << speed << " m/s" << endl;
}

void HumanMobility::enterPausedState()
{
    state = PAUSED;
    speed = 0.0;
    stateEntryTime = simTime();

    emit(stateSignal, static_cast<int>(state));
    emit(speedSignal, speed);

    // 暂停一段时间后自动恢复移动
    double pauseDuration = std::max(0.1, normal(pauseTimeMean, pauseTimeStdDev));
    scheduleAt(simTime() + pauseDuration, stateChangeTimer);

    EV_INFO << "Entered PAUSED state at " << simTime() << ", duration: " << pauseDuration << "s" << endl;
}
