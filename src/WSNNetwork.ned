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

// WSNNetwork.ned - 更新版本
package wsn_simulation;

import wsn_simulation.nodes.*;
import wsn_simulation.channels.*;

network WSNNetwork
{
    parameters:
        int numClusters = default(5);             // 簇的数量
        int nodesPerCluster = default(200);       // 每个簇的节点数量
        double playgroundSizeX @unit(m) = default(1000m); // 仿真场景X尺寸
        double playgroundSizeY @unit(m) = default(1000m); // 仿真场景Y尺寸
        @display("bgb=$playgroundSizeX,$playgroundSizeY");
        
        // 人类移动模型参数
        bool enableMobility = default(true);     // 是否启用移动性
        string mobilityType = default("humanMobility"); // 移动模型类型
        
        // 通道模型参数
        double pathLossExponent = default(3.5);        // 路径损耗指数
        double shadowingStddev = default(4.0);         // 阴影衰落标准差
        double maxCommunicationRange @unit(m) = default(500m); // 最大通信距离
        
    submodules:
        // 控制节点 (网络中仅有一个)
        controlNode: ControlNode {
            parameters:
                @display("p=500,100;i=device/antennatower;is=s");
                transmitPower = 500mW; // 控制节点发射功率更大
        }
        
        // 簇头节点 (根据numClusters参数创建)
        clusterHeads[numClusters]: ClusterHeadNode {
            parameters:
                @display("i=device/accesspoint;is=s");
                transmitPower = 200mW; // 簇头节点发射功率适中
        }
        
        // 普通节点 (根据numClusters和nodesPerCluster参数创建)
        ordinaryNodes[numClusters*nodesPerCluster]: OrdinaryNode {
            parameters:
                @display("i=device/cellphone;is=s");
                isMobile = enableMobility;
                mobilityType = mobilityType;
        }
        
    connections allowunconnected:
        // 通过无线信道通信，所以这里不需要显式定义连接
}
