// WSNUtils.cc
#include "WSNUtils.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace wsn_simulation;

// 初始化随机数生成器
static std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

double WSNUtils::calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2) + std::pow(z2 - z1, 2));
}

double WSNUtils::calculateDistance2D(double x1, double y1, double x2, double y2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

double WSNUtils::calculateSignalStrength(double transmitPower, double distance, double frequency,
                                       double pathLossExponent, double shadowingStdDev)
{
    // 自由空间路径损耗 + 多路径衰落
    double wavelength = 3e8 / frequency; // 光速/频率
    double referenceDistance = 1.0; // 参考距离1米

    // 参考距离处的路径损耗
    double pathLoss0 = 20 * std::log10(4 * M_PI * referenceDistance / wavelength);

    // 距离大于参考距离时的额外路径损耗
    double additionalLoss = 0.0;
    if (distance > referenceDistance) {
        additionalLoss = 10 * pathLossExponent * std::log10(distance / referenceDistance);
    }

    // 阴影衰落
    double shadowing = gaussianRandom(0, shadowingStdDev);

    // 接收功率 = 发送功率 - 路径损耗 - 阴影衰落
    return transmitPower - pathLoss0 - additionalLoss - shadowing;
}

double WSNUtils::calculateAngle(double x1, double y1, double x2, double y2)
{
    double angle = std::atan2(y2 - y1, x2 - x1);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle;
}

double WSNUtils::calculateAngleBetweenVectors(double dir1X, double dir1Y, double dir2X, double dir2Y)
{
    double dotProduct = dir1X * dir2X + dir1Y * dir2Y;
    double mag1 = std::sqrt(dir1X * dir1X + dir1Y * dir1Y);
    double mag2 = std::sqrt(dir2X * dir2X + dir2Y * dir2Y);

    if (mag1 == 0 || mag2 == 0) {
        return 0;
    }

    double cosAngle = dotProduct / (mag1 * mag2);
    // 确保cosAngle在[-1, 1]范围内，避免浮点误差导致的问题
    cosAngle = std::min(1.0, std::max(-1.0, cosAngle));

    return std::acos(cosAngle);
}

std::vector<double> WSNUtils::normalizeVector(double x, double y, double z)
{
    double length = std::sqrt(x * x + y * y + z * z);

    if (length == 0) {
        return {0, 0, 0};
    }

    return {x / length, y / length, z / length};
}

std::vector<double> WSNUtils::normalizeVector2D(double x, double y)
{
    double length = std::sqrt(x * x + y * y);

    if (length == 0) {
        return {0, 0};
    }

    return {x / length, y / length};
}

double WSNUtils::gaussianRandom(double mean, double stdDev)
{
    std::normal_distribution<double> distribution(mean, stdDev);
    return distribution(rng);
}

double WSNUtils::exponentialRandom(double lambda)
{
    std::exponential_distribution<double> distribution(lambda);
    return distribution(rng);
}

double WSNUtils::calculateBER(double SNR)
{
    // 对于BPSK调制，BER = 0.5 * erfc(sqrt(SNR))
    // 这里使用简化公式
    return 0.5 * std::exp(-SNR / 2);
}

double WSNUtils::calculatePER(double BER, int packetLength)
{
    // 丢包率 = 1 - (1 - BER)^PacketLength
    return 1.0 - std::pow(1.0 - BER, packetLength);
}

bool WSNUtils::isPacketLost(double PER)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(rng) < PER;
}

double WSNUtils::dBmToMw(double dBm)
{
    return std::pow(10, dBm / 10);
}

double WSNUtils::mWTodBm(double mW)
{
    if (mW <= 0) {
        return -std::numeric_limits<double>::infinity();
    }
    return 10 * std::log10(mW);
}

double WSNUtils::calculateDopplerShift(double relativeSpeed, double frequency)
{
    // 多普勒频移计算
    // Fd = (v/c) * f
    // v是相对速度，c是光速，f是载波频率
    double lightSpeed = 3e8; // 光速
    return (relativeSpeed / lightSpeed) * frequency;
}

