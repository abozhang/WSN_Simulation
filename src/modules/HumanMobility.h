// HumanMobility.h
#ifndef __WSN_SIMULATION_HUMAN_MOBILITY_H
#define __WSN_SIMULATION_HUMAN_MOBILITY_H

#include <omnetpp.h>
#include <vector>
#include <deque>

using namespace omnetpp;

namespace wsn_simulation {

// 移动状态枚举
enum MobilityState {
    STANDING,       // 静止状态
    WALKING,        // 步行状态
    RUNNING,        // 快走/跑步状态
    PAUSED          // 短暂停顿
};

// 目标点结构
struct Waypoint {
    double x;
    double y;
    double z;
    simtime_t arrivalTime;
};

class HumanMobility : public cSimpleModule
{
private:
    // 参数
    double initialX;           // 初始X坐标
    double initialY;           // 初始Y坐标
    double initialZ;           // 初始Z坐标
    bool isMobile;             // 是否移动
    std::string mobilityType;  // 移动模型类型
    double playgroundSizeX;    // 场景X尺寸
    double playgroundSizeY;    // 场景Y尺寸
    double speed;              // 当前速度

    // 人类移动特性参数
    double walkSpeed;          // 步行速度 (m/s)
    double runSpeed;           // 快走/跑步速度 (m/s)
    double pauseTimeMean;      // 停顿时间均值 (s)
    double pauseTimeStdDev;    // 停顿时间标准差 (s)
    double stateChangeProbability; // 状态改变概率
    double randomDestRadius;   // 随机目的地半径
    double waypointProximity;  // 到达目标点的最小距离

    // 状态
    double currentX;          // 当前X坐标
    double currentY;          // 当前Y坐标
    double currentZ;          // 当前Z坐标
    double targetX;           // 目标X坐标
    double targetY;           // 目标Y坐标
    double targetZ;           // 目标Z坐标
    MobilityState state;      // 当前移动状态
    simtime_t stateEntryTime; // 状态开始时间
    simtime_t lastPositionUpdateTime; // 上次位置更新时间

    // 方向和速度
    double directionX;
    double directionY;
    double directionZ;

    // 目标点队列
    std::deque<Waypoint> waypoints;

    // 移动轨迹
    std::vector<std::pair<simtime_t, std::tuple<double, double, double>>> trajectory;

    // 定时器
    cMessage *moveTimer;
    cMessage *stateChangeTimer;

    // 统计数据收集
    simsignal_t positionXSignal;
    simsignal_t positionYSignal;
    simsignal_t positionZSignal;
    simsignal_t speedSignal;
    simsignal_t stateSignal;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 辅助函数
    void updatePosition();
    void changeState();
    void generateNewTarget();
    void addWaypoint(double x, double y, double z);
    bool reachedTarget();
    void scheduleNextPositionUpdate();

    // 状态转换
    void enterStandingState();
    void enterWalkingState();
    void enterRunningState();
    void enterPausedState();

public:
    // 公共接口函数
    double getX() const { return currentX; }
    double getY() const { return currentY; }
    double getZ() const { return currentZ; }
    double getSpeed() const { return speed; }
    MobilityState getState() const { return state; }
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_HUMAN_MOBILITY_H
