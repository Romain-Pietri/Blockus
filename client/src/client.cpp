#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "cryptage.hpp"


int socket_to_serveur(RSA* rsa_private_key, RSA* rsa_public_key, const char* message){
    /*
    Fonction qui envoie le socket au serveur et qui reçoit le socket du serveur
    */
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        return 1;
    }

    // Configuration de l'adresse du serveur
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(32034); // Port du serveur
    //serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_addr.s_addr = inet_addr("51.68.123.161"); // Adresse IP du serveur
    
    // Connexion au serveur
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erreur lors de la connexion au serveur." << std::endl;
        close(clientSocket);
        return 1;
    }
    // Chiffrer le message à envoyer
    std::vector<unsigned char> encrypted = chiffrement(rsa_public_key, reinterpret_cast<const unsigned char*>(message), strlen(message));
    size_t encrypted_size = encrypted.size();

    // Envoyer la taille des données chiffrées en bytes
    send(clientSocket, &encrypted_size, sizeof(encrypted_size), 0);

    // Envoyer les données chiffrées
    send(clientSocket, encrypted.data(), encrypted_size, 0);

    //send(clientSocket, message, strlen(message), 0);



   /// Réception de données du serveur
    size_t encrypted_size_serveur;
    recv(clientSocket, &encrypted_size_serveur, sizeof(encrypted_size_serveur), 0);
    std::vector<unsigned char> encrypted_serveur(encrypted_size_serveur);
    recv(clientSocket, encrypted_serveur.data(), encrypted_size_serveur, 0);

    // Déchiffrement du message reçu
    std::vector<unsigned char> decrypted = dechiffrement(rsa_private_key, encrypted_serveur.data(), encrypted_size_serveur);
    std::string decrypted_string(decrypted.begin(), decrypted.end());

    std::cout << "Message déchiffré : " << decrypted_string << std::endl;

    close(clientSocket);
}

int main() {
     RSA* rsa_private_key = getPrivateKey("private_key.pem");
    if (!rsa_private_key) {
        return 1;
    }

    RSA* rsa_public_key = getPublicKey("public_key.pem");
    if (!rsa_public_key) {
        RSA_free(rsa_private_key);
        return 1;
    }


    socket_to_serveur(rsa_private_key, rsa_public_key, "Hello, server!");


    socket_to_serveur(rsa_private_key, rsa_public_key, "Hello, ca marche!");

    
    
    RSA_free(rsa_private_key);
    RSA_free(rsa_public_key);
    
    return 0;
}
