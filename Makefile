# https://stackoverflow.com/questions/3774568/makefile-issue-smart-way-to-scan-directory-tree-for-c-files
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# Notes:
# 	$(LIB) must be last after list of objects when building
# 	When modifying header files, be sure to make clean

OUT = gg
SRC_DIR = src
HDR_DIR = src
OUT_DIR = build
BIN_DIR = bin

CC = g++
FLAG = -Wall -pthread -Isrc
LIBS = -lws2_32 -lwsock32 -lprotobuf
SRC := $(shell find $(SRC_DIR) -name "*.cpp")
HDR := $(shell find $(HDR_DIR) -name "*.h")
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OUT_DIR)/%.o) # Replace variables src/%.cpp to out/%.o in SRC variable

BINARY := $(BIN_DIR)/$(OUT)

default: $(BINARY)

$(BINARY): $(OBJ)
	mkdir -p $(shell dirname $@)
	$(CC) $(FLAG) $^ -o $@ $(LIBS)

# $(OBJ): $(SRC) -- $< only refers to first class in this case
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OUT_DIR) $(BIN_DIR)