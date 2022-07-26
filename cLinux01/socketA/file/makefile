all:fileclient fileserver

fileclient:fileclient.cpp
	g++ -g -o fileclient fileclient.cpp ../_freecplus.cpp

fileserver:fileserver.cpp
	g++ -g -o fileserver fileserver.cpp ../_freecplus.cpp

clean:
	rm -rf fileclient fileserver