#include <iostream>
#include <cstring>
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
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        return 1;
    }
    
    // Configuration de l'adresse du serveur
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(32034); // Port d'écoute

    // Liaison du socket à l'adresse
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Erreur lors de la liaison du socket à l'adresse." << std::endl;
        close(serverSocket);
        return 1;
    }
    
    // Attente de connexions
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Erreur lors de l'attente de connexions." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Le serveur attend des connexions..." << std::endl;

    while (true) {
        // Accepter la connexion entrante
        sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Erreur lors de l'acceptation de la connexion." << std::endl;
            close(serverSocket);
            return 1;
        }
        
        std::cout << "Nouvelle connexion acceptée." << std::endl;


                // Recevoir la taille des données chiffrées
        size_t encrypted_size;
        recv(clientSocket, &encrypted_size, sizeof(encrypted_size), 0);

        // Recevoir les données chiffrées
        std::vector<unsigned char> encrypted(encrypted_size);
        recv(clientSocket, encrypted.data(), encrypted_size, 0);

        // Déchiffrer les données
        std::vector<unsigned char> decrypted = dechiffrement(rsa_private_key, encrypted.data(), encrypted_size);
        std::string decrypted_string(decrypted.begin(), decrypted.end());
        std::cout << "Message déchiffré : " << decrypted_string << std::endl;

        // Traiter le message déchiffré
        // ...

        // Chiffrer et envoyer une réponse
        const char *messageToSend = "<NAME> !";
        std::vector<unsigned char> response_encrypted = chiffrement(rsa_public_key, reinterpret_cast<const unsigned char*>(messageToSend), strlen(messageToSend));
        size_t response_size = response_encrypted.size();

        // Envoyer la taille des données chiffrées en réponse
        send(clientSocket, &response_size, sizeof(response_size), 0);

        // Envoyer les données chiffrées en réponse
        send(clientSocket, response_encrypted.data(), response_size, 0);

        // Fermer la connexion ou attendre de nouvelles données...

    }
    RSA_free(rsa_private_key);
    RSA_free(rsa_public_key);
    
    // Fermer le socket du serveur
    close(serverSocket);

    return 0;
}
