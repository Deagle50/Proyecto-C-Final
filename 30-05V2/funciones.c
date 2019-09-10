#include "funciones.h"

#pragma region menuPrincipal

int menuPrincipal() {
	int o;

	//Menú principal, devuelve el valor de la opción a entrar
	do {
		system("cls");
		printf("\nMenú de facturación\n\n1.Clientes\n\n2.Servicios\n\n3.Presupuestos\n\n4.Facturación\n\n5.Salir\n\n\nEscoge una opción [ ]");
		o = pedirNum(20, 15, "si");
	} while (o<1 || o>tamMenuPrincipal + 1);

	return o;
}

#pragma region clientes

void clientes() {
	int opc;

	//Punteros a función del menú de servicios servicios
	void(*funcionesClientes[tamMenuClientes])() = { altaClientes, modificarClientes, consultarClientes };
	opc = menuClientes();
	while (opc != tamMenuClientes + 1) {
		(*funcionesClientes[opc - 1])();
		opc = menuClientes();
	}
}

int menuClientes() {
	int o;
	char b;

	//Menú de clientes, devuelve el valor de la opción a entrar
	do {
		system("cls");
		printf("\nMenú de clientes\n\n1.Alta\n\n2.Modificar\n\n3.Consultar\n\n4.Salir\n\n\nEscoge una opción [ ]");
		o = pedirNum(20, 13, "si");
	} while (o<1 || o>tamMenuClientes + 1);
	return o;
}

void altaClientes() {
	system("cls");

	FILE* file;
	CLIENTE cli;
	int pos, tam;
	char resp[10];

	//Comprueba, y si da error al abrir el fichero, muestra error
	file = fopen(fclientes, escritura);
	if (file == NULL)
	{
		error(1); return;
	}

	//calcular tamaño fichero y cerrar
	tam = calcularTamanoClientes(file);

	//Muestra la plantilla de clientes
	plantillaClientes();
	//Pedir datos clientes
	pos = tam + 1;
	printf("Escribe los datos del cliente  %d", pos);
	gotoXY(19, 4);
	cli.ncliente = pos;
	printf("%d", cli.ncliente);
	gotoXY(8, 6);
	pedirCadena(cli.nombre, 20, 8, 6);
	gotoXY(11, 8);
	pedirCadena(cli.domicilio, 20, 11, 8);
	gotoXY(4, 10);
	pedirCadena(cli.cp, 5, 4, 10);
	gotoXY(11, 12);
	pedirCadena(cli.municipio, 15, 11, 12);
	gotoXY(5, 14);
	pedirCadena(cli.nif, 10, 5, 14);
	//Pedir confirmación y si es afirmativo escribe los datos
	pedirConfirmacion(resp, file);
	if (strcmp(resp, "si") == 0)
	{
		fseek(file, sizeof(cli) * (pos - 1), SEEK_SET);
		fwrite(&cli, sizeof(cli), 1, file);
		puts("Se guardó el cliente");
		getch();
	}
	else
	{
		puts("No se guardó el cliente.");
		getch();
	}
	//Cierra el fichero y fin de función. Vuelta al menú
	fclose(file);

}

void modificarClientes() {
	system("cls");
	FILE* file;
	CLIENTE cli;
	int pos, tam;
	char resp[10], campo[15];
	//Intenta abrir fichero, si falla, error
	file = fopen(fclientes, escritura);
	if (file == NULL)
	{
		error(1); return;
	}

	/*CALCULAR TAMAÑO FICHERO Y CERRARLO*/
	tam = calcularTamanoClientes(file);
	//TAM=TAMAÑO
	//Si el fichero de clientes está vacío, error
	if (tam == 0)
	{
		error(10);
		fclose(file);
		return;
	}

	//Pedir posición del cliente y mostrar plantilla
	plantillaClientes();
	pos = pedirCliente(19, 4);
	//Si la posición es mayor a la cantidad de clientes, error
	if (pos > tam || pos == 0)
	{
		error(3); fclose(file); return;
	}
	//Abre posición del cliente
	fseek(file, sizeof(cli) * (pos - 1), SEEK_SET);
	fread(&cli, sizeof(cli), 1, file);

	//Muestra los datos al cliente
	mostrarDatosCliente(pos, cli);
	//Pregunta el campo a modificar
	preguntarCampoCliente(campo);
	while (strcmp(campo, "nombre") != 0 && strcmp(campo, "domicilio") != 0 && strcmp(campo, "cp") != 0 && strcmp(campo, "municipio") != 0 && strcmp(campo, "nif") != 0)
	{
		//Si no es ningún campo, da error y vuelve a pedir campo
		error(2);
		preguntarCampoCliente(campo);
	}
	limpiarLinea(1, 21);
	limpiarLinea(1, 20);
	//Pide el valor del campo
	puts("Escribe el valor del dato a modificar");
	//Va a la posición a modificar, obtiene el dato, lo copia en cli
	if (strcmp(campo, "nombre") == 0)
	{
		limpiarLinea(8, 6);
		pedirCadena(cli.nombre, 20, 8, 6);
	}
	if (strcmp(campo, "domicilio") == 0)
	{
		limpiarLinea(11, 8);
		pedirCadena(cli.domicilio, 20, 11, 8);
	}
	if (strcmp(campo, "cp") == 0)
	{
		limpiarLinea(4, 10);
		pedirCadena(cli.cp, 5, 4, 10);
	}
	if (strcmp(campo, "municipio") == 0)
	{
		limpiarLinea(10, 12);
		pedirCadena(cli.municipio, 15, 10, 12);
	}
	if (strcmp(campo, "nif") == 0)
	{
		limpiarLinea(5, 14);
		pedirCadena(cli.nif, 10, 5, 14);
	}

	//Pedir confirmación y si es afirmativo escribe los datos
	pedirConfirmacion(resp, file);
	if (strcmp(resp, "si") == 0)
	{
		fseek(file, sizeof(cli)*(pos - 1), SEEK_SET);//MAL; LO GUARDA EN TAM, NO EN POS-1
		fwrite(&cli, sizeof(cli), 1, file);
		puts("Los cambios se guardaron correctamente."); getch();
	}
	else
	{
		puts("No se guardaron los cambios."); getch();
	}
	fclose(file);//CERRAR
}

