#include "cli/cli.hxx"
#include "test/tests.hxx"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <signal.h>

#include <fcntl.h>

using namespace std;

void dissmiss(int signal)
{
  std::cerr << "dissmised signal: " << signal << endl;
}

int main(int argc, char* argv[])
{
  signal(SIGINT, dissmiss);
  srand(time(NULL));
  // TEST
  // run_test();
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
