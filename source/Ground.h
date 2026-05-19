#ifndef GROUND_H
#define GROUND_H

#include "raylib.h"
#include <box2d.h>

class Ground {
private:
    b2Body* groundBody; // Puntero al cuerpo físico dentro del world
    float width;
    float height;
    float scale; // Para luego convertir de pixeles a metros y que no se perciba todo gigante
    Color color;
public:
    // Al constructor le pasaré el mundo físico, la posición, el tamaño, la escala de conversión y el color
    Ground(b2World& world, float posX, float posY, float w, float h, float s, Color col) {
        width = w;
        height = h;
        scale = s;
        color = col;

        // Body: defino el cuerpo
        b2BodyDef groundDef;
        groundDef.type = b2_staticBody; // No se mueve ni le afecta la gravedad
        groundDef.position.Set(posX / scale, posY / scale);

        // Creo el cuerpo en el mundo físico
        groundBody = world.CreateBody(&groundDef);

        // Shape: le doy forma al cuerpo
        b2PolygonShape groundShape;
        groundShape.SetAsBox((w / 2.0f) / scale, (h / 2.0f) / scale);

        // Fixture: uno la forma con el cuerpo
        groundBody->CreateFixture(&groundShape, 0.0f);
    }

    b2Body* GetBody() { return groundBody; }

    void Draw() {
        b2Vec2 pos = groundBody->GetPosition(); // Para obtener la posición actual del suelo
        
        Rectangle rect = { pos.x * scale, pos.y * scale, width, height }; // forma del rectángulo/suelo
        Vector2 origin = { width / 2.0f, height / 2.0f }; // Para dibujar desde el centro y no desde la esquina superior izquierda

        DrawRectanglePro(rect, origin, 0.0f, color);
    }
};
#endif
