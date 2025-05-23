#include <sodium.h>
#include <vector>

namespace chacha20_poly1305_ns
{
  std::vector<unsigned char> encrypt( std::vector<unsigned char> target, char const*const key );
  std::vector<unsigned char> decrypt( std::vector<unsigned char> target, char const*const key );
}; // namespace ChaCha20 Poly1305
