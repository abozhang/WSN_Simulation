// Battery.cc
#include <omnetpp.h>

using namespace omnetpp;

namespace wsn_simulation {

class Battery : public cSimpleModule
{
private:
    // 参数
    double initialCapacity;
    double voltage;

    // 状态
    double remainingCapacity;
    double consumedEnergy;

    // 统计
    double totalConsumption;
    simtime_t estimatedLifetime;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Battery);

void Battery::initialize()
{
    // 获取参数
    initialCapacity = par("initialCapacity").doubleValue();
    voltage = par("voltage").doubleValue();

    // 初始化状态
    remainingCapacity = initialCapacity;
    consumedEnergy = 0.0;

    // 初始化统计
    totalConsumption = 0.0;
    estimatedLifetime = SIMTIME_MAX;

    EV << "Battery module initialized with capacity=" << initialCapacity << " mAh, voltage=" << voltage << " V" << endl;
}

void Battery::handleMessage(cMessage *msg)
{
    // 处理能耗消息
    if (msg->isName("energy") && msg->getContextPointer()) {
        double consumption = *(static_cast<double*>(msg->getContextPointer()));

        // 更新电池状态
        remainingCapacity -= consumption;
        consumedEnergy += consumption * voltage; // 转换为能量 (mWh)
        totalConsumption += consumption;

        // 估计剩余寿命
        if (totalConsumption > 0) {
            double consumptionRate = totalConsumption / simTime().dbl(); // mAh/s
            if (consumptionRate > 0) {
                double remainingHours = remainingCapacity / consumptionRate / 3600; // 转换为小时
                estimatedLifetime = remainingHours * 3600; // 转换为秒
            }
        }

        // 检查电池是否耗尽
        if (remainingCapacity <= 0) {
            remainingCapacity = 0;
            EV << "Battery depleted at " << simTime() << endl;
        }

        EV << "Battery consumption: " << consumption << " mAh, remaining: " << remainingCapacity << " mAh" << endl;

        // 释放上下文指针
        delete static_cast<double*>(msg->getContextPointer());
    }

    // 释放消息
    delete msg;
}

void Battery::finish()
{
    // 记录统计信息
    recordScalar("remainingCapacity", remainingCapacity);
    recordScalar("consumedEnergy", consumedEnergy);
    recordScalar("totalConsumption", totalConsumption);
    recordScalar("estimatedLifetime", estimatedLifetime.dbl());

    EV << "Battery module finished with stats:" << endl;
    EV << "  Remaining capacity: " << remainingCapacity << " mAh" << endl;
    EV << "  Consumed energy: " << consumedEnergy << " mWh" << endl;
    EV << "  Total consumption: " << totalConsumption << " mAh" << endl;
    EV << "  Estimated lifetime: " << estimatedLifetime << " s" << endl;
}

} // namespace wsn_simulation
