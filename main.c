#include "raylib.h"
#include "box2d/box2d.h"

typedef struct Conversion
{
    float scale;
    float tileSize;
    float screenWidth;
    float screenHeight;
} Conversion;

typedef struct Entity
{
    b2BodyId bodyId;
    Texture texture;
} Entity;

Vector2 ConvertWorldToScreen(b2Vec2 p, Conversion cv)
{
    Vector2 result = {cv.scale * p.x + 0.5f * cv.screenWidth, 0.5f * cv.screenHeight - cv.scale * p.y};
    return result;
}

void DrawEntity(const Entity* entity, Conversion cv)
{
    b2Vec2 p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2){-0.5f * cv.tileSize, 0.5f * cv.tileSize});
    float radians = b2Body_GetAngle(entity->bodyId);

    Vector2 ps = ConvertWorldToScreen(p, cv);

    float textureScale = cv.tileSize * cv.scale / (float)entity->texture.width;

    // Have to negate rotation to account for y-flip
    DrawTextureEx(entity->texture, ps, -RAD2DEG * radians, textureScale, WHITE);

    // I used these circles to ensure the coordinate transformation was correct
    // DrawCircleV(ps, 5.0f, BLACK);
    // p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2){0.0f, 0.0f});
    // ps = ConvertWorldToScreen(p, cv);
    // DrawCircleV(ps, 5.0f, BLUE);
    // p = b2Body_GetWorldPoint(entity->bodyId, (b2Vec2){0.5f * cv.tileSize, -0.5f * cv.tileSize});
    // ps = ConvertWorldToScreen(p, cv);
    // DrawCircleV(ps, 5.0f, RED);
}

int main(void)
{
    int width = 1280, height = 720;
    InitWindow(width, height, "box2d-raylib");

    SetTargetFPS(60);

    float tileSize = 1.0f;
    float scale = 50.0f;

    Conversion cv = {scale, tileSize, (float)width, (float)height};

    b2WorldDef worldDef = b2DefaultWorldDef();
    b2WorldId worldId = b2CreateWorld(&worldDef);

    Texture textures[2] = {0};
    textures[0] = LoadTexture("ground.png");
    textures[1] = LoadTexture("box.png");

    b2Polygon tilePolygon = b2MakeSquare(0.5f * tileSize);

    Entity groundEntities[20] = {0};
    for (int i = 0; i < 20; ++i)
    {
        Entity* entity = groundEntities + i;
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){(1.0f * i - 10.0f) * tileSize, -4.5f - 0.5f * tileSize};

        // I used this rotation to test the world to screen transformation
        //bodyDef.angle = 0.25f * b2_pi * i;

        entity->bodyId = b2CreateBody(worldId, &bodyDef);
        entity->texture = textures[0];
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(entity->bodyId, &shapeDef, &tilePolygon);
    }

    Entity boxEntities[4] = {4};
    for (int i = 0; i < 4; ++i)
    {
        Entity* entity = boxEntities + i;
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = (b2Vec2){0.5f * tileSize * i, -4.0f + tileSize * i};
        entity->bodyId = b2CreateBody(worldId, &bodyDef);
        entity->texture = textures[1];
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.restitution = 0.1f;
        b2CreatePolygonShape(entity->bodyId, &shapeDef, &tilePolygon);
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
            b2World_Step(worldId, deltaTime, 8);
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        const char* message = "Hello Box2D!";
        int fontSize = 36;
        int textWidth = MeasureText("Hello Box2D!", fontSize);  
        DrawText(message, (width - textWidth) / 2, 50, fontSize, LIGHTGRAY);

        for (int i = 0; i < 20; ++i)
        {
            DrawEntity(groundEntities + i, cv);
        }

        for (int i = 0; i < 4; ++i)
        {
            DrawEntity(boxEntities + i, cv);
        }

        EndDrawing();
    }

    UnloadTexture(textures[0]);
    UnloadTexture(textures[1]);

    CloseWindow();

    return 0;
}

