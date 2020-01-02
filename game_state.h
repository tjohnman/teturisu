#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "includes.h"
#include "sprite.h"
#include "helper.h"
#include "piece.h"

#define PIECE_BAG_SIZE 7

struct {
    sprite_t * bricks, * box_tiles, * block_tiles, * hold_caption;
    piece_t current_piece, next_piece, held_piece;
    double time, next_drop_time, drop_step_duration;
    unsigned last_force_down, lock_time_start;
    piece_type_e board[200];
    unsigned piece_lock_animation_delay;
    piece_type_e piece_bag[PIECE_BAG_SIZE];

    unsigned score, level, cleared_line_count;
    SDL_bool game_over, held_piece_already;
    signed game_over_filler_position;
} gameState;

void gameStateInitialize();
void gameStateGameOver();
void gameStateUpdate(double delta);
void gameStateDraw();
void gameStateDrawGhost();
void gameStateWillChangeState(gameState_e state);
void gameStateFillBag();
void gameStateDrawHoldBox();
piece_t gameStateGetPieceFromBag();
void gameStateShuffleBag();
void gameStateFlashLine(unsigned index, SDL_bool line_on);
void gameStateDropLines(unsigned empty_index);
void gameStateHoldPiece();
void gameStateOnPieceLock();
SDL_bool gameMovePieceDown();
SDL_bool pieceIntersectsWithBoard(piece_t * piece, signed offset_x, signed offset_y);
void gameLockPiece();
void gameStateResetLockTimer();
void gameStateCheckLines();
void gameStateLevelUp();

#endif // GAME_STATE_H
