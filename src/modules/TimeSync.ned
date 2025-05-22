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

// modules/TimeSync.ned
package wsn_simulation.modules;

simple TimeSync
{
    parameters:
        string nodeType;      // 节点类型 (控制/簇头/普通)
        bool isTimeBase;      // 是否作为时间基准
        double clockDrift @unit(ppm) = default(100ppm); // 时钟漂移率
        double syncInterval @unit(s) = default(60s);    // 同步间隔
        
    gates:
        input fromApp;        // 来自应用层的消息
        output toApp;         // 发送到应用层的消息
        output toBattery;     // 向电池模块报告能耗
}
