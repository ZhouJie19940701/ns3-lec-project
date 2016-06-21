ns3-lec-project
========
project1：输入姓名、学号、频率，并用 wc 统计输出次数，用 grep 做适当的的筛选

===========================

###实验总的效果图：
![实验效果图](http://ww4.sinaimg.cn/mw1024/ea098a20gw1f4rebp7q2ij20k00b143u.jpg "实验效果图")
####从命令行输入姓名、学号、频率。命令如下：
./waf --run="scratch/project1 zj 2013010901020 5 "  
因为频率是5，而时间设置为3，所以一共输出15个消息。
###wc实验效果图
![wc效果图](http://ww3.sinaimg.cn/mw1024/ea098a20gw1f4rebpiasfj20k2041ace.jpg "wc效果图")
####从命令行输入姓名、学号、频率以及wc命令，并将标准错误指向空，即不输出标准错误。命令如下：
./waf --run="scratch/project1 zj 2013010901020 5" | wc -l 2>>null  
正如上面所说，一共输出了15次。
###grep实验效果图
![grep效果图](http://ww2.sinaimg.cn/mw1024/ea098a20gw1f4reboyh9pj20k004lq5x.jpg "grep效果图")
####从命令行输入姓名、学号、频率以及wc命令，并将输入筛选条件。命令如下：
./waf --run="scratch/project1 zj 2013010901020 5" | grep +18  
由于只筛选时间为18的，故输出相关行信息。
