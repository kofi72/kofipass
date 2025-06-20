add_library(testkpass tests.cpp)
target_link_libraries(testkpass crypto_provider cli slogin folder efolder 2falogin safe_memory cli_tools)
