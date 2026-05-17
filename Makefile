CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -fstack-usage -std=c99
# CFLAGS = -g -Wall -Wextra -Werror -Wpedantic -fstack-usage -std=c99
INCLUDES = -I./include/

SRC_DIR = src
OBJ_DIR = objs
INC_DIR = include
STUB_DIR = $(SRC_DIR)/stub

STUB_C = $(SRC_DIR)/stub.c
STUB_SRC = $(STUB_DIR)/stub.nasm
STUB_BIN = stub.bin

SRCS = $(wildcard $(SRC_DIR)/*.c) $(STUB_C)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

NAME = woody_woodpacker

.PHONY: all
all: help

$(NAME): $(OBJS) $(STUB_C)
	$(CC) $(OBJS) -o $(NAME)

$(STUB_BIN): $(STUB_SRC)
	@nasm -o $(STUB_BIN) -f bin $(STUB_SRC)

$(STUB_C): $(STUB_BIN)
	@xxd -i $(STUB_BIN) > $(STUB_C)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: help
help: ## Show this help message
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: $(NAME) ## Build the project and the stub (default)

.PHONY: run
run: $(NAME) ## Compile and run the my_nm without args
	./$(NAME)

.PHONY: clean
clean: ## Remove build artifacts
	rm -rf $(OBJ_DIR) $(NAME) $(STUB_BIN) $(STUB_C)

.PHONY: re
re: clean build ## Clean and rebuild everything


.PHONY: comp-test
comp-test: ## Compile the binary to be obfuscated
	@gcc test.c -o obfuscated
