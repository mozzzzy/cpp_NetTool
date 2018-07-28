EXECUTE_FILE = DEBUG_MAIN.exe
SOURCE = NetTool.cc
I = -I /usr/local/Cellar/openssl/1.0.2k/include
l = -lssl

$(EXECUTE_FILE): $(SOURCE)
	g++ -DDEBUG -g -o $(EXECUTE_FILE) $I $l $(SOURCE)

clean:
	rm $(EXECUTE_FILE)

remake: clean $(EXECUTE_FILE)

test: $(EXECUTE_FILE)
	./$(EXECUTE_FILE)
