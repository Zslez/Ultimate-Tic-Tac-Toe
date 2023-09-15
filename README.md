# Zuma CPP

Zuma-like game coded in `C++` with `SDL2`

## Used Libraries

- `JsonCPP`
- `SDL2`
- `SDL_image`
- `SDL_ttf`

## Before Publishing To-Do List

- [x] add level progression
- [x] count combo in the background
- [x] show stats at level completion
- [x] hide balls when in hidden parts of the path
- [x] save game also when it is closed or esc-ed on level completion
- [x] display stage-level next to the RESUME button
- [x] add power-ups system
- [x] add different power-ups and save the state of them in Save()
- [x] save high score of user
- [x] display high score of user
- [x] ENCRYPT-DECRYPT JSON LEVEL FILES BEFORE PUBLISHING THE GAME

## After Publishing To-Do List

- [x] make possible to select level among unlocked ones when New Game is clicked
- [x] MAKE GAME FULLSCREEN
- [x] ADD ENDLESS MODE
- [x] make EndlessGame subclasses of the original, to better manage changes
- [x] ADD SYSTEM OF DIFFICULTY VALID FOR EACH LEVEL (needs to be improved)
- [x] save balls as images and display them instead of drawing circles every time, to improve performance
- [x] show points on explosion
- [x] add chain combo bonus points
- [x] Endless mode final screen showing stats
- [x] increase lives every N points

- [x] add basic level creator
- [ ] make it possible to delete piece of path
- [ ] make it possible to set piece of path as hidden
- [ ] make it possible to add more flows

- [ ] make possible to change color palette

- [ ] add coins
- [ ] make balls go backwards and slow down a little in endless mode after each filled bar
- [ ] MAKE KEY AND IV NOT HARDCODED
- [ ] FIND A WAY TO MAKE SAVE DATA FOR RESUME ALWAYS VALID EVEN FOR FUTURE UPDATES (AT MAX DO THAT IF THERE'S AN ERROR IT STARTS FROM THE BEGINNING OF THE SAVED LEVEL, IGNORING OTHER SAVED DATA)
- [ ] add bonus points if time is less than ace time
- [ ] win screen when all levels are finished
- [ ] add an interface for when all lives are lost
- [ ] use the new backwardsStep function to move balls backwards a bit after a contact
- [ ] add a user management interface (`create`, `select`, `delete`)

## Next Big Updates

- [x] endless mode
- [ ] level editor: create, save levels + import others' levels (you must complete your level to be able to share it)
- [ ] multiplayer mode (levels or endless)

## Bug Fixes

- [x] fix that when two important action happen during the same frame (I think) wrong balls explode or more than should explode (it was when a whole chunk exploded and so it called CheckContact because the number of chunks decreased)
- [x] fix that when the level has more than one flow the level animation is bugged
- [x] fix power-up not disappearing after some time
- [ ] fix problem that makes it impossible to have two explosions in the same frame (rare case but I'll work on it)

## CLOC Stats

| Language | files | blank | comment | code |
| --- | --- | --- | --- | --- |
| C/C++ Header | 27 | 1954 | 764 | 3805 |
| JSON | 15 | 0 | 0 | 1456 |
| C++ | 1 | 41 | 25 | 48 |
| TOTAL | 43 | 1995 | 789 | 5309 |
