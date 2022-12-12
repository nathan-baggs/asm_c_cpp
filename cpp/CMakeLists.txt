add_executable(cpp_game
    colour.cpp
    entity.cpp
    main.cpp
    rectangle.cpp
    vector2.cpp
    window.cpp
)

target_link_directories(cpp_game PRIVATE ${sdl_BINARY_DIR})
target_include_directories(cpp_game PRIVATE ${sdl_SOURCE_DIR}/include)
target_compile_features(cpp_game PRIVATE cxx_std_20)

target_link_libraries(cpp_game SDL2::SDL2-static)
