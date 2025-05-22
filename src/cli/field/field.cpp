#include <iostream>

void field_display_name( field_base const*const field )
{
  constexpr const static int name_size = 16;
  std::cout.fill( ' ' );
  std::cout << std::left << std::setw( name_size ) << ( field->name+":" );
}

// show sum

struct : public little_visitor
{
  void visit(text_field& field ) const override
  {
    field_display_name( &field );
    std::cout << field.content;
  }
  void visit(hidden_field& field ) const override
  {
    field_display_name( &field );
    std::cout << "(hidden)";
  }
  void visit(totp_field& field ) const override
  {
    field_display_name( &field );
    std::cout << field.get_totp();
  }
} little_visitor_summary;

// show full

struct : public little_visitor
{
  void visit(text_field& field ) const override
  {
    field_display_name( &field );
    std::cout << field.content << '\n';
    interrupt();
  }
  void visit(hidden_field& field ) const override
  {
    field_display_name( &field );
    std::cout << field.content << '\n';
    interrupt();
  }
  void visit(totp_field& field ) const override
  {
    field_display_name( &field );
    std::cout << field.get_totp();
    interrupt();
  }
} little_visitor_full;

// editor

struct : public little_visitor
{
  void visit(text_field& field ) const override
  {
    char choice = 0;
    while(choice != 'q')
    {
      std::cout << "n) name\n"
        << "c) content\n"
        << "q) exit\n";
      std::cin >> choice;
      std::cin.ignore();
      if(choice == 'n')
      {
        std::cout << "New name: ";
        std::string new_name;
        std::getline(std::cin, new_name);
        field.name = new_name;
      }
      else if(choice == 'c')
      {
        std::cout << "New content: ";
        std::string new_content;
        std::getline(std::cin, new_content);
        field.content = new_content;
      }
    }
  }
  void visit(hidden_field& field ) const override
  {
    char choice = 0;
    while(choice != 'q')
    {
      std::cout << "n) name\n"
        << "c) content\n"
        << "q) exit\n";
      std::cin >> choice;
      std::cin.ignore();
      if(choice == 'n')
      {
        std::cout << "New name: ";
        std::string new_name;
        std::getline(std::cin, new_name);
        field.name = new_name;
      }
      else if(choice == 'c')
      {
        std::cout << "New content: ";
        std::string new_content;
        std::getline(std::cin, new_content);
        field.content = new_content;
      }
    }
  }
  void visit(totp_field& field ) const override
  {
    char choice = 0;
    while(choice != 'q')
    {
      std::cout << "n) name\n"
        << "s) TOTP secret\n"
        << "q) exit\n";
      std::cin >> choice;
      std::cin.ignore();
      if(choice == 'n')
      {
        std::cout << "New name: ";
        std::string new_name;
        std::getline(std::cin, new_name);
        field.name = new_name;
      }
      else if(choice == 's')
      {
        std::cout << "New TOTP secret base32: ";
        std::string new_secret;
        std::getline(std::cin, new_secret);
        field.change_secret(new_secret);
      }
    }
  }
} little_visitor_edit;
