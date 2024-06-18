#include "Game.h"

// Parameters:

// pX: Horizontal position in blocks
// pY: Vertical position in blocks
// pPiece: Piece to draw
// pRotation: 1 of the 4 possible rotations

// Game private methods---------------------------------------------

// Get a random int between two integers
int Game::GetRand (int pA, int pB) {
    return rand() % (pB - pA + 1) + pA;
}

// takes care of the initialization of the game by selecting the first and next piece randomly.
// next piece is shown so the player can see which piece will appear next.
void Game::InitGame() {
    // Init random numbers
    srand ((unsigned int) time(NULL));

    // First piece
    mPiece = GetRand (0, 6);
    mRotation = GetRand (0, 3);
    mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
    mPosY = mPieces->GetYInitialPosition (mPiece, mRotation);

    // Next piece
    mNextPiece = GetRand (0, 6);
    mNextRotation = GetRand (0, 3);
    mNextPosX = BOARD_WIDTH + 5;
    mNextPosY = 5;
}

// iterates through the piece matrix and draws each block of the piece.
// green for the normal blocks and blue for the pivot block
// For drawing the rectangles it calls to DrawRectangle method of the class «IO»
void Game::DrawPiece (int pX, int pY, int pPiece, int pRotation) {
    color mColor; // Color of the block

    // Obtain the position in pixel in the screen of the block we want to draw
    int mPixelsX = mBoard->GetXPosInPixels (pX);
    int mPixelsY = mBoard->GetYPosInPixels (pY);

    // Travel the matrix of blocks of the piece and draw the blocks that are filled
    for (int i = 0; i < PIECE_BLOCKS; i++) {
        for (int j = 0; j < PIECE_BLOCKS; j++) {
            // Get the type of the block and draw it with the correct color
            switch (mPieces->GetBlockType (pPiece, pRotation, j, i)) {
                case 1: mColor = GREEN; break; // For each block of the piece except the pivot
                case 2: mColor = BLUE; break; // For the pivot
            }

            if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0) {
                mIO->DrawRectangle (mPixelsX + i * BLOCK_SIZE,
                mPixelsY + j * BLOCK_SIZE,
                (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
                (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1,
                mColor);
            }
        }
    }
}

//  draws two blue columns that are used as the limits of the boards
// Then draws the board blocks that are flagged as POS_FILLED in a nested loop.
void Game::DrawBoard () {

    // Calculate the limits of the board in pixels
    int mX1 = BOARD_POSITION – (BLOCK_SIZE * (BOARD_WIDTH / 2)) – 1;
    int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
    int mY = mScreenHeight – (BLOCK_SIZE * BOARD_HEIGHT);

    // Check that the vertical margin is not to small
    //assert (mY > MIN_VERTICAL_MARGIN);

    // Rectangles that delimits the board
    mIO->DrawRectangle (mX1 – BOARD_LINE_WIDTH, mY, mX1, mScreenHeight – 1, BLUE);

    mIO->DrawRectangle (mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight – 1, BLUE);

    // Check that the horizontal margin is not to small
    //assert (mX1 > MIN_HORIZONTAL_MARGIN);

    // Drawing the blocks that are already stored in the board
    mX1 += 1;
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
        // Check if the block is filled, if so, draw it
            if (!mBoard->IsFreeBlock(i, j)) {
                mIO->DrawRectangle ( mX1 + i * BLOCK_SIZE,
                mY + j * BLOCK_SIZE,
                (mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
                (mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1,
                RED);
            }
        }
    }
}

// Game public methods----------------------------------------------------------------

// method sets the «next piece» as the current one and resets its position, 
// then selects a new «next piece».
void Game::CreateNewPiece() {
    // The new piece
    mPiece = mNextPiece;
    mRotation = mNextRotation;
    mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
    mPosY = mPieces->GetYInitialPosition (mPiece, mRotation);

    // Random next piece
    mNextPiece = GetRand (0, 6);
    mNextRotation = GetRand (0, 3);
}

// calls the previous methods in order to draw everything.
void Game::DrawScene () {
    DrawBoard (); // Draw the delimitation lines and blocks stored in the board
    DrawPiece (mPosX, mPosY, mPiece, mRotation); // Draw the playing piece
    DrawPiece (mNextPosX, mNextPosY, mNextPiece, mNextRotation); // Draw the next piece
}
