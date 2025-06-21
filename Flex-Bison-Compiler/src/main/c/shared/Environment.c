#include "Environment.h"
#include <glib.h>

/* PUBLIC FUNCTIONS */

const boolean getBooleanOrDefault(const char * name, const boolean defaultValue) {
	const char * value = getStringOrDefault(name, NULL);
	if (value == NULL) {
		return defaultValue;
	}
	else if (strcmp(value, "true") == 0) {
		return true;
	}
	else {
		return false;
	}
}

const char * getStringOrDefault(const char * name, const char * defaultValue) {
	const char * value = getenv(name);
	if (value == NULL) {
		return defaultValue;
	}
	else {
		return value;
	}
}

// Function to free your SymbolTableEntry struct
void free_symbol_entry(gpointer data) {
    if (data == NULL) return;
    
    SymbolTableEntry* entry = (SymbolTableEntry*)data;
    if (entry->component != NULL) {
        free(entry->component);
    }
    free(entry);
}
