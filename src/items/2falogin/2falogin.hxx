#ifndef TWOFALOGIN_HXX_INCLUDED
#define TWOFALOGIN_HXX_INCLUDED

#include "../simple_login/slogin.hxx"
#include <string>

std::basic_string<uint8_t> string_to_ustring(std::string str);

struct twofa_login : public SimpleLogin
{
  static constexpr char const*const type = "2falogin";
  std::basic_string<uint8_t> totp_secret;

  uint32_t get_totp() const;

  twofa_login(nlohmann::json data);
  twofa_login(const std::string name, const std::string login, const std::string password, const std::string website, const std::string totp_secret);

  nlohmann::json json() override;
  void accept_visit(display_visitor& visitor) override;
  void set_secret(std::string secret_base32);
};

#endif // TWOFALOGIN_HXX_INCLUDED
