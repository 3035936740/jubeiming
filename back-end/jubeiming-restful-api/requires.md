# 这个是做什么的?  
这个是告诉你C++的依赖环境的  

## 环境
环境使用Ubuntu22.04

编译请务必先安装以下内容
gcc-12, asio, cmake, ninja

<pre><code lang="bash">$ sudo apt install gcc cmake ninja-build
$ sudo apt install libasio-dev
</code></pre>

使用的是gcc12望周知  
如何切换版本?  
找到项目中bash/chvers.sh  
在wsl的linux环境中运行下面指令  
`$ sudo ./chvers.sh gcc 12`  

使用的三方库呢?  
webapi的核心库: [crow](https://github.com/CrowCpp/Crow)


## 如何编译呢?
双击exe完成安装,成功了(bushi  
我已经给你了鸡蛋,牛奶,黄油 剩下的自己去做吧!