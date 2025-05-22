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

// HumanMobility.ned
package wsn_simulation.modules;

simple HumanMobility
{
    parameters:
        double initialX @unit(m);  // 初始X坐标
        double initialY @unit(m);  // 初始Y坐标
        double initialZ @unit(m);  // 初始Z坐标
        bool isMobile = default(false); // 是否移动
        string mobilityType = default("humanMobility"); // 移动模型类型: "static", "randomWaypoint", "randomDirection", "gaussMarkov", "humanMobility"
        
        // 人类移动特性参数
        double walkSpeed @unit(mps) = default(1.2mps);  // 步行速度 (m/s)
        double runSpeed @unit(mps) = default(3.0mps);   // 快走/跑步速度 (m/s)
        double pauseTimeMean @unit(s) = default(30s);   // 停顿时间均值 (s)
        double pauseTimeStdDev @unit(s) = default(10s); // 停顿时间标准差 (s)
        double stateChangeProbability = default(0.1);   // 状态改变概率
        double randomDestRadius @unit(m) = default(50m); // 随机目的地半径
        double waypointProximity @unit(m) = default(1m); // 到达目标点的最小距离
        
        // 统计信号
        @signal[positionX](type=double);
        @signal[positionY](type=double);
        @signal[positionZ](type=double);
        @signal[speed](type=double);
        @signal[state](type=int);
        
        @statistic[positionX](title="X position"; record=vector,stats; interpolationmode=linear);
        @statistic[positionY](title="Y position"; record=vector,stats; interpolationmode=linear);
        @statistic[positionZ](title="Z position"; record=vector,stats; interpolationmode=linear);
        @statistic[speed](title="Speed"; record=vector,stats; interpolationmode=linear);
        @statistic[state](title="Mobility state"; record=vector,stats; interpolationmode=none);
        
        @display("i=block/cogwheel");
}
