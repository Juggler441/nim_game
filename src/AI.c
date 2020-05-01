#include <stdio.h> // printf
#include <stdlib.h> // malloc, free, exit, EXIT_FAILURE
#include <time.h> // time
#include <game_logic.h> // win, loser
#include <sticks.h> // stick
#include <AI.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static int *_crop_board(const int *board, int board_len) { /*
	retorna un puntero a un board reducido sin ceros
	el board reducido termina en 0
	recuerda liberar la memoria retornada usando free() (stdlib.h) */
	int i, x = 0;
	int *board_cp;
	// +1 para poder añadir el 0 final en caso de que no se reduzca nada
	// y evitar recibir SIGTRAP
	if (! (board_cp = (int *) malloc((board_len + 1) * sizeof *board)))
		exit(EXIT_FAILURE);
	for (i = 0; i < board_len; i++)
		if (! (board[i] == 0)) // || board[i] == 1))
			board_cp[x++] = board[i];
	board_cp[x] = 0;
	return board_cp;
}

static int _boardlen(const int *board_cp) {
	const int *i = board_cp;
	while (*board_cp++);
	return board_cp - i;
}

static int _nim_sum(const int *arr, int arr_len) {
	int res = 0;
	while (arr_len --> 0) {
		res ^= *arr++;
	}
	return res;
}

chboard random_move(const int *board, int board_len) { /*
	devuelve un chboard aleatorio (pero correcto) */
	int *board_cp, board_cp_len, i;
	chboard ret;
	srand((unsigned) time(NULL));
	board_cp = _crop_board(board, board_len);
	board_cp_len = _boardlen(board_cp);
	i = rand() % board_cp_len; // 0 <= i < board_cp_len
	ret.out = board_cp[i];
	ret.in = rand() % board_cp[i];
	free(board_cp);
	return ret;
}

chboard perfect_AI(int *board, int board_len, int turn, int num_players) { /*
	Nim game has been mathematically solved. see:
	https://en.wikipedia.org/wiki/Nim#Mathematical_theory
	para que perfect_AI() funcione necesita
	estar en una partida de 2 jugadores (num_players = 2) */
	int nim, in, i;
	chboard ret;
	nim = _nim_sum(board, board_len);
	if (! nim)
		return simple_AI(board, board_len, turn, num_players);
	for (i = 0; i < board_len; i++) {
		if ((in = board[i] ^ nim) < board[i]) {
			ret.out = board[i];
			ret.in = in;
			break;
		}
	}
	if (i == board_len)
		return simple_AI(board, board_len, turn, num_players);
	board[i] = ret.in;
	if (win(board, board_len) && ! loser(board, board_len, 0, 2))
		! ret.in ? ret.in++ : ret.in--; // misère game special case
	board[i] = ret.out;
	return ret;
}

chboard simple_AI(int *board, int board_len, int turn, int num_players) { /*
	estrategia: mirar todas las posibilidades y elegir la que gana
	si no puede ganar directamente hace un movimiento aleatorio
	turn : numero del turno de la AI */
	int i, tmp;
	chboard ret;
	for (i = 0; i < board_len; i++) {
		tmp = board[i];
		while (board[i] --> 0) {
			if (win(board, board_len) &&
			loser(board, board_len, turn, num_players) != turn) {
				ret.out = tmp;
				ret.in = board[i];
				board[i] = tmp;
				return ret;
			}
		}
		board[i] = tmp;
	}
	#ifdef LOG_MOVES
		printf("\nusing random AI\n");
	#endif
	return random_move(board, board_len);
}

chboard AI_move(int *board, int board_len, int turn,
				 int num_players, int perfect_ai) {
	if (perfect_ai)
		return perfect_AI(board, board_len, turn, num_players);
	else return simple_AI(board, board_len, turn, num_players);
}

int chboard2index(chboard ch, int *index, const stick *sticks, int sticks_len) { /*
	devuelve los indices de los sticks que van a morir en funcion de un chboard
	devuelve a traves del parametro index como index2kill() (pos)
	devuelve 0 en caso de error (chboard no valido) */
	int i, x;
	for (i = 0; i < sticks_len; i++) {
		if (sticks[i].alive) {
			x = i;
			while (i + 1 < sticks_len &&
			sticks[i].row == sticks[i + 1].row &&
			sticks[i + 1].alive) i++;
			if (i + 1 - x == ch.out) {
				index[0] = x + ch.in;
				index[1] = i;
				return 1;
			}
		}
	}
	return 0;
}
