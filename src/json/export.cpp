constexpr bool debug = 1;
#include <iostream>

#include "export.hxx"
#include "../items/folder/folder.hxx"
#include "../items/2falogin/2falogin.hxx"
#include "../crypto/openssl/base64.hxx"
#include <string>
#include <stdexcept>

std::shared_ptr<Item> new_item_from_json(safe_string jsonString);
std::shared_ptr<Item> new_item_from_json(const nlohmann::json& jsonObject);

SimpleLogin     createSimpleLogin     (nlohmann::json data);
twofa_login     createTwofa_login     (nlohmann::json data);
Folder          createFolder          (nlohmann::json data);
EncryptedFolder createEncryptedFolder (nlohmann::json data);
UnknownItem     createUnknownItem     (nlohmann::json data);

std::shared_ptr<Item> new_item_from_json(safe_string jsonString)
{
  nlohmann::json jsonObject = nlohmann::json::parse(jsonString);
  return new_item_from_json(jsonObject);
}

std::shared_ptr<Item> new_item_from_json(const nlohmann::json& jsonObject)
{
  if(not jsonObject.is_object())
  {
    if constexpr(debug)
      std::cerr << "FOUND INVALID OBJECT: \n"
        << jsonObject << "\n IS EXPECTED TO BE JSON OBJECT\n";
    throw std::invalid_argument("Tried to construct an item with invalid json data");
  }
  if(not jsonObject.contains("type"))
    throw std::invalid_argument("Tried to construct an item of unknown type");
  const std::string objType = jsonObject["type"];

  if      ( objType == SimpleLogin       ::type )
    return std::make_shared<SimpleLogin    > (createSimpleLogin      (jsonObject));

  else if ( objType == Folder            ::type )
    return std::make_shared<Folder         > (createFolder           (jsonObject));

  else if ( objType == EncryptedFolder   ::type )
    return std::make_shared<EncryptedFolder> (createEncryptedFolder  (jsonObject));

  else if ( objType == twofa_login       ::type )
    return std::make_shared<twofa_login    > (createTwofa_login      (jsonObject));

  else
    return std::make_shared<UnknownItem    > (createUnknownItem      (jsonObject));
}

SimpleLogin     createSimpleLogin     (nlohmann::json data)
{
  return SimpleLogin(data["name"], data["login"], data["passwd"], data["website"]);
}
twofa_login     createTwofa_login     (nlohmann::json data)
{
  return twofa_login(data["name"], data["login"], data["passwd"], data["website"], data["secret"]);
}
Folder          createFolder          (nlohmann::json data)
{
  Folder folder_retn(data["name"]);
  auto json_array_content = data["content"];
  if(not json_array_content.is_array())
    throw std::invalid_argument("Folder content is not an array");
  for(auto &i : json_array_content)
    folder_retn.content.emplace_back( new_item_from_json(static_cast<safe_string>(i)) );
  return folder_retn;
}

EncryptedFolder createEncryptedFolder (nlohmann::json data)
{
  return EncryptedFolder(data["name"], data["content"]);
}

UnknownItem     createUnknownItem     (nlohmann::json data)
{
  return UnknownItem(data["name"], data["type"], data.dump());
}

// Might replace dynamic_cast with static_cast since i see no place for error
safe_string jsonify(Item const*const item)
{
  const std::string objType = item->get_type();

  if      ( objType == SimpleLogin       ::type )
    return jsonify(dynamic_cast<SimpleLogin const*const>     (item));

  else if ( objType == Folder            ::type )
    return jsonify(dynamic_cast<Folder const*const>          (item));

  else if ( objType == EncryptedFolder   ::type )
    return jsonify(dynamic_cast<EncryptedFolder const*const> (item));

  else if ( objType == twofa_login       ::type )
    return jsonify(dynamic_cast<twofa_login const*const>     (item));

  else
    return jsonify(dynamic_cast<UnknownItem const*const>     (item));
}

nlohmann::json jsonify_Item(Item const*const ptr)
{
  nlohmann::json jsonObject;
  jsonObject["name"] = ptr->name;
  jsonObject["type"] = ptr->get_type();
  return jsonObject;
}
nlohmann::json jsonify_SimpleLogin(SimpleLogin const*const ptr)
{
  auto jsonObject = jsonify_Item(ptr);
  jsonObject["login"] = ptr->login;
  jsonObject["passwd"] = ptr->password;
  jsonObject["website"] = ptr->website;
  return jsonObject;
}

safe_string jsonify(const SimpleLogin      *item)
{
  nlohmann::json retn = jsonify_SimpleLogin(item);
  return retn.dump();
}

safe_string jsonify(const twofa_login      *item)
{
  
  nlohmann::json retn = jsonify_SimpleLogin(item);
  retn["secret"] = base64_encode(ustring_to_string(item->totp_secret));
  return retn.dump();
}
safe_string jsonify(const Folder           *item)
{
  nlohmann::json retn = jsonify_Item(item);
  nlohmann::json content_array = nlohmann::json::array();
  for(auto &element : item->content)
    content_array.push_back(jsonify(element.get()));
  retn["content"] = content_array;
  return retn.dump();
}
safe_string jsonify(const EncryptedFolder  *item)
{
  item->close(); // Apply changes (HEAD -> `content_base64`)
  nlohmann::json json_obj = jsonify_Item(item);
  json_obj["content"] = item->content_base64;
  return json_obj.dump();
}
safe_string jsonify(const UnknownItem      *item)
{
  return item->dump;
}
