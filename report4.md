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
####PyViz截图
![PyViz截图](http://ww2.sinaimg.cn/mw690/ea098a20gw1f5fon86d11j20k90h9whh.jpg "PyViz截图")  
####通信截图
![通信截图](http://ww2.sinaimg.cn/mw690/ea098a20gw1f5fon8tsgwj20j70bwqb1.jpg "通信截图")  
##三、抓包文件
在相关文档里。


