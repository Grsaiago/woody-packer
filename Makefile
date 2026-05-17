CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -fstack-usage -std=c99
# CFLAGS = -g -Wall -Wextra -Werror -Wpedantic -fstack-usage -std=c99
INCLUDES = -I./include/

SRC_DIR = src
OBJ_DIR = objs
INC_DIR = include
STUB_DIR = $(SRC_DIR)/stub

SRCS = $(wildcard $(SRC_DIR)/*.c)
STUB_SRC = $(STUB_DIR)/stub.nasm
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

NAME = woody_woodpacker
STUB = stub.bin
STUB_HEADER = include/stub.h

.PHONY: all
all: help

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(STUB): $(STUB_SRC)
	@nasm -o $(STUB) -f bin $(STUB_SRC)

$(STUB_HEADER): $(STUB)
	@xxd -i $(STUB) > $(STUB_HEADER)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: help
help: ## Show this help message
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: $(NAME) $(STUB) $(STUB_HEADER) ## Build the project and the stub (default)

.PHONY: run
run: $(NAME) ## Compile and run the my_nm without args
	./$(NAME)

.PHONY: clean
clean: ## Remove build artifacts
	rm -rf $(OBJ_DIR) $(NAME)

.PHONY: re
re: clean build ## Clean and rebuild everything


.PHONY: comp-test
comp-test: ## Compile the binary to be obfuscated
	@gcc test.c -o obfuscated
