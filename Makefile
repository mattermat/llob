FLAGS := -g
FLAGS += -Iinclude
FLAGS += -MMD -MP
FLAGS += -Wall
FLAGS += -Wno-unused-result
FLAGS += -Wno-unused-function
FLAGS += -Wno-unused-variable
FLAGS += -Wno-missing-braces
FLAGS += -Wno-uninitialized
FLAGS += -Wno-shift-count-overflow
FLAGS += -Werror
FLAGS += -Winline
FLAGS += -std=gnu11
FLAGS += -march=native
# FLAGS += -O3

LIBS := -lm

DEFS := -DT1=1

SRC_DIR := src
OBJ_DIR := obj
BUILD_DIR := build
OUTPUT := $(BUILD_DIR)/main

SRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# get the source dir tree to replicate it for the OBJ dir
STRUCTURE := $(shell find $(SRC_DIR) -type d)
OBJ_DIRS := $(subst $(SRC_DIR),$(OBJ_DIR),$(STRUCTURE))
OBJ_DIRS := $(filter-out $(OBJ_DIR), $(OBJ_DIRS))

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(OBJ) | $(BUILD_DIR)
	gcc $(FLAGS) $(DEFS) $^ $(LIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc $(FLAGS) $(DEFS) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR): $(OBJ_DIRS)
	mkdir -p $@

$(OBJ_DIRS):
	mkdir -p $@

clean:
	@$(RM) -rv $(BUILD_DIR) $(OBJ_DIR)