#include "raylib.h"
#include "box2d/box2d.h"

#include <assert.h>

// This shows how to use Box2D v3 with raylib.
// It also show how to use Box2D with pixel units.

typedef struct Entity
{
	b2BodyId bodyId;
	b2Vec2 extent;
	Texture texture;
} Entity;

void DrawEntity(const Entity* entity)
{
	b2Vec2 p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2) { -entity->extent.x, -entity->extent.y });
	b2Rot rotation = b2Body_GetRotation(entity->bodyId);
	float radians = b2Rot_GetAngle(rotation);

	Vector2 ps = {p.x, p.y};

	DrawTextureEx(entity->texture, ps, RAD2DEG * radians, 1.0f, WHITE);

	// I used these circles to ensure the coordinates are correct
	//DrawCircleV(ps, 5.0f, BLACK);
	//p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2){0.0f, 0.0f});
	//ps = (Vector2){ p.x, p.y };
	//DrawCircleV(ps, 5.0f, BLUE);
	//p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2){ entity->extent.x, entity->extent.y });
	//ps = (Vector2){ p.x, p.y };
	//DrawCircleV(ps, 5.0f, RED);
}

#define GROUND_COUNT 14
#define BOX_COUNT 10

int main(void)
{
	int width = 1920, height = 1080;
	InitWindow(width, height, "box2d-raylib");

	SetTargetFPS(60);

	// 128 pixels per meter is a appropriate for this scene. The boxes are 128 pixels wide.
	float lengthUnitsPerMeter = 128.0f;
	b2SetLengthUnitsPerMeter(lengthUnitsPerMeter);

	b2WorldDef worldDef = b2DefaultWorldDef();

	// Realistic gravity is achieved by multiplying gravity by the length unit.
	worldDef.gravity.y = 9.8f * lengthUnitsPerMeter;
	b2WorldId worldId = b2CreateWorld(&worldDef);

	Texture groundTexture = LoadTexture("ground.png");
	Texture boxTexture = LoadTexture("box.png");

	b2Vec2 groundExtent = { 0.5f * groundTexture.width, 0.5f * groundTexture.height };
	b2Vec2 boxExtent = { 0.5f * boxTexture.width, 0.5f * boxTexture.height };

	b2Polygon groundPolygon = b2MakeBox(groundExtent.x, groundExtent.y);
	b2Polygon boxPolygon = b2MakeBox(boxExtent.x, boxExtent.y);

	Entity groundEntities[GROUND_COUNT] = { 0 };
	for (int i = 0; i < GROUND_COUNT; ++i)
	{
		Entity* entity = groundEntities + i;
		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.position = (b2Vec2){ (2.0f * i + 2.0f) * groundExtent.x, height - groundExtent.y - 100.0f};

		// I used this rotation to test the world to screen transformation
		//bodyDef.rotation = b2MakeRot(0.25f * b2_pi * i);

		entity->bodyId = b2CreateBody(worldId, &bodyDef);
		entity->extent = groundExtent;
		entity->texture = groundTexture;
		b2ShapeDef shapeDef = b2DefaultShapeDef();
		b2CreatePolygonShape(entity->bodyId, &shapeDef, &groundPolygon);
	}

	Entity boxEntities[BOX_COUNT] = { 0 };
	int boxIndex = 0;
	for (int i = 0; i < 4; ++i)
	{
		float y = height - groundExtent.y - 100.0f - (2.5f * i + 2.0f) * boxExtent.y - 20.0f;

		for (int j = i; j < 4; ++j)
		{
			float x = 0.5f * width + (3.0f * j - i - 3.0f) * boxExtent.x;
			assert(boxIndex < BOX_COUNT);

			Entity* entity = boxEntities + boxIndex;
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = (b2Vec2){ x, y };
			entity->bodyId = b2CreateBody(worldId, &bodyDef);
			entity->texture = boxTexture;
			entity->extent = boxExtent;
			b2ShapeDef shapeDef = b2DefaultShapeDef();
			b2CreatePolygonShape(entity->bodyId, &shapeDef, &boxPolygon);

			boxIndex += 1;
		}
	}

	bool pause = true;

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_P))
		{
			pause = !pause;
		}

		if (pause == false)
		{
			float deltaTime = GetFrameTime();
			b2World_Step(worldId, deltaTime, 4);
		}

		BeginDrawing();
		ClearBackground(DARKGRAY);

		const char* message = "Hello Box2D!";
		int fontSize = 36;
		int textWidth = MeasureText("Hello Box2D!", fontSize);
		DrawText(message, (width - textWidth) / 2, 50, fontSize, LIGHTGRAY);

		for (int i = 0; i < GROUND_COUNT; ++i)
		{
			DrawEntity(groundEntities + i);
		}

		for (int i = 0; i < BOX_COUNT; ++i)
		{
			DrawEntity(boxEntities + i);
		}

		EndDrawing();
	}

	UnloadTexture(groundTexture);
	UnloadTexture(boxTexture);

	CloseWindow();

	return 0;
}
