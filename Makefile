BUILD_DIR := build

ERM := $(abspath $(lastword $(MAKEFILE_LIST)))

.PHONY: all clean run help

all: $(BUILD_DIR)
	@cmake -S $(PWD) -B $(BUILD_DIR)
	@$(MAKE) --no-print-directory -C $(BUILD_DIR)

compile_commands: $(BUILD_DIR)
	@cmake -S $(PWD) -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=1

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		cmake --build $(BUILD_DIR) --target clean -- --no-print-directory; \
		rm -rf $(BUILD_DIR); \
	fi

run: all
	@cd build && echo; ./VulkanEngine || true

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... compile_commands"
	@echo "... clean"
	@echo "... run"
	@echo "... help"

$(BUILD_DIR):
	@mkdir -p $@

