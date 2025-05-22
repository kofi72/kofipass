#pragma once

#ifndef CRYPTO_PROVIDER_HXX_INCLUDED
#define CRYPTO_PROVIDER_HXX_INCLUDED

#include "../memory/memory.hxx"
#include <filesystem>
#include <memory>

class encryption_error {};

class crypto_provider
{
protected:
  safe_string key;
  safe_string password;
public:
  crypto_provider(const safe_string& password);
  virtual ~crypto_provider() = default;

  virtual std::string encrypt(const safe_string& ) = 0;
  virtual safe_string decrypt(std::string ) = 0;
  virtual safe_string load_file(std::filesystem::path) = 0;
  virtual void export_to_file(std::filesystem::path, const safe_string &content) = 0;
  virtual safe_string kdf(std::string salt) const = 0;

  bool operator==(const crypto_provider&) const;
};

class chacha20_poly1305 : public crypto_provider
{
  public:
  chacha20_poly1305(const safe_string& password);

  std::string encrypt(const safe_string&) override;
  safe_string decrypt(std::string) override;
  safe_string load_file(std::filesystem::path) override;
  void export_to_file(std::filesystem::path, const safe_string &content) override;
  safe_string kdf(std::string salt) const override;
};

#endif // CRYPTO_PROVIDER_HXX_INCLUDED
