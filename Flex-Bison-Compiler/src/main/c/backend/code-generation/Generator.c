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


	_generateProgram(program);

	// seguimos con el programa
	// el resto de los generates...

	// Si todo salio bien, cerramos el archivo
	fprintf(f, "</html>\n");
	fclose(f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static const char _expressionTypeToCharacter(const ExpressionType type);
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
// static const char _expressionTypeToCharacter(const ExpressionType type) {
// 	switch (type) {
// 		case ADDITION: return '+';
// 		case DIVISION: return '/';
// 		case MULTIPLICATION: return '*';
// 		case SUBTRACTION: return '-';
// 		default:
// 			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
// 			return '\0';
// 	}
// }

/**
 * Generates the output of a constant.
 */
//  

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s",
		"</body>\n"
		"</html>\n\n"
	);
}

static void _generateHeader(const unsigned int indentationLevel, Header * header){
	_output(indentationLevel, "%s", "<header>\n");
	if(header->type == HEADER_BODY){
		_generateBody(1 + indentationLevel, header->body);
	}
	_output(indentationLevel, "%s", "</header>\n");
}

static void _generateFooter(const unsigned int indentationLevel, Footer * footer){
	_output(indentationLevel, "%s", "<footer>\n");
	if(footer->type == HEADER_BODY){
		_generateBody(1 + indentationLevel, footer->body);
	}
	_output(indentationLevel, "%s", "</footer>\n");
}

static void _generateBody(const unsigned int indentationLevel, Body * body){
	_output(indentationLevel, "%s", "<body>\n");
	if(body->type == BODY_EXPRESSION_BODY){
		_generateExpression(1 + indentationLevel, body->expressionB);
		_generateBody(1 + indentationLevel, body->bodyB);
	}
	else if(body->type == BODY_EXPRESSION){
		_generateExpression(1 + indentationLevel, body->expression);
	}
	_output(indentationLevel, "%s", "</body>\n");
}

static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	switch (expression->type) {
		case EXPRESSION_SIMPLE_EXPRESSION:
			_generateSimpleExpression(1 + indentationLevel, expression->simpleExpression);
			break;
		case EXPRESSION_COMPLEX_EXPRESSION:
			_generateComplexExpression(1 + indentationLevel, expression->complexExpression);
			break;
		case EXPRESSION_COMPONENTE:
			_generateComponent(1 + indentationLevel, expression->component);
			break;
		case EXPRESSION_STRING:
			_generateString(1 + indentationLevel, expression->string); //Es así?
			break;
		case EXPRESSION_ID:
			_generateComponentId(1 + indentationLevel, expression->componentId);
			break;
		case EXPRESSION_ID_SIMPLEEXPRESSION:
			_generateSimpleExpressionId(1 + indentationLevel, expression->simpleExpression,expression->simpleId);
			break;
		case EXPRESSION_ID_COMPLEXEXPRESSION:
			_generateComplexExpressionId(1 + indentationLevel, expression->complexExpression, expression->complexId);
			break;
		default:
			logError(_logger, "Unknown expression type: %d", expression->type);
			break;
	}
}

static void _generateSimpleExpression(const unsigned int indentationLevel, SimpleExpression * simpleExpression) {
	switch (simpleExpression->type) {
		case SEXPRESSION_TEXT:
			_generateText(1 + indentationLevel, simpleExpression->text);
			break;
		case SEXPRESSION_IMG:
			_generateImage(1 + indentationLevel, simpleExpression->img);
			break;
		case SEXPRESSION_TITLE:
			_generateTitle(1 + indentationLevel, simpleExpression->title);
			break;
		case SEXPRESSION_SUBTITLE:
			_generateSubtitle(1 + indentationLevel, simpleExpression->subtitle);
			break;
		case SEXPRESSION_LINK:
			_generateLink(1 + indentationLevel, simpleExpression->link);
			break;
		default:
			logError(_logger, "Unknown simple expression type: %d", simpleExpression->type);
			break;
	}
}

static void _generateImage(const unsigned int indentationLevel, Image * img) {
    _output(indentationLevel, "<img src=\"%s\" alt=\"%s\">\n", img->url, img->alternative);
}


static void _generateTitle(const unsigned int indentationLevel, Title * title) {
	_output(indentationLevel, "<h1>%s</h1>\n", title->string);
}

static void _generateLink(const unsigned int indentationLevel, Link * link) {
	_output(indentationLevel, "<a href=\"%s\">", link->href->url);
	_generateSimpleExpression(1 + indentationLevel, link->simpleExpression);
	_output(indentationLevel, "</a>\n");
}

static void _generateSubtitle(const unsigned int indentationLevel, Subtitle * subtitle) {
	_output(indentationLevel, "%s", "<h2>");
	_output(1 + indentationLevel, "%s", subtitle->string);
	_output(indentationLevel, "%s", "</h2>\n");
}

//TODO Mancio tema CSS
static void _generateText(const unsigned int indentationLevel, Text * text) {
	switch (text->type) {
		case TEXT_MODIFIED_TEXT:
			_generateModifiedText(1 + indentationLevel, text->modifier);
			break;
		case TEXT_SIMPLE_TEXT:
			_generateSimpleText(1 + indentationLevel, text->string);
			break;
		default:
			logError(_logger, "Unknown text type: %d", text->type);
			break;
	}
}

static void _generateModifiedText(const unsigned int indentationLevel, Modifier * modifier) {

}

/**
 * Generates the output of an expression.
 */
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
// 	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
// 	switch (expression->type) {
// 		case ADDITION:
// 		case DIVISION:
// 		case MULTIPLICATION:
// 		case SUBTRACTION:
// 			_generateExpression(1 + indentationLevel, expression->leftExpression);
// 			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, expression->rightExpression);
// 			break;
// 		case FACTOR:
// 			_generateFactor(1 + indentationLevel, expression->factor);
// 			break;
// 		default:
// 			logError(_logger, "The specified expression type is unknown: %d", expression->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
}

/**
 * Generates the output of a factor.
 */
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
// 	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
// 	switch (factor->type) {
// 		case CONSTANT:
// 			_generateConstant(1 + indentationLevel, factor->constant);
// 			break;
// 		case EXPRESSION:
// 			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, factor->expression);
// 			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
// 			break;
// 		default:
// 			logError(_logger, "The specified factor type is unknown: %d", factor->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
	switch (program->type)
	{
	case PROGRAM_EMPTY:
		return;
		break;
	case PROGRAM_HEADER_FOOTER_BODY:
		_generateHeader(1,program->headerFull);
		_generateBody(1,program->bodyFull);
		_generateFooter(1,program->footerFull);
		break;

	case PROGRAM_HEADER_FOOTER:

		break;
	case PROGRAM_HEADER_BODY:
		break;
	
	case PROGRAM_HEADER:
		break;
	
	case PROGRAM_FOOTER_BODY:
		break;
	
	case PROGRAM_FOOTER:

		break;

	case PROGRAM_BODY:
		break;
	}
	
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s",
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"    <title>Generated HTML</title>\n"
		"</head>\n"
		"<body>\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
