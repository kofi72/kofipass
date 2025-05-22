# SODIUM
include(sodium/FindSodium.cmake)
add_library(chacha20 sodium/chacha20poly1305.cpp)
target_link_libraries(chacha20 sodium)

# OPENSSL
add_library(rijndael openssl/rijndael.cpp)
add_library(base64 openssl/base64.cpp)

target_link_libraries(base64 crypto)

# ARGON2
add_library(argon2_wrapper argon2/argon2_wrapper.cpp)
target_link_libraries(argon2_wrapper argon2)

# CPPOTP

add_library(cppotp_bytes libcppotp/bytes.cpp)
add_library(cppotp_sha1 libcppotp/sha1.cpp)
add_library(cppotp libcppotp/otp.cpp)
target_link_libraries(cppotp cppotp_sha1 cppotp_bytes)

# CRYPTO
add_library(crypto_provider crypto_provider.cpp)
target_link_libraries(crypto_provider argon2_wrapper safe_memory chacha20)
