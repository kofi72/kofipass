#include "field.hxx"
#include "../../../crypto/openssl/base64.hxx"
#include "../../../crypto/libcppotp/otp.h"
#include <memory>

// build field

struct fieldFactory
{
  virtual std::unique_ptr<field_base> create( nlohmann::json data ) const = 0;
};

struct textFieldFactory final : public fieldFactory
{
  std::unique_ptr<field_base> create( nlohmann::json data ) const  override
  {
    return std::make_unique<text_field>(data);
  }
};

struct hiddenFieldFactory final : public fieldFactory
{
  std::unique_ptr<field_base> create( nlohmann::json data ) const  override
  {
    return std::make_unique<hidden_field>(data);
  }
};

struct totpFieldFactory final : public fieldFactory
{
  std::unique_ptr<field_base> create( nlohmann::json data ) const  override
  {
    return std::make_unique<totp_field>(data);
  }
};

std::unique_ptr<field_base> build_field( nlohmann::json data )
{
  std::unique_ptr<fieldFactory> Factory;
  if( data["type"] == text_field::type )
    Factory = std::make_unique<textFieldFactory>();
  else if( data["type"] == hidden_field::type )
    Factory = std::make_unique<hiddenFieldFactory>();
  else if( data["type"] == totp_field::type )
    Factory = std::make_unique<totpFieldFactory>();
  else
    throw std::runtime_error( "Unsupported item type: " + data["type"].get<std::string>() );
  return Factory->create( data );
}

std::unique_ptr<field_base> new_field_from_json( nlohmann::json data )
{
  // check if has valid 'type' field
  if( data.find( "type" ) != data.end() && data["type"].is_string() )
    return build_field( data );
  else
    throw std::runtime_error( "Missing or invalid 'type' field in JSON data" );
}

// field_base

field_base::field_base(safe_string name)
:name(name) {}

nlohmann::json field_base::json()
{
  nlohmann::json json_obj;
  json_obj["name"] = this->name;
  return json_obj;
}

// text_field

text_field::text_field(safe_string name, safe_string content)
:field_base(name), content(content) {}

text_field::text_field(nlohmann::json data)
:field_base(data["name"]), content(data["content"]) {assert(data["type"] == this->type);}

nlohmann::json text_field::json()
{
  nlohmann::json json_obj = field_base::json();
  json_obj["type"] = this->type;
  json_obj["content"] = this->content;
  return json_obj;
}

void text_field::accept_visit(const little_visitor& visitor)
{
  visitor.visit(*this);
}

// hidden_field

hidden_field::hidden_field(safe_string name, safe_string content)
:text_field(name, content) {}

hidden_field::hidden_field(nlohmann::json data)
:text_field(data["name"], data["content"]) {assert(data["type"] == this->type);}

nlohmann::json hidden_field::json()
{
  nlohmann::json json_obj = text_field::json();
  json_obj["type"] = this->type;
  return json_obj;
}

void hidden_field::accept_visit(const little_visitor& visitor)
{
  visitor.visit(*this);
}

// totp_field

totp_field::totp_field(safe_string name, safe_string totp_secret_base32)
:field_base(name), totp_secret(decodeBase32(totp_secret_base32)) {}

totp_field::totp_field(nlohmann::json data)
:field_base(data["name"]), totp_secret(base64_decode(data["totpsecret"])) {assert(data["type"] == this->type);}

uint32_t totp_field::get_totp() const
{
  return CppTotp::totp(string_to_ustring(this->totp_secret), time(NULL), 0, 30);
}

void totp_field::change_secret(safe_string new_secret)
{
  totp_secret = decodeBase32(new_secret);
}

nlohmann::json totp_field::json()
{
  nlohmann::json json_obj = field_base::json();
  json_obj["type"] = this->type;
  json_obj["totpsecret"] = base64_encode(this->totp_secret);
  return json_obj;
}

void totp_field::accept_visit(const little_visitor& visitor)
{
  visitor.visit(*this);
}
