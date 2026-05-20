#include "Game.h"

Game::Game() {
	InitWindow(screenWidth, screenHeight, "MAVII - Unidad 3");
	SetTargetFPS(60);

	// MUNDO FÍSICO
	b2Vec2 gravity(0.0f, 9.8f);
	world = new b2World(gravity);

	// Riel superior
	// Como es un static body reutilizo la clase que usaba para el suelo
	rail = new Ground(*world, 500.0f, 85.0f, 800.0f, 20.0f, SCALE, DARKGRAY);

	// Bloque móvil dinámico (para colocar sobre el riel)
	// Utilizo la clase PhysicsBox
	railBlock = new PhysicsBox(*world, 500.0f, 85.0f, 120.0f, 30.0f, SCALE, 0.0f, LIGHTGRAY);

	// PRISMATIC JOINT (estructura del riel)
	b2PrismaticJointDef prismaticDef;
	b2Vec2 axis(1.0f, 0.0f); // Sólo movimiento horizontal (x)

	// ------------------->(bodyA "riel",    bodyB "bloque móvil", anchor "centro del bloque",             axis "x")
	prismaticDef.Initialize(rail->GetBody(), railBlock->GetBody(), railBlock->GetBody()->GetWorldCenter(), axis);

	// Límites de desplazamiento
	prismaticDef.enableLimit = true;
	prismaticDef.lowerTranslation = -350.0f / SCALE; // Límite máximo izquierdo
	prismaticDef.upperTranslation = 350.0f / SCALE; // Límite máximo derecho

	// Habilito el motor del prismatic joint
	prismaticDef.enableMotor = true;
	prismaticDef.maxMotorForce = 100.0f; // Fuerza máxima del motor
	prismaticDef.motorSpeed = 0.0f; // Por defecto apagado (sin velocidad)

	// Creo el prismatic joint en el mundo
	// Guardo la referencia para poder controlar su velocidad en el update
	m_prismaticJoint = (b2PrismaticJoint*)world->CreateJoint(&prismaticDef);


	// CAJAS COLGANTES
	// Utilizo la clase PhysicsBox
	boxA = new PhysicsBox(*world, 400.0f, 300.0f, 50, 50, SCALE, 0, BROWN);
	boxB = new PhysicsBox(*world, 600.0f, 300.0f, 50, 50, SCALE, 0, DARKBROWN);

	// DISTANCE JOINT x2
	// Soga caja A (izquierda)
	b2DistanceJointDef jointDefA;
	b2Vec2 anchorA(-40.0f / SCALE, 0.0f); // Punto izquierdo en el bloque móvil

	// ---------------> (bodyA "bloque móvil", bodyB "boxA",    anchorA "punto específico del bloque móvil", anchorB "centro de boxA")
	jointDefA.Initialize(railBlock->GetBody(), boxA->GetBody(), railBlock->GetBody()->GetWorldPoint(anchorA), boxA->GetBody()->GetWorldCenter());
	world->CreateJoint(&jointDefA);

	// Soga caja B (derecha)
	b2DistanceJointDef jointDefB;
	b2Vec2 anchorB(40.0f / SCALE, 0.0f); // Punto derecho en el bloque móvil

	// ---------------> (bodyA "bloque móvil", bodyB "boxB",    anchorA "punto específico del bloque móvil",  anchorB "centro de boxB")
	jointDefB.Initialize(railBlock->GetBody(), boxB->GetBody(), railBlock->GetBody()->GetWorldPoint(anchorB), boxB->GetBody()->GetWorldCenter());
	world->CreateJoint(&jointDefB);
}

Game::~Game() {
	delete rail;
	delete railBlock;
	delete boxA;
	delete boxB;
	delete world;
	CloseWindow();
}

void Game::RunGame() {
	while (!WindowShouldClose()) {
		Update();
		Draw();
	}
}

void Game::Update() {
	float speed = 0.0f;
	float motorSpeed = 8.0f;

	// Inputs para controlar el bloque móvil
	if (IsKeyDown(KEY_RIGHT)) { speed = motorSpeed; } // Velocidad hacia la derecha
	else if (IsKeyDown(KEY_LEFT)) { speed = -motorSpeed; } // Velocidad hacia la izquierda (negativa)

	// Aplico la velocidad al motor del joint
	m_prismaticJoint->SetMotorSpeed(speed);

	// Avanzamos la simulación
	world->Step(1.0f / 60.0f, 8, 3);
}

void Game::Draw() {
	BeginDrawing();
	ClearBackground(RAYWHITE);

	// Dibujo del riel
	rail->Draw();

	// Obtengo los puntos de anclaje y los convierto a pixeles
	b2Vec2 getAnchorA = railBlock->GetBody()->GetWorldPoint(b2Vec2(-40.0f / SCALE, 0.0f));
	b2Vec2 getAnchorB = railBlock->GetBody()->GetWorldPoint(b2Vec2(40.0f / SCALE, 0.0f));

	Vector2 anchorA = { getAnchorA.x * SCALE, getAnchorA.y * SCALE };
	Vector2 anchorB = { getAnchorB.x * SCALE, getAnchorB.y * SCALE };

	// Obtengo la posición de las cajas colgantes
	Vector2 posBoxA = { boxA->GetBody()->GetPosition().x * SCALE, boxA->GetBody()->GetPosition().y * SCALE };
	Vector2 posBoxB = { boxB->GetBody()->GetPosition().x * SCALE, boxB->GetBody()->GetPosition().y * SCALE };

	// Dibujo las cuerdas
	DrawLineV(anchorA, posBoxA, GRAY); // Izquierda
	DrawLineV(anchorB, posBoxB, GRAY); // Derecha

	// Dibujo el bloque móvil
	railBlock->Draw();
	// Dibujo las cajas colgantes
	boxA->Draw();
	boxB->Draw();

	// Dibujo de los puntos/ruedas de anclaje
	DrawCircleV(anchorA, 4, MAROON);
	DrawCircleV(anchorB, 4, MAROON);

	// Información controles
	DrawText("DESPLAZAMIENTO: Flecha izquierda / Derecha", 200, 20, 25, DARKGRAY);

	EndDrawing();
}