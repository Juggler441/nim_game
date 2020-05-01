#include <pos.h>

#if 0
int delta_x(int res_x, int i) { /*
	res_x : ancho de resolucion disponible para la piramide
	i : numero de palitos
	retorna el ancho cada subdivision */
	return res_x / (i + 1);
}
#endif

int delta_y(int res_y, int p, int p_y) { /*
	res_y : altura de resolucion disponible para la piramide
	p : numero de pisos
	p_y : altura de cada piso
	retorna la separacion entre los pisos */
	res_y -= p_y;
	return res_y / (p - 1);
}

int padding(res *res, int x, int y) { /*
	res : resolucion de la pantalla
	x : margen a cada lado en horizontal
	y : margen vertical
	escribe en res la resolucion disponible para la piramide
	retorna 0 en caso de error */
	res->x -= 2 * x;
	res->y -= 2 * y;
	if (res->x <= 0 || res->y <= 0)
		return 0;
	else return 1;
}

int init_pos(res *pos, int pos_len, res res, int base,
			 int pisos, int p_y, int pico) { /*
	pos : el indice de cada palo en pos es el mismo que en sticks
	ambos recorren la piramide de abajo a arriba y de izquierda a derecha
	pos_len : pos_len = number_of_sticks() (helpers)
	res : res = padding()
	base : numero de palitos en la base
	pisos : numero de pisos
	p_y : altura de cada piso
	pico : true o false si tiene o no tiene pico
	escribe en pos todas las posiciones de los palitos
	se indica el punto mas inferior de cada palito
	retorna el numero de palitos escritos en pos
	retorna 0 en caso de error */
	int d_x = delta_x(res.x, base);
	int d_y = delta_y(res.y, pisos, p_y);
	int x = d_x, i = 0, y = res.y;
	int base_tmp, x_tmp;
	while (pisos --> 0) {
		base_tmp = base;
		x_tmp = x;
		if (!pisos && !(base % 2) && pico) { // pisos == 0 && es_par && pico
			if (i >= pos_len) return 0;
			pos[i].x = x - d_x / 2;
			pos[i++].y = y;
		}
		while (base_tmp --> 0){
			if (i >= pos_len) return 0;
			pos[i].x = x_tmp;
			x_tmp += d_x;
			pos[i++].y = y;
		}
		y -= d_y;
		x += d_x;
		base -= 2;
	}
	return i;
}

int index2kill(const res *pos, int pos_len, int *index,
			   res from, res to, int base, int p_y) { /*
	index : array vacio de tamaño 2 inicializado a {0, 0}
	from : donde se apreto el raton
	to : donde se solto el raton
	p_y : altura de cada piso
	transforma index[] para pasarle los indices a kill() (sticks)
	index[0] = x; index[1] = y
	[x, y] es el rango de los indices de los sticks tachados
	retorna 0 en caso de error. errores detectados:
		que esten en filas distintas
		no haber tachado nada */
	int i;
	res tmp;
	base += 2;
	for (i = 0; ! (i >= pos_len || i < 0); i += base) {
		if (pos[i].y >= from.y && from.y >= pos[i].y - p_y
		&&  pos[i].y >=  to.y  &&  to.y  >= pos[i].y - p_y)
			break;
		base -= 2;
	}
	if (i >= pos_len || i < 0)
			return 0;
	if (to.x < from.x) {
		tmp = from;
		from = to;
		to = tmp;
	}
	if (base == 2)
		base -= 1;
	else base -= 2;
	for (base += i; pos[i].x <= from.x && i < base; i++);
	if (i >= base)
		return 0;
	index[0] = i;
	while (pos[i].x <= to.x && i < base) i++;
	if (i - 1 < index[0]) {
		index[0] = 0;
		return 0;
	}
	index[1] = i - 1;
	return 1;
}

void index2tachadura(const res *pos, res *tachadura,
					 int x, int y, int margin, int p_y) { /*
	tachadura : array de longitud 2
	margin : el margen que deja a los lados
	p_y : altura de cada piso
	escribe en tachadura las coordenadas para tachar de stick[x] a stick[y] */
	tachadura[0].x = pos[x].x - margin;
	tachadura[0].y = pos[x].y - p_y / 2;
	tachadura[1].x = pos[y].x + margin;
	tachadura[1].y = tachadura[0].y;
}
