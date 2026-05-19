#ifndef PHYSICS_BOX_H
#define PHYSICS_BOX_H

#include "raylib.h"
#include <box2d.h>

class PhysicsBox {
private:
    b2Body* boxBody; // Puntero al cuerpo físico dentro del world
    float width;
    float height;
    float scale; // Para luego convertir de pixeles a metros y que no se perciba todo gigante
    Color color;

public:
    // Al constructor le pasaré el mundo físico, la posición, el tamaño, la escala de conversión, la rotación y el color
    PhysicsBox(b2World& world, float posX, float posY, float w, float h, float s, float angleDegrees, Color col) {
        width = w;
        height = h;
        scale = s;
        color = col;

        // Body: defino el cuerpo
        b2BodyDef boxDef;
        boxDef.type = b2_dynamicBody; // se mueve, rebota y le afecta la gravedad
        boxDef.position.Set(posX / scale, posY / scale);
        boxDef.angle = angleDegrees * DEG2RAD; // Le paso la información en radianes

        // Creo el cuerpo en el mundo físico
        boxBody = world.CreateBody(&boxDef);

        // Shape: le doy forma al cuerpo
        b2PolygonShape boxShape;
        boxShape.SetAsBox((w / 2.0f) / scale, (h / 2.0f) / scale);

        // Fixture: primero le doy propiedades físicas al objeto
        b2FixtureDef boxFixture;
        boxFixture.shape = &boxShape;
        boxFixture.density = 1.0f;
        boxFixture.friction = 0.4f;
        boxFixture.restitution = 0.2f; // Poco rebote
        // Ahora uno la forma junto a sus propiedad con el cuerpo
        boxBody->CreateFixture(&boxFixture);
    }

    b2Body* GetBody() { return boxBody; }

    void Draw() {
        b2Vec2 pos = boxBody->GetPosition(); // Para obtener la posición actual
        float angle = boxBody->GetAngle() * RAD2DEG; // Para obtener la rotación actual. Para dibujarlo lo paso a grados

        Rectangle rect = { pos.x * scale, pos.y * scale, width, height };
        Vector2 origin = { width / 2.0f, height / 2.0f };

        DrawRectanglePro(rect, origin, angle, color);
    }
};
#endif
