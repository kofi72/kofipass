add_library(cli cli.cpp)
add_library(cli_tools tools/tools.cpp)

target_link_libraries(cli_tools crypto_provider)
target_link_libraries(cli slogin folder efolder 2falogin safe_memory cli_tools customitem)
