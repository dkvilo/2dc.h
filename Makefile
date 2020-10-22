CC := gcc -std=c99
2DC_FRAMEWORK := 2dc
SANDBOX_SOURCE := sandbox
BUILD_DIR := build

BIN := 2dc

DEPS_DIR := deps

GLFW_DIR := $(DEPS_DIR)/glfw
GLFW_HEADERS := $(GLFW_DIR)/include/
GLFW_LIB := $(GLFW_DIR)/src/libglfw.3.4.dylib

FINAL_CFLAGS := -I./$(2DC_FRAMEWORK) -I./$(DEPS_DIR) -I./$(GLFW_HEADERS) ./$(GLFW_LIB) -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL

glfw:
	cd $(GLFW_DIR) && cmake -DBUILD_SHARED_LIBS=ON . && make

prepare:
	mkdir build

clean_bin:
	rm $(BUILD_DIR)/$(BIN)

clean:
	rm -rf $(BUILD_DIR)

build:
	make prepare && $(CC) -w $(SANDBOX_SOURCE)/*.c $(FINAL_CFLAGS) -o $(BUILD_DIR)/$(BIN)

.PHONY:
	build clean glfw prepare