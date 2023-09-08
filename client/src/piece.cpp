#include "piece.hpp"

Carre::Carre(int x, int y, int couleur){
    this->x = x;
    this->y = y;
    this->couleur = couleur;
}

int Carre::getCouleur(){
    return this->couleur;
}
int Carre::getX(){
    return this->x;
}
int Carre::getY(){
    return this->y;
}
void Carre::setCouleur(int couleur){
    this->couleur = couleur;
}
void Carre::setX(int x){
    this->x = x;
}
void Carre::setY(int y){
    this->y = y;
}

