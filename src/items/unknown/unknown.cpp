#include "unknown.hxx"

UnknownItem::UnknownItem(safe_string name, std::string type, safe_string dump)
:Item(name), type(std::move(type)), dump(std::move(dump))
{}

void UnknownItem::accept_visit(item_visitor &visitor)
{
  visitor.visit( *this );
}

std::string UnknownItem::get_type() const noexcept
{
  return this->type;
}
