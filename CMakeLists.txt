cmake_minimum_required(VERSION 3.20)  # CMake version check
project(kpass_kofipass_passwdman)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wpedantic -Wall -Wextra -g3 -O3)

add_subdirectory(src)

# install
install(TARGETS kpass DESTINATION bin)

# cpack
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "1.0.2")
set(CPACK_PACKAGE_CONTACT "45hx1w89h@mozmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "kofi")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "ChaCha 20 Poly 1305 ARGON2 password manager cli")
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS YES)

include(CPack)
