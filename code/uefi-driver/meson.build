bootloader_sources = files(
    './source/main.cc'
)

executable('driver.efi', bootloader_sources,
    link_args: '-Wl,-subsystem:efi_runtime_driver',
    include_directories: [
        core_lib_headers,
        uefi_lib_headers
    ]
)
