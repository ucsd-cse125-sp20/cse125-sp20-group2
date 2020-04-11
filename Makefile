# https://stackoverflow.com/questions/3774568/makefile-issue-smart-way-to-scan-directory-tree-for-c-files
# Notes:
# 	$(LIB) must be last after list of objects

OUT = ggee
SRC_DIR = src
HDR_DIR = src

CC = g++
LIB = -lws2_32 -lwsock32 
SRC := $(shell find $(SRC_DIR) -name "*.cpp")
HDR := $(shell find $(HDR_DIR) -name "*.h")

all: $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIB)

# client: client.cpp
# 	g++ -lwsock32 client.cpp -lws2_32 -o client.exe
# server: server.cpp
# 	g++ -lwsock32 server.cpp -lws2_32 -o server.exe