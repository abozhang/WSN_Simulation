// WSNUtils.h
#ifndef __WSN_SIMULATION_WSN_UTILS_H
#define __WSN_SIMULATION_WSN_UTILS_H

#include <omnetpp.h>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

using namespace omnetpp;

namespace wsn_simulation {

/**
 * 工具类，提供各种通用函数
 */
class WSNUtils
{
public:
    /**
     * 计算两点之间的欧几里得距离
     * @param x1 第一点X坐标
     * @param y1 第一点Y坐标
     * @param z1 第一点Z坐标
     * @param x2 第二点X坐标
     * @param y2 第二点Y坐标
     * @param z2 第二点Z坐标
     * @return 两点之间的距离
     */
    static double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2);

    /**
     * 计算两点之间的2D距离（忽略Z坐标）
     * @param x1 第一点X坐标
     * @param y1 第一点Y坐标
     * @param x2 第二点X坐标
     * @param y2 第二点Y坐标
     * @return 两点之间的2D距离
     */
    static double calculateDistance2D(double x1, double y1, double x2, double y2);

    /**
     * 计算信号强度
     * @param transmitPower 发射功率 (dBm)
     * @param distance 距离 (m)
     * @param frequency 频率 (Hz)
     * @param pathLossExponent 路径损耗指数
     * @param shadowingStdDev 阴影衰落标准差
     * @return 接收功率 (dBm)
     */
    static double calculateSignalStrength(double transmitPower, double distance, double frequency,
                                         double pathLossExponent, double shadowingStdDev);

    /**
     * 计算两点之间的角度（弧度）
     * @param x1 第一点X坐标
     * @param y1 第一点Y坐标
     * @param x2 第二点X坐标
     * @param y2 第二点Y坐标
     * @return 角度（弧度），范围[0, 2π)
     */
    static double calculateAngle(double x1, double y1, double x2, double y2);

    /**
     * 计算两个方向向量之间的角度（弧度）
     * @param dir1X 第一个方向向量X分量
     * @param dir1Y 第一个方向向量Y分量
     * @param dir2X 第二个方向向量X分量
     * @param dir2Y 第二个方向向量Y分量
     * @return 角度（弧度），范围[0, π]
     */
    static double calculateAngleBetweenVectors(double dir1X, double dir1Y, double dir2X, double dir2Y);

    /**
     * 归一化向量
     * @param x 向量X分量
     * @param y 向量Y分量
     * @param z 向量Z分量
     * @return 归一化后的向量 {x, y, z}
     */
    static std::vector<double> normalizeVector(double x, double y, double z);

    /**
     * 归一化2D向量
     * @param x 向量X分量
     * @param y 向量Y分量
     * @return 归一化后的向量 {x, y}
     */
    static std::vector<double> normalizeVector2D(double x, double y);

    /**
     * 生成高斯随机数
     * @param mean 均值
     * @param stdDev 标准差
     * @return 高斯随机数
     */
    static double gaussianRandom(double mean, double stdDev);

    /**
     * 生成指数分布随机数
     * @param lambda 参数λ
     * @return 指数分布随机数
     */
    static double exponentialRandom(double lambda);

    /**
     * 计算误码率(BER)，基于BPSK调制
     * @param SNR 信噪比(线性值，非dB)
     * @return 误码率
     */
    static double calculateBER(double SNR);

    /**
     * 计算丢包率(PER)
     * @param BER 误码率
     * @param packetLength 数据包长度(比特)
     * @return 丢包率
     */
    static double calculatePER(double BER, int packetLength);

    /**
     * 基于丢包率决定是否丢弃数据包
     * @param PER 丢包率
     * @return 是否丢包
     */
    static bool isPacketLost(double PER);

    /**
     * dBm转换为mW
     * @param dBm dBm值
     * @return mW值
     */
    static double dBmToMw(double dBm);

    /**
     * mW转换为dBm
     * @param mW mW值
     * @return dBm值
     */
    static double mWTodBm(double mW);

    /**
     * 计算多普勒频移
     * @param relativeSpeed 相对速度(m/s)
     * @param frequency 载波频率(Hz)
     * @return 多普勒频移(Hz)
     */
    static double calculateDopplerShift(double relativeSpeed, double frequency);

    /**
     * 计算相对速度
     * @param speed1 第一个节点速度
     * @param dir1X 第一个节点方向X分量
     * @param dir1Y 第一个节点方向Y分量
     * @param speed2 第二个节点速度
     * @param dir2X 第二个节点方向X分量
     * @param dir2Y 第二个节点方向Y分量
     * @return 相对速度(m/s)
     */
    static double calculateRelativeSpeed(double speed1, double dir1X, double dir1Y,
                                       double speed2, double dir2X, double dir2Y);

    /**
     * 计算传播延迟
     * @param distance 距离(m)
     * @return 延迟(s)
     */
    static double calculatePropagationDelay(double distance);

    /**
     * 计算时钟偏差
     * @param clockDrift 时钟漂移率(ppm)
     * @param timeInterval 时间间隔(s)
     * @return 时钟偏差(s)
     */
    static double calculateClockOffset(double clockDrift, double timeInterval);

    /**
     * 获取当前系统时间
     * @return 当前时间(s)
     */
    static double getCurrentTime();

    /**
     * 将整数转换为二进制字符串
     * @param value 整数值
     * @param length 字符串长度(位数)
     * @return 二进制字符串
     */
    static std::string intToBinaryString(int value, int length);

    /**
     * 将二进制字符串转换为整数
     * @param binaryString 二进制字符串
     * @return 整数值
     */
    static int binaryStringToInt(const std::string& binaryString);

    /**
     * 计算数组平均值
     * @param values 数值数组
     * @return 平均值
     */
    static double calculateAverage(const std::vector<double>& values);

    /**
     * 计算数组标准差
     * @param values 数值数组
     * @param mean 平均值(如果为负数则自动计算)
     * @return 标准差
     */
    static double calculateStdDev(const std::vector<double>& values, double mean = -1.0);

    /**
     * 限制值在指定范围内
     * @param value 输入值
     * @param min 最小值
     * @param max 最大值
     * @return 限制后的值
     */
    static double clamp(double value, double min, double max);

    /**
     * 检查点是否在矩形区域内
     * @param x 点X坐标
     * @param y 点Y坐标
     * @param x1 矩形左下角X坐标
     * @param y1 矩形左下角Y坐标
     * @param x2 矩形右上角X坐标
     * @param y2 矩形右上角Y坐标
     * @return 是否在矩形内
     */
    static bool isPointInRectangle(double x, double y, double x1, double y1, double x2, double y2);

    /**
     * 计算字符串的CRC32校验码
     * @param data 输入字符串
     * @return CRC32校验码
     */
    static uint32_t calculateCRC32(const std::string& data);

    /**
     * 生成UUID字符串
     * @return UUID字符串
     */
    static std::string generateUUID();

    /**
     * 格式化日志消息
     * @param module 模块名
     * @param message 消息内容
     * @return 格式化后的日志消息
     */
    static std::string formatLogMessage(const std::string& module, const std::string& message);

    /**
     * 解析16进制字符串为字节数组
     * @param hexString 16进制字符串
     * @return 字节数组
     */
    static std::vector<uint8_t> hexStringToBytes(const std::string& hexString);

    /**
     * 将字节数组转换为16进制字符串
     * @param bytes 字节数组
     * @return 16进制字符串
     */
    static std::string bytesToHexString(const std::vector<uint8_t>& bytes);
};

} // namespace wsn_simulation

#endif // __WSN_SIMULATION_WSN_UTILS_H
