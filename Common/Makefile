
# Compiler and flags
CC = cc
CFLAGS = -Wall -Iinc -I../Common/inc

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

# Source files
COMMON_OBJS = ./obj/common.o
SRCS = $(SRC_DIR)/common.c
OBJS = $(OBJ_DIR)/common.o

all: $(OBJS)

# Compiling .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)    # Create the obj directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@


# Clean target to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)