#include "raylib.h"
#include "box2d/box2d.h"

typedef struct Conversion
{
    float scale;
    float halfWidth;
    float halfHeight;
} Conversion;

typedef struct Entity
{
    b2BodyId bodyId;
    int width;
    int height;
} Entity;

Vector2 ConvertPositionToRayLib(b2Vec2 p, Conversion cv)
{
    Vector2 result = {cv.scale * p.x + cv.halfWidth, cv.halfHeight - cv.scale * p.y};
    return result;
}

b2Vec2 ConvertPositionToBox2D(Vector2 p, Conversion cv)
{
    b2Vec2 result = {(p.x - cv.halfWidth) / cv.scale, (cv.halfHeight - p.y) / cv.scale};
    return result;
}

b2Polygon RectangleToPolygon(int width, int height, float scale)
{
    return b2MakeBox(0.5f * width / scale, 0.5f * height / scale);
}

void DrawEntity(const Entity* entity, Conversion cv, Color color)
{
    b2Vec2 p = b2Body_GetPosition(entity->bodyId);
    float radians = b2Body_GetAngle(entity->bodyId);

    Vector2 ps = ConvertPositionToRayLib(p, cv);
    Rectangle rec = {ps.x, ps.y, entity->width, entity->height};

    // local pivot
    Vector2 origin = {0.5f * entity->width, 0.5f * entity->height};

    DrawRectanglePro(rec, origin, RAD2DEG * radians, color);
}

int main(void)
{
    int width = 800, height = 450;
    InitWindow(width, height, "box2d-raylib");

    SetTargetFPS(60);

    float scale = 10.0f;
    float halfWidth = 0.5f * width;
    float halfHeight = 0.5f * height;

    Conversion cv = {scale, halfWidth, halfHeight};

    b2WorldDef worldDef = b2_defaultWorldDef;
    b2WorldId worldId = b2CreateWorld(&worldDef);

    Entity groundEntity = {0};
    {
        b2BodyDef bodyDef = b2_defaultBodyDef;
        bodyDef.position = ConvertPositionToBox2D((Vector2){halfWidth, 400.0f}, cv);
        groundEntity.bodyId = b2CreateBody(worldId, &bodyDef);
        groundEntity.width = 600;
        groundEntity.height = 20;

        b2Polygon groundBox = RectangleToPolygon(groundEntity.width, groundEntity.height, scale);
        b2CreatePolygonShape(groundEntity.bodyId, &b2_defaultShapeDef, &groundBox);
    }

    Entity entity = {0};
    b2BodyDef bodyDef = b2_defaultBodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = ConvertPositionToBox2D((Vector2){halfWidth, 100.0f}, cv);
    entity.bodyId = b2CreateBody(worldId, &bodyDef);
    entity.width = 25;
    entity.height = 25;

    b2Polygon box = RectangleToPolygon(entity.width, entity.height, scale);
    b2ShapeDef shapeDef = b2_defaultShapeDef;
    shapeDef.restitution = 0.9f;
    b2CreatePolygonShape(entity.bodyId, &shapeDef, &box);

    while (!WindowShouldClose())
    {
        b2World_Step(worldId, 1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Hello Box2D!", 350, 50, 20, LIGHTGRAY);
        DrawEntity(&groundEntity, cv, GREEN);
        DrawEntity(&entity, cv, PINK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
