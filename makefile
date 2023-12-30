# 编译器
CC=gcc

# 编译选项
CFLAGS=-I.

# 源文件
SRC = $(wildcard *.c)

# 对象文件
OBJ = $(SRC:%.c=output/%.o)

# 输出目录
OUTPUT_DIR=output

# 最终的可执行程序
EXECUTABLE=output/myprogram

# 默认目标
all: clean build $(EXECUTABLE)

# 创建输出目录
build:
	@mkdir -p $(OUTPUT_DIR)

# 编译最终的可执行程序
$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# 通用规则来编译所有的.o文件
output/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# 清理规则
clean:
	@echo "清理输出目录..."
	@rm -rf $(OUTPUT_DIR)

.PHONY: all build clean