void consultarClientes() {
	FILE *file;
	CLIENTE cli;
	int pos, tam;
	char resp[10];
	file = fopen(fclientes, lectura);
	if (file == NULL)
	{
		error(1); return;
	}
	//CALCULAR TAMAÑO FICHERO
	tam = calcularTamanoClientes(file);
	//Si el fichero está vacío, error
	if (tam == 0)
	{
		error(10);
		fclose(file);
		return;
	}
	//Hacer una vez y repetir mientras respuesta sea "sí"
	do {
		//Mostrar datos
		plantillaClientes();
		//Pedir posición del cliente, si es 0 o si es mayor que el tamaño, error
		pos = pedirCliente(19, 4);
		if (pos > tam || pos == 0)
		{
			error(3); fclose(file); return;
		}
		//Abrir cliente*pos
		fseek(file, sizeof(cli) * (pos - 1), SEEK_SET);
		fread(&cli, sizeof(cli), 1, file);
		//Muestra los datos del cliente
		mostrarDatosCliente(pos, cli);
		gotoXY(1, 16);
		pausar();
		//Preguntar si continuar
		pedirContinuar(file, resp);
		//Repetir mientras respuesta sea sí
	} while (strcmp(resp, "si") == 0);
	//Cerrar fichero
}

#pragma region extraclientes

void mostrarDatosCliente(int pos, CLIENTE cli) {
	//Muestra los datos del cliente CLI
	gotoXY(19, 4);
	cli.ncliente = pos;
	printf("%d", cli.ncliente);
	gotoXY(8, 6);
	puts(cli.nombre);
	gotoXY(11, 8);
	puts(cli.domicilio);
	gotoXY(4, 10);
	puts(cli.cp);
	gotoXY(11, 12);
	puts(cli.municipio);
	gotoXY(5, 14);
	puts(cli.nif);
}

void preguntarCampoCliente(char campo[]) {
	//pide y valida el nombre del campo a modificar
	limpiarLinea(1, 20);
	limpiarLinea(1, 21);
	gotoXY(1, 20);
	puts("Escribe el campo a modificar del cliente");
	pedirCadena(campo, 15, 1, 21);
	_strlwr(campo);
}

#pragma endregion

#pragma endregion

#pragma region servicios

void servicios() {
	int opc;
	//Punteros a funciones del menú de servicios
	void(*funcionesServicios[tamMenuServicios])() = { altaServicio,modificarServicio,consultarServicio };
	opc = menuServicios();
	while (opc != tamMenuServicios + 1) {
		(*funcionesServicios[opc - 1])();
		opc = menuServicios();
	}
}

int menuServicios() {
	int o;
	//Menú de servicios, devuelve el valor de la opción a entrar
	do {
		system("cls");
		puts("\nMenú de servicios");
		puts("\n1.Alta");
		puts("\n2.Modificar");
		puts("\n3.Consultar");
		puts("\n4.Salir");
		puts("\nEscoge una opción: [ ]");
		o = pedirNum(21, 12, "si");
	} while (o<1 || o>tamMenuServicios + 1);
	return o;
}

void altaServicio() {
	system("cls");
	FILE* file;
	SERVICIO ser;
	int pos, tam;
	char resp[10], b;

	//Intenta abrir el archivo y si falla da error
	file = fopen(fservicios, escritura);
	if (file == NULL)
	{
		error(1); return;
	}

	//calcular tamaño fichero
	tam = calcularTamanoServicios(file);

	//Muestra la plantilla de servicios
	plantillaServicios();
	//Pedir datos servicio
	pos = tam + 1;
	printf("\nEscribe los datos del servicio %d", pos);
	gotoXY(21, 4);
	ser.nservicio = pos;
	printf("%d", ser.nservicio);
	gotoXY(14, 6);
	pedirCadena(ser.denominacion, 20, 14, 6);
	gotoXY(14, 8);
	scanf("%f%c", &ser.precio, &b);
	gotoXY(5, 10);
	scanf("%f%c", &ser.pvp, &b);
	//Pedir confirmación y si es afirmativo escribe los datos
	pedirConfirmacion(resp, file);

	if (strcmp(resp, "si") == 0)
	{
		fseek(file, sizeof(ser) * (pos - 1), SEEK_SET);
		fwrite(&ser, sizeof(ser), 1, file);
		puts("Se guardó el servicio");
		getch();

	}
	else
	{
		puts("No se guardó el servicio.");
		getch();
	}
	fclose(file);//CERRAR
}

