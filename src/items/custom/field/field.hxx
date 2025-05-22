#ifndef FIELD_HXX_INCLUDED
#define FIELD_HXX_INCLUDED

#include "../../json/item_json.hxx"
#include "../../../memory/memory.hxx"
#include <string>

struct text_field;
struct hidden_field;
struct totp_field;

struct little_visitor
{
  virtual void visit(text_field&) const = 0;
  virtual void visit(hidden_field&) const = 0;
  virtual void visit(totp_field&) const = 0;
};

struct little_visitable
{
  virtual void accept_visit(const little_visitor& visitor) = 0;
};

struct field_base : public jsonExportable, public little_visitable
{
  safe_string name;
  field_base(safe_string name);
  virtual ~field_base() = default;
  virtual nlohmann::json json() override;
  virtual void accept_visit(const little_visitor& visitor) override = 0;
};

struct text_field : public field_base
{
  static constexpr char const*const type = "textfield";
  safe_string content;
  text_field(safe_string name, safe_string content);
  text_field(nlohmann::json data);
  virtual ~text_field() = default;
  virtual nlohmann::json json() override;
  virtual void accept_visit(const little_visitor& visitor) override;
};

struct hidden_field : public text_field
{
  static constexpr char const*const type = "hiddenfield";
  hidden_field(safe_string name, safe_string content);
  hidden_field(nlohmann::json data);
  virtual ~hidden_field() = default;
  virtual nlohmann::json json() override;
  virtual void accept_visit(const little_visitor& visitor) override;
};

struct totp_field : public field_base
{
  static constexpr char const*const type = "totpfield";
  safe_string totp_secret;
  totp_field(safe_string name, safe_string totp_secret_base32);
  totp_field(nlohmann::json data);
  virtual ~totp_field() = default;
  uint32_t get_totp() const;
  void change_secret(safe_string new_secret);
  virtual nlohmann::json json() override;
  virtual void accept_visit(const little_visitor& visitor) override;
};

std::unique_ptr<field_base> new_field_from_json( nlohmann::json data );

#endif // FIELD_HXX_INCLUDED
