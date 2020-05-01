#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

int max_len_board(int base, int n);
int init_board(int *board, int base, int n, int pico);
int change_board(int *board, int board_len, int old, int *new, int new_len);
int next_turn(int turn, int num_players);
int win(const int *board, int board_len);
int loser(const int *board, int board_len, int turn, int num_players);

#endif
