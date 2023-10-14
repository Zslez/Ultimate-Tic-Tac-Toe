# Ultimate Tic-Tac-Toe

Ultimate Tic-Tac-Toe game coded in C++, with a MiniMax algorithm computer opponent that is currently able to defeat (not always though[*](#tests-and-analysis)) the Medium difficulty of the AI on [this website](https://michaelxing.com/UltimateTTT/v3/).

In the `game` folder there is also a `game 3x3.h` file, that I used to first try the MiniMax algorithm on the simpler standard version of Tic-Tac-Toe.

## Changelog

See [here](.\CHANGELOG).

## Tests and Analysis

**Version 1.1**:

- The new algorithm managed to defeat the Hard difficulty of Xing's `AI` once, and draw twice
- Out of `6` consecutive games played as X against the Medium difficulty of Xing's `AI` it scored `3W/1D/2L`
- With only `6` as search depth but an improved evaluation sistem, the average move takes around `3 seconds` to compute, but it appears to be much stronger than earlier

**Version 1.0**:

- Out of `10` consecutive games played as X against the Medium difficulty of Xing's `AI` it scored `4W/2D/4L`
- With `7` as search depth, the average move takes around `1.5 seconds` to compute, however the algorithm has sometimes peaks of more than `20 seconds` for a move, usually around the middle of the game, when a few small boards get completed

## To-Do List

- [x] improve the evaluation algorithm, as for now it is quite basic
- [ ] speed up the MiniMax algorithm, it needs to perform deeper searches to be better, as it already evaluates the position quite well
- [ ] create a standalone cpp file to calculate the next move of a given position independently from the GUI (allowing future versions of the algorithm to play some given positions against older versions to see the improvement of the algorithm)
- [ ] allow the player to choose whether to play as X or O
- [ ] add time limit to moves, picking the best found at that point
- [ ] improve the evaluation algorithm even more, if possible

## Bug Fixes

No particular bug was encountered so far.
Report one by contacting me in any way.

## [CLOC](https://github.com/AlDanial/cloc) Stats

| Language | files | blank | comment | code |
| --- | --- | --- | --- | --- |
| C/C++ Header | 13 | 888 | 365 | 1398 |
| C++ | 2 | 39 | 32 | 37 |
| TOTAL | 15 | 927 | 397 | 1435 |
