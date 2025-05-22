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

// ControlNode.ned
package wsn_simulation.nodes;

module ControlNode extends NodeBase
{
    parameters:
        nodeType = "control";
        @display("i=device/antennatower");
        frequency = -1; // 控制节点支持所有频段
        isTimeBase = true; // 控制节点是时间基准
        initialBatteryCapacity = 5000mAh; // 控制节点电池容量更大
        
    gates:
        // 继承自NodeBase的gates
}

