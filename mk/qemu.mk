OVMF := $(BUILD_DIR)/artifacts/ovmf.fd
QEMU_FAT_DIR := $(BUILD_DIR)/qemu/efifs

$(OVMF):
	@echo Downloading ovmf.fd...
	@[ -d "$(BUILD_DIR)/artifacts" ] || mkdir -p "$(BUILD_DIR)/artifacts"
	@wget https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd -O $(@)

$(QEMU_FAT_DIR): $(DIST_DIR)/bootloader.efi
	@[ -d "$(QEMU_FAT_DIR)/EFI/BOOT" ] || mkdir -p "$(QEMU_FAT_DIR)/EFI/BOOT"
	@cp $(DIST_DIR)/bootloader.efi $(QEMU_FAT_DIR)/EFI/BOOT/BOOTX64.EFI

.PHONY: qemu
qemu: dist $(OVMF) $(QEMU_FAT_DIR)
	qemu-system-x86_64 \
		--bios $(OVMF) \
		-drive file=fat:rw:$(QEMU_FAT_DIR),format=raw,media=disk
