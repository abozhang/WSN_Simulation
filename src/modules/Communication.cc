// Communication.cc
#include <omnetpp.h>
#include "messages/BaseMessage_m.h"

using namespace omnetpp;

namespace wsn_simulation {

class Communication : public cSimpleModule
{
private:
    // 参数
    std::string nodeType;
    int supportedFrequencies;
    double transmitPower;
    double bitRate;
    double sensitivity;

    // 统计
    int receivedPackets;
    int sentPackets;
    int droppedPackets;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // 辅助函数
    bool isFrequencySupported(int frequency);
    void updateBatteryStatus(double consumption);
};

Define_Module(Communication);

void Communication::initialize()
{
    // 获取参数
    nodeType = par("nodeType").stringValue();
    supportedFrequencies = par("supportedFrequencies");
    transmitPower = par("transmitPower");
    bitRate = par("bitRate");
    sensitivity = par("sensitivity");

    // 初始化统计
    receivedPackets = 0;
    sentPackets = 0;
    droppedPackets = 0;

    EV << "Communication module initialized with supportedFrequencies=" << supportedFrequencies << endl;
}

void Communication::handleMessage(cMessage *msg)
{
    // 检查消息来源
    if (msg->arrivedOn("radioIn")) {
        // 来自其他节点的无线消息
        BaseMessage *baseMsg = check_and_cast<BaseMessage *>(msg);

        // 检查频段是否支持
        if (isFrequencySupported(baseMsg->getFrequency())) {
            // 转发到应用层
            send(msg, "toApp");
            receivedPackets++;
        } else {
            // 频段不支持，丢弃消息
            EV << "Dropped message due to unsupported frequency: " << baseMsg->getFrequency() << endl;
            delete msg;
            droppedPackets++;
        }
    } else if (msg->arrivedOn("fromApp")) {
        // 来自应用层的消息
        BaseMessage *baseMsg = check_and_cast<BaseMessage *>(msg);

        // 检查频段是否支持
        if (isFrequencySupported(baseMsg->getFrequency())) {
            // 发送到无线信道
            send(msg, "radioIn");
            sentPackets++;

            // 更新能耗
            updateBatteryStatus(transmitPower * 0.001); // 假设每mW功率消耗0.001单位电量
        } else {
            // 频段不支持，丢弃消息
            EV << "Cannot send message due to unsupported frequency: " << baseMsg->getFrequency() << endl;
            delete msg;
            droppedPackets++;
        }
    } else {
        // 未知来源，丢弃消息
        EV << "Received message from unknown gate" << endl;
        delete msg;
    }
}

void Communication::finish()
{
    // 记录统计信息
    recordScalar("receivedPackets", receivedPackets);
    recordScalar("sentPackets", sentPackets);
    recordScalar("droppedPackets", droppedPackets);

    EV << "Communication module finished with stats:" << endl;
    EV << "  Received packets: " << receivedPackets << endl;
    EV << "  Sent packets: " << sentPackets << endl;
    EV << "  Dropped packets: " << droppedPackets << endl;
}

bool Communication::isFrequencySupported(int frequency)
{
    // 检查频段是否支持
    // 控制节点支持所有频段
    if (supportedFrequencies == -1) {
        return true;
    }

    // 簇头和普通节点只支持特定频段
    return (frequency == supportedFrequencies);
}

void Communication::updateBatteryStatus(double consumption)
{
    // 向电池模块报告能耗
    cPacket *batteryMsg = new cPacket("energy");
    batteryMsg->setByteLength(sizeof(double));
    batteryMsg->setContextPointer(new double(consumption));
    send(batteryMsg, "toBattery");
}

} // namespace wsn_simulation
