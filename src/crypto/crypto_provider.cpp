#include "crypto_provider.hxx"
#include "openssl/sha256.hxx"
#include "sodium/chacha20poly1305.hxx"
#include "argon2/argon2_wrapper.hxx"

#include <cstdlib>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cassert>
#include <sys/random.h>

#include <sodium.h>

std::string load_file_content(const std::filesystem::path& filePath)
{
  std::ifstream inputFile(filePath, std::ios::binary);
  if (!inputFile.good())
    throw std::runtime_error("Unable to open file");
  std::string content((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
  if (inputFile.bad())
    throw std::runtime_error("Error reading file");
  inputFile.close();
  return content;
}

void write_file_content(const std::filesystem::path& filePath, const std::string& content)
{
  std::ofstream outputFile(filePath, std::ios::binary);
  if (!outputFile.good())
    throw std::runtime_error("Unable to open file");
  outputFile.write(content.c_str(), content.size());
  if (!outputFile.good())
    throw std::runtime_error("Error writing to file");
  outputFile.close();
}

crypto_provider::crypto_provider(const safe_string &password)
:password(password){}

bool crypto_provider::operator==(const crypto_provider &obj) const
{
  return this->key == obj.key;
}

// ==================================
//       ChaCha20 Poly1305
// ==================================
chacha20_poly1305::chacha20_poly1305(const safe_string& password)
:crypto_provider(password){}

std::string chacha20_poly1305::encrypt(const safe_string& plaintext)
{
  char * salt = new char[crypto_pwhash_SALTBYTES];
  getrandom(salt, crypto_pwhash_SALTBYTES, 0);
  std::string salt_s = std::string(salt, crypto_pwhash_SALTBYTES);
  key = kdf(salt_s);
  std::vector<unsigned char> ciphertext_bytes = chacha20_poly1305_ns::encrypt(std::vector<unsigned char>(plaintext.begin(), plaintext.end()), key.c_str());
  std::string result = std::string(ciphertext_bytes.begin(), ciphertext_bytes.end());
  result = salt_s + result;
  return result;
}
safe_string chacha20_poly1305::decrypt(std::string ciphertext) 
{
  key = kdf(ciphertext.substr(0, crypto_pwhash_SALTBYTES));
  ciphertext = ciphertext.substr(crypto_pwhash_SALTBYTES);
  std::vector<unsigned char> plaintext_bytes  = chacha20_poly1305_ns::decrypt(std::vector<unsigned char>(ciphertext.begin(), ciphertext.end()), key.c_str());
  return std::move(std::string(plaintext_bytes.begin(), plaintext_bytes.end()));
}
safe_string chacha20_poly1305::load_file(std::filesystem::path file_path) 
{
  std::string ciphertext = load_file_content(file_path);
  return decrypt(ciphertext);
}
void chacha20_poly1305::export_to_file(std::filesystem::path file_path, const safe_string &content) 
{
  std::string ciphertext = encrypt(content);
  write_file_content(file_path, ciphertext);
}
safe_string chacha20_poly1305::kdf(std::string salt) const
{
  assert(sodium_init() >= 0);
  const size_t KEY_LEN = crypto_aead_chacha20poly1305_ietf_KEYBYTES;
  unsigned char * key = new unsigned char[KEY_LEN];
  if (crypto_pwhash(
            key, KEY_LEN,
            password.c_str(), password.length(),
            reinterpret_cast<const unsigned char*>(salt.c_str()),
            crypto_pwhash_OPSLIMIT_MODERATE,
            crypto_pwhash_MEMLIMIT_MODERATE,
            crypto_pwhash_ALG_ARGON2ID13))
            throw std::runtime_error("OOM");
  safe_string key_s = std::string(reinterpret_cast<char*>(key), KEY_LEN);
  memset(key, 0, KEY_LEN);
  delete[] key;
  return key_s;
}
