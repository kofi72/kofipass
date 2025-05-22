add_library(item_base item_base/item_base.cpp)

add_library(slogin simple_login/slogin.cpp)
add_library(folder folder/folder.cpp)
add_library(efolder encrypted_folder/efolder.cpp)
add_library(2falogin 2falogin/2falogin.cpp)

add_library(field custom/field/field.cpp)
add_library(customitem custom/custom.cpp)

add_library(items items.cpp)

target_link_libraries(slogin item_base)
target_link_libraries(folder item_base items)
target_link_libraries(efolder base64 crypto_provider folder item_base)
target_link_libraries(2falogin slogin cppotp)
target_link_libraries(customitem field item_base cppotp base64)
