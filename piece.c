#include "piece.h"
#include "helper.h"
#include "mtwister.h"

piece_t pieceCreate(piece_type_e type) {
    piece_t piece;
    piece.tile_index = type;
    for(unsigned i=0; i<16; ++i) piece.data[i] = empty;

    switch(type) {
        default: break;
        case i:
            piece.size = 4;
            piece.data[8] = type;
            piece.data[9] = type;
            piece.data[10] = type;
            piece.data[11] = type;
        break;

        case o:
            piece.size = 4;
            piece.data[5] = type;
            piece.data[6] = type;
            piece.data[9] = type;
            piece.data[10] = type;
        break;

        case j:
            piece.size = 3;
            piece.data[4] = type;
            piece.data[8] = type;
            piece.data[9] = type;
            piece.data[10] = type;
        break;

        case l:
            piece.size = 3;
            piece.data[6] = type;
            piece.data[8] = type;
            piece.data[9] = type;
            piece.data[10] = type;
        break;

        case s:
            piece.size = 3;
            piece.data[5] = type;
            piece.data[6] = type;
            piece.data[8] = type;
            piece.data[9] = type;
        break;

        case t:
            piece.size = 3;
            piece.data[5] = type;
            piece.data[8] = type;
            piece.data[9] = type;
            piece.data[10] = type;
        break;

        case z:
            piece.size = 3;
            piece.data[4] = type;
            piece.data[5] = type;
            piece.data[9] = type;
            piece.data[10] = type;
        break;
    }

    piece.x = 5 - (piece.size+1)/2;
    piece.y = -2;

    return piece;
}
piece_t pieceCreateRandom(piece_type_e previous_type) {
    MTRand r = seedRand(time(0));
    piece_type_e type = (signed)(genRand(&r)*7);

    while(type == previous_type) {
        type = (signed)(genRand(&r)*7);
    }

    return pieceCreate(type);
}

void pieceDraw(piece_t * piece) {
    pieceDrawScaled(piece, 1.0);
}

void pieceDrawScaled(piece_t * piece, double scale) {
    spriteSetFrame(helperData.blocksSprite, 5);

    for(unsigned idx=0; idx<16; ++idx) {
        if(piece->data[idx] == -1) continue;

        if(piece->tile_index == ghost) SDL_SetTextureColorMod(helperData.blocksSprite->texture, 255, 255, 255);
        else switch(piece->data[idx]) {
            default: break;
            case i: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_I); break;
            case o: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_O); break;
            case t: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_T); break;
            case s: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_S); break;
            case z: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_Z); break;
            case j: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_J); break;
            case l: SDL_SetTextureColorMod(helperData.blocksSprite->texture, PIECE_COLOR_L); break;
        }

        unsigned x = idx%4;
        unsigned y = idx/4;
        _pieceDrawBlockScaled(piece, x, y, piece->tile_index == ghost ? ghost : 5, scale);

    }
    SDL_SetTextureColorMod(helperData.blocksSprite->texture, 255, 255, 255);
}

void _pieceDrawBlock(piece_t * piece, int x, int y, piece_type_e block_index) {
    _pieceDrawBlockScaled(piece, x, y, block_index, 1.0);
}

void _pieceDrawBlockScaled(piece_t * piece, int x, int y, piece_type_e block_index, double scale) {
    spriteSetFrame(helperData.blocksSprite, block_index);
    helperData.blocksSprite->x = WIDTH / 2 - 68 + (piece->x + x) * helperData.blocksSprite->frame_width * scale;
    helperData.blocksSprite->y = (piece->y + y) * helperData.blocksSprite->frame_height * scale;
    spriteDrawScaled(helperData.blocksSprite, scale);
}

piece_t pieceRotateCW(piece_t piece) {
    piece_t original_piece = piece;
    piece_t new_piece = piece;

    if(original_piece.size == 4) {
        new_piece.data[0] = original_piece.data[12];
        new_piece.data[1] = original_piece.data[8];
        new_piece.data[2] = original_piece.data[4];
        new_piece.data[3] = original_piece.data[0];
        new_piece.data[4] = original_piece.data[13];
        new_piece.data[5] = original_piece.data[9];
        new_piece.data[6] = original_piece.data[5];
        new_piece.data[7] = original_piece.data[1];
        new_piece.data[8] = original_piece.data[14];
        new_piece.data[9] = original_piece.data[10];
        new_piece.data[10] = original_piece.data[6];
        new_piece.data[11] = original_piece.data[2];
        new_piece.data[12] = original_piece.data[15];
        new_piece.data[13] = original_piece.data[11];
        new_piece.data[14] = original_piece.data[7];
        new_piece.data[15] = original_piece.data[3];
    } else if(original_piece.size == 3) {
        new_piece.data[4] = original_piece.data[12];
        new_piece.data[5] = original_piece.data[8];
        new_piece.data[6] = original_piece.data[4];
        new_piece.data[8] = original_piece.data[13];
        new_piece.data[10] = original_piece.data[5];
        new_piece.data[12] = original_piece.data[14];
        new_piece.data[13] = original_piece.data[10];
        new_piece.data[14] = original_piece.data[6];
    }
    return new_piece;
}

piece_t pieceRotateCCW(piece_t piece) {
    piece_t original_piece = piece;
    piece_t new_piece = piece;

    if(original_piece.size == 4) {
        new_piece.data[0] = original_piece.data[3];
        new_piece.data[1] = original_piece.data[7];
        new_piece.data[2] = original_piece.data[11];
        new_piece.data[3] = original_piece.data[15];
        new_piece.data[4] = original_piece.data[2];
        new_piece.data[5] = original_piece.data[6];
        new_piece.data[6] = original_piece.data[10];
        new_piece.data[7] = original_piece.data[14];
        new_piece.data[8] = original_piece.data[1];
        new_piece.data[9] = original_piece.data[5];
        new_piece.data[10] = original_piece.data[9];
        new_piece.data[11] = original_piece.data[13];
        new_piece.data[12] = original_piece.data[0];
        new_piece.data[13] = original_piece.data[4];
        new_piece.data[14] = original_piece.data[8];
        new_piece.data[15] = original_piece.data[12];
    } else if(original_piece.size == 3) {
        new_piece.data[12] = original_piece.data[4];
        new_piece.data[8] = original_piece.data[5];
        new_piece.data[4] = original_piece.data[6];
        new_piece.data[13] = original_piece.data[8];
        new_piece.data[5] = original_piece.data[10];
        new_piece.data[14] = original_piece.data[12];
        new_piece.data[10] = original_piece.data[13];
        new_piece.data[6] = original_piece.data[14];
    }
    return new_piece;
}
