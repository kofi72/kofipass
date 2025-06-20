#include "../../items/items.hxx"
#include "../tools/tools.hxx"

#include <iostream>
#include <string>

class : public display_visitor
{
public:
  void visit( Folder& item ) override
  {
    std::cout << color::cyan << item.name << color::normal;
  }
  void visit( SimpleLogin& item ) override
  {
    std::cout << item.name;
  }
  void visit( EncryptedFolder& item ) override
  {
    if(item.is_open())
      std::cout << color::green;
    else
      std::cout << color::red;
    std::cout << item.name << color::normal;
  }
  void visit( twofa_login& item) override
  {
    std::cout << item.name;
  }
  /*
  void visit(customItem& item) override
  {
    std::cout << item.name;
  }
  // */
}item_summary;
