#include "items.hxx"

struct itemFactory
{
  virtual std::unique_ptr<Item> create( nlohmann::json data ) const = 0;
};

struct SimpleLoginFactory final : itemFactory
{
  std::unique_ptr<Item> create( nlohmann::json data ) const override
  {
    return std::make_unique<SimpleLogin>( data );
  }
};

struct FolderFactory final : itemFactory
{
  std::unique_ptr<Item> create( nlohmann::json data ) const override
  {
    return std::make_unique<Folder>( data );
  }
};

struct EncryptedFolderFactory final : itemFactory
{
  std::unique_ptr<Item> create( nlohmann::json data ) const override
  {
    return std::make_unique<EncryptedFolder>( data );
  }
};

struct twofa_loginFactory final : itemFactory
{
  std::unique_ptr<Item> create( nlohmann::json data ) const override
  {
    return std::make_unique<twofa_login>( data );
  }
};

struct customItemFactory final : itemFactory
{
  std::unique_ptr<Item> create( nlohmann::json data ) const override
  {
    return std::make_unique<customItem>( data );
  }
};

std::unique_ptr<Item> build_item( nlohmann::json data )
{
  std::unique_ptr<itemFactory> Factory;
  if( data["type"] == SimpleLogin::type )
    Factory = std::make_unique<SimpleLoginFactory>();
  else if( data["type"] == Folder::type )
    Factory = std::make_unique<FolderFactory>();
  else if( data["type"] == EncryptedFolder::type )
    Factory = std::make_unique<EncryptedFolderFactory>();
  else if( data["type"] == twofa_login::type )
    Factory = std::make_unique<twofa_loginFactory>();
  else if( data["type"] == customItem::type )
    Factory = std::make_unique<customItemFactory>();
  else
    throw std::runtime_error( "Unsupported item type: " + data["type"].get<std::string>() );
  return Factory->create( data );
}

std::unique_ptr<Item> new_item_from_json( nlohmann::json data )
{
  // check if has valid 'type' field
  if( data.find( "type" ) != data.end() && data["type"].is_string() )
    return build_item( data );
  else
    throw std::runtime_error( "Missing or invalid 'type' field in JSON data" );
}
