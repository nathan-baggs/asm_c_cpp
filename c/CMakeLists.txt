add_executable(c_game
    c_list.c
    c_rectangle.c
    c_vector2.c
    c_window.c
    main.c
)

target_link_directories(c_game PRIVATE ${sdl_BINARY_DIR})
target_include_directories(c_game PRIVATE ${sdl_SOURCE_DIR}/include)

target_link_libraries(c_game SDL2::SDL2-static)
