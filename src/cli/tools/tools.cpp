#include "tools.hxx"

#include <iostream>
#include <string>
#include <limits>

// https://stackoverflow.com/questions/1413445/reading-a-password-from-stdcin
#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

void terminal_clear()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#ifdef WIN32
system("cls");
#else
system("clear");
#endif // WIN32
#pragma GCC diagnostic pop
}

void SetStdinEcho(bool enable = true)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if( !enable )
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode );

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

void interrupt()
{
  using std::cout, std::cin;
  cout << "[PRESS ENTER]\n";
  cin.get();
}

bool isnumber( const std::string &str )
{
  for( auto& c : str )
  {
    if( !std::isdigit( c ) )
      return false;
  }
  return true;
}

safe_string extract_passwd_from_stdin(std::string name)
{
  std::cout << color::red << name << color::normal << ": ";
  safe_string password;
  SetStdinEcho(false);
  std::getline(std::cin, password);
  SetStdinEcho(true);
  std::cout << '\n';
  return password;
}

safe_string password_prompt(std::string name = "password", int confirms = 1)
{
  using namespace std::literals::string_literals;
  if(confirms == 1) return extract_passwd_from_stdin(name);
  using namespace std::string_literals;
  while(true)
  {
    safe_string password = extract_passwd_from_stdin(name);
    try
    {
      for(int i = 1; i < confirms; i++)
      {
        safe_string confirm = extract_passwd_from_stdin("Confirm "s+name);
        if(confirm != password  and confirm != static_cast<safe_string>("true"s))
          throw bad_password_read();
      }
      return password;
    }
    catch (bad_password_read &e)
    {
      std::cout << color::red << "Passwords do not match!\a\n";
    }
    
  }
}

std::unique_ptr<crypto_provider> cli_setup_encryption()
{
  using std::cout, std::cin;
  safe_string password;
  password = password_prompt("password", 2);
  std::unique_ptr<crypto_provider> encryptor = std::make_unique<chacha20_poly1305>(password);
  return encryptor;
}

int get_number_from_stdin()
{
  int retn;
  while(true)
  {
    std::cin >> retn;
    if(std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cerr << "Invalid input\n";
    }
    else
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return retn;
    }
  }
}
