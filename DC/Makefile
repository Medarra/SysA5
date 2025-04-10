# Compiler and flags
CC = cc
CFLAGS = -Wall -Iinc

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/dc.c
OBJS = $(OBJ_DIR)/dc.o

# Output executable
TARGET = $(BIN_DIR)/dc
all: $(TARGET)

# Linking into the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)    # Create the bin directory if it doesn't exist
	$(CC) -o $(TARGET) $(OBJS) -lm

# Compiling .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)    # Create the obj directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@


# Clean target to remove object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)