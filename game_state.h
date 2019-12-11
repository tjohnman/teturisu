#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "includes.h"
#include "sprite.h"
#include "helper.h"
#include "piece.h"

struct {
    sprite_t * bricks, * box_tiles;
    double high_score;
    piece_t current_piece, next_piece;
    double time, next_drop_time, drop_step_duration;
} gameState;

void gameStateInitialize();
void gameStateUpdate(double delta);
void gameStateDraw();
void gameStateWillChangeState(gameState_e state);

void gameStateOnPieceSet();

#endif // GAME_STATE_H