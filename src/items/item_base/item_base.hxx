#ifndef ITEM_BASE_HXX_INCLUDED
#define ITEM_BASE_HXX_INCLUDED

#include "../visit/visitable.hxx"
#include "../json/item_json.hxx"
#include "../../memory/memory.hxx"

#include <memory>

struct Item : public jsonExportable, public visitable
{
  safe_string name;

  Item(const safe_string& name);
  virtual ~Item() = default;

  nlohmann::json json() override;
};

std::unique_ptr<Item> new_item_from_json(nlohmann::json data);

#endif // ITEM_BASE_HXX_INCLUDED