void modificarServicio() {
	FILE* file;
	SERVICIO ser;
	int pos, tam;
	char resp[3], campo[15], b;


	file = fopen(fservicios, escritura);
	if (file == NULL)
	{
		error(1); return;
	}
	/*Calcular tamaño fichero y cerrarlo si está vacío*/
	tam = calcularTamanoServicios(file);
	if (tam == 0)
	{
		error(11);
		fclose(file);
		return;
	}

	//Pedir posición del servicio y mostrar plantilla
	plantillaServicios();
	pos = pedirServicio(20, 4);
	if (pos > tam&&pos == 0)
	{
		//Si está fuera de los límites, error
		error(5); fclose(file); return;
	}

	//Leer datos
	fseek(file, sizeof(ser) * (pos - 1), SEEK_SET);
	fread(&ser, sizeof(ser), 1, file);
	//Muestra los datos del servicio
	mostrarDatosServicio(pos, ser);
	//Pide el campo del servicio a modificar
	preguntarCampoServicio(campo);
	while (strcmp(campo, "denominacion") != 0 && strcmp(campo, "precio") != 0 && strcmp(campo, "precio coste") != 0 && strcmp(campo, "pvp") != 0)
	{
		//Si el campo no está bien, error
		error(2);
		preguntarCampoServicio(campo);
	}
	limpiarLinea(1, 20);
	limpiarLinea(1, 21);
	//Pide el valor del campo
	puts("Escribe el valor del dato a modificar");
	//Va a la posición a modificar, obtiene el dato, lo copia en ser
	if (strcmp(campo, "denominacion") == 0)
	{
		limpiarLinea(14, 6);
		pedirCadena(ser.denominacion, 20, 14, 6);
	}
	if (strcmp(campo, "precio") == 0 || strcmp(campo, "precio coste") == 0)
	{
		limpiarLinea(14, 8); scanf("%f%c", &ser.precio, &b);
	}
	if (strcmp(campo, "pvp") == 0)
	{
		limpiarLinea(5, 10); scanf("%f%c", &ser.pvp, &b);
	}

	//Pedir confirmación y si es afirmativo escribe los datos
	pedirConfirmacion(resp, file);
	if (strcmp(resp, "si") == 0)
	{
		fseek(file, sizeof(ser)*(pos - 1), SEEK_SET);
		fwrite(&ser, sizeof(ser), 1, file);
		puts("Los cambios se guardaron correctamente."); getch();
	}
	else
	{
		puts("No se guardaron los cambios."); getch();
	}
	fclose(file);//CERRAR
}

void consultarServicio() {
	FILE *file;
	SERVICIO ser;
	int pos, tam;
	char resp[10];

	//Si falla al abrir el fichero, error
	file = fopen(fservicios, lectura);
	if (file == NULL)
	{
		error(1); return;
	}
	/*Calcular tamaño fichero*/

	tam = calcularTamanoServicios(file);
	if (tam == 0)
	{
		//Si el tamaño es cero, error (el fichero está vacío)
		error(11);
		fclose(file);
		return;
	}
	//Hacer una vez y repetir mientras respuesta sea "sí"
	do {
		//Mostrar datos
		plantillaServicios();
		//Pedir servicio a mostrar
		pos = pedirServicio(20, 4);
		//Si la posición no es correcta, error
		if (pos > tam || pos == 0)
		{
			error(5); fclose(file); return;
		}
		//Lee el fichero de servicio*pos
		fseek(file, sizeof(ser) * (pos - 1), SEEK_SET);
		fread(&ser, sizeof(ser), 1, file);
		//Muestra los datos del servicio
		mostrarDatosServicio(pos, ser);
		gotoXY(1, 12);
		pausar();
		//Preguntar si continuar
		pedirContinuar(file, resp);
	} while (strcmp(resp, "si") == 0);
	//Cerrar fichero
	fclose(file);

}

#pragma region extraservicios

void mostrarDatosServicio(int pos, SERVICIO ser) {
	//Muestra los datos del servicio
	gotoXY(20, 4);
	ser.nservicio = pos;
	printf("%d", ser.nservicio);
	gotoXY(14, 6);
	puts(ser.denominacion);
	gotoXY(14, 8);
	printf("%.2f", ser.precio);
	gotoXY(5, 10);
	printf("%.2f", ser.pvp);
}

void preguntarCampoServicio(char campo[]) {
	//pide y valida el nombre del campo a modificar
	limpiarLinea(1, 20);
	limpiarLinea(1, 21);
	gotoXY(1, 20);
	puts("Escribe el campo a modificar del servicio");
	pedirCadena(campo, 15, 1, 21);
	_strlwr(campo);
}

#pragma endregion

#pragma endregion

#pragma region presupuestos

