DEBUG_MAIN.exe: NetTool.cc
	g++ -g -DDEBUG -o DEBUG_MAIN.exe NetTool.cc

clean:
	rm DEBUG_MAIN.exe
