#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Constant * IntegerConstantSemanticAction(const int value);
/*
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
*/
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
/*Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);*/

//TODO please check, not sure if this is correct.

Program * ProgramSemanticAction(Body * body, Header * header, Footer * footer, ProgramType type);
Header * HeaderSemanticAction(Body * body, HeaderFooterType type);
Body * BodySemanticAction(Expression * expression, Body * body, BodyType type);
Expression * ExpressionSemanticAction(char * id, char * string, ComplexExpression * complexExpression, SimpleExpression * simpleExpression, Component * component,ExpressionType type);
SimpleExpression * SimpleExpressionSemanticAction(Text * text, Img * img, Title * title, Subtitle * subtitle, Link * link, SimpleExpressionType type);
Text * TextSemanticAction(char * string, Modifier * modifier, TextType type);
Modifier * ModifierSemanticAction(Modifier * modifier, ColorType color, ModifierType type);
Img * ImgSemanticAction(char * url, char * alternative);
Title * TitleSemanticAction(char * string);
Subtitle * SubtitleSemanticAction(char * string);
Link * LinkSemanticAction(char * href, SimpleExpression * simpleExpression);
Href * HrefSemanticAction(char * url, char * id, HrefType type);
Seccion * SeccionSemanticAction(Body * body, SeccionType type);
Tabla * TablaSemanticAction(FilaTabla * filaTabla, TablaType type);
Navegador * NavegadorSemanticAction(FilaNav * filaNav, NavegadorType type);
PuntoPorPunto * PuntoPorPuntoSemanticAction(FilaPPP * filaPPP, PuntoPorPuntoType type);
FilaPPP * FilaPPPSemanticAction(Expression * expression,FilaPPP * filaPPP ,FilaPPPType type);
FilaNav * FilaNavSemanticAction(char * id, char * navName,FilaNav * filaNav, FilaNavType type);
FilaTabla * FilaTablaSemanticAction(ColumnaTabla * columnaTabla, FilaTabla * filaTabla, FilaTablaType type);
ColumnaTabla * ColumnaTablaSemanticAction(SimpleExpression * simpleExpression, ColumnaTabla * columnaTabla, ColumnaTablaType type);
Component * ComponentSemanticAction(char * id, Body * body, ComponentType type);
ComplexExpression * ComplexExpressionSemanticAction(Seccion * seccion, Tabla * tabla, Navegador * navegador, PuntoPorPunto * puntoPorPunto, ComplexExpressionType type);



#endif
