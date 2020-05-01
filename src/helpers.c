int pisos2base(int pisos, int pico, int par) { /*
	pisos : numero de pisos
	pico : true o false si tiene o no tiene pico
	par : true o false si es par o impar
	retorna el numero de palitos en la base */
	int base = 3;
	if (par) base = 2;
	if (pico) pisos--;
	while (--pisos > 0) base += 2;
	return base;
}

int base2pisos(int base, int pico) { /*
	base : numero de palitos en la base
	pico : true o false si tiene o no tiene pico
	retorna el numero de pisos */
	int pisos = 1;
	if (pico) pisos++;
	while ((base -= 2) >= 2) pisos++;
	return pisos;
}

int number_of_sticks(int base, int pico) { /*
	base : numero de palitos en la base
	pico : true o false si tiene o no tiene pico
	retorna el numero de palitos total */
	if (base < 4) {
		if (pico)
			return base + 1;
		else return base;
	}
	return base + number_of_sticks(base - 2, pico);
}
