// WSNChannelModel.h
#ifndef __WSN_SIMULATION_CHANNELS_WSN_CHANNEL_MODEL_H
#define __WSN_SIMULATION_CHANNELS_WSN_CHANNEL_MODEL_H

#include <omnetpp.h>

using namespace omnetpp;

namespace wsn_simulation {

class WSNChannelModel : public cDelayChannel
{
private:
    // 通道参数
    double pathLossExponent;
    double shadowingStddev;
    double carrierFrequency;
    double receiverSensitivity;
    double fadingMargin;
    double dopplerShift;
    double thermalNoise;
    double bitRate;
    double maxDistance;

    // 统计信号
    simsignal_t bitErrorSignal;
    simsignal_t propagationLossSignal;

protected:
    // 初始化参数
    virtual void initialize() override;

    // 处理消息传输
    virtual cChannel::Result processMessage(cMessage *msg, const SendOptions& options, simtime_t t) override;

    // 通道模型计算函数
    double calculatePathLoss(double distance, double frequency);
    double calculateShadowing();
    double calculateDopplerShift(double relativeSpeed, double frequency);
    double calculateReceivedPower(double transmitPower, double pathLoss, double shadowing);
    double calculateBER(double receivedPower, double thermalNoise, double bitRate);
    double calculatePER(double ber, int packetLengthBits);
    bool isPacketLost(double per);
    simtime_t calculateDelay(double distance);
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_CHANNELS_WSN_CHANNEL_MODEL_H
