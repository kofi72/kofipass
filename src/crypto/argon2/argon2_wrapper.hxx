#pragma once

#ifndef ARGON2_WRAPPER_HXX_INCLUDED
#define ARGON2_WRAPPER_HXX_INCLUDED

#include "../../memory/memory.hxx"
#include <string>

safe_string argon2_kdf(const safe_string& password);
#endif // ARGON2_WRAPPER_HXX_INCLUDED
