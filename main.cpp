#include <raylib.h>

struct Laser 
{
    float x;
    float y;
    bool active;
}; 
struct Alien 
{
    float x;
    float y;
    bool active;
    Texture2D alien;
}; 
struct Barrier
{ 
    Rectangle box;
    int health;
    bool active;
}; 
struct AlienLaser
{
    float x;
    float y;
    bool active;
}; 
void Restart(int& spaceship_x, int& spaceship_y, int& lives, int& score, bool& gameOver, bool& win,
    Laser lasers[], const int maxLasers, Alien aliens[][10], const int rows, const int cols, Texture2D alienImages[]);
int main()
{
    InitWindow(800, 800, "Space Shooter");
    SetTargetFPS(60);

    Texture2D spaceship = LoadTexture("Graphics/spaceship.png");
    Texture2D alienImages[3];
    alienImages[0] = LoadTexture("Graphics/alien_1.png");
    alienImages[1] = LoadTexture("Graphics/alien_2.png");
    alienImages[2] = LoadTexture("Graphics/alien_3.png");
    int spaceship_x = 400;
    int spaceship_y = 750;
    int lives = 3; 
    int score = 0; 
    int highScore = 0; 
    int direction = 1;
    int alienSpeed = 3;
    int alienX = 100, alienY = 100, spaceX = 60, spaceY = 60;
    const int maxLasers = 20;
    const int rows = 5;
    const int cols = 10;
    const int moveDown = 5; 
    const int barrierCount = 3;
    const int maxAlienLasers= 20;
    bool gameOver = false;
    bool win = false;
    bool gameStarted = false;
    Rectangle shipRect = { (float)spaceship_x, (float)spaceship_y, (float)spaceship.width, (float)spaceship.height };
    Laser lasers[maxLasers] = {};
    for (int i = 0; i < maxLasers; i++) 
        lasers[i].active = false;
    Alien aliens[rows][cols];
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            aliens[i][j].x = alienX + j * spaceX;
            aliens[i][j].y = alienY + i * spaceY;
            aliens[i][j].active = true;
            // assigning diff alienimage to each row
            if (i < 2) aliens[i][j].alien = alienImages[0];
            else if (i == 2) aliens[i][j].alien = alienImages[1];
            else aliens[i][j].alien = alienImages[2];
        }
    }
    Barrier barriers[barrierCount];
    for (int i = 0; i < barrierCount; i++)
    {
        barriers[i].box.x = 150 + i * 200;
        barriers[i].box.y = 550;
        barriers[i].box.width = 80;
        barriers[i].box.height = 40;
        barriers[i].health = 10;
        barriers[i].active = true;
    }
    AlienLaser alienLasers[maxAlienLasers] = {};
    for (int i = 0; i < maxAlienLasers; i++) 
        alienLasers[i].active = false;

    while (!WindowShouldClose())
    {
        //start screen 
        if (!gameStarted)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("SPACE SHOOTER", 200, 300, 50, WHITE);
            DrawText("Press space/enter key to start", 180, 400, 30, LIGHTGRAY);
            EndDrawing();
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
                gameStarted = true;
        }
        else
        {

            ClearBackground(BLACK);
            if (!gameOver && !win)
            {
                //spaceship movement
                if (IsKeyDown(KEY_RIGHT) && spaceship_x < GetScreenWidth() - spaceship.width)
                    spaceship_x += 5;
                if (IsKeyDown(KEY_LEFT) && spaceship_x > 0)
                    spaceship_x -= 5;
                shipRect.x = (float)spaceship_x;

                //laser shoot
                if (IsKeyPressed(KEY_SPACE))
                {
                    for (int i = 0; i < maxLasers; i++)
                    {
                        if (!lasers[i].active)
                        {
                            lasers[i].active = true;
                            lasers[i].x = spaceship_x + spaceship.width / 2 - 2;
                            lasers[i].y = spaceship_y;
                            break;
                        }
                    }
                }
                //update lasers
                for (int i = 0; i < maxLasers; i++)
                {
                    if (lasers[i].active)
                        lasers[i].y -= 10;
                    if (lasers[i].y < 0)
                        lasers[i].active = false;
                }
                //alien movement
                bool hitEdge = false;
                for (int i = 0; i < rows && !hitEdge; i++)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        if (aliens[i][j].active)
                        {
                            int alienW = aliens[i][j].alien.width;
                            if (direction == 1 && (aliens[i][j].x + alienW >= 800))
                            {
                                hitEdge = true;
                                break;
                            }
                            if (direction == -1 && (aliens[i][j].x <= 0))
                            {
                                hitEdge = true;
                                break;
                            }
                        }
                    }
                }
                for (int i = 0; i < rows; i++)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        if (aliens[i][j].active)
                        {
                            aliens[i][j].x += alienSpeed * direction;
                            if (hitEdge)
                            {
                                aliens[i][j].y += moveDown;
                            }
                        }
                    }
                }
                //reverse direction
                if (hitEdge) direction *= -1;

                // alien laserss 
                if (GetRandomValue(0, 100) < 5)
                {
                    for (int r = 0; r < rows; r++)
                    {
                        for (int c = 0; c < cols; c++)
                        {
                            if (aliens[r][c].active)
                            {
                                for (int k = 0; k < maxAlienLasers; k++)
                                {
                                    if (!alienLasers[k].active)
                                    {
                                        alienLasers[k].active = true;
                                        alienLasers[k].x = aliens[r][c].x + aliens[r][c].alien.width / 2 - 2;
                                        alienLasers[k].y = aliens[r][c].y + aliens[r][c].alien.height;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                // update alien laserss
                for (int i = 0; i < maxAlienLasers; i++)
                {
                    if (alienLasers[i].active)
                    {
                        alienLasers[i].y += 5;
                        if (alienLasers[i].y > GetScreenHeight())
                            alienLasers[i].active = false;


                        Rectangle bulletRect = { alienLasers[i].x, alienLasers[i].y, 4, 10 };
                        //alienLasers hit barrier
                        for (int b = 0; b < barrierCount; b++)
                        {
                            if (barriers[b].active)
                            {
                                if (CheckCollisionRecs(bulletRect, barriers[b].box))
                                {
                                    alienLasers[i].active = false;
                                    barriers[b].health--;
                                    if (barriers[b].health <= 0) barriers[b].active = false;
                                }
                            }
                        }

                        //alienLasers hit spaceship
                        if (CheckCollisionRecs(bulletRect, shipRect))
                        {
                            alienLasers[i].active = false;
                            lives--;
                            if (lives <= 0) gameOver = true;
                        }
                    }
                }


                for (int i = 0; i < maxLasers; i++)
                {
                    if (lasers[i].active)
                    {
                        Rectangle laserRect = { lasers[i].x, lasers[i].y, 4, 10 };
                        //spaceshipLasers hit aliens
                        for (int r = 0; r < rows; r++)
                        {
                            for (int c = 0; c < cols; c++)
                            {
                                if (aliens[r][c].active)
                                {
                                    Rectangle alienRect = { aliens[r][c].x, aliens[r][c].y,
                                                            aliens[r][c].alien.width, aliens[r][c].alien.height };
                                    if (CheckCollisionRecs(laserRect, alienRect))
                                    {
                                        lasers[i].active = false;
                                        aliens[r][c].active = false;
                                        score += 10;
                                        if (score > highScore) highScore = score; //update highscore
                                    }
                                }
                            }
                        }
                        //spaceshipLasers hit barriers
                        for (int b = 0; b < barrierCount; b++)
                        {
                            if (barriers[b].active)
                            {
                                if (CheckCollisionRecs(laserRect, barriers[b].box))
                                {
                                    lasers[i].active = false;
                                    barriers[b].health--;
                                    if (barriers[b].health <= 0) barriers[b].active = false;
                                }
                            }
                        }
                    }
                }

                //aliens touch spaceship
                for (int r = 0; r < rows; r++)
                {
                    for (int c = 0; c < cols; c++)
                    {
                        if (aliens[r][c].active)
                        {
                            Rectangle alienRect = { aliens[r][c].x, aliens[r][c].y,
                                                    aliens[r][c].alien.width, aliens[r][c].alien.height };
                            if (CheckCollisionRecs(alienRect, shipRect))
                            {
                                aliens[r][c].active = false;
                                lives--;
                                if (lives <= 0)
                                    gameOver = true;
                            }
                        }
                    }
                }
                //win
                bool allAliensDead = true;
                for (int r = 0; r < rows; r++)
                {
                    for (int c = 0; c < cols; c++)
                    {
                        if (aliens[r][c].active)
                            allAliensDead = false;
                    }
                }
                if (allAliensDead)
                    win = true;
                //if any alien crosses the spaceship
                for (int r = 0; r < rows; r++)
                {
                    for (int c = 0; c < cols; c++)
                    {
                        if (aliens[r][c].active)
                        {
                            if (aliens[r][c].y + aliens[r][c].alien.height >= spaceship_y)
                            {
                                gameOver = true; //game over 
                            }
                        }
                    }
                }
            }

            //drawing
            BeginDrawing();
            ClearBackground(BLACK);
            //draw spaceship
            DrawTexture(spaceship, spaceship_x, spaceship_y, WHITE);
            //draw lasers
            for (int i = 0; i < maxLasers; i++)
                if (lasers[i].active) 
                    DrawRectangle(lasers[i].x, lasers[i].y, 4, 10, RED);
            //draw aliens
            for (int r = 0; r < rows; r++)
            {
                for (int c = 0; c < cols; c++)
                {
                    if (aliens[r][c].active)
                        DrawTexture(aliens[r][c].alien, aliens[r][c].x, aliens[r][c].y, WHITE);
                }
            
             }
                //draw barriers
            for (int i = 0; i < barrierCount; i++)
                if (barriers[i].active)
                {
                    Color c = GRAY;
                    if (barriers[i].health <= 2) 
                        c = DARKGRAY;
                    DrawRectangleRec(barriers[i].box, c);
                }
            //draw alienLasers
            for (int i = 0; i < maxAlienLasers; i++)
                if (alienLasers[i].active)
                    DrawRectangle(alienLasers[i].x, alienLasers[i].y, 4, 10, ORANGE);

            // draw Text
            DrawText(TextFormat("Lives: %i", lives), 10, 10, 20, WHITE);
            DrawText(TextFormat("Score: %i", score), 10, 40, 20, WHITE);
            DrawText(TextFormat("Highscore: %i", highScore), 10, 70, 20, WHITE);

            // game over text
            if (gameOver)
            {
                DrawText("GAME OVER", 300, 400, 50, RED);
                DrawText("Press R to Restart or ESC to Exit", 180, 500, 25, LIGHTGRAY);
                if (IsKeyPressed(KEY_R))
                {
                    Restart(spaceship_x, spaceship_y, lives, score, gameOver, win, lasers, maxLasers, aliens, rows, cols, alienImages);
                    for (int i = 0; i < maxAlienLasers; i++)
                        alienLasers[i].active = false;
                    for (int i = 0; i < barrierCount; i++)
                        barriers[i].active = true;
                }
                if (IsKeyPressed(KEY_ESCAPE)) break;
            }
            //win text
            if (win)
            {
                DrawText("YOU WIN!", 300, 400, 50, GREEN);
                DrawText("Press R to Restart or ESC to Exit", 180, 500, 25, LIGHTGRAY);
                if (IsKeyPressed(KEY_R))
                {
                    Restart(spaceship_x, spaceship_y, lives, score, gameOver, win, lasers, maxLasers, aliens, rows, cols, alienImages);
                    for (int i = 0; i < maxAlienLasers; i++)
                        alienLasers[i].active = false;
                    for (int i = 0; i < barrierCount; i++)
                        barriers[i].active = true;
                }
                if (IsKeyPressed(KEY_ESCAPE))
                    break;
            }

            EndDrawing();
        }
    }

    for (int i = 0; i < 3; i++)
        UnloadTexture(alienImages[i]);

    UnloadTexture(spaceship);

    CloseWindow();

    return 0;
}
// restart function
void Restart(int& spaceship_x, int& spaceship_y, int& lives, int& score, bool& gameOver, bool& win,
     Laser lasers[], const int maxLasers,Alien aliens[][10], const int ROWS, const int COLS, Texture2D alienImages[])
{
    spaceship_x = 400;
    spaceship_y = 750;
    lives = 3;
    score = 0;
    gameOver = false;
    win = false;
    for (int i = 0; i < maxLasers; i++)
        lasers[i].active = false;
    int alienX = 100, alienY = 100, spaceX = 60, spaceY = 60;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            aliens[i][j].x = alienX + j * spaceX;
            aliens[i][j].y = alienY + i * spaceY;
            aliens[i][j].active = true;
            //assigning diff aliens to each row
            if (i < 2)
                aliens[i][j].alien = alienImages[0];
            else if (i == 2)
                aliens[i][j].alien = alienImages[1];
            else 
                aliens[i][j].alien = alienImages[2];
        }
    }
}
