#include "raylib.h"
#include "box2d/box2d.h"

struct Conversion
{
    float scale;
    float halfWidth;
    float halfHeight;
};

b2Vec2 convertPosition(b2Vec2 v, struct Conversion c)
{
    b2Vec2 result = {c.scale * v.x + c.halfWidth, c.halfHeight - c.scale * v.y};
    return result;
}

int main(void)
{
    int width = 800, height = 450;
     InitWindow(width, height, "box2d-raylib");

    float scale = 10.0f;
    float halfWidth = 0.5f * width;
    float halfHeight = 0.5f * height;

    struct Conversion conversion = {scale, halfWidth, halfHeight};

    b2WorldDef worldDef = b2_defaultWorldDef;
    b2WorldId worldId = b2CreateWorld(&worldDef);

    b2BodyId groundId = b2CreateBody(worldId, &b2_defaultBodyDef);
    b2Polygon groundBox = b2MakeOffsetBox(20.0f, 2.0f, (b2Vec2){0.0f, -2.0f}, 0.0f);
    b2CreatePolygonShape(groundId, &b2_defaultShapeDef, &groundBox);

    b2BodyDef bodyDef = b2_defaultBodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){0.0f, 10.0f};
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    b2Polygon box = b2MakeSquare(1.0f);
    b2ShapeDef shapeDef = b2_defaultShapeDef;
    shapeDef.restitution = 0.9f;
    b2CreatePolygonShape(bodyId, &shapeDef, &box);

    Rectangle groundRec = {-200.0f + halfWidth, halfHeight, 400.0f, 4.0f};

    while (!WindowShouldClose())
    {
        b2World_Step(worldId, 1.0f / 60.0f, 8, 3);

        BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("Hello Box2D!", 190, 200, 20, LIGHTGRAY);
            DrawRectangleRec(groundRec, GREEN);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
