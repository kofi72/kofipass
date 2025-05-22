#ifndef BASE64_CPP_INCLUDED
#define BASE64_CPP_INCLUDED

#include "base64.hxx"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <cstring>
#include <bitset>

std::string base64_encode( const std::string input )
{
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  // Create a base64 filter BIO and a memory BIO.
  b64 = BIO_new( BIO_f_base64() );
  bio = BIO_new( BIO_s_mem() );
  bio = BIO_push( b64, bio );

  // Write the input data to the BIO.
  BIO_write( bio, input.c_str(), static_cast<int>( input.length() ) );
  BIO_flush( bio );

  // Get a pointer to the memory BIO's buffer.
  BIO_get_mem_ptr( bio, &bufferPtr );
  BIO_set_close( bio, BIO_NOCLOSE );

  // Create a std::string from the BIO's buffer.
  std::string encoded( bufferPtr->data, bufferPtr->length );

  // Free the BIO objects.
  BIO_free_all( bio );

  return encoded;
}

std::string base64_decode( const std::string input )
{
  BIO *bio, *b64;
  char *buffer = new char[input.length()];
  memset( buffer, 0, input.length() );

  b64 = BIO_new( BIO_f_base64() );
  bio = BIO_new_mem_buf( input.c_str(), static_cast<int>( input.length() ) );
  bio = BIO_push( b64, bio );

  int decoded_length = BIO_read( bio, buffer, static_cast<int>( input.length() ) );

  std::string decoded( buffer, decoded_length );

  BIO_free_all( bio );
  delete[] buffer;

  return decoded;
}

bool inside(std::string str, char c)
{
  for(const auto &i : str)
    if(i == c)
      return true;
  return false;
}

// TOTP

std::basic_string<uint8_t> string_to_ustring(std::string str)
{
  std::basic_string<uint8_t> result;
  for(const auto &c : str)
    result += (uint8_t)c;
  return result;
}
std::string ustring_to_string(std::basic_string<uint8_t> str)
{
  std::string result;
  for(const auto &c : str)
    result += (char)c;
  return result;
}


std::string decodeBase32(const std::string& input) {
    static const std::string base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string bits;
    std::string output;

    for (char c : input) {
        if (inside(base32Chars, c)) {
            int val = base32Chars.find(c);
            bits += std::bitset<5>(val).to_string();
        }
    }

    for (size_t i = 0; i + 8 <= bits.size(); i += 8) {
        std::bitset<8> byte(bits.substr(i, 8));
        output += static_cast<char>(byte.to_ulong());
    }

    return output;
}

#endif // BASE64_CPP_INCLUDED
