#ifndef CLI_HXX_INCLUDED
#define CLI_HXX_INCLUDED

#include <string>

bool confirm(std::string what);

void cli_new_file();
void cli_open_file();
void cli_open_file(std::string file_path);


#endif // CLI_HXX_INCLUDED
