#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */


Program * ProgramSemanticAction(Body * body, Header * header, Footer * footer, ProgramType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Program * program = calloc(1, sizeof(Program));
    program->body = body;
    program->header = header;
    program->footer = footer;
    program->type = type;
    return program;
}

Header * HeaderSemanticAction(Body * body, HeaderFooterType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Header * header = calloc(1, sizeof(Header));
    header->body = body;
    header->type = type;
    return header;
}

Footer * FooterSemanticAction(Body * body, HeaderFooterType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Footer * footer = calloc(1, sizeof(Footer));
    footer->body = body;
    footer->type = type;
    return footer;
}

Body * BodySemanticAction(Expression * expression, Body * body, BodyType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Body * newBody = calloc(1, sizeof(Body));
    newBody->expression = expression;
    newBody->body = body;
    newBody->type = type;
    return newBody;
}

Expression * ExpressionSemanticAction(char * id, char * string, ComplexExpression * complexExpression, SimpleExpression * simpleExpression, Component * component, ExpressionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = calloc(1, sizeof(Expression));
    expression->id = id;
    expression->string = string;
    expression->complexExpression = complexExpression;
    expression->simpleExpression = simpleExpression;
    expression->component = component;
    expression->type = type;
    return expression;
}

SimpleExpression * SimpleExpressionSemanticAction(Text * text, Img * img, Title * title, Subtitle * subtitle, Link * link, SimpleExpressionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    SimpleExpression * simpleExpression = calloc(1, sizeof(SimpleExpression));
    simpleExpression->text = text;
    simpleExpression->img = img;
    simpleExpression->title = title;
    simpleExpression->subtitle = subtitle;
    simpleExpression->link = link;
    simpleExpression->type = type;
    return simpleExpression;
}

Text * TextSemanticAction(char * string, Modifier * modifier, TextType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Text * text = calloc(1, sizeof(Text));
    text->string = string;
    text->modifier = modifier;
    text->type = type;
    return text;
}

Modifier * ModifierSemanticAction(Modifier * modifier, ColorType color, ModifierType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Modifier * newModifier = calloc(1, sizeof(Modifier));
    newModifier->modifier = modifier;
    newModifier->color = color;
    newModifier->type = type;
    return newModifier;
}

Img * ImgSemanticAction(char * url, char * alternative) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Img * img = calloc(1, sizeof(Img));
    img->url = url;
    img->alternative = alternative;
    return img;
}

Title * TitleSemanticAction(char * string) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Title * title = calloc(1, sizeof(Title));
    title->string = string;
    return title;
}

Subtitle * SubtitleSemanticAction(char * string) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Subtitle * subtitle = calloc(1, sizeof(Subtitle));
    subtitle->string = string;
    return subtitle;
}

Link * LinkSemanticAction(char * href, SimpleExpression * simpleExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Link * link = calloc(1, sizeof(Link));
    link->href = href;
    link->simpleExpression = simpleExpression;
    return link;
}

Href * HrefSemanticAction(char * url, char * id, HrefType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Href * href = calloc(1, sizeof(Href));
    href->url = url;
    href->id = id;
    href->type = type;
    return href;
}

Seccion * SeccionSemanticAction(Body * body, SeccionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Seccion * seccion = calloc(1, sizeof(Seccion));
    seccion->body = body;
    seccion->type = type;
    return seccion;
}

Tabla * TablaSemanticAction(FilaTabla * filaTabla, TablaType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Tabla * tabla = calloc(1, sizeof(Tabla));
    tabla->filaTabla = filaTabla;
    tabla->type = type;
    return tabla;
}

Navegador * NavegadorSemanticAction(FilaNav * filaNav, NavegadorType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Navegador * navegador = calloc(1, sizeof(Navegador));
    navegador->filaNav = filaNav;
    navegador->type = type;
    return navegador;
}

PuntoPorPunto * PuntoPorPuntoSemanticAction(FilaPPP * filaPPP, PuntoPorPuntoType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    PuntoPorPunto * puntoPorPunto = calloc(1, sizeof(PuntoPorPunto));
    puntoPorPunto->filaPPP = filaPPP;
    puntoPorPunto->type = type;
    return puntoPorPunto;
}

FilaPPP * FilaPPPSemanticAction(Expression * expression, FilaPPP * filaPPP, FilaPPPType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaPPP * newFilaPPP = calloc(1, sizeof(FilaPPP));
    newFilaPPP->expression = expression;
    newFilaPPP->filaPPP = filaPPP;
    newFilaPPP->type = type;
    return newFilaPPP;
}

FilaNav * FilaNavSemanticAction(char * id, char * navName, FilaNav * filaNav, FilaNavType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaNav * newFilaNav = calloc(1, sizeof(FilaNav));
    newFilaNav->id = id;
    newFilaNav->string = navName;
    newFilaNav->filaNav = filaNav;
    newFilaNav->type = type;
    return newFilaNav;
}

FilaTabla * FilaTablaSemanticAction(ColumnaTabla * columnaTabla, FilaTabla * filaTabla, FilaTablaType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaTabla * newFilaTabla = calloc(1, sizeof(FilaTabla));
    newFilaTabla->columnaTabla = columnaTabla;
    newFilaTabla->filaTabla = filaTabla;
    newFilaTabla->type = type;
    return newFilaTabla;
}

ColumnaTabla * ColumnaTablaSemanticAction(SimpleExpression * simpleExpression, ColumnaTabla * columnaTabla, ColumnaTablaType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ColumnaTabla * newColumnaTabla = calloc(1, sizeof(ColumnaTabla));
    newColumnaTabla->expression = simpleExpression;
    newColumnaTabla->columnaTabla = columnaTabla;
    newColumnaTabla->type = type;
    return newColumnaTabla;
}

Component * ComponentSemanticAction(char * id, Body * body, ComponentType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Component * component = calloc(1, sizeof(Component));
    component->id = id;
    component->body = body;
    component->type = type;
    return component;
}

ComplexExpression * ComplexExpressionSemanticAction(Seccion * seccion, Tabla * tabla, Navegador * navegador, PuntoPorPunto * puntoPorPunto, ComplexExpressionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ComplexExpression * complexExpression = calloc(1, sizeof(ComplexExpression));
    complexExpression->seccion = seccion;
    complexExpression->tabla = tabla;
    complexExpression->navegador = navegador;
    complexExpression->puntoPorPunto = puntoPorPunto;
    complexExpression->type = type;
    return complexExpression;
}
