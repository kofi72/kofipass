#pragma once

#ifndef TOOLS_HXX_INCLUDED
#define TOOLS_HXX_INCLUDED

#include "../../crypto/crypto_provider.hxx"
// #include "../../memory/memory.hxx"
// ^included by crypto provider

#include <string>

namespace color
{
  const std::string normal  = "\033[0m";
  const std::string red     = "\033[31;1m";
  const std::string green   = "\033[32;1m";
  const std::string yellow  = "\033[33;1m";
  const std::string blue    = "\033[34;1m";
  const std::string magenta = "\033[35;1m";
  const std::string cyan    = "\033[36;1m";
};

void terminal_clear();
void SetStdinEcho(bool enable);

void interrupt();
bool isnumber( const std::string &str );

class bad_password_read{};
safe_string password_prompt(std::string name/* = "password" */, int confirms);
std::unique_ptr<crypto_provider> cli_setup_encryption();

int get_number_from_stdin();

#endif // TOOLS_HXX_INCLUDED
