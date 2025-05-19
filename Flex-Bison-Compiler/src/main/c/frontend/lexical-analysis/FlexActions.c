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

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
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

// aca tuve que hacerle un calloc al semanticValue->string xq sino explotaba
Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext)
{
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->string = calloc(1 + lexicalAnalyzerContext->length, sizeof(char));
	char *semanticValueString = lexicalAnalyzerContext->semanticValue->string;
	semanticValueString = strncpy(semanticValueString, lexicalAnalyzerContext->lexeme, lexicalAnalyzerContext->length);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return STRING;
}

Token IdentifierLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->string = calloc(1 + lexicalAnalyzerContext->length, sizeof(char));
	char *semanticValueString = lexicalAnalyzerContext->semanticValue->string;
	semanticValueString = strncpy(semanticValueString, lexicalAnalyzerContext->lexeme, lexicalAnalyzerContext->length);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return ID;
}
