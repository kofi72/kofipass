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

twofa_login::twofa_login(SimpleLogin &&simplelogin, std::basic_string<uint8_t> totp_secret)
:SimpleLogin(simplelogin), totp_secret(totp_secret)
{}

void twofa_login::accept_visit(item_visitor& visitor)
{
  visitor.visit( *this );
}

void twofa_login::set_secret(std::string secret_base32)
{
  this->totp_secret = string_to_ustring(decodeBase32(secret_base32));
}
std::string twofa_login::get_type() const noexcept 
{
  return this->type;
}