void presupuestos() {
	FILE *filec, *files, *filep;
	CLIENTE cli;
	SERVICIO ser;
	PRESUPUESTO pre;
	int poscli, posser;
	int tamc, tams, tamp;
	int y = 8;
	int cont = 0;
	char resp[10], b;

#pragma region abrir1yabrir2

	filec = fopen(fclientes, lectura);
	if (filec == NULL)
	{
		error(1); return;
	}
	files = fopen(fservicios, lectura);
	if (files == NULL)
	{
		error(1); fclose(filec); return;
	}
	filep = fopen(fpresupuestos, escritura);
	if (files == NULL)
	{
		error(1); fclose(files); fclose(filec); return;
	}
	tamc = calcularTamanoClientes(filec);
	tams = calcularTamanoServicios(files);
	tamp = calcularTamanoPresupuestos(filep);

	if (tamc == 0)
	{
		error(10);
		fclose(filec);
		fclose(files);
		fclose(filep);
		return;
	}
	if (tams == 0)
	{
		error(11);
		fclose(filec);
		fclose(files);
		fclose(filep);
		return;
	}

#pragma endregion

	plantillaPresupuestos();
	poscli = pedirCliente(20, 04);

	//Hacer mientras NºCliente(pos) sea distinto de 0
	while (poscli != 0) {
		if (poscli > tamc)
		{
			error(4);
			fclose(filec);
			fclose(files);
			fclose(filep);
			return;
		}
		y = 8;

		fseek(filec, sizeof(cli)*(poscli - 1), SEEK_SET);
		fread(&cli, sizeof(cli), 1, filec);
		gotoXY(38, 4);
		puts("Nombre del cliente:");
		gotoXY(58, 4);
		puts(cli.nombre);
		/*Pedir servicio y repetir mientras servicio sea distinto de 0*/
		posser = pedirServicio(1, y);
		while (posser != 0) {
			/*Si el servicio es mayor a la cantidad de servicios, error*/
			if (posser > tams)
			{
				error(6);
				fclose(filec);
				fclose(files);
				fclose(filep);
				return;
			}
			fseek(files, sizeof(ser)*(posser - 1), SEEK_SET);
			fread(&ser, sizeof(ser), 1, files);

			gotoXY(23, y);
			puts(ser.denominacion);
			gotoXY(38, y);//38,06
			printf("%.2f", ser.pvp);
			gotoXY(1, 20);
			puts("Escribe la cantidad de horas tomadas en el servicio");
			gotoXY(45, y);
			scanf("%f%c", &pre.horas, &b);
			pre.ncliente = poscli;
			pre.nservicio = posser;
			pre.pvp = ser.pvp;


			pedirConfirmacion(resp, filep);
			if (strcmp(resp, "si") == 0)
			{
				//guardar en fichero presupuestos
				cont++;
				if (cont < 10) {
					fseek(filep, sizeof(pre)*(tamp), SEEK_SET);
					fwrite(&pre, sizeof(pre), 1, filep);
					puts("Se guardó el presupuesto");
					getch();
					limpiarLinea(1, 20);
					limpiarLinea(1, 21);
					limpiarLinea(1, 22);
					y++;
					tamp = calcularTamanoPresupuestos(filep);
				}
				else {
					error(15);
					fclose(filec);
					fclose(files);
					fclose(filep);
					pausar();
					return;
				}
			}
			else
			{
				puts("No se guardó el presupuesto.");
				getch();
			}


			//Pedir siguiente
			posser = pedirServicio(1, y);
		}

		gotoXY(1, 20);
		limpiarLinea(1, 20);
		pausar();
		//Volver a hacer
		plantillaPresupuestos();
		poscli = pedirCliente(20, 04);
		cont = 0;
	}

	fclose(filec);
	fclose(files);
	fclose(filep);
	gotoXY(1, 20);
	pausar();
}

#pragma endregion

#pragma region facturacion

