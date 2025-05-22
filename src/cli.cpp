#include "cli/cli.hxx"
#include "test/tests.hxx"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
  srand(time(NULL));
  run_test();
  if(argc == 2)
  {
    cli_open_file(argv[1]);
    return 0;
  }
  for( ;; )
  {
    cout
        << "o) open file\n"
        << "n) new file\n"
        << "q) exit\n"
        << "?>";

    char input;
    cin >> input;
    cin.ignore();

    switch( input )
    {
    case 'o':
      cli_open_file();
      break;
    case 'n':
      cli_new_file();
      break;
    case 'q':
      exit( EXIT_SUCCESS );
      break;
    }
  }
  return 0;
}
