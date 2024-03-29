#include "game_state.h"
#include "input.h"
#include "mtwister.h"
#include "piece.h"
#include "audio.h"

void gameStateInitialize() {
    gameState.bricks = spriteCreate("assets/side-bricks.png", 16, 16);
    gameState.box_tiles = spriteCreate("assets/box-tiles-dark.png", 8, 8);
    gameState.block_tiles = spriteCreate("assets/blocks.png", 8, 8);

    gameState.mute_indicator = spriteCreate("assets/mute-indicator.png", 32, 32);
    gameState.mute_indicator->x = WIDTH - 32;
    gameState.mute_indicator->y = HEIGHT - 32;

    gameState.paused = 0;
    gameState.paused_label = spriteCreate("assets/paused.png", 58, 20);
    gameState.paused_label->x = WIDTH / 2 - 29;
    gameState.paused_label->y = HEIGHT / 2 - 10;

    gameState.hold_caption = spriteCreate("assets/hold.png", 0, 0);
    gameState.hold_caption->x = 166;
    gameState.hold_caption->y = 65;

    gameState.score = 0;
    gameState.next_piece = pieceCreateRandom();
    gameState.held_piece = pieceCreate(empty);
    gameState.cleared_line_count = 0;

    gameState.piece_lock_animation_delay = SDL_FALSE;
    gameState.drop_step_duration = 1.5;
    gameState.time = 0;
    gameState.lock_time_start = 0;
    gameState.next_drop_time = gameState.time + gameState.drop_step_duration;

    gameState.game_over = SDL_FALSE;
    gameState.game_over_filler_position = 19;

    gameState.level = 0;

    for(unsigned i=0; i<200; ++i) gameState.board[i] = empty;
    for(unsigned i=0; i<PIECE_BAG_SIZE; ++i) gameState.piece_bag[i] = empty;

    if (!gameState.audio_drop) {
        gameState.audio_drop = Mix_LoadWAV("assets/sfx/drop.ogg");
    }
    if (!gameState.audio_gameover) {
        gameState.audio_gameover = Mix_LoadWAV("assets/sfx/gameover.ogg");
    }
    if (!gameState.audio_line) {
        gameState.audio_line = Mix_LoadWAV("assets/sfx/line.ogg");
    }
    if (!gameState.audio_rotate) {
        gameState.audio_rotate = Mix_LoadWAV("assets/sfx/rotate.ogg");
    }
    if (!gameState.audio_set) {
        gameState.audio_set = Mix_LoadWAV("assets/sfx/set.ogg");
        Mix_VolumeChunk(gameState.audio_set, 32);
    }
    if (!gameState.audio_tetris) {
        gameState.audio_tetris = Mix_LoadWAV("assets/sfx/tetris.ogg");
    }

    gameStateOnPieceLock();
}