void facturacion() {
	FILE *filec, *files, *filep, *filef;
	CLIENTE cli;
	SERVICIO ser;
	PRESUPUESTO pre;
	FACTURA fac;
	int tamc, tams, tamp, tamf;
	int y = 13;
	int f = 0;
	int primera = 1;
	int encontrado = 0;

	//Datos de fecha
	time_t d;
	struct tm *tm;
	char fecha[15];
	d = time(NULL);
	tm = localtime(&d);
	strftime(fecha, 15, "%d/%m/%Y", tm);

#pragma region abrirficheros

	filec = fopen(fclientes, lectura);
	if (filec == NULL)
	{
		error(1); fclose(filec); return;
	}
	files = fopen(fservicios, lectura);
	if (files == NULL)
	{
		error(1); fclose(files); return;
	}
	filep = fopen(fpresupuestos, lectura);
	if (files == NULL)
	{
		error(1); fclose(filep); return;
	}

	tamc = calcularTamanoClientes(filec);
	if (tamc == 0)
	{
		error(10);
		fclose(filec);
		fclose(files);
		fclose(filep);

		return;
	}
	tams = calcularTamanoServicios(files);
	if (tams == 0)
	{
		error(11);
		fclose(filec);
		fclose(files);
		fclose(filep);
		return;
	}
	tamp = calcularTamanoPresupuestos(filep);
	if (tamp == 0)
	{
		error(12);
		fclose(filec);
		fclose(files);
		fclose(filep);
		return;
	}


	filef = fopen(ffacturas, escritura);
	if (fopen == NULL)
	{
		error(1);
		fclose(filec);
		fclose(files);
		fclose(filep);
		return;
	}

#pragma endregion

	//Pedir número inicial de factura
	fac.nfactura = pedirNumFactura();

	//Mientras haya clientes
	for (int c = 0; c < tamc; c++)
	{
#pragma region ficherotxt
		char num[1], fichero[50];
		sprintf(fichero, "factura_simplificada_%d.txt", c);
		FILE *filer;
		filer = fopen(fichero, textoescritura);
		if (filer == NULL)
		{
			error(1);
			return;

		}
		
#pragma endregion

		fac.base = 0;
		fac.iva = 0;
		y = 13;
#pragma region plantillayfecha
		//Mostrar plantilla, con fecha
		plantillaFacturas();

		gotoXY(81, y - 4);
		printf("%7d", fac.nfactura);

		gotoXY(20, y - 4);
		printf("%s", fecha);
#pragma endregion

		primera = 1;
		//Mientras haya presupuestos, comprobar si ncliente de presupuesto es el mismo que el del cliente actual
		for (int p = 0; p < tamp; p++)
		{
			//Leer presupuesto
			fseek(filep, sizeof(pre) * (p), SEEK_SET);
			fread(&pre, sizeof(pre), 1, filep);
			//Si el cliente actual es el mismo que el del presupuesto...
			if (c == pre.ncliente - 1)
			{
				//Si es el primer presupuesto del cliente, mostrar datos
				if (primera == 1)
				{
					fseek(filec, sizeof(cli)*c, SEEK_SET);
					fread(&cli, sizeof(cli), 1, filec);
					fprintf(filer, "\nFACTURA Nº: %d\n\n", fac.nfactura);


					fprintf(filer, "Fecha: %s\n\n", fecha);
					//Mostrar datos del cliente y escribir en fichero factura
					gotoXY(90, 2);
					printf("%d", cli.ncliente);
					gotoXY(90, 3);
					puts(cli.nombre);
					gotoXY(90, 4);
					puts(cli.domicilio);
					gotoXY(90, 5);
					puts(cli.cp);
					gotoXY(90, 6);
					puts(cli.municipio);
					gotoXY(90, 7);
					puts(cli.nif);
					fprintf(filer, "Nº Cliente: %d\n", cli.ncliente);
					fprintf(filer, "Nombre:\t %-20s\tNIF: %10s\n", cli.nombre, cli.nif);
					fprintf(filer, "Domicilio: %-20s\tCP: %5s\n", cli.domicilio, cli.cp);
					fprintf(filer, "Municipio: %-15s\n\n", cli.municipio);
					fprintf(filer, "------------------------------------------------------------------\n");
					fprintf(filer, "%12s%25s%8s%10s%11s\n", "NºServicio", "Denominación", "PVP", "Horas", "Importe");
					fprintf(filer, "------------------------------------------------------------------\n");
					primera = 0;
				}

				//Encontrar datos de servicio
				encontrado = 0;

				for (int s = 0; s < tams&&encontrado == 0; s++)
				{
					fseek(files, sizeof(ser)*s, SEEK_SET);
					fread(&ser, sizeof(ser), 1, files);
					if (ser.nservicio == pre.nservicio)
					{
						encontrado = 1;
						fseek(files, sizeof(ser)*s, SEEK_SET);
						fread(&ser, sizeof(ser), 1, files);
#pragma region datosServicio
						//Escribir datos servicio
						gotoXY(3, y);
						printf("%d", pre.nservicio);

						gotoXY(20, y);
						puts(ser.denominacion);

						gotoXY(70, y);
						printf("%.2f", ser.pvp);

						gotoXY(90, y);
						printf("%.2f", pre.horas);

						//Calcular importe
						gotoXY(108, y);
						printf("%.2f", pre.horas*ser.pvp);
						fprintf(filer, "%12d", ser.nservicio);
						fprintf(filer, "%25s", ser.denominacion);
						fprintf(filer, "%8.2f", ser.pvp);
						fprintf(filer, "%10.2f", pre.horas);
						fprintf(filer, "%10.2f€\n", pre.horas*ser.pvp);
						fprintf(filer, "------------------------------------------------------------------\n");
#pragma endregion

#pragma region resumenypie

						//Calcular datos
						fac.base += pre.horas * ser.pvp;
						fac.iva = fac.base*0.21;


#pragma endregion
						y++;
					}
				}

			}
		}
		//Escribir pie
		gotoXY(90, 27);
		printf("%.2f", fac.base);

		gotoXY(90, 28);
		printf("%.2f", fac.iva);
		gotoXY(90, 29);
		printf("%.2f", fac.base + fac.iva);
		fac.ncliente = c + 1;
		fprintf(filer, "\nBase: \t%10.2f€\n", fac.base);
		fprintf(filer, "IVA: \t%10.2f€\n", fac.iva);
		fprintf(filer, "Total: \t%10.2f€\n", fac.base + fac.iva);
		//Escribir factura
		fseek(filef, sizeof(fac)*f, SEEK_SET);
		fwrite(&fac, sizeof(fac), 1, filef);

		fac.nfactura++, f++;
		pausar();

	}




	//cerrar todos los ficheros
	fclose(files);
	fclose(filep);
	fclose(filef);
	//Abrir fichero facturas en lectura
	filef = fopen(ffacturas, lectura);
	if (filef == NULL)
	{
		error(1);
		fclose(filep);
		fclose(filec);
		fclose(files);
		return;
	}

	plantillaResumen();
	int x = 60;
	float total = 0;
	float ivatotal = 0;
	y = 8;
	tamf = calcularTamanoFacturas(filef);
	for (f = 0; f < tamf; f++)
	{
		//Leer factura
		fseek(filef, sizeof(fac)*f, SEEK_SET);
		fread(&fac, sizeof(fac), 1, filef);


		for (int c = 0; c < tamc; c++)
		{
			//leer cliente
			fseek(filec, sizeof(cli)*c, SEEK_SET);
			fread(&cli, sizeof(cli), 1, filec);
			//Comprobar que el ncliente de factura y el de cli son el mismo
			if (fac.ncliente == cli.ncliente&&fac.base != 0)//No imprimirá los datos 
			{
				gotoXY(3, y);
				printf("%d", fac.nfactura);
				gotoXY(20, y);
				puts(cli.nombre);
				gotoXY(x + 2, y);
				puts(cli.nif);
				gotoXY(x + 14, y);
				printf("%24.2f", fac.base);
				gotoXY(x + 44, y);
				printf("%15.2f", fac.iva);
				total += fac.base;
				ivatotal += +fac.iva;
				y++;
			}
		}
	}
	//Escribir pie
	gotoXY(x + 13, 28);
	printf("%24.2fE", total);
	gotoXY(x + 44, 28);
	printf("%14.2fE", ivatotal);
	pausar();

}

#pragma endregion

#pragma endregion

#pragma region ficheros

int abrirFicherosTodos() {
	//COMPRUEBA SI ESTÁN CREADOS LOS FICHEROS, SI NO, LOS CREA (Solo se ejecuta al inicio del programa)
	//Devuelven un '1' si ha habido algún error, y se terminará la ejecución del programa
	FILE *filec, *files, *filep, *filef;
	filec = fopen(fclientes, apertura);
	if (filec == NULL)
	{
		error(1);
		return 1;
	}

	files = fopen(fservicios, apertura);
	if (files == NULL)
	{
		error(1);
		fclose(filec);
		return 1;

	}
	filep = fopen(fpresupuestos, apertura);
	if (filep == NULL)
	{
		error(1);
		fclose(filec);
		fclose(files);
		return 1;
	}
	filef = fopen(ffacturas, apertura);
	if (filef == NULL)
	{
		error(1);
		fclose(filec);
		fclose(filep);
		fclose(files);
		return 1;
	}
	return 0;
}
//Las siguientes cuatro funciones calculan el tamaño de los ficheros y lo devuelven
int calcularTamanoClientes(FILE *file) {
	int tam;
	CLIENTE cli;

	fseek(file, 0, SEEK_END);
	tam = ftell(file);
	fseek(file, 0, SEEK_SET);
	tam -= ftell(file);
	tam = tam / sizeof(cli);

	return tam;
}

