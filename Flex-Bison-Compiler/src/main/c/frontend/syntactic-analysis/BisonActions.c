#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

void initializeBisonActionsModule()
{
    _logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule()
{
    if (_logger != NULL)
    {
        destroyLogger(_logger);
    }
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char *functionName);
static boolean checkIdExistanceAndAddToSymbolTable(char *id, idType type);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char *functionName)
{
    logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Program *ProgramSemanticAction(Body *body, Header *header, Footer *footer, ProgramType type, CompilerState *compilerState)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Program *program = calloc(1, sizeof(Program));
    compilerState->abstractSyntaxtTree = program;
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
        program->headerHF = header;
        program->footerHF = footer;
        break;
    case PROGRAM_HEADER_BODY:
        program->headerHB = header;
        program->bodyHB = body;
        break;
    case PROGRAM_FOOTER_BODY:
        program->footerFB = footer;
        program->bodyFB = body;
        break;
    case PROGRAM_HEADER_FOOTER_BODY:
        program->headerFull = header;
        program->footerFull = footer;
        program->bodyFull = body;
        break;
    }
    program->type = type;
    if (0 < flexCurrentContext())
    {
        logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
        compilerState->succeed = false;
    }
    else
    {
        compilerState->succeed = true;
    }
    return program;
}

Header *HeaderSemanticAction(Body *body, HeaderFooterType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Header *header = calloc(1, sizeof(Header));
    header->body = body;
    header->type = type;
    return header;
}

Footer *FooterSemanticAction(Body *body, HeaderFooterType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Footer *footer = calloc(1, sizeof(Footer));
    footer->body = body;
    footer->type = type;
    return footer;
}

Body *BodySemanticAction(Expression *expression, Body *body, BodyType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Body *newBody = calloc(1, sizeof(Body));
    switch (type)
    {
    case BODY_EXPRESSION:
        newBody->expression = expression;
        break;
    case BODY_EXPRESSION_BODY:
        newBody->expressionB = expression;
        newBody->bodyB = body;
        break;
    case BODY_EMPTY:
        break;
    default:
        logError(_logger, "Unknown body type: %d", type);
        break;
    }

    return newBody;
}

Expression *ExpressionSemanticAction(char *id, char *string, ComplexExpression *complexExpression, SimpleExpression *simpleExpression, Component *component, ExpressionType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression *expression = calloc(1, sizeof(Expression));
    switch (type)
    {
    case EXPRESSION_SIMPLE_EXPRESSION:
        expression->simpleExpression = simpleExpression;
        break;
    case EXPRESSION_COMPLEX_EXPRESSION:
        expression->complexExpression = complexExpression;
        break;
    case EXPRESSION_ID_SIMPLEEXPRESSION:
        expression->simpleExpressionId = simpleExpression;
        if(checkIdExistanceAndAddToSymbolTable(id, SIMPLE_ID))
        {
            expression->simpleId = id;
        }
        else
        {
            expression->simpleId = "UNDEFINED"; 
            logError(_logger, "Attempting to redefine ID '%s'.", id);
        }
        break;
    case EXPRESSION_ID_COMPLEXEXPRESSION:
        expression->complexExpressionId = complexExpression;
        if(checkIdExistanceAndAddToSymbolTable(id, SIMPLE_ID))
        {
            expression->complexId = id;
        }
        else
        {
            expression->complexId = "UNDEFINED"; 
            logError(_logger, "Attempting to redefine ID '%s'.", id);
        }
        break;
    
    case EXPRESSION_ID:
        expression->componentId = id;
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
static boolean checkIdExistanceAndAddSimpleIdToSymbolTable(char *id, idType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);

    ///TODO me acabo de dar cuenta que enrealidad vamos a tener que por un lado
    ///vamos a tener los id de los divs o expresiones, y por el otro los ids
    ///del los componentes reutilizables, asi que hay que chequear el tipo de alguna manera
    if (g_hash_table_contains(currentCompilerState()->symbolTable, id))
    {
        logError(_logger, "The identifier '%s' already exists in the symbol table.", id);
        return false;
    }
    else
    {
        //creo un SymbolTableEntry y asigno el typo si es un componente o no
        SymbolTableEntry* entry = malloc(sizeof(SymbolTableEntry));
        entry->type = type; 

        g_hash_table_insert(currentCompilerState()->symbolTable, g_strdup(id), entry);
    }
    return true;
}

static boolean checkIdExistanceAndAddComponentToSymbolTable(char * id)
{
     if (g_hash_table_contains(currentCompilerState()->symbolTable, id))
    {
        logError(_logger, "The identifier '%s' already exists in the symbol table.", id);
    }
}

SimpleExpression *SimpleExpressionSemanticAction(Text *text, Image *img, Title *title, Subtitle *subtitle, Link *link, SimpleExpressionType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    SimpleExpression *simpleExpression = calloc(1, sizeof(SimpleExpression));
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

Text *TextSemanticAction(char *string, Modifier *modifier, TextType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Text *text = calloc(1, sizeof(Text));
    /// capaz sirve que me pases tmb la longitud del string para ahorrar un strlen
    // text->string = calloc(1, sizeof(char) * (strlen(string) + 1));
    text->string = string;
    text->modifier = modifier;
    text->type = type;
    return text;
}

Modifier *ModifierSemanticAction(Modifier *modifier, ColorType color, ModifierType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Modifier *newModifier = calloc(1, sizeof(Modifier));
    switch (type)
    {
    case MODIFIER_COLOR_MOD:
        newModifier->color = color;
        newModifier->modifierWithColor = modifier;
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

Image *ImgSemanticAction(char *url, char *alternative)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Image *img = calloc(1, sizeof(Image));
    img->url = url;
    img->alternative = alternative;
    return img;
}

Title *TitleSemanticAction(char *string)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Title *title = calloc(1, sizeof(Title));
    title->string = string;
    return title;
}

Subtitle *SubtitleSemanticAction(char *string)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Subtitle *subtitle = calloc(1, sizeof(Subtitle));
    subtitle->string = string;
    return subtitle;
}

Link *LinkSemanticAction(Href *href, SimpleExpression *simpleExpression)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Link *link = calloc(1, sizeof(Link));
    link->href = href;
    link->simpleExpression = simpleExpression;
    return link;
}

