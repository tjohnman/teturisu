#ifndef PIECE_H
#define PIECE_H

#include "includes.h"

typedef enum {
    empty = -1,
    i = 0,
    o = 1,
    j = 2,
    l = 3,
    s = 4,
    t = 5,
    z = 6,
} piece_type_e;

typedef struct {
    uint8_t size;
    piece_type_e tile_index, data[16];
    unsigned x, y;
} piece_t;

piece_t pieceCreate(piece_type_e type);
piece_t pieceCreateRandom();
void pieceDraw(piece_t * piece);
void pieceDrawP(piece_t * piece, unsigned x, unsigned y);

#endif // PIECE_H