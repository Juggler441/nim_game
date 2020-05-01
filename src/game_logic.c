static int _sum_range(int x, int y) { /*
	retorna la suma de todos los enteros en el rango [x, y] */
	int res = 0;
	while (x <= y) {
		res += x;
		x++;
	}
	return res;
}

#if 0
static int _sum_range(int x, int y) { /*
	retorna la suma de todos los enteros en el rango [x, y] */
	if (y <= x) return x;
	return y + _sum_range(x, y - 1);
}
#endif

int max_len_board(int base, int n) { /*
	base : numero de palitos en la base
	n : n = number_of_sticks() (helpers)
	retorna el numero maximo de posibles palitos sueltos
	si no tiene pico retorna 1 mas de la cuenta */
	if (base % 2) // es impar
		return 1 + _sum_range(2, 1 + (base - 1) / 2);
	else return 1 + n / 2;
}

int init_board(int *board, int base, int n, int pico) { /*
	board : array de enteros para guardar el estado del juego
	la longitud del array debe ser igual a max_len_board()
	base : numero de palitos en la base
	n : n = number_of_sticks() (helpers)
	pico : true o false si tiene o no tiene pico
	retorna el tamaño de la parte rellenada de board */
	int i = 0;
	if (pico) n--;
	while (n > 0) {
		n -= base;
		board[i++] = base;
		base -= 2;
	}
	if (pico) board[i++] = 1;
	return i;
}

int change_board(int *board, int board_len, int old, int *new, int new_len) { /*
	board : array de enteros para guardar el estado del juego
	board_len : el tamaño de la parte rellenada de board
	old : el numero a cambiar
	new : un array con los nuevos numeros a introducir
	new_len : longitud de new (debe ser 1 o 2)
	escribe los cambios en board
	retorna el nuevo tamaño de board
	retorna 0 en caso de error */
	int i;
	if (!(1 <= new_len && new_len <= 2))
		return 0; // bad input
	for (i = 0; i < board_len; i++)
		if (board[i] == old) {
			board[i] = new[0];
			break;
		}
	if (i == board_len)
		return 0; // old was not fount
	if (new_len > 1) {
		board[board_len] = new[1];
		return ++board_len;
	}
	return board_len;
}

int next_turn(int turn, int num_players) {
	if (! (++turn < num_players))
		return 0;
	return turn;
}

int win(const int *board, int board_len) { /*
	board_len : el tamaño de la parte rellenada de board
	se ejecuta despues del movimiento del jugador actual
	retorna true si se puede determinar un ganador
	retorna false en caso contrario
	solo se puede determinar la victoria si solo quedan palitos sueltos */
	int i;
	for (i = 0; i < board_len; i++)
		if (! (board[i] == 1 || board[i] == 0))
			return 0;
	return 1;
}

int loser(const int *board, int board_len, int turn, int num_players) { /*
	board_len : el tamaño de la parte rellenada de board
	se ejecuta despues de win()
	retorna el numero del jugador que pierde
	en caso de que turn = 0 y num_players = 2:
	retorna false si el que acaba de mover pierde
	true en caso contrario */
	int i, count = 0;
	for (i = 0; i < board_len; i++)
		if (board[i] == 1) count++;
	i = count % num_players; // si es divisible pierde el jugador actual
	while (i --> 0)
		turn = next_turn(turn, num_players);
	return turn;
}
