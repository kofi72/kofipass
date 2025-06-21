#include "slogin.hxx"

SimpleLogin::SimpleLogin(const safe_string name, const safe_string login, const safe_string password, const safe_string website)
  :Item( name ), login( login ), password( password ), website(website) {}

void SimpleLogin::accept_visit( item_visitor& visitor )
{
  visitor.visit( *this );
}

std::string SimpleLogin::get_type() const noexcept 
{
  return this->type;
}
