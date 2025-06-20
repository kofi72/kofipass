add_subdirectory(memory)
add_subdirectory(crypto)
add_subdirectory(items)
add_subdirectory(json)
add_subdirectory(cli)
add_subdirectory(test)

add_executable(kpass cli.cpp)
target_link_libraries(kpass cli testkpass)
