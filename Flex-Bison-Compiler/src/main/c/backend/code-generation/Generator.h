#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>





/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
void generate(CompilerState * compilerState);

static void _generateProgram(Program * program, FILE* output);
static void _generateCSS(FILE* css);

static void _generateHeader(const unsigned int indentationLevel, Header * header, FILE* output);
static void _generateFooter(const unsigned int indentationLevel, Footer * footer, FILE* output);
static void _generateBody(const unsigned int indentationLevel, Body * body, FILE* output);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression, FILE* output);
static void _generateSimpleExpression(const unsigned int indentationLevel, SimpleExpression * simpleExpression, FILE* output);
static void _generateSimpleExpressionId(const unsigned int indentationLevel, SimpleExpression * simpleExpression, const char * simpleId, FILE* output);
static void _generateComplexExpressionId(const unsigned int indentationLevel, ComplexExpression * complexExpression, const char * complexId, FILE* output);
static void _generateComplexExpression(const unsigned int indentationLevel, ComplexExpression * complexExpression, FILE* output);
static void _generateComponent(const unsigned int indentationLevel, Component * component, FILE* output);
static void _generateComponentId(const unsigned int indentationLevel, const char * componentId, FILE* output);
static void _generateString(const unsigned int indentationLevel, const char * string, FILE* output);
static void _generatePrologue(FILE* output);
static void _generateEpilogue(FILE* output);
static void _output(FILE* output,const unsigned int indentationLevel, const char * const format, ...);
static char * _outputToString(const unsigned int indentationLevel, const char * const format, ...);
static void _generateLink(const unsigned int indentationLevel, Link * link, FILE* output);
static void _generateText(const unsigned int indentationLevel, Text * text, FILE* output);
static void _generateImage(const unsigned int indentationLevel, Image * image, FILE* output);
static void _generateTitle(const unsigned int indentationLevel, Title * title, FILE* output);
static void _generateSubtitle(const unsigned int indentationLevel, Subtitle * subtitle, FILE* output);
static void _generateHref(const unsigned int indentationLevel, Href * href, FILE* output);
static void _generateModifier(const unsigned int indentationLevel, Modifier * modifier, FILE* output);
static void _generateModifiedText(const unsigned int indentationLevel, Modifier * modifier, FILE* output);
static void _generatePPP(const unsigned int indentationLevel, PuntoPorPunto * puntoPorPunto, FILE* output);
static void _generateSection(const unsigned int indentationLevel, Seccion * seccion, FILE* output);
static void _generateTable(const unsigned int indentationLevel, Table * table, FILE* output);
static void _generateNavigator(const unsigned int indentationLevel, Navegador * navegador, FILE* output);
static void _generateRowPPP(const unsigned int indentationLevel, FilaPPP * filaPPP, FILE* output);
static void _generateRow(const unsigned int indentationLevel, FilaTabla * filaTabla, FILE* output);
static void _generateSimpleText(const unsigned int indentationLevel, const char * string, FILE* output);
static void _generateRowNav(const unsigned int indentationLevel, FilaNav * filaNav, FILE* output);
static void _generateColumn(const unsigned int indentationLevel, ColumnaTabla * columnaTabla, FILE* output);
static char* _generateProgramAsString(Program* program);
static char* _generateHeaderAsString(const unsigned int indentationLevel, Header* header);
static char* _generateFooterAsString(const unsigned int indentationLevel, Footer* footer);
static char* _generateBodyAsString(const unsigned int indentationLevel, Body* body);
static char* _generateExpressionAsString(const unsigned int indentationLevel, Expression* expression);
static char* _generateSimpleExpressionAsString(const unsigned int indentationLevel, SimpleExpression* simpleExpression);
static char* _generateImageAsString(const unsigned int indentationLevel, Image* img);
static char* _generateTitleAsString(const unsigned int indentationLevel, Title* title);
static char* _generateSubtitleAsString(const unsigned int indentationLevel, Subtitle* subtitle);
static char* _generateTextAsString(const unsigned int indentationLevel, Text* text);
static char* _generateModifiedTextAsString(const unsigned int indentationLevel, Modifier* modifier);
static char* _generateSimpleTextAsString(const unsigned int indentationLevel, const char* string);
static char* _generateLinkAsString(const unsigned int indentationLevel, Link* link);
static char* _generateComplexExpressionAsString(const unsigned int indentationLevel, ComplexExpression* complexExpression);
static char* _generateComponentAsString(const unsigned int indentationLevel, Component* component);
static char* _generateComponentIdAsString(const unsigned int indentationLevel, const char* componentId);
static char* _generateStringAsString(const unsigned int indentationLevel, const char* string);
static char* _generateSectionAsString(const unsigned int indentationLevel, Seccion* section);
static char* _generateTableAsString(const unsigned int indentationLevel, Table* table);
static char* _generateNavigatorAsString(const unsigned int indentationLevel, Navegador* nav);
static char* _generatePPPAsString(const unsigned int indentationLevel, PuntoPorPunto* ppp);
static char* _generateSimpleExpressionIdAsString(const unsigned int indentationLevel, SimpleExpression* simpleExpression, const char* simpleId);
static char* _generateComplexExpressionIdAsString(const unsigned int indentationLevel, ComplexExpression* complexExpression, const char* complexId);
static char* _generateRowAsString(const unsigned int indentationLevel, FilaTabla* tableRow);
static char* _generateColumnAsString(const unsigned int indentationLevel, ColumnaTabla* columnaTabla);
static char* _generateRowNavAsString(const unsigned int indentationLevel, FilaNav* navRow);
static char* _generateRowPPPAsString(const unsigned int indentationLevel, FilaPPP* pppRow);



#endif
