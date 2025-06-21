#pragma once
#ifndef EXPORT_ITEMS_TO_JSON
#define  EXPORT_ITEMS_TO_JSON

#include <memory>
#include "../items/items.hxx"
#include "../memory/memory.hxx" // safe_string

std::shared_ptr<Item> new_item_from_json(safe_string data);

safe_string jsonify(const Item*            item);
safe_string jsonify(const SimpleLogin*     item);
safe_string jsonify(const twofa_login*     item);
safe_string jsonify(const Folder*          item);
safe_string jsonify(const EncryptedFolder* item);
safe_string jsonify(const UnknownItem*     item);

#endif
