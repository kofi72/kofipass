#ifndef ITEM_JSON_HXX_INCLUDED
#define ITEM_JSON_HXX_INCLUDED

#include "../../json/json.hpp"

struct jsonExportable
{
  virtual ~jsonExportable() = default;
  virtual nlohmann::json json() = 0;
};

#endif // ITEM_JSON_HXX_INCLUDED
