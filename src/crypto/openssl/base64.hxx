#pragma once

#ifndef BASE64_HXX_INCLUDED
#define BASE64_HXX_INCLUDED

#include <cstdint>
#include <string>

std::string base64_encode( const std::string input );
std::string base64_decode( const std::string input );
// TOTP
std::basic_string<uint8_t> string_to_ustring(std::string str);
std::string ustring_to_string(std::basic_string<uint8_t> str);
std::string decodeBase32(const std::string& input);

#endif // BASE64_HXX_INCLUDED
