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

static void _generateProgram(Program * program);
static void _generateHeader(const unsigned int indentationLevel, Header * header);
static void _generateFooter(const unsigned int indentationLevel, Footer * footer);
static void _generateBody(const unsigned int indentationLevel, Body * body);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateSimpleExpression(const unsigned int indentationLevel, SimpleExpression * simpleExpression);
static void _generateSimpleExpressionId(const unsigned int indentationLevel, SimpleExpression * simpleExpression, const char * simpleId);
static void _generateComplexExpressionId(const unsigned int indentationLevel, ComplexExpression * complexExpression, const char * complexId);
static void _generateComplexExpression(const unsigned int indentationLevel, ComplexExpression * complexExpression);
static void _generateComponent(const unsigned int indentationLevel, Component * component);
static void _generateComponentId(const unsigned int indentationLevel, const char * componentId);
static void _generateString(const unsigned int indentationLevel, const char * string);
static void _generatePrologue(void);
static void _generateEpilogue(const int value);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static void _generateLink(const unsigned int indentationLevel, Link * link);
static void _generateText(const unsigned int indentationLevel, Text * text);
static void _generateImage(const unsigned int indentationLevel, Image * image);
static void _generateTitle(const unsigned int indentationLevel, Title * title);
static void _generateSubtitle(const unsigned int indentationLevel, Subtitle * subtitle);
static void _generateHref(const unsigned int indentationLevel, Href * href);
static void _generateModifier(const unsigned int indentationLevel, Modifier * modifier);
static void _generateModifiedText(const unsigned int indentationLevel, Modifier * modifier);
static void _generatePPP(const unsigned int indentationLevel, PuntoPorPunto * puntoPorPunto);
static void _generateSection(const unsigned int indentationLevel, Seccion * seccion);
static void _generateTable(const unsigned int indentationLevel, Table * table);
static void _generateNavigator(const unsigned int indentationLevel, Navegador * navegador);
static void _generateRowPPP(const unsigned int indentationLevel, FilaPPP * filaPPP);
static void _generateRow(const unsigned int indentationLevel, FilaTabla * filaTabla);
static void _generateSimpleText(const unsigned int indentationLevel, const char * string);
static void _generateRowNav(const unsigned int indentationLevel, FilaNav * filaNav);
static void _generateColumn(const unsigned int indentationLevel, ColumnaTabla * columnaTabla);



#endif
