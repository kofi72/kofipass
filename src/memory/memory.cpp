#include "memory.hxx"

#include <cstdlib>

safe_string::safe_string() : std::string() {}
safe_string::safe_string( const char* s ) : std::string( s ) {}
safe_string::safe_string( const std::string& s ) : std::string( s ) {}
safe_string::safe_string( const safe_string& s ) : std::string( s ) {}
safe_string::safe_string( const std::string&& s) : std::string( s ) {}
safe_string::~safe_string()
{
  secure_erase();
}

safe_string& safe_string::operator=( const safe_string& s )
{
  if ( this == &s )
    return *this;
  secure_erase();
  std::string::operator=( s );
  return *this;
}

void safe_string::clear() noexcept
{
  secure_erase();
  std::string::clear();
}

void safe_string::secure_erase()
{
  for ( auto& ch : *this )
  {
    ch = rand();
    ch = 0;
  }
}
