all:
	g++ -o ch childproc.cpp LogProcess.cpp 
	g++ -o pr parentproc.cpp MainProcess.cpp LogProcess.cpp ConnMgr.cpp StorageMgr.cpp -pthread
clean:
	rm -rf ch pr myfifo
