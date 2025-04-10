
# Compiler and flags
CC = cc
CFLAGS = -Wall -Iinc -I../Common/inc

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

# Source files
COMMON_OBJS = ../Common/obj/common.o
SRCS = $(SRC_DIR)/dp-1.c
OBJS = $(OBJ_DIR)/dp-1.o

# Output executable
TARGET = $(BIN_DIR)/dp-1
all: $(TARGET)

# Linking into the final executable
$(TARGET): $(OBJS) $(COMMON_OBJS)
	@mkdir -p $(BIN_DIR)    # Create the bin directory if it doesn't exist
	$(CC) -o $(TARGET) $(OBJS) $(COMMON_OBJS)

# Compiling .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)    # Create the obj directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@


# Clean target to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)