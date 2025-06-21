#pragma once
#ifndef UNKNOWN_ITEM_HXX
#define UNKNOWN_ITEM_HXX 1

#include "../item_base/item_base.hxx"
#include <string>

struct UnknownItem : public Item
{
  std::string type;
  safe_string dump;
  UnknownItem(safe_string name, std::string type, safe_string dump);
  void accept_visit(item_visitor& visitor) override;
  std::string get_type() const noexcept override;
};

#endif
