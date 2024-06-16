#include "Board.h"

// Parameters------------------------------------------------

// pX: Horizontal position in blocks
// pY: Vertical position in blocks
// pPiece: Piece to draw
// pRotation: 1 of the 4 possible rotations

// Board Private Methods------------------------------------

void Board::InitBoard() {
    // Initialize the board blocks with free positions
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            mBoard[i][j] = POS_FREE;
        }
    }
}

// method that erases a line and moves all the blocks of upper positions one row down.
// It just starts from the line that has to be removed, and then, iterating through the 
// board in a nested loop, moves all the blocks of the upper lines one row done.
void Board::DeleteLine (int pY) {
    // Moves all the upper lines one row down
    for (int j = pY; j > 0; j--) {
        for (int i = 0; i < BOARD_WIDTH; i++) {
            mBoard[i][j] = mBoard[i][j-1];
        }
    }
}

// Board Public Methods------------------------------------------------------

// stores a piece in the board by filling the appropriate blocks as POS_FILLED. 
// There is a nested loop that iterates through the piece matrix and store the blocks in the board.
void Board::StorePiece (int pX, int pY, int pPiece, int pRotation) {
    // Store each block of the piece into the board
    for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++) {
        for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++) {
            // Store only the blocks of the piece that are not holes
            if (mPieces->GetBlockType (pPiece, pRotation, j2, i2) != 0) {
                mBoard[i1][j1] = POS_FILLED;
            }
        }
    }
}

// checks if there are blocks in the first row. That means the game is over.
bool Board::IsGameOver() {
    //If the first line has blocks, then, game over
    for (int i = 0; i < BOARD_WIDTH; i++) {
        if (mBoard[i][0] == POS_FILLED) {
            return true;
        }
    }
    return false;
}

// method that removes all the lines that should be erased from the board.
// uses the DeleteLine method in order to erase that line 
// and move all the upper lines one row down
void Board::DeletePossibleLines () {
    for (int j = 0; j < BOARD_HEIGHT; j++) {
        int i = 0;
        while (i < BOARD_WIDTH) {
            if (mBoard[i][j] != POS_FILLED) {
                break;
            }
            i++;
        }

        if (i == BOARD_WIDTH) {
            DeleteLine(j);
        }
    }
}

// method that checks out if a board block is filled or not.
// Returns 1 (true) if the this block of the board is empty, 0 if it is filled
bool Board::IsFreeBlock (int pX, int pY) {
    if (mBoard [pX][pY] == POS_FREE) {
        return true;
    } else {
        return false;
    }
}

// Returns the horizontal position (in pixels) of the block given like parameter
int Board::GetXPosInPixels (int pPos) {
    return ( ( BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) ) + (pPos * BLOCK_SIZE) );
}

// Returns the vertical position (in pixels) of the block given like parameter
int Board::GetYPosInPixels (int pPos) {
    return ( (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT)) + (pPos * BLOCK_SIZE) );
}

// Check if the piece can be stored at this position without any collision
// Returns true if the movement is possible, false if it not possible
bool Board::IsPossibleMovement (int pX, int pY, int pPiece, int pRotation) {

    // Checks collision with pieces already stored in the board or the board limits
    // This is just to check the 5×5 blocks of a piece with the appropriate area in the board
    for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++) {
        for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++) {
            // Check if the piece is outside the limits of the board
            if ( i1 < 0 || i1 > BOARD_WIDTH - 1 || j1 > BOARD_HEIGHT - 1) {
                if (mPieces->GetBlockType (pPiece, pRotation, j2, i2) != 0) {
                    return 0;
                }
            }

        // Check if the piece have collisioned with a block already stored in the map
            if (j1 >= 0) {
                if ((mPieces->GetBlockType (pPiece, pRotation, j2, i2) != 0) && (!IsFreeBlock (i1, j1)) ) {
                    return false;
                }
            }
        }
    }

    // No collision
    return true;
}