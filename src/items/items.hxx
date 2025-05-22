#pragma once
#ifndef ITEMS_HXX_INCLUDED
#define ITEMS_HXX_INCLUDED

#include "encrypted_folder/efolder.hxx"
#include "2falogin/2falogin.hxx"
#include "custom/custom.hxx"

#include <memory>

std::unique_ptr<Item> new_item_from_json( nlohmann::json data );

#endif // ITEMS_HXX_INCLUDED