Href *HrefSemanticAction(char *url, char *id, HrefType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Href *href = calloc(1, sizeof(Href));
    href->url = url;
    href->id = id;
    href->type = type;
    return href;
}

Seccion *SeccionSemanticAction(Body *body, SeccionType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Seccion *seccion = calloc(1, sizeof(Seccion));
    seccion->body = body;
    seccion->type = type;
    return seccion;
}

Table *TablaSemanticAction(FilaTabla *filaTabla, TableType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Table *tabla = calloc(1, sizeof(Table));
    tabla->filaTabla = filaTabla;
    tabla->type = type;
    return tabla;
}

Navegador *NavegadorSemanticAction(FilaNav *filaNav, NavegadorType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Navegador *navegador = calloc(1, sizeof(Navegador));
    navegador->filaNav = filaNav;
    navegador->type = type;
    return navegador;
}

PuntoPorPunto *PuntoPorPuntoSemanticAction(FilaPPP *filaPPP, PuntoPorPuntoType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    PuntoPorPunto *puntoPorPunto = calloc(1, sizeof(PuntoPorPunto));
    puntoPorPunto->filaPPP = filaPPP;
    puntoPorPunto->type = type;
    return puntoPorPunto;
}

FilaPPP *FilaPPPSemanticAction(Expression *expression, FilaPPP *filaPPP, FilaPPPType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaPPP *newFilaPPP = calloc(1, sizeof(FilaPPP));
    switch (type)
    {
    case FILAPPP_EXPRESSION:
        newFilaPPP->expression = expression;
        break;
    case FILAPPP_EXPRESSION_FILAPPP:
        newFilaPPP->expressionFila = expression;
        newFilaPPP->filaPPP = filaPPP;
        break;
    }
    newFilaPPP->type = type;
    return newFilaPPP;
}

FilaNav *FilaNavSemanticAction(char *id, char *navName, FilaNav *filaNav, FilaNavType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaNav *newFilaNav = calloc(1, sizeof(FilaNav));
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

FilaTabla *FilaTablaSemanticAction(ColumnaTabla *columnaTabla, FilaTabla *filaTabla, FilaTablaType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FilaTabla *newFilaTabla = calloc(1, sizeof(FilaTabla));
    switch (type)
    {
    case FILA_COL:
        newFilaTabla->columnaTabla = columnaTabla;

        break;
    case FILA_FILA_TABLA:
        newFilaTabla->columnaTablaConFila = columnaTabla;
        newFilaTabla->filaTabla = filaTabla;
        break;

    default:
        break;
    }
    newFilaTabla->type = type;
    return newFilaTabla;
}

ColumnaTabla *ColumnaTablaSemanticAction(SimpleExpression *simpleExpression, ColumnaTabla *columnaTabla, ColumnaTablaType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ColumnaTabla *newColumnaTabla = calloc(1, sizeof(ColumnaTabla));
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

Component *ComponentSemanticAction(char *id, Body *body, ComponentType type)
{
   
    ///Current compilerState es una funcion de SyntacticAnalyzer.h el compiler state esta hecho para que
    ///todas las fases puedan acceder a el. ahi guardo la tabla de simbolos que la van a poder ver todos.
    ///Se llena en bison al ver la gramatica.


    GHashTable * symbolTable = currentCompilerState()->symbolTable;
    if(!g_hash_table_contains(symbolTable, id))
    {
        SymbolTableEntry* entry = malloc(sizeof(SymbolTableEntry));
        entry->type = COMPONENT_ID;
        ///TODO 
        ///Yo aca estoy metiendo en la tabla de simbolos una entrada que va a ser de componente y 
        ///va a estar vacia. por ahora mi idea es que cuando el back lea este componente, que venga a la entrada
        ///de la tabla y que me escriba el html del componente y la meta en la SymbolTableEntry.
        ///la otra opcion es meter el componente entero ahora en la tabla y que dsp el back cada vez que lo 
        ///lea lo tiene que pasar a su html correspondiente.
        ///me parece que la primera opcion es mejor.

        g_hash_table_insert(symbolTable, g_strdup(id), entry);
        
    }
    else
    {
        logError(_logger, "Component with id '%s' is already defined.", id);
    }
    
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Component *component = calloc(1, sizeof(Component));
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

ComplexExpression *ComplexExpressionSemanticAction(Seccion *seccion, Table *tabla, Navegador *navegador, PuntoPorPunto *puntoPorPunto, ComplexExpressionType type)
{
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ComplexExpression *complexExpression = calloc(1, sizeof(ComplexExpression));
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
}