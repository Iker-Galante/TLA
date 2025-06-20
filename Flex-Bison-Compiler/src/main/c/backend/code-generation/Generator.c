#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;

void initializeGeneratorModule()
{
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule()
{
	if (_logger != NULL)
	{
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void generate(Program *program, TableOfSymbols *tableOfSymbols)
{
	if (program == NULL)
	{
		logError(_logger, "Program is NULL, cannot generate code.");
		return;
	}

	logDebugging(_logger, "Generating code...\n");

	FILE *f = fopen("index.html", "w");

	if (f == NULL)
	{
		logError(_logger, "Could not open file for writing: index.html");
		return;
	}

	fprintf(f, "<!DOCTYPE html>\n<html>\n");

	// seguimos con el programa
	// el resto de los generates...

	// Si todo salio bien, cerramos el archivo
	fprintf(f, "</html>\n");
	fclose(f);
}
