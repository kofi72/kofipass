#include <string>
#include <iostream>

#include "../tools/tools.hxx"

std::unique_ptr<field_base> textFieldCreator()
{
  safe_string name, content;
  std::cout << "Field name: ";
  std::getline( std::cin, name );
  std::cout << "Field content: ";
  std::getline( std::cin, content );
  return std::make_unique<text_field>( name, content );
}
std::unique_ptr<field_base> hiddenFieldCreator()
{
  safe_string name, content;
  std::cout << "Field name: ";
  std::getline( std::cin, name );
  std::cout << "Field content: ";
  SetStdinEcho(false);
  std::getline( std::cin, content );
  SetStdinEcho(true);
  return std::make_unique<hidden_field>( name, content );
}
std::unique_ptr<field_base> totpFieldCreator()
{
  safe_string name, totp_secret_base32;
  std::cout << "Field name: ";
  std::getline( std::cin, name );
  std::cout << "base32 totp secret: ";
  std::getline( std::cin, totp_secret_base32 );
  return std::make_unique<totp_field>( name, totp_secret_base32 );
}
std::unique_ptr<field_base> customFieldCreator()
{
  for( ;; )
  {
    std::cout << "Field type:\n"
              << "1) text field\n"
              << "2) hidden text field\n"
              << "3) TOTP field\n";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    switch( choice )
    {
    case '1':
      return textFieldCreator();
      break;
    case '2':
      return hiddenFieldCreator();
      break;
    case '3':
      return totpFieldCreator();
      break;
    default:
      std::cout << "No such field type\n";
      break;
    }
  }
}

struct : public display_visitor
{
  void visit( SimpleLogin& item )
  {
    using std::cin, std::cout;
    char choice = 0;
    while( choice != 'q' )
    {
      cout
          << "n) name\n"
          << "l) login\n"
          << "p) password\n"
          << "q) exit\n";
      cin >> choice;
      cin.ignore();
      if( choice == 'n' )
      {
        std::string new_name;
        std::getline( cin, new_name );
        item.name = new_name;
      }
      else if( choice == 'l' )
      {
        std::string new_login;
        std::getline( cin, new_login );
        item.login = new_login;
      }
      else if( choice == 'p' )
      {
        std::string new_password = password_prompt( "New password" );
        item.password = new_password;
      }
    }
  }
  void visit( Folder& item )
  {
    using std::cin, std::cout;
    char choice = 0;
    while( choice != 'q' )
    {
      cout
          << "n) edit name\n"
          << "q) exit\n";
      cin >> choice;
      cin.ignore();
      if( choice == 'n' )
      {
        std::string new_name;
        std::getline( cin, new_name );
        item.name = new_name;
      }
    }
  }
  void visit( EncryptedFolder& item )
  {
    using std::cin, std::cout;
    char choice = 0;
    while( choice != 'q' )
    {
      cout
          << "n) edit name\n"
          << "q) exit\n";
      cin >> choice;
      cin.ignore();
      if( choice == 'n' )
      {
        std::string new_name;
        std::getline( cin, new_name );
        item.name = new_name;
      }
    }
  }
  void visit( twofa_login& item )
  {
    using std::cin, std::cout;
    char choice = 0;
    while( choice != 'q' )
    {
      cout
          << "n) name\n"
          << "l) login\n"
          << "p) password\n"
          << "t) TOTP secret\n"
          << "q) exit\n";
      cin >> choice;
      cin.ignore();
      if( choice == 'n' )
      {
        std::string new_name;
        std::getline( cin, new_name );
        item.name = new_name;
      }
      else if( choice == 'l' )
      {
        std::string new_login;
        std::getline( cin, new_login );
        item.login = new_login;
      }
      else if( choice == 'p' )
      {
        std::string new_password = password_prompt( "New password" );
        item.password = new_password;
      }
      else if( choice == 't' )
      {
        std::string new_secret;
        std::getline( cin, new_secret );
        item.set_secret( new_secret );
      }
    }
  }
  void visit( customItem& item )
  {
    char choice = 0;
    while( choice != 'q' )
    {
      std::cout << color::magenta << "CONTENT:\n" << color::normal;
      int index = 0;
      for( auto it = item.content.begin(); it < item.content.end(); it++ )
      {
        std::cout << index++ << ") "; (*it)->accept_visit( little_visitor_summary );
        std::cout << "\n";
      }
      std::cout
          << color::magenta << "MENU:\n" << color::normal
          << "n) edit name\n"
          << "a) add field\n"
          << "d) delete field\n"
          << "e) edit field\n"
          << "q) exit\n";

      std::cin >> choice;
      std::cin.ignore();
      if( choice == 'n' )
      {
        std::string new_name;
        std::getline( std::cin, new_name );
        item.name = new_name;
      }
      else if( choice == 'a' )
      {
        std::cout << "Creating field...\n";
        item.content.emplace_back(customFieldCreator());
      }
      else if(choice == 'd')
      {
        std::cout << "Removing field...\n";
        std::cout << "index?>";
        int index = get_number_from_stdin();
        if(index < 0 || index >= (int)item.content.size())
          std::cout << color::red << "No such index!" << color::normal << '\n';
        else
          item.content.erase(item.content.begin()+index);
      }
      else if(choice == 'e')
      {
        std::cout << "index?>";
        int index = get_number_from_stdin();
        if(index < 0 || index >= (int)item.content.size())
          std::cout << color::red << "No such index!" << color::normal << '\n';
        else
          item.content[index]->accept_visit(little_visitor_edit);
      }
    }
  }
} item_editor;

