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

// modules/Battery.ned
package wsn_simulation.modules;

simple Battery
{
    parameters:
        double initialCapacity @unit(mAh); // 初始电池容量
        double voltage @unit(V) = default(3.3V); // 电池电压
        
    gates:
        input energyConsumption; // 接收能耗报告
}
