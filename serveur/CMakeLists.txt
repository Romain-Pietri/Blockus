cmake_minimum_required(VERSION 3.10)

project(serveur_mdp)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

# Add the source files
set(SOURCES
   src/serveur.cpp
   src/cryptage.cpp
   src/cryptage.hpp
)

# Declare the executable
add_executable(serveur ${SOURCES})

# Specify include directories and link libraries
target_include_directories(serveur PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(serveur PRIVATE ${OPENSSL_LIBRARIES})
