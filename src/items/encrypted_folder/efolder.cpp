#include "efolder.hxx"
#include "../../crypto/openssl/base64.hxx"

#include <stdexcept>
#include <utility>
#include <iostream>

bool EncryptedFolder::is_open()
{
  if( !content )
    return false;
  if( !encryptor )
    return false;
  return true;
}

// open() will create Folder object, by decrypting encrypted json from content_base64
void EncryptedFolder::open( std::unique_ptr<crypto_provider> enc )
{
  constexpr bool debug = false;
  if constexpr(debug) std::cerr << __func__ << "(" << enc.get() << ") where content = " << content.get() << " and encryptor = " << encryptor.get() << '\n';
  if( content )
  {
    return; // already open
  }
  if( !enc )
    throw std::logic_error( "encryption provider is nullptr" );
  if constexpr(debug) std::cerr << "moving encryptor...\n";
  encryptor = std::move(enc);
  if constexpr(debug) std::cerr << __func__ << "(" << enc.get() << ") where content = " << content.get() << " and encryptor = " << encryptor.get() << '\n';
  try
  {
    content = std::make_unique<Folder>(
      nlohmann::json::parse(
        encryptor->decrypt(
          base64_decode( content_base64 )
        )
      )
    );
  }
  catch( nlohmann::json_abi_v3_11_3::detail::parse_error &e )
  {
    encryptor = nullptr;
    throw encryption_error();
  }

}

// close() writes encrypted changes to this->content_base64
// it doesn't accualy close the folder
void EncryptedFolder::close()
{
  if( !content)
    return; // not opened
  if( !encryptor )
    throw std::logic_error( "EncryptedFolder open, but cannot be closed - no encryption provider" );
  content_base64 = base64_encode( encryptor->encrypt( content->json().dump() ) );
}

std::unique_ptr<Folder> EncryptedFolder::extract_folder()
{
  return std::move(content);
}

EncryptedFolder::EncryptedFolder( nlohmann::json data )
  :Item( data["name"] ), content_base64( data["content"] ){}
EncryptedFolder::EncryptedFolder( safe_string name, std::unique_ptr<crypto_provider> enc )
  :Item( name ), encryptor(std::move(enc)), content(std::make_unique<Folder>(name))
{
  this->close();
}

nlohmann::json EncryptedFolder::json()
{
  this->close();
  nlohmann::json json_obj = Item::json();
  json_obj["type"] = this->type;
  json_obj["content"] = this->content_base64;
  return json_obj;
}
void EncryptedFolder::accept_visit( display_visitor& visitor )
{
  visitor.visit( *this );
}
