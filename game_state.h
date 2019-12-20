#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "includes.h"
#include "sprite.h"
#include "helper.h"
#include "piece.h"

struct {
    sprite_t * bricks, * box_tiles, * block_tiles;
    piece_t current_piece, next_piece;
    double time, next_drop_time, drop_step_duration;
    unsigned last_force_down, lock_time_start;
    piece_type_e board[200];
    unsigned piece_lock_animation_delay;

    unsigned score, level, cleared_line_count;
} gameState;

void gameStateInitialize();
void gameStateUpdate(double delta);
void gameStateDraw();
void gameStateDrawGhost();
void gameStateWillChangeState(gameState_e state);

void gameStateOnPieceLock();
SDL_bool gameMovePieceDown();
SDL_bool pieceIntersectsWithBoard(piece_t * piece, signed offset_x, signed offset_y);
void gameLockPiece();
void gameStateResetLockTimer();
void gameStateCheckLines();
void gameStateLevelUp();

#endif // GAME_STATE_H