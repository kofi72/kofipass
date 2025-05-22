#include "tests.hxx"

#include "../cli/tools/tools.hxx"

#include "../items/items.hxx"

#include "../crypto/sodium/chacha20poly1305.hxx"
#include "../crypto/openssl/base64.hxx"

#include "../crypto/crypto_provider.hxx"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <memory>

constexpr bool verbose = false;

void test_crypto_provider(crypto_provider* provider);

void run_test()
{
  srand(time(NULL));

  // ITEM
  {
    if constexpr( verbose )
    {
      std::cerr << color::magenta << "testing items...\n" << color::normal;
      std::cerr << "testing folder(string)... ";
    }
    Folder folder( safe_string( "name" ) );
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing folder::json()... ";
    }
    auto json = folder.json();
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing folder(json)... ";
    }
    Folder folder2( json );
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing SimpleLogin(string,string,string)... ";
    }
    folder2.content.push_back( std::make_unique<SimpleLogin>( "name", "login", "password", "https://example.com/" ) );
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing folder::json()... ";
    }
    json = folder2.json();
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing folder(json)... ";
    }
    Folder folder3( json );
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
    }
  }

  //rijndael
  {
    if constexpr( verbose )
    {
      std::cerr << color::magenta << "testing chacha20_poly1305_ns...\n" << color::normal;
      std::cerr << "testing chacha20_poly1305_ns::encrypt... ";
    }
    std::string plaintext = "Hello plaintext!";
    std::string key = "9MpwvjJkvNe9H7a5sPSpHWFrdVWLiXMa";
    std::vector<unsigned char> plaintext_bytes(plaintext.begin(), plaintext.end());
    std::vector<unsigned char> ciphertext_bytes = chacha20_poly1305_ns::encrypt(plaintext_bytes, key.c_str());
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing chacha20_poly1305_ns::decrypt... ";
    }
    std::vector<unsigned char> plaintext_bytes2 = chacha20_poly1305_ns::decrypt(ciphertext_bytes, key.c_str());
    std::string plaintext2(plaintext_bytes2.begin(), plaintext_bytes2.end());
    if(plaintext != plaintext2)
      throw std::logic_error("x != decrypt(encrypt(x))");
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing chacha20_poly1305_ns::encrypt consistency... ";
    }
    std::vector<unsigned char> ciphertext_bytes2 = chacha20_poly1305_ns::encrypt(plaintext_bytes2, key.c_str());
    std::string ciphertext(ciphertext_bytes.begin(), ciphertext_bytes.end());
    std::string ciphertext2(ciphertext_bytes2.begin(), ciphertext_bytes2.end());
    if(ciphertext == ciphertext2)
      throw std::logic_error("encrypt(x) == encrypt(x) (IV failure)");
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
    }
  }

  // base64
  {
    if constexpr( verbose )
    {
      std::cerr << color::magenta << "testing base64...\n" << color::normal;
      std::cerr << "testing base64_encode... ";
    }
    std::string plaintext = "meow meow meow meow";
    std::string base64 = base64_encode(plaintext);
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing base64_decode... ";
    }
    std::string text = base64_decode(base64);
    if(text != plaintext)
      throw std::logic_error("x != base64_decode(base64_encode(x))");
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
    }
  }

  // crypto_provider
  {
    if constexpr( verbose )
    {
      std::cerr << color::magenta << "testing crypto_provider...\n" << color::normal;
      std::cerr << "testing chacha20_poly1305::encrypt... ";
    }
    std::string password = "Hello pass-world!";
    test_crypto_provider(new chacha20_poly1305(password));
    if constexpr( verbose )
    {
      std::cerr << color::green << "OK\n" << color::normal;
      std::cerr << "testing chacha20_poly1305 key consistency... ";
    }
    chacha20_poly1305 p1(password), p2(password);
    if(not (p1 == p2))
      throw std::logic_error("chacha20_poly1305 inconsistent");
    if constexpr( verbose )
      std::cerr << color::green << "OK\n" << color::normal;
  }
}

void test_crypto_provider(crypto_provider* provider)
{
  std::string plaintext = "Hello world!";
  std::string ciphertext = provider->encrypt(plaintext);
  std::string decrypted = provider->decrypt(ciphertext);
  if(decrypted != plaintext)
    throw std::logic_error("x != decrypt(encrypt(x))");
}
