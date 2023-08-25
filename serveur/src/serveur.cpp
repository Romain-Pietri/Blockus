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

        // Recevoir et afficher les données du client
        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Erreur lors de la réception des données." << std::endl;
            close(clientSocket);
            continue;
        }

        buffer[bytesRead] = '\0';
          
        std::cout << "Message reçu du client : " << buffer << std::endl;
        //decrypter le message recu et le stocker dans une variable string
        std::vector<unsigned char> decrypted = dechiffrement(rsa_private_key, reinterpret_cast<const unsigned char*>(buffer), strlen(buffer));
        std::string message = reinterpret_cast<const char*>(decrypted.data());
        std::cout << "Message reçu du client : " << message << std::endl;
        
      
        if(clientSocket){
        // Envoyer un message au client
            const char *messageToSend = "<NAME> !";
            //chiffrer le message a envoyer
            std::vector<unsigned char> encrypted = chiffrement(rsa_public_key, reinterpret_cast<const unsigned char*>(messageToSend), strlen(messageToSend));
            //mettre le message chiffrer dans une string
            messageToSend = reinterpret_cast<const char*>(encrypted.data());

            send(clientSocket, messageToSend, strlen(messageToSend)+1, MSG_CONFIRM | MSG_NOSIGNAL );
            sleep(1);
            shutdown(clientSocket , SHUT_RDWR);// Arrêter le flux en écriture sur le socket
        }
        // Fermer la connexion avec le client
        close(clientSocket);
    }
    RSA_free(rsa_private_key);
    RSA_free(rsa_public_key);
    
    // Fermer le socket du serveur
    close(serverSocket);

    return 0;
}
