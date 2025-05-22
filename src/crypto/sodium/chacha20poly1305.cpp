#include <sodium.h>
#include <vector>
#include <stdexcept>
#include <cstring>

#include "chacha20poly1305.hxx"

std::vector<unsigned char> chacha20_poly1305_ns::encrypt( std::vector<unsigned char> target, char const*const key )
{
  if (sodium_init() < 0)
    throw std::runtime_error("Libsodium initialization failed");

  const size_t nonce_len = crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;

  std::vector<unsigned char> nonce(nonce_len);
  randombytes_buf(nonce.data(), nonce_len);

  std::vector<unsigned char> ciphertext(target.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES);

  unsigned long long ciphertext_len;
  crypto_aead_xchacha20poly1305_ietf_encrypt(
    ciphertext.data(), &ciphertext_len,
    target.data(), target.size(),
    nullptr, 0,                      // no additional data
    nullptr,                        // no secret nonce
    nonce.data(),
    reinterpret_cast<const unsigned char*>(key)
  );

  // prepend nonce to ciphertext
  ciphertext.resize(ciphertext_len);
  ciphertext.insert(ciphertext.begin(), nonce.begin(), nonce.end());

  return ciphertext;
}
std::vector<unsigned char> chacha20_poly1305_ns::decrypt( std::vector<unsigned char> target, char const*const key )
{
  if (sodium_init() < 0)
    throw std::runtime_error("Libsodium initialization failed");

  const size_t nonce_len = crypto_aead_xchacha20poly1305_ietf_NPUBBYTES;

  if (target.size() < nonce_len + crypto_aead_xchacha20poly1305_ietf_ABYTES)
    throw std::invalid_argument("Ciphertext too short");

  std::vector<unsigned char> nonce(target.begin(), target.begin() + nonce_len);
  std::vector<unsigned char> ciphertext(target.begin() + nonce_len, target.end());

  std::vector<unsigned char> decrypted(ciphertext.size() - crypto_aead_xchacha20poly1305_ietf_ABYTES);
  unsigned long long decrypted_len;

  if (crypto_aead_xchacha20poly1305_ietf_decrypt(
      decrypted.data(), &decrypted_len,
      nullptr,
      ciphertext.data(), ciphertext.size(),
      nullptr, 0,
      nonce.data(),
      reinterpret_cast<const unsigned char*>(key)
    ) != 0)
  {
    throw std::runtime_error("Decryption failed or data is tampered");
  }

  decrypted.resize(decrypted_len);
  return decrypted;
}
