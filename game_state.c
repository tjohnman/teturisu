#include "game_state.h"

void gameStateInitialize() {
    gameState.bricks = spriteCreate("assets/side-bricks.png", 16, 16);
    gameState.box_tiles = spriteCreate("assets/box-tiles-dark.png", 8, 8);

    gameState.high_score = 0;
    gameState.next_piece = pieceCreateRandom();

    gameState.drop_step_duration = 1.5;
    gameState.time = 0;
    gameState.next_drop_time = gameState.time + gameState.drop_step_duration;

    gameStateOnPieceSet();
}

void gameStateUpdate(double delta) {
    gameState.time += delta;

    if(gameState.time >= gameState.next_drop_time) {
        gameState.next_drop_time = gameState.time + gameState.drop_step_duration;
        gameState.current_piece.y += 1;
    }
}

void gameStateDraw() {
    for(unsigned i=0; i<HEIGHT/gameState.bricks->frame_height; ++i) {
        gameState.bricks->x = WIDTH / 2 - 68 - gameState.bricks->frame_width;
        gameState.bricks->y = i * gameState.bricks->frame_height;
        spriteDraw(gameState.bricks);

        gameState.bricks->x = WIDTH / 2 + 12;
        gameState.bricks->y = i * gameState.bricks->frame_height;
        spriteDraw(gameState.bricks);
    }

    fillRect(WIDTH / 2 - 68, 0, WIDTH / 2 + 16 - gameState.bricks->frame_width * 2, HEIGHT);

    drawNumber(gameState.high_score, WIDTH - 8, 8, 1);

    unsigned box_x = 128;
    unsigned box_y = 24;

    fillRect(box_x+8, box_y+8, 40, 16);

    spriteSetFrame(gameState.box_tiles, 0);
    gameState.box_tiles->x = box_x;
    gameState.box_tiles->y = box_y;
    spriteDraw(gameState.box_tiles);

    spriteSetFrame(gameState.box_tiles, 7);
    for(unsigned i=1; i<6; ++i) {
        gameState.box_tiles->x = box_x + i*8;
        spriteDraw(gameState.box_tiles);
    }
    spriteSetFrame(gameState.box_tiles, 6);
    gameState.box_tiles->x = box_x + 48;
    spriteDraw(gameState.box_tiles);

    spriteSetFrame(gameState.box_tiles, 1);
    gameState.box_tiles->x = box_x;
    gameState.box_tiles->y = box_y+8;
    spriteDraw(gameState.box_tiles);
    gameState.box_tiles->y = box_y+16;
    spriteDraw(gameState.box_tiles);

    spriteSetFrame(gameState.box_tiles, 5);
    gameState.box_tiles->x = box_x+48;
    gameState.box_tiles->y = box_y+8;
    spriteDraw(gameState.box_tiles);
    gameState.box_tiles->y = box_y+16;
    spriteDraw(gameState.box_tiles);

    spriteSetFrame(gameState.box_tiles, 2);
    gameState.box_tiles->x = box_x;
    gameState.box_tiles->y = box_y + 24;
    spriteDraw(gameState.box_tiles);

    spriteSetFrame(gameState.box_tiles, 3);
    for(unsigned i=1; i<6; ++i) {
        gameState.box_tiles->x = box_x + i*8;
        spriteDraw(gameState.box_tiles);
    }
    spriteSetFrame(gameState.box_tiles, 4);
    gameState.box_tiles->x = box_x + 48;
    spriteDraw(gameState.box_tiles);

    pieceDraw(&gameState.current_piece);
    pieceDrawP(&gameState.next_piece, 16, 3);
}

void gameStateWillChangeState(gameState_e state) {
    spriteDestroy(gameState.bricks);
    spriteDestroy(gameState.box_tiles);
}

void gameStateOnPieceSet() {
    gameState.current_piece = gameState.next_piece;
    gameState.next_piece = pieceCreateRandom(gameState.current_piece.tile_index);
}