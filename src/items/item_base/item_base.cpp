#include "item_base.hxx"

Item::Item( const safe_string& name )
  :name( name ) {}

nlohmann::json Item::json()
{
  nlohmann::json json_obj;
  json_obj["name"] = this->name;
  return json_obj;
}
