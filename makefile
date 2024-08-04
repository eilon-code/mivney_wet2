# Compiler settings
CC = gcc
CXX = g++

# Directories
DIR = code/
GAME_DIR = $(DIR)game/
DATA_DIR = $(DIR)data_structures/
BUILD_DIR = build/

# Source files
MAIN_SRC = main24b2.cpp
OCEAN_SRC = $(DIR)pirates24b2.cpp

# Object files
MAIN_OBJ = $(BUILD_DIR)wet2.o
TREE_OBJ = $(BUILD_DIR)tree.o

# All object files
OBJS = $(MAIN_OBJ) $(TREE_OBJ)

# Executable name
EXEC = prog

# Flags
DEBUG_FLAG = # Leave empty, assign -g for debug
COMP_FLAG = -std=c++11 -Wall -Werror
INCLUDE_FLAGS = -I$(DIR)

# Default target
$(EXEC): $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $(BUILD_DIR)$@

# Main object file
$(MAIN_OBJ): $(MAIN_SRC) wet2util.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $(INCLUDE_FLAGS) $< -o $@

# Ocean object file
$(TREE_OBJ): $(OCEAN_SRC) $(DIR)pirates24b2.h wet2util.h $(GAME_DIR)pirate.h $(GAME_DIR)fleet.h $(DATA_DIR)linked_list.h $(DATA_DIR)hash_table.h $(DATA_DIR)up_tree.h | $(BUILD_DIR)
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $(INCLUDE_FLAGS) $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean build directory
clean:
	@rm -rf $(BUILD_DIR)
