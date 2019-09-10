#pragma once
#pragma region principio

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

#define ROJO     "\x1b[31m"
#define BLANCO   "\x1b[37m"

#define tamMenuPrincipal 4
#define tamMenuClientes 3
#define tamMenuServicios 3
#define tamErrores 15

#define apertura "ab+"
#define lectura "rb"
#define escritura "rb+"
#define textoescritura "w"
#define textoapertura "a"

#define fclientes "ficheroclientes.dat"
#define fservicios "ficheroservicios.dat"
#define fpresupuestos "ficheropresupuestos.dat"
#define ffacturas "ficherofacturas.dat"

typedef struct {
	int ncliente;
	char nombre[20];
	char domicilio[20];
	char cp[6];
	char municipio[15];
	char nif[10];
}CLIENTE;

typedef struct {
	int nservicio;
	char denominacion[20];
	float precio;
	float pvp;
}SERVICIO;

typedef struct {
	int ncliente;
	int nservicio;
	float pvp;
	float horas;
}PRESUPUESTO;

typedef struct {
	int nfactura;
	int ncliente;
	float base;
	float iva;
}FACTURA;

#pragma endregion

#pragma region principal
int menuPrincipal();

#pragma region clientes

void clientes();

int menuClientes();

void altaClientes();

void modificarClientes();

void consultarClientes();

#pragma region extraclientes

void mostrarDatosCliente(int pos, CLIENTE cli);

void preguntarCampoCliente(char campo[]);

#pragma endregion

#pragma endregion

#pragma region servicios

void servicios();

int menuServicios();

void altaServicio();

void modificarServicio();

void consultarServicio();

#pragma region extraservicios

void mostrarDatosServicio(int pos, SERVICIO ser);

void preguntarCampoServicio(char c[]);

#pragma endregion

#pragma endregion

#pragma region presupuestos

void presupuestos();

#pragma endregion

#pragma region facturacion

void facturacion();

#pragma endregion

#pragma endregion

#pragma region ficheros

int abrirFicherosTodos();

int calcularTamanoClientes(FILE *file);

int calcularTamanoServicios(FILE *file);

int calcularTamanoPresupuestos(FILE *file);

int calcularTamanoFacturas(FILE *file);

#pragma endregion

#pragma region peticiones

//PETICIONES

void pedirConfirmacion(char c[], FILE *f);

int pedirCliente(int x, int y);

int pedirServicio(int x, int y);

void pedirContinuar(FILE* f, char c[]);

void preguntarCampoCliente(char c[]);

void pedirCadena(char c[], int tam, int x, int y);

int pedirNum(int x, int y, char borrar[3]);

int pedirNumFactura();

#pragma endregion

#pragma region errores

//ERRORES

void error(int x);

void errorFichero();

void errorCampo();

void errorCliente1();

void errorCliente2();

void errorServicio1();

void errorServicio2();

void errorSiono();

void errorStringTamano();

void errorStringVacio();

void errorNoCliente();

void errorNoServicio();

void errorNoPresupuesto();

void errorNum();

void errorPresupuesto();

void errorTamanoFichero();

#pragma endregion

#pragma region diseño

void plantillaClientes();

void plantillaServicios();

void plantillaPresupuestos();

void plantillaFacturas();

void plantillaResumen();

void limpiarLinea(int x, int y);

#pragma endregion

#pragma region extras

//EXTRAS

void gotoXY(int x, int y);

void pausar();

#pragma endregion