#include "custom.hxx"

customItem::customItem(const safe_string& name)
:Item(name) {}

customItem::customItem(nlohmann::json data)
:Item( data["name"] )
{
  assert(data["type"] == this->type);
  auto content_array = data["content"];
  if( !content_array .is_array() )
    throw std::logic_error( "custom item content is not a json array" );
  for( auto& i : content_array )
    content.emplace_back( new_field_from_json( i ) );
}

nlohmann::json customItem::json()
{
  nlohmann::json json_obj = Item::json();
  json_obj["type"] = this->type;

  nlohmann::json content_arr = nlohmann::json::array();
  for( auto it = content.begin(); it < content.end(); it++ )
    content_arr.push_back( (*it)->json() );
  json_obj["content"] = content_arr;
  return json_obj;
}

void customItem::accept_visit(display_visitor& visitor)
{
  visitor.visit(*this);
}
