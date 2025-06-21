#include <string>
#include <iostream>

#include "../tools/tools.hxx"
struct : public item_visitor
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
      switch(choice)
      {
        case 'n':
        {
          std::string new_name;
          std::getline( cin, new_name );
          item.name = new_name;
        }
        break;
        case 'l':
        {
          std::string new_login;
          std::getline( cin, new_login );
          item.login = new_login;
        }
        break;
        case 'p':
          item.password = password_prompt( "New password", 2);
        break;
        case 'q':
          break;
        case 't':
        case 'r': // fall-through
          {
            cout << color::red << "[HALT] " << color::normal << "This action cannot be performed on this object!\n";
            std::string dummy;
            std::getline( cin, dummy );
          }
        break;
        default:
          cout << color::red << "[WARN]" << color::normal << " ACTION DISCARDED: `" << choice << "`\n";
        break;
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
      switch(choice)
      {
        case 'n':
        {
          std::string new_name;
          std::getline( cin, new_name );
          item.name = new_name;
        }
        break;
        case 'p':
        case 'l': // fall-through
        case 't': // fall-through
        case 'r': // fall-through
          {
            cout << color::red << "[HALT] " << color::normal << "This action cannot be performed on this object!\n";
            std::string dummy;
            std::getline( cin, dummy );
          }
        break;
        case 'q':
          break;
        default:
          cout << color::red << "[WARN]" << color::normal << " ACTION DISCARDED: `" << choice << "`\n";
        break;
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
      switch(choice)
      {
        case 'n':
        {
          std::string new_name;
          std::getline( cin, new_name );
          item.name = new_name;
        }
        break;
        case 'p':
        case 'l': // fall-through
        case 't': // fall-through
        case 'r': // fall-through
          {
            cout << color::red << "[HALT] " << color::normal << "This action cannot be performed on this object!\n";
            std::string dummy;
            std::getline( cin, dummy );
          }
        break;
        case 'q':
          break;
        default:
          cout << color::red << "[WARN]" << color::normal << " ACTION DISCARDED: `" << choice << "`\n";
        break;
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
          << "n) name"          "\n"
          << "l) login"         "\n"
          << "p) password"      "\n"
          << "t) TOTP secret"   "\n"
          << "q) exit"          "\n";
      cin >> choice;
      cin.ignore();
      switch(choice)
      {
        case 'n':
        {
          std::string new_name;
          std::getline( cin, new_name );
          item.name = new_name;
        }
        break;
        case 'l':
        {
          std::string new_login;
          std::getline( cin, new_login );
          item.login = new_login;
        }
        break;
        case 'p':
          item.password = password_prompt( "New password", 2);
        break;
        case 't':
        {
          std::string new_secret;
          std::getline( cin, new_secret );
          item.set_secret( new_secret );
        }
        break;
        case 'q':
          break;
        case 'r': // fall-through
          {
            cout << color::red << "[HALT] " << color::normal << "This action cannot be performed on this object!\n";
            std::string dummy;
            std::getline( cin, dummy );
          }
        break;
        default:
          cout << color::red << "[WARN]" << color::normal << " ACTION DISCARDED: `" << choice << "`\n";
        break;
      }
    }
  }
  void visit( UnknownItem& item)
  {
    using std::cin, std::cout;
    char choice = 0;
    while(choice != 'q')
    {
      cout 
        << color::red << "[WARNING] Unknown item type""\n"
        << "r) raw"                                   "\n"
        << "q) exit"                                  "\n";
      cin >> choice;
      cin.ignore();
      switch(choice)
      {
        case 'r':
        {
          safe_string new_raw;
          std::getline( cin, new_raw );
          item.dump = new_raw; // sync might be problematic while not relying on json implementation. The object should be superseeded?
          cout << "Item dump was updated, yet the object information is desynchronized. It will take effect the next time vault is exported.\n";
        }
        break;
        case 'p':
        case 'l': // fall-through
        case 't': // fall-through
        case 'n': // fall-through
          {
            cout << color::red << "[HALT] " << color::normal << "This action cannot be performed on this object!\n";
            std::string dummy;
            std::getline( cin, dummy );
          }
        break;
        break;
        case 'q':
          break;
      }
    }
  }
  /*
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
  // */
} item_editor;
