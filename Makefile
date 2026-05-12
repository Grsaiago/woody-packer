CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -Wpedantic -fstack-usage -std=c99
INCLUDES = -I./include/

SRC_DIR = src
OBJ_DIR = objs

NAME = woody

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all
all: help

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: help
help: ## Show this help message
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: $(NAME) ## Build the project (default)

.PHONY: run
run: $(NAME) ## Compile and run the my_nm without args
	./$(NAME)

.PHONY: clean
clean: ## Remove build artifacts
	rm -rf $(OBJ_DIR) $(NAME)

.PHONY: re
re: clean build ## Clean and rebuild everything
