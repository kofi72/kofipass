#pragma once
#ifndef EFOLDER_HXX_INCLUDED
#define EFOLDER_HXX_INCLUDED

//#include "../item_base/item_base.hxx"
#include "../folder/folder.hxx"
#include "../../crypto/crypto_provider.hxx"

struct EncryptedFolder : public Item
{
  static constexpr char const*const type = "encryptedfolder";
  mutable safe_string content_base64;

  mutable std::shared_ptr<crypto_provider> encryptor;
  std::shared_ptr<Item> content;

  bool is_open();
  // open() will create Folder object, by decrypting encrypted json from content_base64
  void open(std::shared_ptr<crypto_provider> encryptor);

  // close() writes encrypted changes to this->content_base64
  // it doesn't accualy close the folder
  void close() const;
  std::shared_ptr<Folder> extract_folder();

  EncryptedFolder(safe_string name, std::shared_ptr<crypto_provider> enc);
  EncryptedFolder( safe_string name, std::string content_base64);

  void accept_visit(item_visitor& visitor) override;
  std::string get_type() const noexcept override;
};

#endif // EFOLDER_HXX_INCLUDED