int calcularTamanoServicios(FILE *file) {
	int tam;
	SERVICIO ser;

	fseek(file, 0, SEEK_END);
	tam = ftell(file);
	fseek(file, 0, SEEK_SET);
	tam -= ftell(file);
	tam = tam / sizeof(ser);

	return tam;
}

int calcularTamanoPresupuestos(FILE *file) {
	int tam;
	PRESUPUESTO pre;

	fseek(file, 0, SEEK_END);
	tam = ftell(file);
	fseek(file, 0, SEEK_SET);
	tam -= ftell(file);
	tam = tam / sizeof(pre);

	return tam;
}

int calcularTamanoFacturas(FILE *file) {
	int tam;
	FACTURA fac;

	fseek(file, 0, SEEK_END);
	tam = ftell(file);
	fseek(file, 0, SEEK_SET);
	tam -= ftell(file);
	tam = tam / sizeof(fac);

	return tam;
}

#pragma endregion

#pragma region peticiones

//PETICIONES
//En estas peticiones no he usado "pedirNum" cuando requería un int porque no he hecho que valide si metes números con más de una cifra
void pedirConfirmacion(char c[], FILE *f) {
	//Pide confirmación para guardar
	limpiarLinea(1, 20);
	limpiarLinea(1, 21);
	gotoXY(1, 20);
	puts("¿Quieres guardar?:");
	pedirCadena(c, 2, 1, 21);
	_strlwr(c);
	//Volverá a pedir confirmación mientras no sea "si" o "no" la respuesta
	while (strcmp(c, "si") != 0 && strcmp(c, "no") != 0)
	{
		error(7);
		limpiarLinea(1, 20);
		limpiarLinea(1, 21);
		gotoXY(1, 20);
		puts("¿Quieres guardar?");
		pedirCadena(c, 2, 1, 21);
		_strlwr(c);
	}
}

int pedirCliente(int x, int y) {
	int o;
	char b;
	//Pide cliente mientras sea menor que 0
	do {
		gotoXY(1, 20);
		printf("Escribe el número del cliente\n");
		limpiarLinea(x, y);
		scanf("%d%c", &o, &b);
	} while (o < 0);
	return o;
}

int pedirServicio(int x, int y) {

	int o;
	char b;

	//Pide servicio mientras sea menor que 0
	do {
		gotoXY(1, 20);
		printf("Escribe el número del servicio\n");
		limpiarLinea(x, y);
		scanf("%d%c", &o, &b);
	} while (o < 0);
	return o;
}

void pedirContinuar(FILE *f, char c[]) {
	//Pregunta si repetir mientras la respuesta no sea "si" o "no"
	limpiarLinea(1, 20);
	limpiarLinea(1, 21);
	gotoXY(1, 20);
	puts("¿Deseas repetir?");
	pedirCadena(c, 2, 1, 21);
	_strlwr(c);
	while (strcmp(c, "si") != 0 && strcmp(c, "no") != 0)
	{
		error(7);
		limpiarLinea(1, 20);
		limpiarLinea(1, 21);
		gotoXY(1, 20);
		puts("¿Deseas repetir?");
		pedirCadena(c, 2, 1, 21);
		_strlwr(c);
	}
	if (strcmp(c, "no") == 0)
	{
		//Si la respuesta es no, cierra el fichero y return
		fclose(f);
		return;
	}
}

void pedirCadena(char c[], int tam, int x, int y) {
	//Función general para pedir cualquier cadena validando su tamaño (int tam). (importante para los datos de las estructuras)
	//Volverá a x,y mientras no se meta una cadena con un tamaño adecuado
	gets(c);
	while (strlen(c) > tam || strlen(c) == 0)
	{
		if (strlen(c) > tam)
			//Error si el tamaño es mayor al permitido
			error(8);
		if (strlen(c) == 0)
			//Error si la cadena está vacía
			error(9);
		limpiarLinea(x, y);
		gets(c);
	}
}

int pedirNum(int x, int y, char borrar[3]) {
	int encontrado = 0, ret;
	char num, blanco;
	//Pide un número, aunque lo guarda en un char para que si se mete un carácter en un int no pete el programa
	//Cuando sea un número, lo convierte a int y lo devuelve
	//No he conseguido que no de problemas si metes un "enter" una vez y luego un número
	//Solo se usa en el menú
	gotoXY(x, y);
	scanf("%c%c", &num, &blanco);
	if (isdigit(num)) {
		encontrado = 1;
		ret = num - '0';
		return ret;
	}

	while (encontrado != 1)
	{
		error(13);
		gotoXY(x, y);
		//Esto es para el menú. Si meten mal el número, volverá a poner el corchete del pedir número [ ]
		printf(" ]                                                                       ");
		gotoXY(x, y);
		scanf("%c%c", &num, &blanco);
		if (isdigit(num)) {
			encontrado = 1;
			ret = num - '0';
			return ret;
		}
	}
}

int pedirNumFactura() {
	//Pide número inicial de factura mientras sea menor que 1
	int n;
	char b;
	do {
		system("cls");
		puts("Escribe el número de factura inicial:");
		scanf("%d%c", &n, &b);
	} while (n < 1);
	return n;
}
#pragma endregion

#pragma region errores

//ERRORES

