#pragma once
#ifndef SLOGIN_HXX_INCLUDED
#define SLOGIN_HXX_INCLUDED

#include "../item_base/item_base.hxx"

struct SimpleLogin : public Item
{
  static constexpr char const*const type = "simplelogin";
  safe_string login, password, website;

  SimpleLogin(const safe_string name, const safe_string login, const safe_string password, const safe_string website);
  SimpleLogin(nlohmann::json data);
  virtual ~SimpleLogin() = default;

  nlohmann::json json() override;
  void accept_visit(display_visitor& visitor) override;
};

#endif // SLOGIN_HXX_INCLUDED
