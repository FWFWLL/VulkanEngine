BUILD_DIR := build

ERM := $(abspath $(lastword $(MAKEFILE_LIST)))

.PHONY: all clean run help

all: $(BUILD_DIR)
	@cmake -S $(PWD) -B $(BUILD_DIR)
	@$(MAKE) --no-print-directory -C $(BUILD_DIR)

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		cmake --build $(BUILD_DIR) --target clean -- --no-print-directory; \
		rm -rf $(BUILD_DIR); \
	fi

run: all
	@cd build && ./HelloCMake

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... run"
	@echo "... help"

$(BUILD_DIR):
	@mkdir -p $@

