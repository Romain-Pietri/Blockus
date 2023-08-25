#ifndef RSA_ENCRYPTION_HPP
#define RSA_ENCRYPTION_HPP

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string>
#include <vector>
#include <cstring>

std::vector<unsigned char> chiffrement(RSA* rsa_pub_key, const unsigned char* message, size_t message_len);
std::vector<unsigned char> dechiffrement(RSA* rsa_private_key, const unsigned char* encrypted, size_t encrypted_len);
RSA* getPrivateKey(const char* private_key_path);
RSA* getPublicKey(const char* public_key_path);

#endif // RSA_ENCRYPTION_HPP
