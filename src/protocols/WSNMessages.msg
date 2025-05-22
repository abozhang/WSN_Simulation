//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
namespace wsn_simulation;

// 基础消息
message BaseMessage
{
    int srcId;                // 源节点ID
    int destId = -1;          // 目标节点ID (-1表示广播)
    int frequency;            // 使用的频段
    int sequenceNumber;       //消息序列号
    double timestamp;         // 消息生成时间戳
    int messageType;          // 消息类型
    int hopCount = 0;         // 跳数计数
}

// 时间消息
message TimeMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    double referenceTime;     // 基准时间(ms级)
    double accuracy;          // 精度等级
    string identityInfo;      // 身份标识
}

// 校时消息
message TimeCalibrationMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    double sendTime;          // 发送时间(ns级)
    double receiveTime;       // 接收时间(ns级)
    double accuracy;          // 精度等级
    int calibrationType;      // 校准类型
}

// 态势消息
message StatusMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    double positionX;         // 地理位置X坐标
    double positionY;         // 地理位置Y坐标
    double positionZ;         // 地理位置Z坐标
    int nodeType;             // 网络责任标识 (控制/簇头/中继/普通成员)
    int status;               // 节点状态
    int messageFrequency;     // 运行频段
    double batteryLevel;      // 电池电量
}

// 态势汇聚消息
message StatusAggregationMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    int nodeCount;            // 本周期内接收的成员态势信息数量
    int activeNodes;          // 活跃节点数量
    string resourceUsage;     // 周期内资源使用情况
}

// 网络管理指令
message ManagementCommand
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    int targetId;             // 目标编识号
    int commandType;          // 网络执行标识类型
    double extensionParam;    // 延时/延时执行标识
    string commandData;       // 变更责任标识
    int responseFlag;         // 需要应答标识
}

// 簇内寻址消息
message ClusterAddressingMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    string sourceIdentity;    // 源发端标识号
    string targetIdentity;    // 目标编识号
    string addressingQuery;   // 簇内寻址查询
}

// 簇间寻址消息
message InterClusterAddressingMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    string sourceCluster;     // 源发端簇标识号
    string targetCluster;     // 目标簇标识号
    string addressingQuery;   // 簇间寻址查询
}

// 波形参数设置消息
message WaveformConfigMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    int waveformType;         // 波形参数标识 (比如: 速率档位)
    double parameterValue;    // 参数值
    int configResponse;       // 应答标记
}

// 时空同步消息
message SpaceTimeSyncMessage
{
    int srcId;
    int destId = -1;
    int frequency;
    int sequenceNumber;
    double timestamp;
    int messageType;
    int hopCount = 0;
    
    double referenceX;        // 参考X坐标
    double referenceY;        // 参考Y坐标
    double referenceZ;        // 参考Z坐标
    double referenceTime;     // 参考时间
    int syncType;             // 同步类型
}