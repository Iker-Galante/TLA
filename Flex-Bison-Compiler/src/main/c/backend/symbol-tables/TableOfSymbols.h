#ifndef TABLE_OF_SYMBOLS_H
#define TABLE_OF_SYMBOLS_H

#include <string.h>

extern TableOfSymbols *tableOfSymbols;

typedef struct node
{
    char *attribute;
    char *string; // esto es null excepto para casos que tienen valores extra como Text: string, etc.
    struct node *next;
} node;

typedef struct list
{
    struct node *head;
    struct node *tail;
} list;

typedef struct tableOfSymbols
{
    char *name;
    list *symbols;
    list *errors;
    int size;
} TableOfSymbols;

// si hacemos uso de la tabla, agregar los respectivos metodos para el list y la tabla

#endif