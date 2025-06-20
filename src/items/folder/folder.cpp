#include "folder.hxx"
#include <utility>

Folder::Folder( Folder&& obj ) noexcept : Item( std::move( obj.name ) ), content( std::move( obj.content ) ) {}

Folder::Folder( const safe_string& name )
  :Item( name ) {}

Folder::Folder(Folder& obj) noexcept
:Item(obj.name),content(obj.content)
{}

void Folder::accept_visit( display_visitor& visitor )
{
  visitor.visit( *this );
}

std::string Folder::get_type() const noexcept 
{
  return this->type;
}
