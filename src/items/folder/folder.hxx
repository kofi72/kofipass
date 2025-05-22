#pragma once
#ifndef FOLDER_HXX_INCLUDED
#define FOLDER_HXX_INCLUDED

#include "../item_base/item_base.hxx"

#include <memory>

struct Folder : public Item
{
  static constexpr char const*const type = "folder";
  std::vector<std::unique_ptr<Item>> content;

  Folder(Folder&& obj)noexcept;
  Folder(const safe_string& name);
  Folder(nlohmann::json data);
  virtual ~Folder() = default;

  nlohmann::json json() override;
  void accept_visit(display_visitor& visitor) override;
};

#endif // FOLDER_HXX_INCLUDED