void gameStateUpdate(double delta) {
    if(!gameState.paused) {
        gameState.time += delta;

        if(gameState.game_over) {
            if(gameState.game_over_filler_position == -1) {
                SDL_Delay(2000);
                gameStateInitialize();
                return;
            }

            for(unsigned i=0; i<10; ++i) {
                gameState.board[gameState.game_over_filler_position * 10 + i] = rand()%7;
            }

            gameState.game_over_filler_position--;
            SDL_Delay(50);
            return;
        }

        if(gameState.piece_lock_animation_delay != 1 && gameState.piece_lock_animation_delay > 0) return;

        if(gameState.lock_time_start == 0 && gameState.time >= gameState.next_drop_time) {
            gameState.next_drop_time = gameState.time + gameState.drop_step_duration;
            playSound(gameState.audio_drop);
            gameMovePieceDown();
        }

        if (justPressed(SDLK_m)) {
            audioMuted = !audioMuted;
        }

        if(justPressed(SDLK_LEFT) || keyRepeat(SDLK_LEFT)) {
            if(!pieceIntersectsWithBoard(&gameState.current_piece, -1, 0)) {
                gameState.current_piece.x--;
                gameStateResetLockTimer();
            }
        }

        if(justPressed(SDLK_RIGHT) || keyRepeat(SDLK_RIGHT)) {
            if(!pieceIntersectsWithBoard(&gameState.current_piece, 1, 0)) {
                gameState.current_piece.x++;
                gameStateResetLockTimer();
            }
        }

        if(isKeyDown(SDLK_DOWN) && SDL_GetTicks() > gameState.last_force_down + 50) {
            if(gameMovePieceDown()) {
                ++gameState.score;
                gameState.last_force_down = SDL_GetTicks();
            }
        }

        if(justPressed(SDLK_SPACE)) {
            while(!pieceIntersectsWithBoard(&gameState.current_piece, 0, 1)) {
                gameMovePieceDown();
                gameState.score += 2;
            }
            gameLockPiece();
        }

        if(justPressed(SDLK_c) || justPressed(SDLK_LSHIFT)) {
            gameStateHoldPiece();
        }

        if(justPressed(SDLK_x) || justPressed(SDLK_UP)) {
            piece_t rotated_piece = pieceRotateCW(gameState.current_piece);
            playSound(gameState.audio_rotate);
            if(pieceIntersectsWithBoard(&rotated_piece, 0, 0)) {
                if(!pieceIntersectsWithBoard(&rotated_piece, -1, 0)) rotated_piece.x--;
                else if(!pieceIntersectsWithBoard(&rotated_piece, 1, 0)) rotated_piece.x++;
                else if(!pieceIntersectsWithBoard(&rotated_piece, 0, -1)) rotated_piece.y--;
                else return;
            }

            gameState.current_piece = rotated_piece;
            gameStateResetLockTimer();
        }

        if(justPressed(SDLK_z) || justPressed(SDLK_LCTRL)) {
            piece_t rotated_piece = pieceRotateCCW(gameState.current_piece);
            playSound(gameState.audio_rotate);
            if(pieceIntersectsWithBoard(&rotated_piece, 0, 0)) {
                if(!pieceIntersectsWithBoard(&rotated_piece, -1, 0)) rotated_piece.x--;
                else if(!pieceIntersectsWithBoard(&rotated_piece, 1, 0)) rotated_piece.x++;
                else if(!pieceIntersectsWithBoard(&rotated_piece, 0, -1)) rotated_piece.y--;
                else return;
                while(!pieceIntersectsWithBoard(&rotated_piece, 0, 1)) rotated_piece.y++;
            }

            gameState.current_piece = rotated_piece;
            gameStateResetLockTimer();
        }

        if(gameState.lock_time_start != 0 && SDL_GetTicks() > gameState.lock_time_start + 500) {
            gameLockPiece();
            playSound(gameState.audio_set);
        }
    }

    if(justPressed(SDLK_ESCAPE) || justPressed(SDLK_F1)) {
        gameState.paused = abs(gameState.paused - 1);
    }
}

void gameStateLevelUp() {
    ++gameState.level;
    gameState.drop_step_duration *= 0.75;
}

void gameStateFlashLine(unsigned index, SDL_bool line_on) {
    SDL_Rect r;
    r.x = WIDTH / 2 - 68;
    r.y = (index)*gameState.block_tiles->frame_height;
    r.w = 10 * gameState.block_tiles->frame_width;
    r.h = gameState.block_tiles->frame_height;

    SDL_SetRenderDrawColor(renderer, line_on ? 127 : 0, line_on ? 127 : 0, line_on ? 127 : 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void gameStateDropLines(unsigned empty_index) {
    for(unsigned y=empty_index; y>0; --y) {
        for(unsigned x=0; x<10; ++x) gameState.board[x + y*10] = gameState.board[x + (y-1)*10];
    }
    
    for(unsigned x=0; x<10; ++x) gameState.board[x] = empty;
}

void gameStateCheckLines() {
    unsigned line_count = 0;
    SDL_bool cleared_lines[20];

    for(unsigned y=0; y<20; ++y) {
        SDL_bool line = SDL_TRUE;
        for(unsigned x=0; x<10; ++x) {
            if(gameState.board[x + y*10] == empty) {
                line = SDL_FALSE;
                break;
            };
        }
        if(line) {
            cleared_lines[y] = SDL_TRUE;
            ++line_count;
        } else cleared_lines[y] = SDL_FALSE;
    }

    if(line_count) {
        for(unsigned y=0; y<20; ++y) {
            if(cleared_lines[y] != SDL_TRUE) continue;
            gameStateFlashLine(y, SDL_TRUE);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        for(unsigned y=0; y<20; ++y) {
            if(cleared_lines[y] != SDL_TRUE) continue;
            gameStateFlashLine(y, SDL_FALSE);
        }


        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        for(unsigned y=0; y<20; ++y) {
            if(cleared_lines[y] != SDL_TRUE) continue;
            gameStateFlashLine(y, SDL_TRUE);
            for(unsigned x=0; x<10; ++x) gameState.board[x + y*10] = empty;
            gameStateDropLines(y);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(100);

        switch(line_count) {
            default: break;
            case 1: gameState.score += 100 * (gameState.level + 1); break;
            case 2: gameState.score += 300 * (gameState.level + 1); break;
            case 3: gameState.score += 500 * (gameState.level + 1); break;
            case 4: gameState.score += 800 * (gameState.level + 1); break;
        }

        gameState.cleared_line_count += line_count;
        if(gameState.cleared_line_count % 10 == 0) gameStateLevelUp();

        if (line_count < 4) {
            playSound(gameState.audio_line);
        } else {
            playSound(gameState.audio_tetris);
        }
    }
}

void gameStateResetLockTimer() {
    gameState.lock_time_start = pieceIntersectsWithBoard(&gameState.current_piece, 0, 1) ? SDL_GetTicks() : 0;
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

    drawNumber(gameState.score, WIDTH - 8, 8, 1);

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

    gameStateDrawHoldBox();

    spriteSetFrame(gameState.block_tiles, 5);

    for(unsigned idx=0; idx<200; ++idx) {
        if(gameState.board[idx] == empty) continue;

        //spriteSetFrame(gameState.block_tiles, gameState.board[idx]);
        gameState.block_tiles->x = WIDTH / 2 - 68 + (idx%10) * gameState.block_tiles->frame_width;
        gameState.block_tiles->y = idx/10 * gameState.block_tiles->frame_height;

        switch(gameState.board[idx]) {
            default: break;
            case i: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_I); break;
            case o: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_O); break;
            case t: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_T); break;
            case s: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_S); break;
            case z: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_Z); break;
            case j: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_J); break;
            case l: SDL_SetTextureColorMod(gameState.block_tiles->texture, PIECE_COLOR_L); break;
        }

        spriteDraw(gameState.block_tiles);
    }

    SDL_SetTextureColorMod(gameState.block_tiles->texture, 255, 255, 255);

    if(!gameState.game_over) {
        gameStateDrawGhost();
        pieceDraw(&gameState.current_piece);

        gameState.next_piece.x = 14;
        gameState.next_piece.y = 3;
        pieceDraw(&gameState.next_piece);

        if(gameState.piece_lock_animation_delay != (unsigned)-1) {
            if(gameState.piece_lock_animation_delay == 0) gameStateCheckLines();
            else --gameState.piece_lock_animation_delay;
        }
    }

    if(gameState.paused) {
        fillRectColor(0, 0, WIDTH, HEIGHT, 0, 0, 0);
        spriteDraw(gameState.paused_label);
    }

    if(audioMuted) {
        spriteDraw(gameState.mute_indicator);
    }
}

