#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include "Type.h"
#include <glib.h>


/**
 * The general status of a compilation.
*/
typedef enum {
	SUCCEED = 0,
	FAILED = 1
} CompilationStatus;

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	// The root node of the AST.
	void * abstractSyntaxtTree;

	// A flag that indicates the current state of the compilation so far.
	boolean succeed;

	// TODO: Add an stack to handle nested scopes.
	// TODO: Add a symbol table.
	GHashTable * symbolTable;

	///unDeclaredSymbols es una lista que va a ir metiendo los ids que no estan declarados.
	///la idea es que al final de la compilacion recorrerla y tirar errores por cada uno que este en la lista en cuestion
	GPtrArray * unDeclaredSymbols;
	// TODO: Add configuration.
	// TODO: ...

	// The computed value of the entire program (only for the calculator).
	int value;
} CompilerState;



#endif
