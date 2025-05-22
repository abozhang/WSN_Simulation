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

// modules/Mobility.ned
package wsn_simulation.modules;

simple Mobility
{
    parameters:
        double initialX @unit(m);  // 初始X坐标
        double initialY @unit(m);  // 初始Y坐标
        double initialZ @unit(m);  // 初始Z坐标
        bool isMobile = default(false); // 是否移动
        string mobilityType = default("static"); // 移动模型类型
}
