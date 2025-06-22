#ifndef TYPE_HEADER
#define TYPE_HEADER

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum {
	COMPONENT_ID = 0,
	SIMPLE_ID = 1,
	COMPONENT_ID_INITIALIZED_AND_CREATED = 2,
} idType;

typedef struct {
	idType type;
	char * component; //es opcional, solo si el type es COMPONENT_ID. la idea es que esto sea basicamente un vomito de html
	///que voy a pegar cada vez que llame al componente.
} SymbolTableEntry;

typedef int Token;

#endif
