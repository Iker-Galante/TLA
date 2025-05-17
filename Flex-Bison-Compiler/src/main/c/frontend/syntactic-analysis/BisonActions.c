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
    switch (type)
    {
    case PROGRAM_EMPTY:
    break;
    case PROGRAM_HEADER:
        program->header = header;
        break;
    case PROGRAM_FOOTER:
        program->footer = footer;
        break;
    case PROGRAM_BODY:
        program->body = body;
        break;
    case PROGRAM_HEADER_FOOTER:
        program->header = header;
        program->footer = footer;
        break;
    case PROGRAM_HEADER_BODY:
        program->header = header;
        program->body = body;
        break;
    case PROGRAM_FOOTER_BODY:
        program->footer = footer;
        program->body = body;
        break;
    case PROGRAM_HEADER_FOOTER_BODY:
        program->header = header;
        program->footer = footer;
        program->body = body;
        break;
    }
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
    switch (type)
    {
    case EXPRESSION_SIMPLE_EXPRESSION:
        expression->simpleExpression = simpleExpression;
        break;
    case EXPRESSION_COMPLEX_EXPRESSION:
        expression->complexExpression = complexExpression;
        break;
        case EXPRESSION_ID_SIMPLEEXPRESSION:
        expression->simpleExpression = simpleExpression;
        expression->id = id;
        break;
    case EXPRESSION_ID_COMPLEXEXPRESSION:
        expression->complexExpression = complexExpression;
        expression->id = id;
        break;
    case EXPRESSION_ID:
        expression->componentId = component;
        break;
        case EXPRESSION_STRING:
        expression->string = string;
        break;
    case EXPRESSION_COMPONENTE:
        expression->component = component;
        break;
    
    default:
        break;
    }
    expression->type = type;
    return expression;
}

SimpleExpression * SimpleExpressionSemanticAction(Text * text, Img * img, Title * title, Subtitle * subtitle, Link * link, SimpleExpressionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    SimpleExpression * simpleExpression = calloc(1, sizeof(SimpleExpression));
    switch (type)
    {
    case SEXPRESSION_TEXT:
        simpleExpression->text = text;
        break;
    case SEXPRESSION_IMG:
        simpleExpression->img = img;
        break;
        case SEXPRESSION_TITLE:
        simpleExpression->title = title;
        break;
        case SEXPRESSION_SUBTITLE:
        simpleExpression->subtitle = subtitle;
        break;
        case SEXPRESSION_LINK:
        simpleExpression->link = link;
        break;
    }
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
    switch (type)
    {
    case MODIFIER_COLOR_MOD:
        newModifier->color = color;
        newModifier->modifier = modifier;
        break;
    case MODIFIER_EMPTY:
    break;
    case MODIFIER_MODIFIER:
        newModifier->modifier = modifier;
        break;
    }
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

Link * LinkSemanticAction(Href * href, SimpleExpression * simpleExpression) {
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
    switch (type)
    {
    case FILAPPP_EXPRESSION:
        newFilaPPP->expression = expression;
        break;
        case FILAPPP_EXPRESSION_FILAPPP:
        newFilaPPP->expression = expression;
        newFilaPPP->filaPPP = filaPPP;
    break;
    }
    newFilaPPP->type = type;
    return newFilaPPP;
}

FilaNav * FilaNavSemanticAction(char * id, char * navName, FilaNav * filaNav, FilaNavType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaNav * newFilaNav = calloc(1, sizeof(FilaNav));
    switch (type)
    {
    case FILANAV_FILA_NAVEGADOR:
        newFilaNav->filaNav = filaNav;
        newFilaNav->id = id;
        newFilaNav->string = navName;
        break;
        case FILANAV_SIMPLE:
        newFilaNav->id = id;
        newFilaNav->string = navName;
        break;
    }
    newFilaNav->type = type;
    return newFilaNav;
}

FilaTabla * FilaTablaSemanticAction(ColumnaTabla * columnaTabla, FilaTabla * filaTabla, FilaTablaType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaTabla * newFilaTabla = calloc(1, sizeof(FilaTabla));
    switch (type)
    {
    case FILA_COL:
        newFilaTabla->columnaTabla = columnaTabla;

        break;
        case FILA_FILA_TABLA:
        newFilaTabla->columnaTabla = columnaTabla;
        newFilaTabla->filaTabla = filaTabla;
        break;
    
    default:
        break;
    }
    newFilaTabla->type = type;
    return newFilaTabla;
}

ColumnaTabla * ColumnaTablaSemanticAction(SimpleExpression * simpleExpression, ColumnaTabla * columnaTabla, ColumnaTablaType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ColumnaTabla * newColumnaTabla = calloc(1, sizeof(ColumnaTabla));
    switch (type)
    {
    case COLUMNA_COL:
        newColumnaTabla->expression = simpleExpression;
        newColumnaTabla->columnaTabla = columnaTabla;
        break;
    case COLUMNA_FIN_FILA:
        break;
    
    }
    newColumnaTabla->type = type;
    return newColumnaTabla;
}

Component * ComponentSemanticAction(char * id, Body * body, ComponentType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Component * component = calloc(1, sizeof(Component));
    switch (type)
    {
    case COMPONENT_COMPONENT:
        component->body = body;
        component->id = id;
        break;
    case COMPONENT_EMPTY:
        component->id = id;
        break;
    }
    component->type = type;
    return component;
}

ComplexExpression * ComplexExpressionSemanticAction(Seccion * seccion, Tabla * tabla, Navegador * navegador, PuntoPorPunto * puntoPorPunto, ComplexExpressionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ComplexExpression * complexExpression = calloc(1, sizeof(ComplexExpression));
    switch (type)
    {
    case CEXPRESSION_PUNTO_POR_PUNTO:
        complexExpression->puntoPorPunto = puntoPorPunto;
        break;
    case CEXPRESSION_SECCION:
        complexExpression->seccion = seccion;
        break;
    case CEXPRESSION_TABLA:
        complexExpression->tabla = tabla;
        break;
    case CEXPRESSION_NAVEGADOR:
        complexExpression->navegador = navegador;
        break;
    complexExpression->type = type;
    return complexExpression;
}
