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

// 更新应用层模块以实现接口
package wsn_simulation.modules;

simple Application like IApplication
{
    parameters:
        string nodeType; // 节点类型 (控制/簇头/普通)
        int nodeId;      // 节点ID
        
    gates:
        input fromComm;           // 来自通信模块的消息
        output toComm;            // 发送到通信模块的消息
        input fromTimeSync;       // 来自时间同步模块的消息
        output toTimeSync;        // 发送到时间同步模块的消息
        output toBattery;         // 向电池模块报告能耗
}
