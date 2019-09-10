#include "funciones.h"

main() {
	setlocale(LC_ALL, "");
	int ret = abrirFicherosTodos();
	//Si devuelve error, fin del programa
	if (ret == 1)
		return;
	int opc;
	//Men� con punteros a funciones del men� principal
	void(*funcionesPrincipal[tamMenuPrincipal])() = { clientes, servicios, presupuestos, facturacion };
	opc = menuPrincipal();
	while (opc != tamMenuPrincipal + 1) {
		(*funcionesPrincipal[opc - 1])();
		opc = menuPrincipal();
	}
	system("cls");
	system("pause");
	//FIN PROGRAMA
}