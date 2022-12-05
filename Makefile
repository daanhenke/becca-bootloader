.PHONY: build
build:

PROJECT_DIR := .
BUILD_DIR := build
DIST_DIR := $(BUILD_DIR)/dist

include ./mk/build.mk
include ./mk/dist.mk
include ./mk/qemu.mk
