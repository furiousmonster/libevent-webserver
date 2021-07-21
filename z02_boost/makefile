CXX=g++

#链接时，用到boost库时 -lboost_thread  -lboost_system
BOOSTLIB= -lboost_thread 

CxxFlagesDebug=-g -w -ggdb -std=c++11 -fshort-wchar  -c

Project=execute


src=testBoost-Thread.cpp
#src=$(wildcard *.cpp)
obj=$(src:.cpp=.o)
$(Project):$(obj)
	$(CXX) -o $(Project) $(obj) $(BOOSTLIB)

%.o:%.cpp
	$(CXX) -MM $^ 
	$(CXX) $(CxxFlagesDebug) $^ -o $@


.PHONY:DotObjs
DotObjs:
	-rm -f $(obj) 
.PHONY:DotExecute
DotExecute:
	-rm -f $(Project) 
.PHONY:clean
clean:DotObjs DotExecute
————————————————
版权声明：本文为CSDN博主「liangwenhao1108」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/liangwenhao1108/article/details/105474277/