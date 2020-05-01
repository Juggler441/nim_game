#include <sticks.h>
#define false 0
#define true 1

#if 0 // NOT USED
int sticks2base(const stick *sticks) { /*
	retorna el tamaño de la base de la piramide */
	int i, base;
	for (i = 0, base = 0; !sticks[i].row; i++, base++);
	return base;
}
#endif

void init_sticks(stick *sticks, int sticks_len, int base) { /*
	sticks_len : sticks_len = number_of_sticks() (helpers) */
	int i, d_i = 0, row = 0;
	stick *stick;
	// base = sticks2base(sticks); // no se puede usar antes de inicializar
	for (i = 0; i < sticks_len; i++) {
		stick = &sticks[i];
		stick->alive = true;
		stick->row = row;
		d_i++;
		if (d_i >= base) {
			row++;
			d_i = 0;
			base -= 2;
		}
	}
}

int error_check(const stick *sticks, int x, int y) { /*
	[x, y] el rango de los indices de los sticks tachados
	retorna false en caso de error
	errores:
		si ya estan muertos
		si estan en filas distintas */
	int i;
	for (i = x; i <= y; i++)
		if (! sticks[i].alive || sticks[i].row != sticks[x].row)
			return false;
	return true;
}

int look_before(const stick *sticks, int x) {
	int i = x - 1;
	while (sticks[i].row == sticks[x].row && sticks[i].alive) i--;
	return x - i - 1; // -1 porque sticks[x] y stick[i] estan muertos o fuera
}

int look_after(const stick *sticks, int y) {
	int i = y + 1;
	while (sticks[i].row == sticks[y].row && sticks[i].alive) i++;
	return i - y - 1;
}

void kill(stick *sticks, int x, int y) { /*
	[x, y] el rango de los indices de los sticks tachados
	deberian estar en la misma fila y vivos
	usa error_check() para verificar los errores antes de llamar a kill() */
	int i = x;
	stick *stick;
	if (x > y) return; // avoid -Wmaybe-uninitialized
	while (i <= y) {
		stick = &sticks[i++];
		stick->alive = false;
		stick->connect_to = i;
	}
	stick->connect_to = 0; // dead but not connected
}
