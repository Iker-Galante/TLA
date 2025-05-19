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

/*Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);*/

//TODO please check, not sure if this is correct.

Program * ProgramSemanticAction(Body * body, Header * header, Footer * footer, ProgramType type, CompilerState * compilerState);
Header * HeaderSemanticAction(Body * body, HeaderFooterType type);
Footer * FooterSemanticAction(Body * body, HeaderFooterType type);
Body * BodySemanticAction(Expression * expression, Body * body, BodyType type);
Expression * ExpressionSemanticAction(char * id, char * string, ComplexExpression * complexExpression, SimpleExpression * simpleExpression, Component * component,ExpressionType type);
SimpleExpression * SimpleExpressionSemanticAction(Text * text, Image * img, Title * title, Subtitle * subtitle, Link * link, SimpleExpressionType type);
Text * TextSemanticAction(char * string, Modifier * modifier, TextType type);
Modifier * ModifierSemanticAction(Modifier * modifier, ColorType color, ModifierType type);
Image * ImgSemanticAction(char * url, char * alternative);
Title * TitleSemanticAction(char * string);
Subtitle * SubtitleSemanticAction(char * string);
Link * LinkSemanticAction(Href * href, SimpleExpression * simpleExpression);
Href * HrefSemanticAction(char * url, char * id, HrefType type);
Seccion * SeccionSemanticAction(Body * body, SeccionType type);
Table * TablaSemanticAction(FilaTabla * filaTabla, TableType type);
Navegador * NavegadorSemanticAction(FilaNav * filaNav, NavegadorType type);
PuntoPorPunto * PuntoPorPuntoSemanticAction(FilaPPP * filaPPP, PuntoPorPuntoType type);
FilaPPP * FilaPPPSemanticAction(Expression * expression,FilaPPP * filaPPP ,FilaPPPType type);
FilaNav * FilaNavSemanticAction(char * id, char * navName,FilaNav * filaNav, FilaNavType type);
FilaTabla * FilaTablaSemanticAction(ColumnaTabla * columnaTabla, FilaTabla * filaTabla, FilaTablaType type);
ColumnaTabla * ColumnaTablaSemanticAction(SimpleExpression * simpleExpression, ColumnaTabla * columnaTabla, ColumnaTablaType type);
Component * ComponentSemanticAction(char * id, Body * body, ComponentType type);
ComplexExpression * ComplexExpressionSemanticAction(Seccion * seccion, Table * tabla, Navegador * navegador, PuntoPorPunto * puntoPorPunto, ComplexExpressionType type);



#endif
