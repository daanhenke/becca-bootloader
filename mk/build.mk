.PHONY: build
build:
	@echo [*] Building...
	@[ -d "$(BUILD_DIR)" ] || mkdir -p "$(BUILD_DIR)"
	@[ -f "$(BUILD_DIR)/meson-private/coredata.dat" ] || meson setup \
		--cross-file $(PROJECT_DIR)/crossfiles/x86_64-uefi-windows.ini \
		$(PROJECT_DIR) "$(BUILD_DIR)"
	@cd "$(BUILD_DIR)" && meson compile
