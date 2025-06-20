#pragma once
#ifndef FOLDER_HXX_INCLUDED
#define FOLDER_HXX_INCLUDED

#include "../item_base/item_base.hxx"

#include <memory>
#include <string>

struct Folder : public Item
{
  static constexpr char const*const type = "folder";
  std::vector<std::shared_ptr<Item>> content;

  Folder(Folder&& obj) noexcept;
  Folder(Folder& obj) noexcept;
  Folder(const safe_string& name);
  virtual ~Folder() = default;

  std::string get_type() const noexcept override;
  void accept_visit(display_visitor& visitor) override;
};


#endif // FOLDER_HXX_INCLUDED
