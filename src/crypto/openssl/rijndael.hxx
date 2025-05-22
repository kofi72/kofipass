#include <vector>
#include <string>

namespace rijndaelh_internal
{
  const constexpr unsigned char iv_size = 16;
  const constexpr unsigned char init    = 16;
  constexpr size_t ceil_padding_128( const size_t & );
};

namespace aes_256_cbc
{
  std::vector<unsigned char> encrypt( std::vector<unsigned char> target, const std::string& key );
  std::vector<unsigned char> decrypt( std::vector<unsigned char> target, const std::string& key );
}; // namespace aes_256_cbc
