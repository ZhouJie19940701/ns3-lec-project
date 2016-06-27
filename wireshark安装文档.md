wireshark安装教程：
--------
####安装编译工具：  
　　	$sudo apt-get install build-essential  
为了成功编译Wireshark，您需要安装GTK+的开发文件和GLib库(libraries)。  
	$sudo apt-get install libgtk2.0-dev libglib2.0-dev  
	
安装Checkinstall以便管理您系统中直接由源代码编译安装的软件。  
  　　$sudo apt-get install checkinstall  
  　　
wireshark源码下载地址：http://www.wireshark.org/download.html（页面中的source code）  
下载后的文件名：  
	wireshark-1.2.2.tar.bz2  
	
cd到文件目录解压：  
	$tar -xvf wireshark-1.2.2.tar.bz2  
	$cd wireshark-1.2.2  
	
编译安装命令如下：  
	$./configure  
	$make  
	$sudo make install  
其中make编译时间会比较长，这样下来就基本安装了。  

在进行./configure编译过程中可能会出现三个问题：  

####问题1、 $./configure执行到最后出错之一  
出现:  
	Could not run GTK+ test program, checking why...***  
	The test program failed to compile or link. See the file config.log for the*** 	
	exact error that occured. This usually means GTK+ is incorrectly installed.configure:   
	error: GTK+ 3 is not available  
####解决：  
	首先确定之前安装的GTK+的开发文件和GLib库是成功安装上的。若成功安装上，则，使用一下命令：  
	$./config  --with-gtk2  
	
####问题2、 $./configure执行到最后出错之二
出现：  
	checking for perl... /usr/bin/perl  
	checking for bison... no  
	checking for byacc... no  
	checking for yacc... no  
	configure: error: I couldn't find yacc (or bison or ...); make sure it's installed and in your path  
####解决：
	执行以下命令：  
	$sudo apt-get install flex bison  
稍作解释：yacc(Yet Another Compiler Compiler)，是Unix/Linux上一个用来生成编译器的编译器（编译器代码生成器）。  

####问题3、$./configure执行到最后出错之三
出现：  
    configure: error: Header file pcap.h not found;   
    if you installed libpcap from source,   
    did you also do "make install-incl", and if you   
    installed a binary package of libpcap, is there also a developer's package of libpcap,  
    and did you also install that package?  
####解决：  
	编译安装libpcap。  
	在www.tcpdump.org页面中可下载源码：libpcap-1.0.0.tar.gz  
	cd到文件目录后，执行以下命令：  
	 $tar -xvf libpcap-1.0.0.tar.gz  
	$cd libpcap-1.0.0.tar.gz  
	$./configure  
	$make  
	$sudo make install  
	
###二、启动
	问题解决后，按照以上步骤就把 wireshark 安装好了，接下来就是启动 wireshark 了。  
	进入wireshark-1.2.2，输入命令：  
	$sudo ./wireshark  
	这里如果不用sudo，则wireshark找不到网络设备接口，这主要与权限有关，启动时注意下就行。  
	
	
	http://www.cnblogs.com/0616--ataozhijia/p/3670898.html
	

	
	