//Punteros a función de errores
void(*errores[tamErrores])() = { errorFichero, errorCampo, errorCliente1, errorCliente2, errorServicio1/*5*/, errorServicio2, errorSiono, errorStringTamano, errorStringVacio ,errorNoCliente/*10*/,errorNoServicio,errorNoPresupuesto,errorNum, errorPresupuesto, errorTamanoFichero /*15*/ };

void error(int pos) {
	//Función de errores, dará el código de error correspondiente y
	//llamará al error pos
	limpiarLinea(1, 25);
	limpiarLinea(1, 26);
	limpiarLinea(1, 27);
	limpiarLinea(1, 28);
	gotoXY(1, 25);
	printf(ROJO"ERROR %d\n", pos);
	gotoXY(1, 26);
	(*errores[pos - 1])();
	pausar();
	limpiarLinea(1, 25);
	limpiarLinea(1, 26);
	limpiarLinea(1, 27);
	limpiarLinea(1, 28);
}

void errorFichero() {
	//1
	puts("Fallo al abrir el archivo."BLANCO);
	return;
}

void errorCampo() {
	//2
	puts("El campo no existe."BLANCO);
	return;
}

void errorCliente1() {
	//3
	//Error al escribir el número del cliente, preguntará si añadir el siguiente
	char resp[10];
	puts("No existe el cliente."BLANCO);
	puts("¿Quieres añadir el siguiente cliente?");
	pedirCadena(resp, 2, 1, 28);
	_strlwr(resp);
	if (strcmp(resp, "si") == 0)
	{
		altaClientes();
	}
}

void errorCliente2() {
	//4
	//Error al escribir el número del cliente. No pregunta si añadir el siguiente porque está en la función de "presupuestos"
	puts("No existe el cliente."BLANCO);
	return;
}

void errorServicio1() {
	//5
	//Error al escribir el número del servicio, preguntará si añadir el siguiente
	char resp[10];
	puts("No existe el servicio."BLANCO);
	puts("¿Quieres añadir el siguiente servicio?");
	pedirCadena(resp, 2, 1, 28);
	_strlwr(resp);
	if (strcmp(resp, "si") == 0)
	{
		altaServicio();
	}
}

void errorServicio2() {
	//6
	//Error al escribir el número del servicios. No pregunta si añadir el siguiente porque está en la función de "presupuestos"
	puts("No existe el servicio."BLANCO);
	return;
}

void errorSiono() {
	//7
	puts("La respuesta no es válida"BLANCO);
	puts("Escribe \"si\" o \"no\"");
	return;
}

void errorStringTamano() {
	//8
	puts("El texto introducido es más grande que el tamaño soportado"BLANCO);
	puts("Vuelve a introducir el texto");
	return;
}

void errorStringVacio() {
	//9
	puts("El texto introducido no puede estar vacío"BLANCO);
	return;
}

void errorNoCliente() {
	//10
	puts("El fichero de clientes está vacío, introduce clientes para acceder a esta función."BLANCO);
	return;
}

void errorNoServicio() {
	//11
	puts("El fichero de servicios está vacío, introduce servicios para acceder a esta función."BLANCO);
	return;
}

void errorNoPresupuesto() {
	//12
	puts("El fichero de presupuestos está vacío, introduce presupuestos para acceder a esta función."BLANCO);
	return;
}

void errorNum() {
	//13
	puts("Introduce un número"BLANCO);
	return;
}

void errorPresupuesto() {
	//14
	puts("No existe el cliente."BLANCO);
	return;
}

void errorTamanoFichero() {
	//15
	puts("Hay demasiados registros, este no se añadirá."BLANCO);
	return;
}

#pragma endregion

#pragma region diseño

#pragma region plantillas

void plantillaClientes() {
	//MUESTRA LA PLANTILLA DE LOS DATOS DEL CLIENTE
	system("cls");

	puts("\nDATOS DE CLIENTE");
	puts("\nNúmero de cliente:");
	puts("\nNombre:");
	puts("\nDomicilio:");
	puts("\nCP:");
	puts("\nMunicipio:");
	puts("\nNIF:\n");
}

void plantillaServicios() {
	//MUESTRA LA PLANTILLA DE LOS DATOS DEL SERVICIO
	system("cls");

	puts("\nDATOS DEL SERVICIO");
	puts("\nNúmero de servicio:");
	puts("\nDenominación:");
	puts("\nPrecio coste:");
	puts("\nPVP:");
}

void plantillaPresupuestos()
{
	//MUESTRA LA PLANTILLA DE PRESUPUESTOS
	system("cls");
	puts("\nFichero de presupuestos");//,02
	gotoXY(1, 4);
	puts("Número de cliente:");//18,04
	gotoXY(1, 6);
	puts("Número de servicio");//19,06
	gotoXY(23, 06);//23,06
	puts("Denominación");
	gotoXY(38, 06);//38,06
	puts("P.V.P");
	gotoXY(45, 06);
	puts("Cantidad de horas");
	gotoXY(1, 7);
	for (int i = 0; i < 120; i++)
		printf("-");
	printf("\n");
}

