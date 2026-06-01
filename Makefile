CC = clang
CFLAGS =	-g -Wall -Wextra -Werror \
			-std=c99 \
			-Wpedantic \
			-Wconversion \
			-Wdouble-promotion \
			-Wno-unused-parameter \
			-Wno-unused-function \
			-Wno-sign-conversion \
			-fstack-usage \
			-fsanitize=undefined \
			-fsanitize-trap

INCLUDES = -I./include/

SRC_DIR = src
OBJ_DIR = objs
INC_DIR = include
STUB_DIR = $(SRC_DIR)/stub
ASM_DIR = $(SRC_DIR)/nasm

STUB_C = $(SRC_DIR)/stub.c
STUB_SRC = $(STUB_DIR)/stub.nasm
STUB_BIN = stub.bin

SRCS =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/file.c \
		$(SRC_DIR)/elf.c \
		$(SRC_DIR)/program_header.c \
		$(SRC_DIR)/program_header_it.c \
		$(SRC_DIR)/stub_substitution.c \
		$(SRC_DIR)/decrypt.c \
		$(SRC_DIR)/stub.c

ASM_C_SRC =	$(ASM_DIR)/encrypt.nasm

OBJS =	$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS +=	$(OBJ_DIR)/encrypt.o

NAME = woody_woodpacker

.PHONY: all
all: help

$(NAME): $(STUB_C) $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(STUB_BIN): $(STUB_SRC)
	nasm -o $(STUB_BIN) -f bin $(STUB_SRC)

$(STUB_C): $(STUB_BIN)
	xxd -i $(STUB_BIN) > $(STUB_C)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.nasm | $(OBJ_DIR)
	nasm -f elf64 $< -o $@

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
