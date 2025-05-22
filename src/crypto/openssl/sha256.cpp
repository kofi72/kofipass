#include "sha256.hxx"

#include <openssl/sha.h>
#include <openssl/evp.h>

#include <string>
#include <stdexcept>

std::string sha256(const std::string& str)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        throw std::runtime_error("Error creating EVP_MD_CTX");
    }
    if (!EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr)) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Error initializing SHA-256 context");
    }
    if (!EVP_DigestUpdate(mdctx, str.c_str(), str.length())) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Error updating SHA-256 context");
    }
    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned int hashLen;
    if (!EVP_DigestFinal_ex(mdctx, hash, &hashLen)) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Error finalizing SHA-256 context");
    }
    EVP_MD_CTX_free(mdctx);
    return std::string(reinterpret_cast<const char*>(hash), hashLen);
}
