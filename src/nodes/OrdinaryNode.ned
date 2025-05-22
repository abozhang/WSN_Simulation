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

// OrdinaryNode.ned
package wsn_simulation.nodes;

module OrdinaryNode extends NodeBase
{
    parameters:
        nodeType = "ordinary";
        @display("i=device/cellphone");
        // frequency将在初始化时设置，对应其所属簇头的频段
        isTimeBase = false; // 普通节点不是时间基准
        initialBatteryCapacity = 1000mAh; // 普通节点电池容量较小
        
    gates:
        // 继承自NodeBase的gates
}