void gameStateDrawGhost() {
    piece_t ghost_piece = gameState.current_piece;
    ghost_piece.tile_index = ghost;
    while(!pieceIntersectsWithBoard(&ghost_piece, 0, 1)) {
        ++ghost_piece.y;
    }

    pieceDraw(&ghost_piece);
}

void gameStateDrawHoldBox() {
    double scale = 0.5;

    unsigned box_x = 155;
    unsigned box_y = 73;

    fillRect(box_x+4, box_y+4, 20, 8);

    spriteSetFrame(gameState.box_tiles, 0);
    gameState.box_tiles->x = box_x;
    gameState.box_tiles->y = box_y;
    spriteDrawScaled(gameState.box_tiles, scale);

    spriteSetFrame(gameState.box_tiles, 7);
    for(unsigned i=1; i<6; ++i) {
        gameState.box_tiles->x = box_x + i*4;
        spriteDrawScaled(gameState.box_tiles, scale);
    }
    spriteSetFrame(gameState.box_tiles, 6);
    gameState.box_tiles->x = box_x + 24;
    spriteDrawScaled(gameState.box_tiles, scale);

    spriteSetFrame(gameState.box_tiles, 1);
    gameState.box_tiles->x = box_x;
    gameState.box_tiles->y = box_y+4;
    spriteDrawScaled(gameState.box_tiles, scale);
    gameState.box_tiles->y = box_y+8;
    spriteDrawScaled(gameState.box_tiles, scale);

    spriteSetFrame(gameState.box_tiles, 5);
    gameState.box_tiles->x = box_x+24;
    gameState.box_tiles->y = box_y+4;
    spriteDrawScaled(gameState.box_tiles, scale);
    gameState.box_tiles->y = box_y+8;
    spriteDrawScaled(gameState.box_tiles, scale);

    spriteSetFrame(gameState.box_tiles, 2);
    gameState.box_tiles->x = box_x;
    gameState.box_tiles->y = box_y + 12;
    spriteDrawScaled(gameState.box_tiles, scale);

    spriteSetFrame(gameState.box_tiles, 3);
    for(unsigned i=1; i<6; ++i) {
        gameState.box_tiles->x = box_x + i*4;
        spriteDrawScaled(gameState.box_tiles, scale);
    }
    spriteSetFrame(gameState.box_tiles, 4);
    gameState.box_tiles->x = box_x + 24;
    spriteDrawScaled(gameState.box_tiles, scale);

    spriteDrawScaled(gameState.hold_caption, scale);
    pieceDrawScaled(&gameState.held_piece, scale);
}

