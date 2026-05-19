#include "Game.h"

Game::Game() {
	InitWindow(screenWidth, screenHeight, "MAVII - Unidad 3");
	SetTargetFPS(60);

	// Inicializar física
	b2Vec2 gravity(0.0f, 9.8f);
	world = new b2World(gravity);

	// Riel que va a estar en la parte superior de la pantalla
	rail = new Ground(*world, 500.0f, 50.0f, 800.0f, 20.0f, SCALE, DARKGRAY);

	// Bloque móvil (para colocar sobre el riel)
	b2BodyDef railBlockDef;
	railBlockDef.type = b2_dynamicBody;
	railBlockDef.position.Set(500.0f / SCALE, 85.0f / SCALE);
	railBlock = world->CreateBody(&railBlockDef);

	b2PolygonShape railBlockShape;
	railBlockShape.SetAsBox((120.0f / 2.0f) / SCALE, (30.0f / 2.0f) / SCALE);

	b2FixtureDef railBlockFixture;
	railBlockFixture.shape = &railBlockShape;
	railBlockFixture.density = 2.0f;
	railBlock->CreateFixture(&railBlockFixture);

	// Prismatic joint
	b2PrismaticJointDef prismaticDef;
	b2Vec2 axis(1.0f, 0.0f); // Sólo movimiento horizontal (x)

	prismaticDef.Initialize(rail->GetBody(), railBlock, railBlock->GetWorldCenter(), axis);

	// Límites de desplazamiento
	prismaticDef.enableLimit = true;
	prismaticDef.lowerTranslation = -350.0f / SCALE;
	prismaticDef.upperTranslation = 350.0f / SCALE;

	// Habilito el motor del prismatic joint
	prismaticDef.enableMotor = true;
	prismaticDef.maxMotorForce = 100.0f; // Velocidad máxima
	prismaticDef.motorSpeed = 0.0f; // Por defecto apagado

	// world->CreateJoint(&prismaticDef);
	m_prismaticJoint = (b2PrismaticJoint*)world->CreateJoint(&prismaticDef); // Guardo el joint en la variable m_prismaticJoint


	// Cajas colgantes
	boxA = new PhysicsBox(*world, 400.0f, 300.0f, 50, 50, SCALE, 0, BROWN);
	boxB = new PhysicsBox(*world, 600.0f, 300.0f, 50, 50, SCALE, 0, DARKBROWN);

	// Distance Joint
	// Soga caja A
	b2DistanceJointDef jointDefA;
	b2Vec2 anchorA(-40.0f / SCALE, 0.0f); // Punto izquierdo en el bloque móvil

	// Inicializamos conectando el bloque móvil con la caja A
	jointDefA.Initialize(railBlock, boxA->GetBody(), railBlock->GetWorldPoint(anchorA), boxA->GetBody()->GetWorldCenter());
	world->CreateJoint(&jointDefA);

	// Soga caja B
	b2DistanceJointDef jointDefB;
	b2Vec2 anchorB(40.0f / SCALE, 0.0f); // Punto derecho en el bloque móvil

	// Inicializamos conectando el bloque móvil con la caja B
	jointDefB.Initialize(railBlock, boxB->GetBody(), railBlock->GetWorldPoint(anchorB), boxB->GetBody()->GetWorldCenter());
	world->CreateJoint(&jointDefB);
}

Game::~Game() {
	delete rail;
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

	// Obtengo la posicion y la rotación del bloque móvil y lo convierto a píxeles
	b2Vec2 getBlockPos = railBlock->GetPosition();
	float BlockAngle = railBlock->GetAngle() * RAD2DEG;

	// Posición central del bloque móvil
	Vector2 blockPos = { getBlockPos.x * SCALE, getBlockPos.y * SCALE };

	// Obtengo los puntos de anclaje y los convierto a pixeles
	b2Vec2 getAnchorA = railBlock->GetWorldPoint(b2Vec2(-40.0f / SCALE, 0.0f));
	b2Vec2 getAnchorB = railBlock->GetWorldPoint(b2Vec2(40.0f / SCALE, 0.0f));

	Vector2 anchorA = { getAnchorA.x * SCALE, getAnchorA.y * SCALE };
	Vector2 anchorB = { getAnchorB.x * SCALE, getAnchorB.y * SCALE };

	// Obtengo la posición de las cajas colgantes
	Vector2 posBoxA = { boxA->GetBody()->GetPosition().x * SCALE, boxA->GetBody()->GetPosition().y * SCALE };
	Vector2 posBoxB = { boxB->GetBody()->GetPosition().x * SCALE, boxB->GetBody()->GetPosition().y * SCALE };

	// Dibujo las cuerdas
	DrawLineV(anchorA, posBoxA, GRAY); // Izquierda
	DrawLineV(anchorB, posBoxB, GRAY); // Derecha

	// Dibujo el bloque móvil
	Rectangle blockRect = { blockPos.x, blockPos.y, 120.0f, 30.0f };
	Vector2 blockOrigin = { 120.0f / 2.0f, 30.0f / 2.0f };
	DrawRectanglePro(blockRect, blockOrigin, BlockAngle, LIGHTGRAY);

	// Dibujo de los puntos/ruedas de anclaje
	DrawCircleV(anchorA, 4, MAROON);
	DrawCircleV(anchorB, 4, MAROON);

	// Dibujo las cajas colgantes
	boxA->Draw();
	boxB->Draw();

	// Información controles
	DrawText("CONTROLES: Flecha izquierda / Derecha para mover el riel", 20, 20, 18, DARKGRAY);

	EndDrawing();
}