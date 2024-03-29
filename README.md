# teturisu
A Tetris clone written in C using SDL2 that more or less complies with the official guidelines.
Uses the _Random generator_ algorithm as described by the guidelines and a public domain implementation of the Mersenne Twister algorithm to shuffle the bag.

Depends on SDL2, SDL2_mixer and SDL2_image. Should be straightforward to build on Linux, macOS, or Windows (MinGW).

![Screenshot](https://raw.githubusercontent.com/tjohnman/teturisu/master/assets/screenshot.png)

## Controls
- Arrow keys: move pieces around.
- Shift and C: hold/switch a piece.
- Up and X: rotate piece clockwise
- Z and Control: rotate piece counterclockwise.
- Down: soft drop.
- Space: hard drop.
- Escape and F1: pause/unpause.
- m: Toggle mute.
