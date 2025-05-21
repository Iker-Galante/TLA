#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule()
{
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule()
{
	if (_logger != NULL)
	{
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	char *escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
				 functionName,
				 escapedLexeme,
				 lexicalAnalyzerContext->currentContext,
				 lexicalAnalyzerContext->length,
				 lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	if (_logIgnoredLexemes)
	{
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	if (_logIgnoredLexemes)
	{
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	if (_logIgnoredLexemes)
	{
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return UNKNOWN;
}

Token KeywordLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

	if (lexicalAnalyzerContext == NULL || lexicalAnalyzerContext->semanticValue == NULL)
	{
		fprintf(stderr, "Error: LexicalAnalyzerContext o semanticValue es NULL.\n");
		exit(EXIT_FAILURE);
	}

	// Reservar memoria para el string
	lexicalAnalyzerContext->semanticValue->string = malloc(lexicalAnalyzerContext->length + 1);
	if (lexicalAnalyzerContext->semanticValue->string == NULL)
	{
		fprintf(stderr, "Error: no se pudo asignar memoria para string.\n");
		exit(EXIT_FAILURE);
	}

	strncpy(lexicalAnalyzerContext->semanticValue->string, lexicalAnalyzerContext->lexeme, lexicalAnalyzerContext->length);
	lexicalAnalyzerContext->semanticValue->string[lexicalAnalyzerContext->length] = '\0';

	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return STRING;
}

Token IdentifierLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->id = calloc(1 + lexicalAnalyzerContext->length, sizeof(char));
	char *semanticValueId = lexicalAnalyzerContext->semanticValue->id;
	semanticValueId = strncpy(semanticValueId, lexicalAnalyzerContext->lexeme, lexicalAnalyzerContext->length);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return ID;
}
