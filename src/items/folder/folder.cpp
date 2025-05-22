#include "folder.hxx"
#include <utility>

Folder::Folder( Folder&& obj ) noexcept : Item( std::move( obj.name ) ), content( std::move( obj.content ) ) {}

Folder::Folder( const safe_string& name )
  :Item( name ) {}

Folder::Folder( nlohmann::json data )
  :Item( data["name"] )
{
  assert( data["type"] == this->type );
  auto content_array = data["content"];
  if( !content_array .is_array() )
    throw std::logic_error( "folder content is no json array" );
  for( auto& i : content_array )
    content.emplace_back( new_item_from_json( i ) );
}

nlohmann::json Folder::json()
{
  using json = nlohmann::json;
  json json_obj = Item::json();
  json_obj["type"] = this->type;
  json content_arr = json::array();
  for( auto it = content.begin(); it < content.end(); it++ )
    content_arr.push_back( ( *it )->json() );
  json_obj["content"] = content_arr;
  return json_obj;
}

void Folder::accept_visit( display_visitor& visitor )
{
  visitor.visit( *this );
}
