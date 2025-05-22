#include "2falogin.hxx"
#include "../../crypto/libcppotp/otp.h"

#include <ctime>
#include <string>
#include "../../crypto/openssl/base64.hxx"

uint32_t twofa_login::get_totp() const
{
  return CppTotp::totp(this->totp_secret, time(NULL), 0, 30);
}

twofa_login::twofa_login(const std::string name, const std::string login, const std::string password, const std::string website, const std::string totp_secret)
:SimpleLogin(name, login, password, website), totp_secret(string_to_ustring(decodeBase32(totp_secret)))
{}

twofa_login::twofa_login(nlohmann::json data)
:SimpleLogin(data),totp_secret(string_to_ustring(base64_decode(data["secret"])))
{}

nlohmann::json twofa_login::json()
{
  nlohmann::json json_obj = SimpleLogin::json();
  json_obj["type"] = this->type;
  json_obj["secret"] = base64_encode(ustring_to_string(this->totp_secret));
  return json_obj;
}

void twofa_login::accept_visit(display_visitor& visitor)
{
  visitor.visit( *this );
}


void twofa_login::set_secret(std::string secret_base32)
{
  this->totp_secret = string_to_ustring(decodeBase32(secret_base32));
}
