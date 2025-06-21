#include "efolder.hxx"
#include "../../crypto/openssl/base64.hxx"
#include "../../json/export.hxx"

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
void EncryptedFolder::open( std::shared_ptr<crypto_provider> enc )
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
  encryptor = enc;
  if constexpr(debug) std::cerr << __func__ << "(" << enc.get() << ") where content = " << content.get() << " and encryptor = " << encryptor.get() << '\n';
  try
  {
    content = new_item_from_json(encryptor->decrypt(base64_decode( content_base64 )));
  }
  catch( nlohmann::json_abi_v3_11_3::detail::parse_error &e )
  {
    encryptor = nullptr;
    throw encryption_error();
  }

}

// close() writes encrypted changes to this->content_base64
// it doesn't accualy close the folder
void EncryptedFolder::close() const
{
  if( !content)
    return; // not opened
  if( !encryptor )
    throw std::logic_error( "EncryptedFolder open, but cannot be closed - no encryption provider" );
  content_base64 = base64_encode( encryptor->encrypt( jsonify(content.get()) ) );
}

std::shared_ptr<Folder> EncryptedFolder::extract_folder()
{
  return std::shared_ptr<Folder>(dynamic_cast<Folder*>(content.get()));
}

EncryptedFolder::EncryptedFolder( safe_string name, std::string content_base64)
  :Item(name), content_base64(content_base64) {}
EncryptedFolder::EncryptedFolder( safe_string name, std::shared_ptr<crypto_provider> enc )
  :Item( name ), encryptor(std::move(enc)), content(std::make_shared<Folder>(name))
{
  this->close();
}

void EncryptedFolder::accept_visit( item_visitor& visitor )
{
  visitor.visit( *this );
}
std::string EncryptedFolder::get_type() const noexcept
{
  return this->type;
}
