// WSNChannelModel.cc
#include "WSNChannelModel.h"
#include <cmath>
#include "../protocols/WSNMessages_m.h"

using namespace omnetpp;

namespace wsn_simulation {

// 通道使用 Define_Channel 而不是 Define_Module
Define_Channel(WSNChannelModel);

void WSNChannelModel::initialize()
{
    cDelayChannel::initialize();

    // 获取通道参数
    pathLossExponent = par("pathLossExponent");
    shadowingStddev = par("shadowingStddev");
    carrierFrequency = par("carrierFrequency").doubleValue();
    receiverSensitivity = par("receiverSensitivity").doubleValue();
    fadingMargin = par("fadingMargin").doubleValue();
    dopplerShift = par("dopplerShift").doubleValue();
    thermalNoise = par("thermalNoise").doubleValue();
    bitRate = par("bitRate").doubleValue();
    maxDistance = par("maxDistance").doubleValue();

    // 注册统计信号
    bitErrorSignal = registerSignal("bitErrorRate");
    propagationLossSignal = registerSignal("propagationLoss");
}

cChannel::Result WSNChannelModel::processMessage(cMessage *msg, const SendOptions& options, simtime_t t)
{
    Enter_Method_Silent();

    // 结果结构
    Result result;
    result.delay = SIMTIME_ZERO;
    result.discard = false;

    // 获取发送和接收模块 - 使用正确的成员变量名
    cModule *senderModule = getSourceGate()->getOwnerModule();
    cModule *receiverModule = getDestinationGate()->getOwnerModule();

    // 计算发送和接收节点之间的距离
    double senderX, senderY, senderZ;
    double receiverX, receiverY, receiverZ;

    // 尝试获取移动模块的位置参数
    cModule *senderMobility = senderModule->getSubmodule("mobility");
    cModule *receiverMobility = receiverModule->getSubmodule("mobility");

    if (senderMobility && receiverMobility) {
        senderX = senderMobility->par("initialX").doubleValue();
        senderY = senderMobility->par("initialY").doubleValue();
        senderZ = senderMobility->par("initialZ").doubleValue();

        receiverX = receiverMobility->par("initialX").doubleValue();
        receiverY = receiverMobility->par("initialY").doubleValue();
        receiverZ = receiverMobility->par("initialZ").doubleValue();
    } else {
        // 如果没有移动模块，使用节点模块的位置参数
        senderX = senderModule->par("x").doubleValue();
        senderY = senderModule->par("y").doubleValue();
        senderZ = senderModule->par("z").doubleValue();

        receiverX = receiverModule->par("x").doubleValue();
        receiverY = receiverModule->par("y").doubleValue();
        receiverZ = receiverModule->par("z").doubleValue();
    }

    double distance = sqrt(pow(senderX - receiverX, 2) + pow(senderY - receiverY, 2) + pow(senderZ - receiverZ, 2));

    // 如果超出最大通信距离，直接丢弃数据包
    if (distance > maxDistance) {
        EV_INFO << "Packet discarded: distance " << distance << "m exceeds maximum communication range "
                << maxDistance << "m" << endl;
        result.discard = true;
        return result;
    }

    // 获取BaseMessage中的频段信息
    BaseMessage *baseMsg = dynamic_cast<BaseMessage *>(msg);
    double frequency = carrierFrequency;

    if (baseMsg) {
        // 根据消息频段调整载波频率
        // 假设频段1-5分别对应不同频率
        int messageFrequency = baseMsg->getFrequency();

        if (messageFrequency > 0 && messageFrequency <= 5) {
            // 频段1-5对应433MHz到915MHz的频率
            frequency = 433e6 + (messageFrequency - 1) * 120e6;
        }
    }

    // 计算路径损耗
    double pathLoss = calculatePathLoss(distance, frequency);

    // 计算阴影衰落
    double shadowing = calculateShadowing();

    // 假设发送功率为100mW (20dBm)
    double transmitPower = 20.0; // dBm

    // 如果是簇头或控制节点，使用更高的发送功率
    std::string senderType = senderModule->par("nodeType").stdstringValue();
    if (senderType == "control") {
        transmitPower = 27.0; // dBm (500mW)
    } else if (senderType == "clusterHead") {
        transmitPower = 23.0; // dBm (200mW)
    }

    // 计算接收功率
    double receivedPower = calculateReceivedPower(transmitPower, pathLoss, shadowing);

    // 发射统计信号
    emit(propagationLossSignal, pathLoss + shadowing);

    // 如果接收功率低于接收灵敏度，丢弃数据包
    if (receivedPower < receiverSensitivity) {
        EV_INFO << "Packet discarded: received power " << receivedPower
                << "dBm is below receiver sensitivity " << receiverSensitivity << "dBm" << endl;
        result.discard = true;
        return result;
    }

    // 计算误码率
    double ber = calculateBER(receivedPower, thermalNoise, bitRate);
    emit(bitErrorSignal, ber);

    // 计算丢包率 - 使用正确的方法获取包长度
    int packetLengthBits = msg->getByteLength() * 8; // 字节转换为比特
    if (packetLengthBits <= 0) packetLengthBits = 1024; // 默认包长

    double per = calculatePER(ber, packetLengthBits);

    // 根据丢包率决定是否丢弃数据包
    if (isPacketLost(per)) {
        EV_INFO << "Packet lost due to bit errors. BER: " << ber << ", PER: " << per << endl;
        result.discard = true;
        return result;
    }

    // 计算传输延迟
    result.delay = calculateDelay(distance);

    // 考虑多普勒效应
    // 假设发送和接收节点有相对运动
    double relativeSpeed = 0.0; // m/s

    // 如果是普通节点，假设有一定的相对速度
    if (senderType == "ordinary" || receiverModule->par("nodeType").stdstringValue() == "ordinary") {
        // 人行走速度约为1-2 m/s
        cRNG *globalRNG = cSimulation::getActiveSimulation()->getContext()->getRNG(0);
        relativeSpeed = uniform(globalRNG, 0.0, 1.5);
    }

    // 计算多普勒频移
    double dopplerFrequencyShift = calculateDopplerShift(relativeSpeed, frequency);

    // 多普勒效应对通信的影响（简化模型）
    if (fabs(dopplerFrequencyShift) > 1000) { // 如果频移超过1kHz
        // 增加BER，导致更大概率的丢包
        double increasedBer = ber * (1 + fabs(dopplerFrequencyShift) / 10000);
        double increasedPer = calculatePER(increasedBer, packetLengthBits);

        if (isPacketLost(increasedPer)) {
            EV_INFO << "Packet lost due to Doppler effect. Frequency shift: "
                    << dopplerFrequencyShift << " Hz" << endl;
            result.discard = true;
            return result;
        }
    }

    EV_INFO << "Packet successfully transmitted. Distance: " << distance
            << "m, Received power: " << receivedPower << "dBm, Delay: "
            << result.delay << "s" << endl;

    return result;
}

double WSNChannelModel::calculatePathLoss(double distance, double frequency)
{
    // 自由空间路径损耗模型
    double wavelength = 3e8 / frequency; // 光速/频率
    double referenceDistance = 1.0; // 参考距离1米

    // 参考距离处的路径损耗
    double pathLoss0 = 20 * log10(4 * M_PI * referenceDistance / wavelength);

    // 距离大于参考距离时的额外路径损耗
    double additionalLoss = 0.0;
    if (distance > referenceDistance) {
        additionalLoss = 10 * pathLossExponent * log10(distance / referenceDistance);
    }

    return pathLoss0 + additionalLoss;
}

double WSNChannelModel::calculateShadowing()
{
    // 对数正态阴影衰落
    cRNG *globalRNG = cSimulation::getActiveSimulation()->getContext()->getRNG(0);
    return normal(globalRNG, 0, shadowingStddev);
}

double WSNChannelModel::calculateDopplerShift(double relativeSpeed, double frequency)
{
    // 多普勒频移计算
    // Fd = (v/c) * f
    // v是相对速度，c是光速，f是载波频率
    double lightSpeed = 3e8; // 光速
    return (relativeSpeed / lightSpeed) * frequency;
}

double WSNChannelModel::calculateReceivedPower(double transmitPower, double pathLoss, double shadowing)
{
    // 接收功率 = 发送功率 - 路径损耗 - 阴影衰落
    return transmitPower - pathLoss - shadowing;
}

double WSNChannelModel::calculateBER(double receivedPower, double thermalNoise, double bitRate)
{
    // 功率单位从dBm转换为mW
    double rxPowerMw = pow(10, receivedPower / 10);
    double noiseMw = pow(10, thermalNoise / 10);

    // 计算信噪比
    double snr = rxPowerMw / noiseMw;

    // 对于BPSK调制，BER = 0.5 * erfc(sqrt(SNR))
    // 这里使用简化公式
    return 0.5 * exp(-snr / 2);
}

double WSNChannelModel::calculatePER(double ber, int packetLengthBits)
{
    // 丢包率 = 1 - (1 - BER)^PacketLength
    return 1.0 - pow(1.0 - ber, packetLengthBits);
}

bool WSNChannelModel::isPacketLost(double per)
{
    // 根据丢包率随机决定是否丢失数据包
    cRNG *globalRNG = cSimulation::getActiveSimulation()->getContext()->getRNG(0);
    return uniform(globalRNG, 0.0, 1.0) < per;
}

simtime_t WSNChannelModel::calculateDelay(double distance)
{
    // 无线信号以光速传播
    double propagationDelay = distance / 3e8; // 距离/光速

    // 考虑处理延迟，假设为10微秒
    double processingDelay = 10e-6;

    // 总延迟
    return SimTime(propagationDelay + processingDelay);
}

} // namespace wsn_simulation
