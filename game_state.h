#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "includes.h"

void gameStateInitialize();
void gameStateUpdate(double delta);
void gameStateDraw();
void gameStateWillChangeState(gameState_e state);

#endif // GAME_STATE_H