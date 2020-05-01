#ifndef AI_H
#define AI_H

typedef struct {
	// chboard.out : un numero contenido en board que queremos cambiar
	// chboard.in : que numero introducir en board en el lugar de chboard.out
	int out; // old
	int in; // new
} chboard;

chboard perfect_AI(int *board, int board_len, int turn, int num_players);
chboard simple_AI(int *board, int board_len, int turn, int num_players);
chboard AI_move(int *board, int board_len, int turn,
				int num_players, int perfect_ai);
chboard random_move(const int *board, int board_len);
int chboard2index(chboard ch, int *index, const stick *sticks, int sticks_len);

#endif
