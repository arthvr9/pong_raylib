#include <raylib.h>
#include <iostream>
#include <vector>
#include <raymath.h>

constexpr int h = 450;
constexpr int w = 800;

using namespace std;

struct Paddle
{
    Rectangle rec;
    bool leftOrRight; // true = left | false = right
    float speed = 400.0f;

    Paddle(bool leftOrRight, float x, float y)
    {
        this->leftOrRight = leftOrRight;
        this->rec = {x, y, 15.0f, 100.0f};
    }
};

struct Ball
{
    Vector2 pos;
    Vector2 speed;
    float ray;

    Ball(Vector2 pos, Vector2 speed, float ray)
    {
        this->pos = pos;
        this->speed = speed;
        this->ray = ray;
    }
};

void drawRackets(float dt, vector<Paddle> &paddles)
{
    Paddle &leftInstance = paddles[0];
    Paddle &rightInstance = paddles[1];

    if (IsKeyDown(KEY_W) && leftInstance.rec.y > 0)
    {
        leftInstance.rec.y = leftInstance.rec.y - leftInstance.speed * dt;
    }

    if (IsKeyDown(KEY_S) && leftInstance.rec.y < (h - leftInstance.rec.height))
    {
        leftInstance.rec.y = leftInstance.rec.y + leftInstance.speed * dt;
    }

    if (IsKeyDown(KEY_UP) && rightInstance.rec.y > 0)
    {
        rightInstance.rec.y = rightInstance.rec.y - rightInstance.speed * dt;
    }

    if (IsKeyDown(KEY_DOWN) && rightInstance.rec.y < (h - leftInstance.rec.height))
    {
        rightInstance.rec.y = rightInstance.rec.y + rightInstance.speed * dt;
    }
}

void drawBall(Ball &ball, float dt, vector<Paddle> &paddles)
{
    ball.pos = ball.pos + (ball.speed * dt);

    if ((ball.pos.y - ball.ray) < 0)
    {
        ball.pos.y = ball.ray;
        ball.speed.y = -ball.speed.y;
    }

    if ((ball.pos.y + ball.ray) > h)
    {
        ball.pos.y = h - ball.ray;
        ball.speed.y = -ball.speed.y;
    }
}

void checkCollision(Ball &ball, Paddle paddle) {
    if (CheckCollisionCircleRec(ball.pos, ball.ray, paddle.rec)) {
        float paddleCenter = paddle.rec.y + paddle.rec.height / 2;

        float offset = (ball.pos.y - paddleCenter) / (paddle.rec.height / 2);

        ball.speed.x = -ball.speed.x * 1.05f;
        ball.speed.y = offset * 50.0f;
    }
}

void initGame()
{
    InitWindow(w, h, "Pong");
    SetTargetFPS(60);

    vector<Paddle> paddles;

    float midX = GetScreenWidth() / 2.0f;
    float midY = GetScreenHeight() / 2.0f;

    Paddle left = {true, midX - 365.0f, midY};
    Paddle right = {false, midX + 350.0f, midY};

    paddles.push_back(left);
    paddles.push_back(right);

    Ball ball = {{midX, midY}, {300, 200}, 15};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color());
        float dt = GetFrameTime();
        DrawRectangleRec(paddles[0].rec, WHITE);
        DrawRectangleRec(paddles[1].rec, WHITE);
        DrawCircleV(ball.pos, ball.ray, WHITE);
        drawRackets(dt, paddles);
        checkCollision(ball, paddles[0]);
        checkCollision(ball, paddles[1]);
        drawBall(ball, dt, paddles);
        EndDrawing();
    }

    CloseWindow();
}

int main()
{
    initGame();
}