void gameStateHoldPiece() {
    if(gameState.held_piece_already) return;
    piece_t previously_held_piece = gameState.held_piece;

    gameState.held_piece = pieceCreate(gameState.current_piece.tile_index);
    gameState.held_piece.x = 33;
    gameState.held_piece.y = 18;
    
    if(previously_held_piece.tile_index != empty) {
        gameState.current_piece = previously_held_piece;
    } else {
        gameState.current_piece = gameState.next_piece;
        gameState.next_piece = gameStateGetPieceFromBag();
    }
    gameState.current_piece.x = 3;
    gameState.current_piece.y = -3;

    gameState.held_piece_already = SDL_TRUE;
}

void gameStateWillChangeState(gameState_e state) {
    spriteDestroy(gameState.bricks);
    spriteDestroy(gameState.box_tiles);
    spriteDestroy(gameState.block_tiles);
}

void gameStateOnPieceLock() {
    gameState.current_piece = gameState.next_piece;
    gameState.current_piece.x = 3;
    gameState.current_piece.y = -3;
    gameState.next_piece = gameStateGetPieceFromBag();
    gameState.lock_time_start = 0;
    gameState.piece_lock_animation_delay = 10;
    gameState.held_piece_already = SDL_FALSE;
}

SDL_bool gameMovePieceDown() {
    if(pieceIntersectsWithBoard(&gameState.current_piece, 0, +1)) {
        if(gameState.lock_time_start == 0) gameState.lock_time_start = SDL_GetTicks();
        return SDL_FALSE;
    }
    gameState.current_piece.y++;
    return SDL_TRUE;
}

void gameLockPiece() {
    if(!pieceIntersectsWithBoard(&gameState.current_piece, 0, 1)) {
        gameStateResetLockTimer();
        return;
    }

    playSound(gameState.audio_set);

    for(unsigned i=0; i<16; i++) {
        signed piece_x = i%4;
        signed piece_y = i/4;

        if(gameState.current_piece.data[piece_x + piece_y * 4] == empty) continue;

        signed board_x = gameState.current_piece.x + piece_x;
        signed board_y = gameState.current_piece.y + piece_y;

        if(board_y < 0) {
            gameStateGameOver();
            return;
        }

        if(board_x < 0 || board_x >= 10 || board_y > 20) continue;

        gameState.board[board_x + board_y * 10] = gameState.current_piece.data[piece_x + piece_y * 4];
    }
    gameStateOnPieceLock();
}

void gameStateGameOver() {
    playSound(gameState.audio_gameover);
    gameState.game_over = SDL_TRUE;
}

SDL_bool pieceIntersectsWithBoard(piece_t * piece, signed offset_x, signed offset_y) {
    for(unsigned i=0; i<16; i++) {
        signed piece_x = i%4;
        signed piece_y = i/4;

        if(piece->data[piece_x + piece_y * 4] == empty) continue;

        signed board_x = piece->x + piece_x + offset_x;
        signed board_y = piece->y + piece_y + offset_y;

        if(board_x < 0 || board_x >= 10 || board_y >= 20 || (board_y >= 0 && gameState.board[board_x + board_y * 10] != empty)) {
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

void gameStateFillBag() {
    gameState.piece_bag[0] = i;
    gameState.piece_bag[1] = o;
    gameState.piece_bag[2] = s;
    gameState.piece_bag[3] = z;
    gameState.piece_bag[4] = l;
    gameState.piece_bag[5] = j;
    gameState.piece_bag[6] = t;

    gameStateShuffleBag();
}

void gameStateShuffleBag() {
    piece_type_e tmp;

    MTRand r = seedRand(time(0));

    for(unsigned i=0; i<PIECE_BAG_SIZE*10; ++i) {
        int j = (piece_type_e)((genRand(&r)*PIECE_BAG_SIZE));
        tmp = gameState.piece_bag[i%PIECE_BAG_SIZE];
        gameState.piece_bag[i%PIECE_BAG_SIZE] = gameState.piece_bag[j];
        gameState.piece_bag[j] = tmp;
    }
}

void gameStateCleanup() {
    Mix_FreeChunk(gameState.audio_drop);
    Mix_FreeChunk(gameState.audio_gameover);
    Mix_FreeChunk(gameState.audio_line);
    Mix_FreeChunk(gameState.audio_rotate);
    Mix_FreeChunk(gameState.audio_set);
    Mix_FreeChunk(gameState.audio_tetris);
}

piece_t gameStateGetPieceFromBag() {
    if(gameState.piece_bag[6] == empty) gameStateFillBag();

    piece_type_e next = gameState.piece_bag[6];
    for(unsigned i=PIECE_BAG_SIZE-1; i>0; --i) {
        gameState.piece_bag[i] = gameState.piece_bag[i-1];
    }
    gameState.piece_bag[0] = empty;
    
    return pieceCreate(next);
}
