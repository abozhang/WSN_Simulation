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

// NodeBase.ned - 更新版本
package wsn_simulation.nodes;

import wsn_simulation.modules.*;

module NodeBase
{
    parameters:
        string nodeType;              // 节点类型 (control/clusterHead/ordinary)
        string displayString;         // 显示字符串
        int nodeId;                   // 节点ID
        int frequency = default(-1);  // 频段ID (-1表示支持所有频段)
        bool isTimeBase = default(false); // 是否作为时间基准
        double initialBatteryCapacity @unit(mAh) = default(1000mAh); // 初始电池容量
        double x @unit(m);            // X坐标
        double y @unit(m);            // Y坐标
        double z @unit(m) = default(0m); // Z坐标
        bool isMobile = default(false);  // 是否移动
        string mobilityType = default("static"); // 移动模型类型
        double transmitPower @unit(mW) = default(100mW); // 发射功率
        
    gates:
        input radioIn @directIn;      // 无线电输入接口
        
    submodules:
        // 应用层模块 - 根据节点类型动态选择
        application: <nodeType + "Application"> like IApplication {
            parameters:
                @display("p=100,50");
        }
        
        communication: Communication {
            parameters:
                nodeType = nodeType;
                supportedFrequencies = frequency;
                transmitPower = transmitPower;
                @display("p=100,150");
        }
        
        timeSync: TimeSync {
            parameters:
                nodeType = nodeType;
                isTimeBase = isTimeBase;
                @display("p=200,50");
        }
        
        // 移动模型 - 根据是否移动选择不同实现
        mobility: <mobilityType == "humanMobility" && isMobile ? "HumanMobility" : "Mobility"> {
            parameters:
                initialX = x;
                initialY = y;
                initialZ = z;
                isMobile = isMobile;
                mobilityType = mobilityType;
                @display("p=200,150");
        }
        
        battery: Battery {
            parameters:
                initialCapacity = initialBatteryCapacity;
                @display("p=300,100");
        }
        
    connections:
        radioIn --> communication.radioIn;
        
        application.toComm --> communication.fromApp;
        communication.toApp --> application.fromComm;
        
        application.toTimeSync --> timeSync.fromApp;
        timeSync.toApp --> application.fromTimeSync;
        
        communication.toBattery --> battery.energyConsumption;
        timeSync.toBattery --> battery.energyConsumption;
        application.toBattery --> battery.energyConsumption;
}
