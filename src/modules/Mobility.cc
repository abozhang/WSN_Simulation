// Mobility.cc
#include <omnetpp.h>

using namespace omnetpp;

namespace wsn_simulation {

class Mobility : public cSimpleModule
{
private:
    // 参数
    double initialX;
    double initialY;
    double initialZ;
    bool isMobile;
    std::string mobilityType;

    // 状态
    double currentX;
    double currentY;
    double currentZ;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // 辅助函数
    void updatePosition();
};

Define_Module(Mobility);

void Mobility::initialize()
{
    // 获取参数
    initialX = par("initialX").doubleValue();
    initialY = par("initialY").doubleValue();
    initialZ = par("initialZ").doubleValue();
    isMobile = par("isMobile");
    mobilityType = par("mobilityType").stringValue();

    // 初始化位置
    currentX = initialX;
    currentY = initialY;
    currentZ = initialZ;

    EV << "Mobility module initialized at position (" << currentX << ", " << currentY << ", " << currentZ << ")" << endl;

    // 如果节点是移动的，则设置定时器更新位置
    if (isMobile) {
        cMessage *updateMsg = new cMessage("updatePosition");
        scheduleAt(simTime() + 1.0, updateMsg);
    }
}

void Mobility::handleMessage(cMessage *msg)
{
    // 处理位置更新定时器
    if (msg->isName("updatePosition")) {
        updatePosition();
        scheduleAt(simTime() + 1.0, msg);
    } else {
        // 未知消息，丢弃
        delete msg;
    }
}

void Mobility::updatePosition()
{
    // 根据移动模型更新位置
    if (mobilityType == "random") {
        // 随机移动模型
        currentX += uniform(-5, 5);
        currentY += uniform(-5, 5);
        currentZ += uniform(-1, 1);
    } else if (mobilityType == "linear") {
        // 线性移动模型
        currentX += 1.0;
        if (currentX > getParentModule()->getParentModule()->par("playgroundSizeX").doubleValue()) {
            currentX = 0;
        }
    } else {
        // 默认静态
    }

    EV << "Position updated to (" << currentX << ", " << currentY << ", " << currentZ << ")" << endl;
}

} // namespace wsn_simulation
