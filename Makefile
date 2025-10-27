.PHONY: all clean build build-fresh

BUILD_DIR := build

all: clean build-fresh run

rerun: build run

clean:
	rm -rf $(BUILD_DIR)

build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

build-fresh:
	cmake -B $(BUILD_DIR) --fresh
	cmake --build $(BUILD_DIR)

run:
	./build/bin/test
