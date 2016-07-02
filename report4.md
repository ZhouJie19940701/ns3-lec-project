ns3-lec-project
========
project4
-----------
##一、实验目的
1、搭建拓扑：两个wifi网络，各有6个sta，1个ap；实现两个wifi网络之间的通信  
2、场景范围：500m*500m  
3、传播模型为：  
	ConstantSpeedPropagationDelayModel  
4、Mac类型为：NqosWifiMacHelper（没有QoS保障的Mac层机制）  
5、移动模型为随机游走模型，设定初始位置为两个六边形，边长10m，sta在6个顶点上，ap在六边形中间；两个六边形中心相距50m  
6、packet size为512byte，包间隔为100ms  
7、使用PyViz查看仿真并截图  
8、自行设定自变量，使用Tracing系统记录数据，通过Matlab处理数据并画图  
##二、实验展示
####拓扑图
![拓扑图](http://ww1.sinaimg.cn/mw690/ea098a20gw1f5fon7ofe2j20nn06675q.jpg "拓扑图")  
####1、PyViz截图
####1.1拓扑图
![PyViz截图](http://ww2.sinaimg.cn/mw690/ea098a20gw1f5fon86d11j20k90h9whh.jpg "PyViz截图")  
####1.2运动图
![运动效果图](http://ww3.sinaimg.cn/mw690/ea098a20gw1f5fsp3ka33j20hv0dfgna.jpg "运动效果图")  
####2、通信截图
![通信截图](http://ww2.sinaimg.cn/mw690/ea098a20gw1f5fon8tsgwj20j70bwqb1.jpg "通信截图")  
####3、matlab表格及图像的截图
####3.1matlab表格
![表格](http://ww1.sinaimg.cn/mw690/ea098a20gw1f5fs4ed35ij20fk0bo0t3.jpg "表格")  
####3.2matlab图像
![曲线图像](http://ww2.sinaimg.cn/mw690/ea098a20gw1f5fs4ewzpej20fk0bodfz.jpg "曲线图像")  
图中所研究的是客户端从发送到接收时的往返时延差，可以看出，当通信不经过P2P信道时（如n2～n7等6个站点），往返时延差明显较小。
以下为matlab代码：  
a=[3,4,5,6,7,8,13,14,15,16,17,18];  
b=[3.0340,4.0161,5.0179,6.0170,7.0170,8.0171,13.0058,14.0071,15.0069,16.0085,17.0154,18.0141];  
data=[a',b',b'-a'];  
figure(1),plot([13 : -1 : 2],b'-a','b*-');  
xlabel('站点号');  
ylabel('时间/s');  
title('客户端往返时延/s');  
colnames = {'发送时间点/s', '接收时间点/s','往返时延差/s'};  
rnames = {'站点2','站点3','站点4','站点5','站点6','站点7','站点8','站点9','站点10','站点11','站点12','站点13'};  
f = figure(2);  
t = uitable(f, 'Data',data,'ColumnName',colnames, 'RowName',rnames);  
##三、抓包文件
在相关文档里。


