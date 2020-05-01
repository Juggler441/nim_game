#ifndef STICKS_H
#define STICKS_H

typedef struct {
	int alive; // true = alive, false = dead
	int connect_to; /* junto con que palitos esta muerto
	se indica el indice del siguiente palito muerto
	si el siguiente palito esta vivo el indice sera 0 */
	int row; /* la fila a la que pertenece
	la primera fila es la numero 0
	rellena este campo al inicializar */
} stick;

int sticks2base(const stick *sticks);
void init_sticks(stick *sticks, int sticks_len, int base);
int error_check(const stick *sticks, int x, int y);
int look_before(const stick *sticks, int x);
int look_after(const stick *sticks, int y);
void kill(stick *sticks, int x, int y);

#endif
