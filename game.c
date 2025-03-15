#include <assert.h>
#include <stdbool.h>

#include "raylib.h"

typedef enum {
    POS_LEFT,
    BRANCH_NONE,
    POS_RIGHT,
} Position;

#define MAX_TREE_CAPACITY 10

int GetRandomBranchPosition(Position previousPosition) {
    switch (previousPosition) {
        case BRANCH_NONE: {
            // Next can be any position
            int r = GetRandomValue(0, 2);
            return r == 0 ? POS_LEFT : r == 1 ? POS_RIGHT : BRANCH_NONE;
        }
        case POS_LEFT:
        case POS_RIGHT: {
            // Next can be *only* the same as previous position or no branch
            int r = GetRandomValue(0, 1);
            return r == 0 ? BRANCH_NONE : previousPosition;
        }
    }
    assert(false);
}

void InitializeTree(Position tree[MAX_TREE_CAPACITY]) {
    tree[0] = BRANCH_NONE;
    for (int i = 7; i < MAX_TREE_CAPACITY; i++) {
        tree[i] = GetRandomBranchPosition(tree[i-1]);
    }
}

void UpdateTree(Position tree[MAX_TREE_CAPACITY]) {
    for (int i = 0; i < MAX_TREE_CAPACITY - 1; i++) {
        tree[i] = tree[i+1];
    }
    tree[MAX_TREE_CAPACITY-1] = GetRandomBranchPosition(tree[MAX_TREE_CAPACITY-2]);
}

#define AUDIO_FOLDER "./resources/audio/"

int main(void) {
    int windowWidth = 640;
    int windowHeight = 480;

    InitWindow(windowWidth, windowHeight, "Timber Chop");
    InitAudioDevice();

    Sound chopSfx = LoadSound(AUDIO_FOLDER"chop.wav");
    Sound hitSfx = LoadSound(AUDIO_FOLDER"hit.wav");

    SetTargetFPS(120);

    Position tree[MAX_TREE_CAPACITY];
    Position playerPos = POS_LEFT;

    InitializeTree(tree);

    int treeWidth = 64;
    int branchWidth = 64;
    int branchHeight = 16;
    int playerWidth = 32;
    int playerHeight = 64;
    int gap = playerHeight;
    int playerCenter[2] = {windowWidth/2-playerWidth/2, windowHeight-playerHeight};

    bool exitWindow = false;
    bool exitWindowRequested = false;

    bool gameOver = false;
    while (!exitWindow) {
        if (WindowShouldClose()) exitWindowRequested = true;

        if (gameOver && IsKeyPressed(KEY_ESCAPE)) {
            exitWindowRequested = true;
        }

        if (exitWindowRequested) {
            exitWindow = true;
        }

        if (!gameOver) {

            bool shouldUpdateTree = true;
            Position goTo = BRANCH_NONE;

            if (IsKeyPressed(KEY_LEFT)) {
                goTo = POS_LEFT;
            } else if (IsKeyPressed(KEY_RIGHT)) {
                goTo = POS_RIGHT;
            }

            if (goTo != BRANCH_NONE) {
                if (tree[0] == goTo) {
                    gameOver = true;
                    shouldUpdateTree = false;
                } else if (tree[1] == goTo) {
                    gameOver = true;
                }

                if (shouldUpdateTree) UpdateTree(tree);
                playerPos = goTo;
                if (gameOver) PlaySound(hitSfx);
                else PlaySound(chopSfx);
            }
        }

        BeginDrawing();
        {
            ClearBackground(GetColor(0x181818FF));
            DrawFPS(50, 50);

            // Draw tree and its branches
            {
                DrawRectangle(windowWidth/2-treeWidth/2, 0, treeWidth, windowHeight, DARKGREEN);
                for (int i = 0; i < MAX_TREE_CAPACITY; i++) {
                    int branchX = windowWidth/2;
                    switch (tree[i]) {
                        case BRANCH_NONE:
                            continue;
                        case POS_LEFT:
                            branchX -= branchWidth*2 - treeWidth / 2;
                            break;
                        case POS_RIGHT:
                            branchX += treeWidth/2;
                            break;
                    }
                    int branchY = windowHeight - branchHeight*2 - (i*gap);
                    if (i == 0) {
                        branchY -= branchHeight;
                    }
                    DrawRectangle(branchX, branchY, branchWidth, branchHeight, GREEN);
                }
            }

            // Draw player
            {
                int pos[2] = {playerCenter[0], playerCenter[1]};
                if (playerPos == POS_LEFT) {
                    pos[0] -= treeWidth;
                } else if (playerPos == POS_RIGHT) {
                    pos[0] += treeWidth;
                }
                DrawRectangle(pos[0], pos[1], playerWidth, playerHeight, RED);
            }

            if (gameOver) {
                int textWidth = MeasureText("GAME OVER", 20);
                DrawText("GAME OVER", windowWidth-textWidth-textWidth/2, 5, 20, WHITE);
            }
        }
        EndDrawing();
    }

    UnloadSound(chopSfx);
    UnloadSound(hitSfx);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
