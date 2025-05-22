#include "../summary/summary.cpp"

#include <string>
#include <iostream>
#include <iomanip>
#include <cctype>

class cli_display_system : public display_visitor
{
private:
  std::unique_ptr<Item> folder_creator() const
  {
    using std::cout, std::cin;
    safe_string folder_name;
    cout << "Enter folder name: ";
    std::getline( cin, folder_name );
    return std::make_unique<Folder>( folder_name );
  }
  std::unique_ptr<Item> simplelogin_creator() const
  {
    using std::cout, std::cin;
    std::string name, login, password, website;
    cout << "Enter name: ";
    std::getline( cin, name );
    cout << "Enter login: ";
    std::getline( cin, login );
    password = password_prompt( "Enter password" );
    cout << "Enter website: ";
    std::getline( cin, website );
    return std::make_unique<SimpleLogin>( name, login, password, website );
  }
  std::unique_ptr<Item> encryptedfolder_creator() const
  {
    using std::cout, std::cin;
    std::string name;
    cout << "Enter name: ";
    std::getline( cin, name );
    return std::make_unique<EncryptedFolder>( name, cli_setup_encryption() );
  }
  std::unique_ptr<Item> twofa_login_creator() const
  {
    using std::cout, std::cin;
    std::string name, login, password, website, secret;
    cout << "Enter name: ";
    std::getline( cin, name );
    cout << "Enter login: ";
    std::getline( cin, login );
    password = password_prompt( "Enter password" );
    cout << "Enter website: ";
    std::getline( cin, website );
    secret = password_prompt( "Enter 2FA secret" );
    return std::make_unique<twofa_login>( name, login, password, website, secret );
  }
  std::unique_ptr<Item> customItem_creator() const
  {
    using std::cout, std::cin;
    safe_string name;
    cout << "Enter item name: ";
    std::getline( cin, name);
    return std::make_unique<customItem>(name);
  }
  std::unique_ptr<Item> new_item_cli() const
  {
    using std::cout, std::cin;
    cout << "Select item type: \n"
         << "1) Folder\n"
         << "2) Simple login\n"
         << "3) Encrypted Folder\n"
         << "4) 2fa login\n"
         << "5) Customizable item\n";
    while( true )
    {
      int choice;
      cout << "?>";
      cin >> choice;
      cin.ignore();
      switch ( choice )
      {
      case 1:
        return folder_creator();
        break;
      case 2:
        return simplelogin_creator();
        break;
      case 3:
        return encryptedfolder_creator();
        break;
      case 4:
        return twofa_login_creator();
        break;
      case 5:
        return customItem_creator();
        break;
      }
    }
  }
  void edit_item_cli( std::unique_ptr<Item>& item )
  {
    item->accept_visit( item_editor );
  }
public:
  void visit( Folder& item ) override
  {
    using std::cout, std::cin;
    cout << "Folder " << color::yellow << item.name  << color::normal << ":\n";

    for( ;; )
    {
      int index = 0;
      cout.fill( '0' );
      cout << std::right;
      cout << color::magenta << "CONTENT:\n" << color::normal;
      for( auto it = item.content.begin(); it < item.content.end(); it++ )
      {
        cout << std::setw( 2 ) << index++ << ") ";
        ( *it )->accept_visit( item_summary );
        cout << '\n';
      }
      cout << "?>";
      std::string choice;
      cin >> choice;
      cin.ignore();
      if( !isnumber( choice ) )
      {
        switch( choice[0] )
        {
        case '?':
        case 'h': // fall-through
          cout << color::magenta << "MENU:\n" << color::normal;
          cout << "a) add entry\n";
          cout << "r) remove entry\n";
          cout << "s) swap entries\n";
          cout << "e) edit entry\n";
          cout << "c) clear terminal\n";
          cout << "q) exit folder\n";
          cout << "d) decrypt encrypted folder\n";
          interrupt();
          break;
        case 'q':
          return;
          break;
        case 'r':
        case 'e': // fall-through
        case 'd': // fall-through
        case 's': // fall-through
          cout << "Index?> ";
          cin >> index;
          cin.ignore();
          if( index >= ( int )item.content.size() || index < 0 )
          {
            cout << color::red << "No such position\n" << color::normal;
            break;
          }
          if( choice[0] == 'r' )
            item.content.erase( item.content.begin()+index );
          else if( choice[0] == 'e' )
            edit_item_cli( item.content[index] );
          else if(choice[0] == 'd')
          {
            EncryptedFolder* efolder = dynamic_cast<EncryptedFolder*>(item.content[index].get());
            if(!efolder)
            {
              cout << color::red << "This item is not an encrypted folder!\n" << color::normal;
              break;
            }
            if(!efolder->is_open())
            {
              cout << color::red << "Folder is not open!\n" << color::normal;
              break;
            }
            std::cerr << "extracting folder...\n";
            std::unique_ptr<Folder> extracted = efolder->extract_folder();
            std::cerr << "replacing item...\n";
            item.content[index] = std::move(extracted);
          }
          else if(choice[0] == 's')
          {
            cout << "Index?> ";
            int index2;
            cin >> index2;
            cin.ignore();
            if( index >= ( int )item.content.size() || index < 0 )
            {
              cout << color::red << "No such position\n" << color::normal;
              break;
            }
            std::swap(item.content[index], item.content[index2]);
          }
          break;
        case 'a':
          item.content.push_back( new_item_cli() );
          break;
        case 'c':
          terminal_clear();
          break;
        }
        continue;
      }
      else // is number
      {
        index = stoi( choice );
        if( index >= ( int )item.content.size() || index < 0 )
        {
          cout << color::red << "No such position\n" << color::normal;
          interrupt();
        }
        else
          item.content[index]->accept_visit( *this );
      }
    }
  }
  void visit( SimpleLogin& item ) override
  {
    using std::cout, std::cin;
    cout << color::yellow << item.name  << color::normal << ":\n"
         << "  login:    " << item.login << '\n'
         << "  password: " << item.password << '\n'
         << "  website:  " << item.website << '\n';
    interrupt();
  }
  void visit( EncryptedFolder& item ) override
  {
    if( !item.is_open() )
    {
      try
      {
        item.open( cli_setup_encryption() );
      }
      catch( encryption_error &e )
      {
        std::cerr << "encryption error!\n";
        return;
      }
    }
    item.content->accept_visit( *this );
  }
  void visit( twofa_login& item )
  {
    using std::cin, std::cout;
    cout << color::yellow << item.name  << color::normal << ":\n"
         << "  login:    " << item.login << '\n'
         << "  password: " << item.password << '\n'
         << "  website:  " << item.website << '\n'
         << "  TOTP:     " << item.get_totp() << '\n';
    interrupt();
  }
  void visit( customItem& item )
  {
    using std::cin, std::cout;
    for( ;; )
    {
      cout << color::yellow << item.name << color::normal << ":\n";
      cout << "q) exit\n"
           << "e) edit\n";
      int index = 0;
      for( auto it = item.content.begin(); it < item.content.end(); it++ )
      {
        cout << index++ << ") "; (*it)->accept_visit( little_visitor_summary );
        cout << "\n";
      }
      cout << "?>";
      std::string choice;
      cin >> choice;
      cin.ignore();
      if( !isnumber( choice ) )
      {
        switch( choice[0] )
        {
        case 'q':
          return;
          break;
        case 'e':
          item.accept_visit(item_editor);
          break;
        }
        continue;
      }
      else
      {
        index = stoi( choice );
        if( index >= ( int )item.content.size() || index < 0 )
        {
          cout << color::red << "No such position\n" << color::normal;
          interrupt();
        }
        else
          item.content[index]->accept_visit( little_visitor_full );
      }
    }
  }
};
