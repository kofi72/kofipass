#include "slogin.hxx"

SimpleLogin::SimpleLogin(const safe_string name, const safe_string login, const safe_string password, const safe_string website)
  :Item( name ), login( login ), password( password ), website(website) {}

SimpleLogin::SimpleLogin( nlohmann::json data )
  :Item( data["name"] ), login( data["login"] ), password( data["passwd"] ), website(data["website"])
{}

nlohmann::json SimpleLogin::json()
{
  nlohmann::json json_obj = Item::json();
  json_obj["type"] = this->type;
  json_obj["login"] = this->login;
  json_obj["passwd"] = this->password;
  json_obj["website"] = this->website;
  return json_obj;
}

void SimpleLogin::accept_visit( display_visitor& visitor )
{
  visitor.visit( *this );
}
