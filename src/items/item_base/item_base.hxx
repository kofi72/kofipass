#ifndef ITEM_BASE_HXX_INCLUDED
#define ITEM_BASE_HXX_INCLUDED

#include "../visit/visitable.hxx"
#include "../json/item_json.hxx"
#include "../../memory/memory.hxx"

#include <memory>

struct Item : public visitable
{
  safe_string name;


  Item(const safe_string& name);
  virtual ~Item() = default;
  virtual std::string get_type() const noexcept = 0;
};

#endif // ITEM_BASE_HXX_INCLUDED