double WSNUtils::calculateRelativeSpeed(double speed1, double dir1X, double dir1Y,
                                     double speed2, double dir2X, double dir2Y)
{
    // 归一化方向向量
    auto dir1 = normalizeVector2D(dir1X, dir1Y);
    auto dir2 = normalizeVector2D(dir2X, dir2Y);

    // 计算两个速度向量
    double v1x = speed1 * dir1[0];
    double v1y = speed1 * dir1[1];
    double v2x = speed2 * dir2[0];
    double v2y = speed2 * dir2[1];

    // 计算相对速度向量
    double relVx = v2x - v1x;
    double relVy = v2y - v1y;

    // 相对速度大小
    return std::sqrt(relVx * relVx + relVy * relVy);
}

double WSNUtils::calculatePropagationDelay(double distance)
{
    // 无线信号以光速传播
    return distance / 3e8; // 距离/光速
}

double WSNUtils::calculateClockOffset(double clockDrift, double timeInterval)
{
    // 时钟偏差 = 漂移率 * 时间间隔
    // 漂移率单位为ppm (parts per million)
    return (clockDrift / 1e6) * timeInterval;
}

double WSNUtils::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / 1e6;
}

std::string WSNUtils::intToBinaryString(int value, int length)
{
    std::string result;
    for (int i = length - 1; i >= 0; i--) {
        result += ((value >> i) & 1) ? '1' : '0';
    }
    return result;
}

int WSNUtils::binaryStringToInt(const std::string& binaryString)
{
    int result = 0;
    for (char c : binaryString) {
        result = (result << 1) | (c == '1' ? 1 : 0);
    }
    return result;
}

double WSNUtils::calculateAverage(const std::vector<double>& values)
{
    if (values.empty()) {
        return 0.0;
    }

    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }

    return sum / values.size();
}

double WSNUtils::calculateStdDev(const std::vector<double>& values, double mean)
{
    if (values.empty()) {
        return 0.0;
    }

    if (mean < 0) {
        mean = calculateAverage(values);
    }

    double sumSquaredDiff = 0.0;
    for (double value : values) {
        double diff = value - mean;
        sumSquaredDiff += diff * diff;
    }

    return std::sqrt(sumSquaredDiff / values.size());
}

double WSNUtils::clamp(double value, double min, double max)
{
    return std::min(max, std::max(min, value));
}

bool WSNUtils::isPointInRectangle(double x, double y, double x1, double y1, double x2, double y2)
{
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

uint32_t WSNUtils::calculateCRC32(const std::string& data)
{
    static const uint32_t crc32_table[] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        // ... (省略部分表项)
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
    };

    uint32_t crc = 0xFFFFFFFF;

    for (char c : data) {
        uint8_t byte = static_cast<uint8_t>(c);
        crc = (crc >> 8) ^ crc32_table[(crc ^ byte) & 0xFF];
    }

    return crc ^ 0xFFFFFFFF;
}

std::string WSNUtils::generateUUID()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;

    // 生成UUID格式：xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    // 其中y是8、9、A或B中的一个，表示变种
    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";

    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";

    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-" << dis2(gen);

    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";

    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }

    return ss.str();
}

std::string WSNUtils::formatLogMessage(const std::string& module, const std::string& message)
{
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] ";
    ss << "[" << module << "] ";
    ss << message;

    return ss.str();
}

std::vector<uint8_t> WSNUtils::hexStringToBytes(const std::string& hexString)
{
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hexString.length(); i += 2) {
        if (i + 1 >= hexString.length()) {
            break;
        }

        std::string byteString = hexString.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

std::string WSNUtils::bytesToHexString(const std::vector<uint8_t>& bytes)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (uint8_t byte : bytes) {
        ss << std::setw(2) << static_cast<int>(byte);
    }

    return ss.str();
}
