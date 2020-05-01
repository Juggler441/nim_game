#include <game_logic.h> // change_board
#include <sticks.h> // stick, look_before, look_after

int update_board(int *board, int board_len, const stick* sticks, int x, int y) { /*
	board_len : el tamaño de la parte RELLENADA de board
	llama a change_board() (game_logic) con otros argumentos
	verifica errores con error_check() antes de usar update_board()
	retorna igual que change_board() */
	int old, tmp;
	int new[2], i = 0;
	if ((tmp = look_before(sticks, x)))
		new[i++] = tmp;
	if ((tmp = look_after(sticks, y)))
		new[i++] = tmp;
	if (i == 0)
		new[i++] = 0;
	old = y - x + 1;
	if (i >= 1) old += new[0];
	if (i == 2) old += new[1];
	return change_board(board, board_len, old, new, i);
}
