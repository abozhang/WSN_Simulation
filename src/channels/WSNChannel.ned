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

// WSNChannel.ned
package wsn_simulation.channels;

import ned.IdealChannel;

channel WSNChannel extends ned.DelayChannel
{
    @class(WSNChannelModel);
    @signal[receiveBegin](type=cMessage);
    @signal[receiveEnd](type=cMessage);
    @statistic[bitError](title="bit error rate"; source=bitErrorRate; record=vector,histogram; interpolationmode=none);
    @statistic[propagationLoss](title="propagation loss (dB)"; source=propagationLoss; record=vector,histogram; interpolationmode=none);
    
    // 通道参数
    double distance @unit(m) = default(0m);         // 发送和接收节点间的距离
    double pathLossExponent = default(3.5);          // 路径损耗指数 (通常在2-4之间)
    double shadowingStddev = default(4.0);           // 阴影衰落标准差 (dB)
    double carrierFrequency @unit(Hz) = default(433MHz); // 载波频率
    double receiverSensitivity @unit(dBm) = default(-100dBm); // 接收灵敏度
    double fadingMargin @unit(dB) = default(10dB);   // 衰落余量
    double dopplerShift @unit(Hz) = default(0Hz);    // 多普勒频移
    double thermalNoise @unit(dBm) = default(-110dBm); // 热噪声功率
    double bitRate @unit(bps) = default(250kbps);    // 传输速率
    double maxDistance @unit(m) = default(500m);     // 最大通信距离
}
