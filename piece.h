#ifndef PIECE_H
#define PIECE_H

#include "includes.h"

#define PIECE_COLOR_I 0, 255, 255
#define PIECE_COLOR_O 255, 255, 0
#define PIECE_COLOR_T 177, 0, 177
#define PIECE_COLOR_S 0, 255, 0
#define PIECE_COLOR_Z 255, 0, 0
#define PIECE_COLOR_J 50, 50, 255
#define PIECE_COLOR_L 255, 127, 0
#define PIECE_COLOR_GHOST 255, 255, 255

typedef enum {
    empty = -1,
    i = 0,
    o = 1,
    j = 2,
    l = 3,
    s = 4,
    t = 5,
    z = 6,
    ghost = 7,
    bedrock = 8
} piece_type_e;

typedef struct {
    uint8_t size;
    piece_type_e tile_index, data[16];
    signed x, y;
} piece_t;

piece_t pieceCreate(piece_type_e type);
piece_t pieceCreateRandom();
void pieceDraw(piece_t * piece);
void _pieceDrawBlock(piece_t * piece, int x, int y, piece_type_e block_index);
piece_t pieceRotateCW(piece_t piece);
piece_t pieceRotateCCW(piece_t piece);

#endif // PIECE_H
