
#include <raylib.h>
/*





void DeaStart::Initialize() {
}

*/
// Goal zone
    Rectangle goal = {670, 170, 60, 70}; // x,y,width,height

    // Game state: true only when BOTH are in the goal
    bool bothInGoal = false;

     while (!WindowShouldClose())
    {
        if (!bothInGoal)
        {
            Rectangle oldP1 = player1;
            Rectangle oldP2 = player2;
            bothInGoal = CheckCollisionRecs(player1, goal) && CheckCollisionRecs(player2, goal) && IsKeyPressed(KEY_SPACE);
        }
        BeginDrawing();

         if (bothInGoal)
        {
            ClearBackground(BLACK);
            DrawText("Success!", 300, 250, 40, GREEN);
            DrawText("Press ENTER to restart", 250, 320, 20, WHITE);

            if (IsKeyPressed(KEY_ENTER))
            {
                player1 = startP1;
                player2 = startP2;
                bothInGoal = false; // restart game
            }
        }
        else
        {
            ClearBackground(SKYBLUE);

            // draw platforms
            for (int i = 0; i < platformCount; i++)
                DrawRectangleRec(platform[i], DARKGRAY);

            DrawRectangleRec(goal, GREEN);
            // draw players
            DrawRectangleRec(player1, BLACK);
            DrawRectangleRec(player2, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}