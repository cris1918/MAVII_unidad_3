#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <box2d.h>
#include <vector>

#include "Ground.h"
#include "PhysicsBox.h"

class Game {
private:
    const float SCALE = 30.0f; // Escala para conversión de píxeles a metros y que los objetos no se perciban como gigantes
    const int screenWidth = 1000;
    const int screenHeight = 600;

    // Mundo físico
    b2World* world;

    // Entidades físicas
    Ground* rail; // Riel estático
    PhysicsBox* railBlock; // Bloque que se desplaza por el riel

    b2PrismaticJoint* m_prismaticJoint; // Para guardar la dirección del prismatic joint y poder controlar el motor del joint

    PhysicsBox* boxA; // Caja A del péndulo
    PhysicsBox* boxB; // Caja B del péndulo

    void Update();
    void Draw();

public:
    Game();
    ~Game();
    void RunGame();
};

#endif
