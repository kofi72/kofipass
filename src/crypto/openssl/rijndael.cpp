#include "rijndael.hxx"

#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>

#include <vector>
#include <cmath>
#include <cassert>
#include <string>

constexpr size_t rijndaelh_internal::ceil_padding_128( const size_t &num )
{
  return ceil( ( num+1 )/16.0 )*16;
}

std::vector<unsigned char> aes_256_cbc::encrypt( std::vector<unsigned char> target, const std::string& key )
{
  size_t clen = rijndaelh_internal::ceil_padding_128( target.size() );
  std::vector<unsigned char> IV( clen + rijndaelh_internal::iv_size );
  RAND_bytes( IV.data(), rijndaelh_internal::iv_size );
  unsigned char *const ciphertext = IV.data()+rijndaelh_internal::iv_size;
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex( ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>( key.c_str() ), IV.data() );

  int write_size;
  EVP_EncryptUpdate( ctx, ciphertext, &write_size, target.data(), target.size() );

  EVP_EncryptFinal_ex( ctx, ciphertext+write_size, &write_size );

  EVP_CIPHER_CTX_free( ctx );

  return IV;
} // encrypt

std::vector<unsigned char> aes_256_cbc::decrypt(std::vector<unsigned char> target, const std::string& key)
{
    std::vector<unsigned char> IV(target.begin(), target.begin() + rijndaelh_internal::iv_size);
    std::vector<unsigned char> ciphertext(target.begin() + rijndaelh_internal::iv_size, target.end());

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), IV.data());

    std::vector<unsigned char> plaintext(ciphertext.size());
    int write_size;
    EVP_DecryptUpdate(ctx, plaintext.data(), &write_size, ciphertext.data(), ciphertext.size());

    int final_size;
    EVP_DecryptFinal_ex(ctx, plaintext.data() + write_size, &final_size);

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(write_size + final_size);

    return plaintext;
}

