#include "../item_base/item_base.hxx"
#include "field/field.hxx"
#include <vector>
#include <memory>

struct customItem : public Item
{
  static constexpr char const*const type = "customitem";
  std::vector<std::unique_ptr<field_base>> content;

  customItem(const safe_string& name);
  customItem(nlohmann::json data);
  virtual ~customItem() = default;

  nlohmann::json json() override;
  void accept_visit(display_visitor& visitor) override;
};