void plantillaFacturas() {
	//MUESTRA LA PLANTILLA DE FACTURAS
	system("cls");
	int xclientes = 70, yservicio = 9;

#pragma region lineas
	//Escribir las líneas de diseño
#pragma region verticales

	//Línea medio
	for (int i = 1; i <= 30; i++)
	{
		gotoXY(xclientes - 2, i);
		printf("|");
	}

	//Línea izquierda
	for (int i = yservicio - 1; i <= 26; i++)
	{
		gotoXY(1, i);
		printf("|");
	}
	//Línea izquierda2
	for (int i = yservicio + 1; i <= 26; i++)
	{
		gotoXY(18, i);
		printf("|");
	}
	//Línea derecha
	for (int i = 1; i <= 30; i++)
	{
		gotoXY(120, i);
		printf("|");
	}
	//Línea derecha 3
	for (int i = yservicio + 1; i <= 30; i++)
	{
		gotoXY(xclientes + 18, i);
		printf("|");
	}

	//Línea derecha 2
	for (int i = yservicio + 1; i <= 26; i++)
	{
		gotoXY(xclientes + 36, i);
		printf("|");
	}

#pragma endregion

#pragma region horizontales
	//Línea encima cliente
	for (int i = xclientes - 2; i < 118; i++)
	{
		gotoXY(i, 1);
		printf("-");
	}
	//Línea debajo cliente
	for (int i = xclientes - 2; i < 120; i++)
	{
		gotoXY(i, 1);
		printf("-");
	}
	//Línea encima presupuesto 1
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, yservicio - 1);
		printf("-");
	}
	//Línea encima presupuesto
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, yservicio + 1);
		printf("-");
	}
	//Línea debajo presupuesto 1
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, yservicio + 3);
		printf("-");
	}
	//Línea abajo 2
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, 26);
		printf("-");
	}
	//Línea abajo 1
	for (int i = xclientes; i < 120; i++)
	{
		gotoXY(i, 30);
		printf("-");
	}



#pragma endregion

#pragma endregion

#pragma region texto
	gotoXY(3, 3);
	puts("FACTURA");
	gotoXY(xclientes, 2);
	puts("Nº Cliente:");//69,2
	gotoXY(xclientes, 3);
	puts("Nombre:");//67,3
	gotoXY(xclientes, 4);
	puts("Domicilio:");//70,4
	gotoXY(xclientes, 5);
	puts("CP:");//63,5
	gotoXY(xclientes, 6);
	puts("Municipio:");//70,6
	gotoXY(xclientes, 7);
	puts("NIF:");
	gotoXY(3, yservicio);
	puts("Fecha:");//9,7
	gotoXY(xclientes, yservicio);
	puts("Nº Factura:");//71,7
	gotoXY(3, yservicio + 2);
	puts("Nº Servicio");//Van debajo, a partir de 3,8
	gotoXY(20, yservicio + 2);
	puts("Denominación");//Van debajo, a partir de 20,8
	gotoXY(xclientes, yservicio + 2);
	puts("PVP");//Van debajo, a partir de 60,8
	gotoXY(xclientes + 20, yservicio + 2);
	puts("Horas");//Van debajo, a partir de 70,8
	gotoXY(xclientes + 38, yservicio + 2);
	puts("Importe");//Van debajo, a partir de 85,8
	gotoXY(xclientes, 27);
	puts("Base imponible:");//75,37 PONER EN 85
	gotoXY(xclientes, 28);
	puts("IVA:");//64,38 PONER EN 85
	gotoXY(xclientes, 29);
	puts("Total:");//66,39 PONER EN 85

#pragma endregion

}

void plantillaResumen() {
	time_t d;
	struct tm *tm;
	char fecha[15];
	d = time(NULL);
	tm = localtime(&d);
	strftime(fecha, 15, "%d/%m/%Y", tm);

	int y = 6;
	int x = 60;

	system("cls");

#pragma region lineas
	//Escribir las líneas de diseño
#pragma region verticales

	//primera línea izquierda
	for (int i = 2; i < 28; i++)
	{
		gotoXY(1, i);
		printf("|");
	}

	//Segunda línea izquierda
	for (int i = y; i < 28; i++)
	{
		gotoXY(18, i);
		printf("|");
	}

	//Línea centro
	for (int i = y; i < 30; i++)
	{
		gotoXY(x, i);
		printf("|");
	}

	//Línea derecha 3
	for (int i = y; i < 30; i++)
	{
		gotoXY(x + 12, i);
		printf("|");
	}

	//Línea derecha 2
	for (int i = y; i < 30; i++)
	{
		gotoXY(x + 39, i);
		printf("|");
	}

	//Línea derecha 1
	for (int i = 2; i < 30; i++)
	{
		gotoXY(120, i);
		printf("|");
	}
#pragma endregion

#pragma region horizontales
	//Línea 1
	for (int i = 0; i <= 120; i++)
	{
		gotoXY(i, 1);
		printf("-");
	}

	//Línea 2
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, y - 1);
		printf("-");
	}
	//Línea 3
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, y + 1);
		printf("-");
	}

	//Línea 4
	for (int i = 2; i < 120; i++)
	{
		gotoXY(i, 27);
		printf("-");
	}
	//Línea 5
	for (int i = x + 1; i < 120; i++)
	{
		gotoXY(i, 29);
		printf("-");
	}

#pragma endregion

#pragma endregion


	gotoXY(40, 3);
	printf("Listado resumen de facturación del %s", fecha);
	gotoXY(3, y);
	printf("Nº Factura");
	gotoXY(20, y);
	printf("Cliente");
	gotoXY(x + 2, y);
	printf("NIF");
	gotoXY(x + 14, y);
	printf("Base imponible");
	gotoXY(x + 41, y);
	printf("IVA");
	gotoXY(x + 2, 28);
	printf("Totales->");

}

#pragma endregion

void limpiarLinea(int x, int y) {
	//Limpia los datos de x,y y vuelve a x,y para escribir de nuevo
	gotoXY(x, y);
	printf("                                                                ");
	gotoXY(x, y);
}

#pragma endregion

#pragma region extras

//EXTRAS

void gotoXY(int x, int y) {
	//El cursor va a las coordenadas XY
	HANDLE h;
	COORD pos;

	pos.X = x - 1;
	pos.Y = y - 1;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, pos);
}

void pausar() {
	//Hace un system("pause") al final de la consola, después limpia el system pause
	gotoXY(1, 30);
	system("pause");
	limpiarLinea(1, 30);
}

#pragma endregion