#include "argon2_wrapper.hxx"
#include <argon2.h>

#include <vector>
#include <stdexcept>
#include <string>

safe_string argon2_kdf(const safe_string& password) {
    // Generate a random salt
    std::vector<uint8_t> salt(16);
    salt = {99,58,32,210,191,244,245,9,106,54,131,171,196,225,29,26};
    // Use a secure random generator in production

    // Parameters
    uint32_t iterations = 12;      // Number of iterations
    uint32_t memory = 1 << 18;    // Memory usage (256 MiB)
    uint32_t parallelism = 4;     // Parallelism factor
    size_t outputLength = 32;     // Desired length of the derived key (32 bytes)

    // Derive the key using Argon2
    std::vector<uint8_t> output(outputLength);
    int result = argon2id_hash_raw(
        iterations,
        memory,
        parallelism,
        password.data(),
        password.size(),
        salt.data(),
        salt.size(),
        output.data(),
        output.size()
    );

    if (result != ARGON2_OK) {
        throw std::runtime_error(argon2_error_message(result));
    }

    // Convert the derived key to a hex string
    std::string derivedKey;
    for (auto byte : output) {
        derivedKey += char(byte);
    }
    return derivedKey;
}
