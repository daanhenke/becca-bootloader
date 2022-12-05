bootloader_sources = files(
    './source/main.cc'
)

executable('bootloader.efi', [
    bootloader_sources,
    core_lib_sources,
    uefi_lib_sources
],
    link_args: '-Wl,-subsystem:efi_application',
    cpp_args: common_cpp_args,
    include_directories: [
        core_lib_headers,
        uefi_lib_headers
    ]
)
