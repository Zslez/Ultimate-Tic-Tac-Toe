# Ultimate Tic-Tac-Toe

Ultimate Tic-Tac-Toe game coded in C++, with a MiniMax algorithm computer opponent that is currently able to defeat (not always though[*](#tests)) the Medium difficulty of the AI on [this website](https://michaelxing.com/UltimateTTT/v3/).

In the `game` folder there is also a `game 3x3.h` file, that I used to first try the MiniMax algorithm on the simpler standard version of Tic-Tac-Toe.

## Changelog

See [here](.\CHANGELOG).

## Tests

**Version 1.0 Analysis**:

- Out of `10` consecutive games played as X against the Medium difficulty of Xing's `AI` it scored `4W/2D/4L`
- With `7` as search depth, the average move takes around `1.5 seconds` to compute, however the algorithm has sometimes peaks of more than `20 seconds` for a move, usually around the middle of the game, when a few small boards get completed

## To-Do List

- [ ] allow the player to choose whether to play as X or O
- [ ] improve the MiniMax algorithm in general, allowing it to perform deeper searches
- [ ] improve the evaluation algorithm, as for now it is quite basic
- [ ] add time limit to moves, picking the best found at that point

## Bug Fixes

No particular bug was encountered so far.
Report one by contacting me in any way.

## CLOC Stats

| Language | files | blank | comment | code |
| --- | --- | --- | --- | --- |
| C/C++ Header | 12 | 899 | 416 | 1341 |
| C++ | 1 | 28 | 22 | 34 |
| TOTAL | 13 | 927 | 438 | 1375 |
