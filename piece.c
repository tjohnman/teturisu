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
            piece.data[5] = type;
            piece.data[6] = type;
            piece.data[10] = type;
        break;

        case l:
            piece.size = 3;
            piece.data[4] = type;
            piece.data[5] = type;
            piece.data[6] = type;
            piece.data[8] = type;
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
            piece.data[4] = type;
            piece.data[5] = type;
            piece.data[6] = type;
            piece.data[9] = type;
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
    for(unsigned i=0; i<16; ++i) {
        if(piece->data[i] == -1) continue;
        unsigned x = i%4;
        unsigned y = i/4;
        if(x >= piece->size || y >= piece->size) continue;
        spriteSetFrame(helperData.blocksSprite, piece->data[i]);
        helperData.blocksSprite->x = WIDTH / 2 - 68 + (piece->x + x) * helperData.blocksSprite->frame_width;
        helperData.blocksSprite->y = (piece->y + y) * helperData.blocksSprite->frame_height;
        spriteDraw(helperData.blocksSprite);
    }
}

void pieceDrawP(piece_t * piece, unsigned x, unsigned y) {
    unsigned _x = piece->x;
    unsigned _y = piece->y;

    piece->x = x - 2;
    piece->y = y;
    pieceDraw(piece);

    piece->x = _x;
    piece->y = _y;
}