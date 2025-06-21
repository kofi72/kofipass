#pragma once
#ifndef SLOGIN_HXX_INCLUDED
#define SLOGIN_HXX_INCLUDED

#include "../item_base/item_base.hxx"

struct SimpleLogin : public Item
{
  static constexpr char const*const type = "simplelogin";
  safe_string login, password, website;

  SimpleLogin(const safe_string name, const safe_string login, const safe_string password, const safe_string website);
  virtual ~SimpleLogin() = default;

  void accept_visit(item_visitor& visitor) override;
  std::string get_type() const noexcept override;
};

#endif // SLOGIN_HXX_INCLUDED
