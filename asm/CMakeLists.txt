enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_ASM_NASM_FLAGS_DEBUG "-g -Fdwarf")

set(SOURCE_FILES
    graphics.asm
    linked_list.asm
    main.asm
    memory.asm
    utils.asm
)
set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE ASM_NASM)

add_executable(asm_game
    ${SOURCE_FILES}
)

target_link_libraries(asm_game X11)
target_link_options(asm_game PRIVATE --dynamic-linker /lib64/ld-linux-x86-64.so.2)