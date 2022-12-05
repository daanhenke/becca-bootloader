$(DIST_DIR):
	@mkdir -p $(DIST_DIR)

.PHONY: dist
dist: build $(DIST_DIR)
	@echo [*] Copying to dist folder...
	@cp $(BUILD_DIR)/code/bootloader/bootloader.efi $(DIST_DIR)
	@cp $(BUILD_DIR)/code/uefi-driver/driver.efi $(DIST_DIR)
