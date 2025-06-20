add_library(item_base item_base/item_base.cpp)

add_library(slogin simple_login/slogin.cpp)
add_library(folder folder/folder.cpp)
add_library(efolder encrypted_folder/efolder.cpp)
add_library(2falogin 2falogin/2falogin.cpp)

target_link_libraries(slogin item_base)
target_link_libraries(folder item_base)
target_link_libraries(efolder base64 crypto_provider folder item_base item_json_export)
target_link_libraries(2falogin slogin cppotp)
