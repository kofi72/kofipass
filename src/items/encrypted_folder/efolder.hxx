#pragma once
#ifndef EFOLDER_HXX_INCLUDED
#define EFOLDER_HXX_INCLUDED

//#include "../item_base/item_base.hxx"
#include "../folder/folder.hxx"
#include "../../crypto/crypto_provider.hxx"

struct EncryptedFolder : public Item
{
  static constexpr char const*const type = "encryptedfolder";
  safe_string content_base64;

  std::unique_ptr<crypto_provider> encryptor;
  std::unique_ptr<Folder> content;

  bool is_open();
  // open() will create Folder object, by decrypting encrypted json from content_base64
  void open(std::unique_ptr<crypto_provider> encryptor);

  // close() writes encrypted changes to this->content_base64
  // it doesn't accualy close the folder
  void close();
  std::unique_ptr<Folder> extract_folder();

  EncryptedFolder(nlohmann::json data);
  EncryptedFolder(safe_string name, std::unique_ptr<crypto_provider> enc);

  nlohmann::json json() override;
  void accept_visit(display_visitor& visitor) override;
};

#endif // EFOLDER_HXX_INCLUDED
