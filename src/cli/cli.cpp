#include "../items/items.hxx"
#include "tools/tools.hxx"
#include "field/field.cpp"
#include "edit/edit.cpp"
#include "display/display.cpp"

#include <iostream>
#include <string>

bool confirm(std::string what)
{
  std::cout << what << "? y/n ";
  char choice;
  while(true)
  {
    std::cin >> choice;
    std::cin.ignore();
    if(choice == 'y')
      return true;
    if(choice == 'n')
      return false;
  }
}

bool cli_open_file(std::string file_path)
{
  using std::cout, std::cin, std::string;
  std::filesystem::path file(file_path);
  if(!std::filesystem::exists(file))
  {
    return cli_open_file(file_path+".kpass");
    cout << color::red << "No such file!\n" << color::normal;
    return false;
  }

  std::unique_ptr<crypto_provider> encryptor = cli_setup_encryption();

  string json_str;
  try
  {
    json_str = encryptor->load_file(file_path);
  }
  catch(std::runtime_error &e)
  {
    cout << color::red << "File load failed\n" << color::normal;
    return false;
  }
  nlohmann::json json;
  try
  {
    json = nlohmann::json::parse(json_str);
  }
  catch(nlohmann::json_abi_v3_11_3::detail::parse_error &e)
  {
    cout << color::red << "Bad password or corrupted data\n" << color::normal;
    return false;
  }
  Folder root(json);

  cli_display_system display;
  root.accept_visit(display);

  // if root folder content changed
  // prompt save changes
  if(json_str !=  root.json().dump())
  {
    if(confirm("save to file"))
    {
      encryptor->export_to_file(file_path, root.json().dump());
      cout << color::green << "Exported to file!\n" << color::normal;
    }
  }
  return true;
}

void cli_open_file()
{
  using std::cout, std::cin, std::string;
  cout << color::magenta << "password manager cli\n" << color::normal;

  cout << "file path: ";
  string file_path;
  cin >> file_path;
  cin.ignore();
  cli_open_file(file_path);
}

void cli_new_file()
{
  using std::cout, std::cin, std::string;
  cout << color::magenta << "create new password vault\n" << color::normal;

  string path;
  cout << "Enter new file path: ";
  cin >> path;
  cin.ignore();

  safe_string name;
  cout << "Enter vault name: ";
  std::getline(cin, name);

  std::unique_ptr<crypto_provider> encryptor = cli_setup_encryption();
  Folder root(name);
  try
  {
    encryptor->export_to_file(path, root.json().dump());
  }
  catch(std::runtime_error &e)
  {
    cout << color::red << "No such directory!\n" << color::normal;
    return;
  }
  cout << color::green << "created new file: " << path << '\n' << color::normal;
}
