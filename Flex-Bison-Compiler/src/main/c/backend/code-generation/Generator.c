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

void generate(CompilerState *compilerState, TableOfSymbols *tableOfSymbols)
{
	logDebugging(_logger, "Generating code...\n");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	if (compilerState->succeed)
	{
		logDebugging(_logger, "Code generation succeeded.");
	}
	else
	{
		logError(_logger, "Code generation failed.");
	}
}
