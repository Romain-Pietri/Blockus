#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string>
#include <vector>
#include <cstring>
#include "cryptage.hpp"
// Chiffre un message avec la clé publique RSA
std::vector<unsigned char> chiffrement(RSA* rsa_pub_key, const unsigned char* message, size_t message_len) {
    int key_size = RSA_size(rsa_pub_key);
    std::vector<unsigned char> encrypted(key_size);

    int encrypted_len = RSA_public_encrypt(message_len, message, encrypted.data(), rsa_pub_key, RSA_PKCS1_PADDING);
    if (encrypted_len == -1) {
        std::cerr << "Erreur lors du chiffrement." << std::endl;
        return {};
    }

    return encrypted;
}

// Déchiffre un message avec la clé privée RSA
std::vector<unsigned char> dechiffrement(RSA* rsa_private_key, const unsigned char* encrypted, size_t encrypted_len) {
    int key_size = RSA_size(rsa_private_key);
    std::vector<unsigned char> decrypted(key_size);

    int decrypted_len = RSA_private_decrypt(encrypted_len, encrypted, decrypted.data(), rsa_private_key, RSA_PKCS1_PADDING);
    if (decrypted_len == -1) {
        std::cerr << "Erreur lors du déchiffrement." << std::endl;
        return {};
    }

    return decrypted;
}
RSA* getPrivateKey(const char* private_key_path) {
    FILE* private_key_file = fopen(private_key_path, "r");
    if (!private_key_file) {
        std::cerr << "Erreur lors de l'ouverture du fichier de clé privée." << std::endl;
        return nullptr;
    }

    RSA* rsa_private_key = PEM_read_RSAPrivateKey(private_key_file, nullptr, nullptr, nullptr);
    fclose(private_key_file);

    if (!rsa_private_key) {
        std::cerr << "Erreur lors du chargement de la clé privée." << std::endl;
    }

    return rsa_private_key;
}

RSA* getPublicKey(const char* public_key_path) {
    FILE* public_key_file = fopen(public_key_path, "r");
    if (!public_key_file) {
        std::cerr << "Erreur lors de l'ouverture du fichier de clé publique." << std::endl;
        return nullptr;
    }

    RSA* rsa_public_key = PEM_read_RSA_PUBKEY(public_key_file, nullptr, nullptr, nullptr);
    fclose(public_key_file);

    if (!rsa_public_key) {
        std::cerr << "Erreur lors du chargement de la clé publique." << std::endl;
    }

    return rsa_public_key;
}

