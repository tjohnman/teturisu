#include "game_state.h"
#include "input.h"

void gameStateInitialize() {
    gameState.bricks = spriteCreate("assets/side-bricks.png", 16, 16);
    gameState.box_tiles = spriteCreate("assets/box-tiles-dark.png", 8, 8);
    gameState.block_tiles = spriteCreate("assets/blocks.png", 8, 8);

    gameState.high_score = 0;
    gameState.next_piece = pieceCreateRandom();

    gameState.drop_step_duration = 1.5;
    gameState.time = 0;
    gameState.next_drop_time = gameState.time + gameState.drop_step_duration;

    for(unsigned i=0; i<200; ++i) gameState.board[i] = empty;

    gameStateOnPieceSet();
}

void gameStateUpdate(double delta) {
    gameState.time += delta;

    if(gameState.time >= gameState.next_drop_time) {
        gameState.next_drop_time = gameState.time + gameState.drop_step_duration;
        gameMovePieceDown();
    }
    
    if(justPressed(SDLK_LEFT) || keyRepeat(SDLK_LEFT)) {
        if(!pieceIntersectsWithBoard(&gameState.current_piece, -1, 0)) {
            gameState.current_piece.x--;
        }
    }

    if(justPressed(SDLK_RIGHT) || keyRepeat(SDLK_RIGHT)) {
        if(!pieceIntersectsWithBoard(&gameState.current_piece, 1, 0)) {
            gameState.current_piece.x++;
        }
    }

    if(justPressed(SDLK_DOWN) || keyRepeat(SDLK_DOWN)) gameMovePieceDown();
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

    for(unsigned i=0; i<200; ++i) {
        if(gameState.board[i] == empty) continue;

        spriteSetFrame(gameState.block_tiles, gameState.board[i]);
        gameState.block_tiles->x = WIDTH / 2 - 68 + (i%10) * gameState.block_tiles->frame_width;
        gameState.block_tiles->y = (i/10) * gameState.block_tiles->frame_height;
        spriteDraw(gameState.block_tiles);
    }

    pieceDraw(&gameState.current_piece);
    pieceDrawP(&gameState.next_piece, 16, 3);
}

void gameStateWillChangeState(gameState_e state) {
    spriteDestroy(gameState.bricks);
    spriteDestroy(gameState.box_tiles);
    spriteDestroy(gameState.block_tiles);
}

void gameStateOnPieceSet() {
    gameState.current_piece = gameState.next_piece;
    gameState.current_piece.y = -2;
    gameState.next_piece = pieceCreateRandom(gameState.current_piece.tile_index);
}

void gameMovePieceDown() {
    if(pieceIntersectsWithBoard(&gameState.current_piece, 0, +1)) {
        gameSetPiece();
        return;    
    }
    gameState.current_piece.y++;
}

void gameSetPiece() {
    for(unsigned i=0; i<16; i++) {
        signed piece_x = i%4;
        signed piece_y = i/4;

        if(gameState.current_piece.data[piece_x + piece_y * 4] == empty) continue;

        signed board_x = gameState.current_piece.x + piece_x;
        signed board_y = gameState.current_piece.y + piece_y;
        if(board_y < 0 || board_x < 0 || board_x >= 10 || board_y > 18) continue;

        gameState.board[board_x + board_y * 10] = gameState.current_piece.data[piece_x + piece_y * 4];
    }
    gameStateOnPieceSet();
}

SDL_bool pieceIntersectsWithBoard(piece_t * piece, signed offset_x, signed offset_y) {
    for(unsigned i=0; i<16; i++) {
        signed piece_x = i%4;
        signed piece_y = i/4;

        if(piece->data[piece_x + piece_y * 4] == empty) continue;

        signed board_x = piece->x + piece_x + offset_x;
        signed board_y = piece->y + piece_y + offset_y;

        if(board_y < 0 || board_x < 0 || board_x >= 10 || board_y >= 18 || gameState.board[board_x + board_y * 10] != empty) {
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}
