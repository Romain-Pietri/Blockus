cmake_minimum_required(VERSION 3.10)

project(client_mdp)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

# Add the source files
set(SOURCES
   src/client.cpp
   src/cryptage.cpp
   src/cryptage.hpp
   src/piece.cpp
   src/piece.hpp
)

# Declare the executable
add_executable(client ${SOURCES})

# Specify include directories and link libraries
target_include_directories(client PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(client PRIVATE ${OPENSSL_LIBRARIES})
