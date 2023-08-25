#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "cryptage.hpp"

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

    // Création du socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        return 1;
    }

    // Configuration de l'adresse du serveur
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(32034); // Port du serveur
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    //serverAddress.sin_addr.s_addr = inet_addr("51.68.123.161"); // Adresse IP du serveur

    // Connexion au serveur
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erreur lors de la connexion au serveur." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Envoi de données au serveur
    const char* message = "Hello, server!";
    //chiffrer le message a envoyer
    std::vector<unsigned char> encrypted = chiffrement(rsa_public_key, reinterpret_cast<const unsigned char*>(message), strlen(message));
    std::cout << "Message chiffré : " << encrypted.data() << std::endl;
    message = reinterpret_cast<const char*>(encrypted.data());


    if (send(clientSocket, message, strlen(message), 0) == -1) {
        std::cerr << "Erreur lors de l'envoi de données." << std::endl;
        close(clientSocket);
        return 1;
    }

    // Réception de données du serveur
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Erreur lors de la réception de données." << std::endl;
        close(clientSocket);
        return 1;
    }

    buffer[bytesRead] = '\0';
    std::cout << "Message reçu du serveur : " << buffer << std::endl;
    //decrypter le message recu et le stocker dans une variable string
   std::vector<unsigned char> decrypted = dechiffrement(rsa_private_key, reinterpret_cast<const unsigned char*>(buffer), strlen(buffer));
    std::string decrypted_string = reinterpret_cast<const char*>(decrypted.data());
    std::cout << "Message déchiffré : " << decrypted_string << std::endl;

    // Fermeture du socket
    close(clientSocket);
    RSA_free(rsa_private_key);
    RSA_free(rsa_public_key);
    
    return 0;
}
