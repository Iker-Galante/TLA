#include "Generator.h"

#include "../../frontend/syntactic-analysis/SyntacticAnalyzer.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 2;
static Logger *_logger = NULL;
static CompilerState * _currentCompilerState = NULL;

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

CompilerState * backendCompilerState()
{
	return _currentCompilerState;
}

void checkUndeclaredIdsAndLog(GPtrArray *undeclaredSymbols) {
	if (undeclaredSymbols == NULL || undeclaredSymbols->len == 0) {
		return;
	}

	GHashTable *symbolTable = _currentCompilerState->symbolTable;
	for (unsigned int i = 0; i < undeclaredSymbols->len; i++) {
		char *id = g_ptr_array_index(undeclaredSymbols, i);

		if (!g_hash_table_contains(symbolTable, id)) {
			logWarning(_logger, "Undeclared identifier '%s' used", id);
		}
	}
}

/** PUBLIC FUNCTIONS */



/** PRIVATE FUNCTIONS */

static void _generateEpilogue(FILE* output);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression, FILE* output);
static void _generateProgram(Program * program, FILE* output);
static void _generatePrologue(FILE* output);
static char * _indentation(const unsigned int indentationLevel);
static void _output(FILE* output,const unsigned int indentationLevel, const char * const format, ...);

static void _generateHeader(const unsigned int indentationLevel, Header * header, FILE* output){
	_output(output,indentationLevel, "%s", "<header>\n");
	if(header->type == HEADER_BODY){
		_generateBody(1 + indentationLevel, header->body, output);
	}
	_output(output,indentationLevel, "%s", "</header>\n");
}

static void _generateFooter(const unsigned int indentationLevel, Footer * footer, FILE* output){
	_output(output,indentationLevel, "%s", "<footer>\n");
	if(footer->type == HEADER_BODY){
		_generateBody(1 + indentationLevel, footer->body, output);
	}
	_output(output,indentationLevel, "%s", "</footer>\n");
}

static void _generateBody(const unsigned int indentationLevel, Body * body, FILE* output){
	if(body->type == BODY_EXPRESSION_BODY){
		_generateExpression(indentationLevel, body->expressionB,output);
		_generateBody(indentationLevel, body->bodyB,output);
	}
	else if(body->type == BODY_EXPRESSION){
		_generateExpression(indentationLevel, body->expression,output);
	}
}

static void _generateExpression(const unsigned int indentationLevel, Expression * expression, FILE* output) {
	switch (expression->type) {
		case EXPRESSION_SIMPLE_EXPRESSION:
			_generateSimpleExpression(indentationLevel, expression->simpleExpression,output);
			break;
		case EXPRESSION_COMPLEX_EXPRESSION:
			_generateComplexExpression(indentationLevel, expression->complexExpression, output);
			break;
		case EXPRESSION_COMPONENTE:
			_generateComponent(indentationLevel, expression->component, output);
			break;
		case EXPRESSION_STRING:
			_generateString(indentationLevel, expression->string, output);
			break;
		case EXPRESSION_ID:
			_generateComponentId(indentationLevel, expression->componentId, output);
			break;
		case EXPRESSION_ID_SIMPLEEXPRESSION:
			_generateSimpleExpressionId(indentationLevel, expression->simpleExpression,expression->simpleId, output);
			break;
		case EXPRESSION_ID_COMPLEXEXPRESSION:
			_generateComplexExpressionId(indentationLevel, expression->complexExpression, expression->complexId, output);
			break;
		default:
			logError(_logger, "Unknown expression type: %d", expression->type);
			break;
	}
}

static void _generateSimpleExpression(const unsigned int indentationLevel, SimpleExpression * simpleExpression, FILE* output) {
	switch (simpleExpression->type) {
		case SEXPRESSION_TEXT:
			_generateText(indentationLevel, simpleExpression->text, output);
			break;
		case SEXPRESSION_IMG:
			_generateImage(indentationLevel, simpleExpression->img, output);
			break;
		case SEXPRESSION_TITLE:
			_generateTitle(indentationLevel, simpleExpression->title, output);
			break;
		case SEXPRESSION_SUBTITLE:
			_generateSubtitle(indentationLevel, simpleExpression->subtitle, output);
			break;
		case SEXPRESSION_LINK:
			_generateLink(indentationLevel, simpleExpression->link, output);
			break;
		default:
			logError(_logger, "Unknown simple expression type: %d", simpleExpression->type);
			break;
	}
}

static void _generateImage(const unsigned int indentationLevel, Image * img, FILE* output) {
	if(img->alternative == NULL){
    _output(output,indentationLevel, "<img src=\"%s\"/>\n", img->url);
	} else {
	_output(output,indentationLevel, "<img src=\"%s\" alt=\"%s\"/>\n", img->url, img->alternative);
	}
}

static void _generateTitle(const unsigned int indentationLevel, Title * title, FILE* output) {
	_output(output,indentationLevel, "<h1>%s</h1>\n", title->string);
}


static void _generateLink(const unsigned int indentationLevel, Link *link, FILE* output) {
    if (!link || !link->href) return;

    const char *hrefValue = "";
    if (link->href->type == HREF_URL) {
        hrefValue = link->href->url;
        _output(output,indentationLevel, "<a href=\"%s\">\n", hrefValue);
    } else if (link->href->type == HREF_ID) {
        _output(output,indentationLevel, "<a href=\"#%s\">\n", link->href->id);
    } else {
        _output(output,indentationLevel, "<a>");
    }
	
	if (link->text) {
		_output(output,indentationLevel + 1, "%s", link->text);
	} else {
    	_generateSimpleExpression(indentationLevel + 1, link->simpleExpression,output);
	}
    

    _output(output,indentationLevel, "</a>\n");
}

static void _generateSubtitle(const unsigned int indentationLevel, Subtitle * subtitle, FILE* output) {
	_output(output,indentationLevel, "<h2>%s</h2>\n", subtitle->string);
}

//TODO ESTOS INLINE PONELES CSS

static void _generateText(const unsigned int indentationLevel, Text * text, FILE* output) {
	switch (text->type) {
		case TEXT_MODIFIED_TEXT:
			_output(output,indentationLevel, "%s", "<div style=\""); // Open a span for modified text
			_generateModifiedText(0, text->modifier,output);
			_output(output,indentationLevel, "%s", "\">\n"); // Close the style attribute
			_generateSimpleText(indentationLevel + 1, text->string,output);
			_output(output,indentationLevel, "%s", "</div>\n"); // Close the span opened in _generateModifiedText
			break;
		case TEXT_SIMPLE_TEXT:
			_generateSimpleText(indentationLevel, text->string,output);
			break;
		default:
			logError(_logger, "Unknown text type: %d", text->type);
			break;
	}
}

//TODO: FIJATE SI PODES OBVIAR EL NULL
static void _generateModifiedText(const unsigned int indentationLevel, Modifier * modifier, FILE* output) {
	if (modifier == NULL) {
		return;
	}
	switch (modifier->type) {
		case MODIFIER_COLOR_MOD:
			switch (modifier->color)
			{
			case COLOR_RED:
				_output(output,indentationLevel, "color: red; ");
				break;
			case COLOR_GREEN:
				_output(output,indentationLevel, "color: green; ");
				break;
			case COLOR_BLUE:
				_output(output,indentationLevel, "color: blue; ");
				break;
			case COLOR_YELLOW:
				_output(output,indentationLevel, "color: yellow; ");
				break;
			case COLOR_ORANGE:
				_output(output,indentationLevel, "color: orange;");
				break;
			default:
				break;
			}
			_generateModifiedText(indentationLevel, modifier->modifierWithColor,output);
			break;
		case MODIFIER_EMPTY:
			// No action needed for empty modifier
			break;
		case MODIFIER_MODIFIER:
			switch (modifier->style)
			{
			case UNDERLINE:
				_output(output,indentationLevel, "text-decoration: underline; ");
				break;
			case BOLD:
				_output(output,indentationLevel, "font-weight: bold; ");
				break;
			case ITALIC:
				_output(output,indentationLevel, "font-style: italic; ");
				break;
			case BIG:
				_output(output,indentationLevel, "font-size: larger; ");
				break;
			case TINY:
				_output(output,indentationLevel, "font-size: smaller; ");
				break;
			case MEDIUM:
				_output(output,indentationLevel, "font-size: medium; ");
				break;
			default:
				logError(_logger, "Unknown style type: %d", modifier->style);
				return; // Exit if an unknown style is encountered
			}
			_generateModifiedText(indentationLevel, modifier->modifier,output);
			break;
		default:
			logError(_logger, "Unknown modifier type: %d", modifier->type);
			break;
	}

}

static void _generateSimpleText(const unsigned int indentationLevel, const char * string, FILE* output) {
	_output(output,indentationLevel, "<p>%s</p>\n", string);
}

static void _generateComplexExpression(const unsigned int indentationLevel, ComplexExpression * complexExpression, FILE* output) {
	switch (complexExpression->type) {
		case CEXPRESSION_SECCION:
			_generateSection(indentationLevel, complexExpression->seccion,output);
			break;
		case CEXPRESSION_TABLA:
			_generateTable(indentationLevel, complexExpression->tabla,output);
			break;
		case CEXPRESSION_NAVEGADOR:
			_generateNavigator(indentationLevel, complexExpression->navegador,output);
			break;
		case CEXPRESSION_PUNTO_POR_PUNTO:
			_generatePPP(indentationLevel, complexExpression->puntoPorPunto,output);
			break;
		default:
			logError(_logger, "Unknown complex expression type: %d", complexExpression->type);
			break;
	}
}


static void _generateSection(const unsigned int indentationLevel, Seccion * section, FILE* output) {
	_output(output,indentationLevel, "%s", "<div>\n");
	if (section->type == SECCION_BODY) {
		_generateBody(1 + indentationLevel, section->body,output);
	}
	_output(output,indentationLevel, "%s", "</div>\n");
}


//TODO: RECORDA METERLE EL BORDER A LA TABLA!
static void _generateTable(const unsigned int indentationLevel, Table * table, FILE* output) {
	_output(output,indentationLevel, "%s", "<table>\n");
	if (table->type == TABLA_FILA_TABLA) {
		_generateRow(1 + indentationLevel, table->filaTabla,output);
	}
	_output(output,indentationLevel, "%s", "</table>\n");
}


static void _generateNavigator(const unsigned int indentationLevel, Navegador * nav, FILE* output) {
	_output(output,indentationLevel, "%s", "<nav>\n");
	if (nav->type == NAVEGADOR_FILA_NAVEGADOR) {
		_generateRowNav(1 + indentationLevel, nav->filaNav,output);
	}
	_output(output,indentationLevel, "%s", "</nav>\n");
}


static void _generatePPP(const unsigned int indentationLevel, PuntoPorPunto * ppp, FILE* output) {
	_output(output,indentationLevel, "%s", "<ul>\n");
	if (ppp->type == PPP_FILA_PUNTO_POR_PUNTO) {
		_generateRowPPP(1 + indentationLevel, ppp->filaPPP,output);
	}
	_output(output,indentationLevel, "%s", "</ul>\n");
}


static void _generateRow(const unsigned int indentationLevel, FilaTabla * tableRow, FILE* output) {
	_output(output,indentationLevel, "%s", "<tr>\n");
	if (tableRow->type == FILA_FILA_TABLA) {
		_generateColumn(1 + indentationLevel, tableRow->columnaTablaConFila,output);
		_output(output,indentationLevel, "%s", "</tr>\n");
		_generateRow(indentationLevel, tableRow->filaTabla,output);
	}
	else if (tableRow->type == FILA_COL) {
		_generateColumn(1 + indentationLevel, tableRow->columnaTabla,output);
	_output(output,indentationLevel, "%s", "</tr>\n");
}
}


static void _generateRowNav(const unsigned int indentationLevel, FilaNav * navRow, FILE* output) {
    while (navRow) {
        if (navRow->id && navRow->string) {
            _output(output,indentationLevel, "<a href=\"#%s\">%s</a>\n", navRow->id, navRow->string,output);
        }
        if (navRow->type == FILANAV_FILA_NAVEGADOR) {
            navRow = navRow->filaNav;
        } else {
            break;
        }
    }
}


static void _generateRowPPP(const unsigned int indentationLevel, FilaPPP * pppRow, FILE* output) {
	_output(output,indentationLevel, "<li>\n");
	if (pppRow->type == FILAPPP_EXPRESSION_FILAPPP) {
		_generateExpression(indentationLevel+1, pppRow->expressionFila,output);
		_output(output,indentationLevel, "%s", "</li>\n");
		_generateRowPPP(indentationLevel , pppRow->filaPPP,output);
	} else if (pppRow->type == FILAPPP_EXPRESSION) {
		_generateExpression(indentationLevel+1, pppRow->expression,output);
		_output(output,indentationLevel, "%s", "</li>\n");
	}
}



static void _generateColumn(const unsigned int indentationLevel, ColumnaTabla * columnaTabla, FILE* output) {
	if (columnaTabla->type == COLUMNA_COL) {
		_output(output,indentationLevel, "%s", "<td>\n");
		_generateSimpleExpression(1 + indentationLevel, columnaTabla->expression,output);
		_output(output,indentationLevel, "%s", "</td>\n");
		_generateColumn(indentationLevel, columnaTabla->columnaTabla,output);
	} else if (columnaTabla->type == COLUMNA_FIN_FILA) {
		// No action needed for end of row
	}
}


/*
 * ESTA TE CREA UN NUEVO COMPONENTE
 */
static void _generateComponent(const unsigned int indentationLevel, Component * component, FILE* output) {

	CompilerState * c = backendCompilerState();
	SymbolTableEntry * entry = g_hash_table_lookup(backendCompilerState()->symbolTable, component->id);
	if (component->type == COMPONENT_COMPONENT) {
		 entry->component = _generateComponentAsString(indentationLevel,component);

	}else {
		entry->component = "";
	}
	entry->type = COMPONENT_ID_INITIALIZED_AND_CREATED;
}


/*
 * ESTA TE SPAWNEA UN COMPONENTE YA CREADO
 */
static void _generateComponentId(const unsigned int indentationLevel, const char * componentId, FILE* output) {
	if (!g_hash_table_contains(backendCompilerState()->symbolTable, componentId)) {
		logError(_logger, "Component with id '%s' is not defined.", componentId);
		return;
	}
	SymbolTableEntry * entry = g_hash_table_lookup(backendCompilerState()->symbolTable, componentId);
	if (entry->type == COMPONENT_ID_INITIALIZED_AND_CREATED) {
		///TODO capaz habria que hacer una forma de que cada componente reutilizable tenga un ID distinto
		///aunque ahora que lo pienso no se realmente para que serviria eso, capaz los hacemos que no tengan IDs y listo
		///pasa que pensaba que vos le podrias hacer un href a un componente reutilizable pero ahora no podrias
		// _output(indentationLevel, "<div id=%s>\n",componentId );
		_output(output,indentationLevel,"%s\n", entry->component);
	}
	else {
		logError(_logger,"Type error, id declared as simpleId used as componentId. Component will be ignored. id: %s", componentId);
	}
}

static void _generateString(const unsigned int indentationLevel, const char * string, FILE* output) {
	_output(output,1 + indentationLevel, "%s", string);
}


static void _generateSimpleExpressionId(const unsigned int indentationLevel, SimpleExpression * simpleExpression, const char * simpleId, FILE* output) {
	if (simpleId) {
		_output(output,indentationLevel, "<div id=\"%s\">\n", simpleId);
	} else {
		_output(output,indentationLevel, "<div>\n");
	}
	_generateSimpleExpression(1 + indentationLevel, simpleExpression,output);
	_output(output,indentationLevel, "</div>\n");
}

static void _generateComplexExpressionId(const unsigned int indentationLevel, ComplexExpression * complexExpression, const char * complexId, FILE* output) {
	if (complexId) {
		_output(output,indentationLevel, "<div id=\"%s\"\n", complexId);
	} else {
		_output(output,indentationLevel, "<div>\n");
	}
	_generateComplexExpression(1 + indentationLevel, complexExpression,output);
	_output(output,indentationLevel, "</div>\n" );
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program, FILE* output) {
	switch (program->type)
	{
	case PROGRAM_EMPTY:
		return;
		break;
	case PROGRAM_HEADER_FOOTER_BODY:
		_generateHeader(1,program->headerFull,output);
		_generateBody(1,program->bodyFull, output);
		_generateFooter(1,program->footerFull,output);
		break;
	case PROGRAM_HEADER_FOOTER:
		_generateHeader(1, program->headerHF,output);
		_generateFooter(1, program->footerHF,output);
		break;
	case PROGRAM_HEADER_BODY:
		_generateHeader(1, program->headerHB,output);
		_generateBody(1, program->bodyHB,output);
		break;
	case PROGRAM_HEADER:
		_generateHeader(1, program->header,output);
		break;
	case PROGRAM_FOOTER_BODY:
		_generateBody(1, program->bodyFB,output);
		_generateFooter(1, program->footerFB,output);
		break;
	case PROGRAM_FOOTER:
		_generateFooter(1, program->footer,output);
		break;
	case PROGRAM_BODY:
		_generateBody(1, program->body,output);
		break;
	default:
		logError(_logger, "Unknown program type: %d", program->type);
		break;
	}
	
}



/* GENERADORES A STRING PARA EL COMPONENTE REUTILIZABLE EN VEZ DE OUTPUT */



/**
 * Generates the HTML for a program and returns it as a heap-allocated string.
 */
static char* _generateProgramAsString(Program* program) {
    char* result = NULL;
    
    switch (program->type) {
    case PROGRAM_EMPTY:
        return strdup(""); // Empty string for empty program
        
    case PROGRAM_HEADER_FOOTER_BODY:
        {
            char* header = _generateHeaderAsString(1, program->headerFull);
            char* body = _generateBodyAsString(1, program->bodyFull);
            char* footer = _generateFooterAsString(1, program->footerFull);
            
            result = concatenate(3, header, body, footer);
            
            free(header);
            free(body);
            free(footer);
        }
        break;
        
    case PROGRAM_HEADER_FOOTER:
        {
            char* header = _generateHeaderAsString(1, program->headerHF);
            char* footer = _generateFooterAsString(1, program->footerHF);
            
            result = concatenate(2, header, footer);
            
            free(header);
            free(footer);
        }
        break;
        
    case PROGRAM_HEADER_BODY:
        {
            char* header = _generateHeaderAsString(1, program->headerHB);
            char* body = _generateBodyAsString(1, program->bodyHB);
            
            result = concatenate(2, header, body);
            
            free(header);
            free(body);
        }
        break;
        
    case PROGRAM_HEADER:
        result = _generateHeaderAsString(1, program->header);
        break;
        
    case PROGRAM_FOOTER_BODY:
        {
            char* body = _generateBodyAsString(1, program->bodyFB);
            char* footer = _generateFooterAsString(1, program->footerFB);
            
            result = concatenate(2, body, footer);
            
            free(body);
            free(footer);
        }
        break;
        
    case PROGRAM_FOOTER:
        result = _generateFooterAsString(1, program->footer);
        break;
        
    case PROGRAM_BODY:
        result = _generateBodyAsString(1, program->body);
        break;
        
    default:
        result = _outputToString(0, "<!-- Unknown program type: %d -->\n", program->type);
        break;
    }
    
    return result;
}

/**
 * Generates the HTML for a header and returns it as a heap-allocated string.
 */
static char* _generateHeaderAsString(const unsigned int indentationLevel, Header* header) {
    char* openTag = _outputToString(indentationLevel, "<header>\n");
    char* content = strdup("");
    char* closeTag = _outputToString(indentationLevel, "</header>\n");
    
    if (header->type == HEADER_BODY) {
        free(content);
        content = _generateBodyAsString(1 + indentationLevel, header->body);
    }
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a footer and returns it as a heap-allocated string.
 */
static char* _generateFooterAsString(const unsigned int indentationLevel, Footer* footer) {
    char* openTag = _outputToString(indentationLevel, "<footer>\n");
    char* content = strdup("");
    char* closeTag = _outputToString(indentationLevel, "</footer>\n");
    
    if (footer->type == HEADER_BODY) {
        free(content);
        content = _generateBodyAsString(1 + indentationLevel, footer->body);
    }
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a body and returns it as a heap-allocated string.
 */
static char* _generateBodyAsString(const unsigned int indentationLevel, Body* body) {
    if (body->type == BODY_EXPRESSION_BODY) {
        char* expr = _generateExpressionAsString(indentationLevel, body->expressionB);
        char* remainingBody = _generateBodyAsString(indentationLevel, body->bodyB);
        
        char* result = concatenate(2, expr, remainingBody);
        
        free(expr);
        free(remainingBody);
        
        return result;
    } 
    else if (body->type == BODY_EXPRESSION) {
        return _generateExpressionAsString(indentationLevel, body->expression);
    }
    else {
        return strdup(""); // Empty body
    }
}


/**
 * Generates the HTML for an expression and returns it as a heap-allocated string.
 */
static char* _generateExpressionAsString(const unsigned int indentationLevel, Expression* expression) {
    switch (expression->type) {
    case EXPRESSION_SIMPLE_EXPRESSION:
        return _generateSimpleExpressionAsString(indentationLevel, expression->simpleExpression);
        
    case EXPRESSION_COMPLEX_EXPRESSION:
        return _generateComplexExpressionAsString(indentationLevel, expression->complexExpression);
        
    case EXPRESSION_COMPONENTE:
        return _generateComponentAsString(indentationLevel, expression->component);
        
    case EXPRESSION_STRING:
        return _generateStringAsString(indentationLevel, expression->string);
        
    case EXPRESSION_ID:
        return _generateComponentIdAsString(indentationLevel, expression->componentId);
        
    case EXPRESSION_ID_SIMPLEEXPRESSION:
        return _generateSimpleExpressionIdAsString(indentationLevel, expression->simpleExpression, expression->simpleId);
        
    case EXPRESSION_ID_COMPLEXEXPRESSION:
        return _generateComplexExpressionIdAsString(indentationLevel, expression->complexExpression, expression->complexId);
        
    default:
        return _outputToString(indentationLevel, "<!-- Unknown expression type: %d -->\n", expression->type);
    }
}

/**
 * Generates the HTML for a simple expression and returns it as a heap-allocated string.
 */
static char* _generateSimpleExpressionAsString(const unsigned int indentationLevel, SimpleExpression* simpleExpression) {
    switch (simpleExpression->type) {
    case SEXPRESSION_TEXT:
        return _generateTextAsString(indentationLevel, simpleExpression->text);
        
    case SEXPRESSION_IMG:
        return _generateImageAsString(indentationLevel, simpleExpression->img);
        
    case SEXPRESSION_TITLE:
        return _generateTitleAsString(indentationLevel, simpleExpression->title);
        
    case SEXPRESSION_SUBTITLE:
        return _generateSubtitleAsString(indentationLevel, simpleExpression->subtitle);
        
    case SEXPRESSION_LINK:
        return _generateLinkAsString(indentationLevel, simpleExpression->link);
        
    default:
        return _outputToString(indentationLevel, "<!-- Unknown simple expression type: %d -->\n", simpleExpression->type);
    }
}


/**
 * Generates the HTML for an image and returns it as a heap-allocated string.
 */
static char* _generateImageAsString(const unsigned int indentationLevel, Image* img) {
    if (img->alternative == NULL) {
    	logWarning(_logger,"Image without alternative text. URL: %s", img->url);
        return _outputToString(indentationLevel, "<img src=\"%s\"/>\n", img->url);
    } else {
        return _outputToString(indentationLevel, "<img src=\"%s\" alt=\"%s\"/>\n", img->url, img->alternative);
    }
}

/**
 * Generates the HTML for a title and returns it as a heap-allocated string.
 */
static char* _generateTitleAsString(const unsigned int indentationLevel, Title* title) {
    return _outputToString(indentationLevel, "<h1>%s</h1>\n", title->string);
}

/**
 * Generates the HTML for a subtitle and returns it as a heap-allocated string.
 */
static char* _generateSubtitleAsString(const unsigned int indentationLevel, Subtitle* subtitle) {
    return _outputToString(indentationLevel, "<h2>%s</h2>\n", subtitle->string);
}

/**
 * Generates the HTML for text and returns it as a heap-allocated string.
 */
static char* _generateTextAsString(const unsigned int indentationLevel, Text* text) {
    switch (text->type) {
    case TEXT_MODIFIED_TEXT:
        {
            char* openSpan = _outputToString(indentationLevel, "<div style=\"");
            char* style = _generateModifiedTextAsString(0, text->modifier);
            char* closeStyle = strdup("\">\n");
            char* content = _generateSimpleTextAsString(indentationLevel + 1, text->string);
            char* closeSpan = strdup("</div>\n");
            
            char* result = concatenate(5, openSpan, style, closeStyle, content, closeSpan);
            
            free(openSpan);
            free(style);
            free(closeStyle);
            free(content);
            free(closeSpan);
            
            return result;
        }
        
    case TEXT_SIMPLE_TEXT:
        return _generateSimpleTextAsString(indentationLevel, text->string);
        
    default:
        return _outputToString(indentationLevel, "<!-- Unknown text type: %d -->\n", text->type);
    }
}

/**
 * Generates the CSS style for modified text and returns it as a heap-allocated string.
 */
static char* _generateModifiedTextAsString(const unsigned int indentationLevel, Modifier* modifier) {
    if (modifier == NULL) {
        return strdup("");
    }
    
    char* result = NULL;
    
    switch (modifier->type) {
    case MODIFIER_COLOR_MOD:
        {
            char* colorStyle = NULL;
            
            switch (modifier->color) {
            case COLOR_RED:
                colorStyle = strdup("color: red;");
                break;
            case COLOR_GREEN:
                colorStyle = strdup("color: green;");
                break;
            case COLOR_BLUE:
                colorStyle = strdup("color: blue;");
                break;
            case COLOR_YELLOW:
                colorStyle = strdup("color: yellow;");
                break;
            case COLOR_ORANGE:
                colorStyle = strdup("color: orange;");
                break;
            default:
                colorStyle = strdup("");
                break;
            }
            
            char* nextModifiers = _generateModifiedTextAsString(indentationLevel, modifier->modifierWithColor);
            result = concatenate(2, colorStyle, nextModifiers);
            
            free(colorStyle);
            free(nextModifiers);
        }
        break;
        
    case MODIFIER_EMPTY:
        result = strdup("");
        break;
        
    case MODIFIER_MODIFIER:
        {
            char* styleStr = NULL;
            
            switch (modifier->style) {
            case UNDERLINE:
                styleStr = strdup("text-decoration: underline;");
                break;
            case BOLD:
                styleStr = strdup("font-weight: bold;");
                break;
            case ITALIC:
                styleStr = strdup("font-style: italic;");
                break;
            case BIG:
                styleStr = strdup("font-size: larger;");
                break;
            case TINY:
                styleStr = strdup("font-size: smaller;");
                break;
            case MEDIUM:
                styleStr = strdup("font-size: medium;");
                break;
            default:
                styleStr = strdup("");
                break;
            }
            
            char* nextModifiers = _generateModifiedTextAsString(indentationLevel, modifier->modifier);
            result = concatenate(2, styleStr, nextModifiers);
            
            free(styleStr);
            free(nextModifiers);
        }
        break;
        
    default:
        result = strdup("");
        break;
    }
    
    return result;
}


/**
 * Generates the HTML for simple text and returns it as a heap-allocated string.
 */
static char* _generateSimpleTextAsString(const unsigned int indentationLevel, const char* string) {
    return _outputToString(indentationLevel, "<p>%s</p>\n", string);
}

/**
 * Generates the HTML for a link and returns it as a heap-allocated string.
 */
static char* _generateLinkAsString(const unsigned int indentationLevel, Link* link) {
    if (!link || !link->href) {
        return strdup("");
    }
    
    char* openTag = NULL;
    
    if (link->href->type == HREF_URL) {
        openTag = _outputToString(indentationLevel, "<a href=\"%s\">\n", link->href->url);
    } else if (link->href->type == HREF_ID) {
        openTag = _outputToString(indentationLevel, "<a href=\"#%s\">\n", link->href->id);
    } else {
        openTag = _outputToString(indentationLevel, "<a>");
    }
    
    char* content = _generateSimpleExpressionAsString(indentationLevel + 1, link->simpleExpression);
    char* closeTag = strdup("</a>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a complex expression and returns it as a heap-allocated string.
 */
static char* _generateComplexExpressionAsString(const unsigned int indentationLevel, ComplexExpression* complexExpression) {
    switch (complexExpression->type) {
    case CEXPRESSION_SECCION:
        return _generateSectionAsString(indentationLevel, complexExpression->seccion);
        
    case CEXPRESSION_TABLA:
        return _generateTableAsString(indentationLevel, complexExpression->tabla);
        
    case CEXPRESSION_NAVEGADOR:
        return _generateNavigatorAsString(indentationLevel, complexExpression->navegador);
        
    case CEXPRESSION_PUNTO_POR_PUNTO:
        return _generatePPPAsString(indentationLevel, complexExpression->puntoPorPunto);
        
    default:
        return _outputToString(indentationLevel, "<!-- Unknown complex expression type: %d -->\n", complexExpression->type);
    }
}

/**
 * Generates the HTML for a component with ID and returns it as a heap-allocated string.
 */
static char* _generateComponentAsString(const unsigned int indentationLevel, Component* component) {
    char* openTag = NULL;
    
    if (component->id) {
        openTag = _outputToString(indentationLevel, "<div id=\"%s\">\n", component->id);
    } else {
        openTag = _outputToString(indentationLevel, "<div>\n");
    }
    
    char* content = strdup("");
    
    if (component->type == COMPONENT_COMPONENT) {
        free(content);
        content = _generateBodyAsString(1 + indentationLevel, component->body);
    }
    
    char* closeTag = _outputToString(indentationLevel, "</div>");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a component reference and returns it as a heap-allocated string.
 */
static char* _generateComponentIdAsString(const unsigned int indentationLevel, const char* componentId) {
    if (!g_hash_table_contains(currentCompilerState()->symbolTable, componentId)) {
        logError(_logger, "Component with id '%s' is not defined.", componentId);
        return strdup("<!-- Component not found -->\n");
    }
    
    char* openTag = NULL;
    
    if (componentId) {
        openTag = _outputToString(indentationLevel, "<div id=\"%s\">\n", componentId);
    } else {
        openTag = _outputToString(indentationLevel, "<div>\n");
    }
    
    // Here you would insert the component content from the symbol table
    
    char* closeTag = _outputToString(indentationLevel, "</div>\n");
    
    char* result = concatenate(2, openTag, closeTag);
    
    free(openTag);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a string literal and returns it as a heap-allocated string.
 */
static char* _generateStringAsString(const unsigned int indentationLevel, const char* string) {
    return _outputToString(indentationLevel, "%s", string);
}


/**
 * Generates the HTML for a section and returns it as a heap-allocated string.
 */
static char* _generateSectionAsString(const unsigned int indentationLevel, Seccion* section) {
    char* openTag = _outputToString(indentationLevel, "<div>\n");
    char* content = strdup("");
    
    if (section->type == SECCION_BODY) {
        free(content);
        content = _generateBodyAsString(1 + indentationLevel, section->body);
    }
    
    char* closeTag = _outputToString(indentationLevel, "</div>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a table and returns it as a heap-allocated string.
 */
static char* _generateTableAsString(const unsigned int indentationLevel, Table* table) {
    char* openTag = _outputToString(indentationLevel, "<table>\n");
    char* content = strdup("");
    
    if (table->type == TABLA_FILA_TABLA) {
        free(content);
        content = _generateRowAsString(1 + indentationLevel, table->filaTabla);
    }
    
    char* closeTag = _outputToString(indentationLevel, "</table>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a navigator and returns it as a heap-allocated string.
 */
static char* _generateNavigatorAsString(const unsigned int indentationLevel, Navegador* nav) {
    char* openTag = _outputToString(indentationLevel, "<nav>\n");
    char* content = strdup("");
    
    if (nav->type == NAVEGADOR_FILA_NAVEGADOR) {
        free(content);
        content = _generateRowNavAsString(1 + indentationLevel, nav->filaNav);
    }
    
    char* closeTag = _outputToString(indentationLevel, "</nav>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a punto-por-punto list and returns it as a heap-allocated string.
 */
static char* _generatePPPAsString(const unsigned int indentationLevel, PuntoPorPunto* ppp) {
    char* openTag = _outputToString(indentationLevel, "<ul>\n");
    char* content = strdup("");
    
    if (ppp->type == PPP_FILA_PUNTO_POR_PUNTO) {
        free(content);
        content = _generateRowPPPAsString(1 + indentationLevel, ppp->filaPPP);
    }
    
    char* closeTag = _outputToString(indentationLevel, "</ul>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}


/**
 * Generates the HTML for a simple expression with ID and returns it as a heap-allocated string.
 */
static char* _generateSimpleExpressionIdAsString(const unsigned int indentationLevel, SimpleExpression* simpleExpression, const char* simpleId) {
    char* openTag = NULL;
    
    if (simpleId) {
        openTag = _outputToString(indentationLevel, "<div id=\"%s\">\n", simpleId);
    } else {
        openTag = _outputToString(indentationLevel, "<div>\n");
    }
    
    char* content = _generateSimpleExpressionAsString(1 + indentationLevel, simpleExpression);
    char* closeTag = _outputToString(indentationLevel, "</div>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a complex expression with ID and returns it as a heap-allocated string.
 */
static char* _generateComplexExpressionIdAsString(const unsigned int indentationLevel, ComplexExpression* complexExpression, const char* complexId) {
    char* openTag = NULL;
    
    if (complexId) {
        openTag = _outputToString(indentationLevel, "<div id=\"%s\">\n", complexId);
    } else {
        openTag = _outputToString(indentationLevel, "<div>\n");
    }
    
    char* content = _generateComplexExpressionAsString(1 + indentationLevel, complexExpression);
    char* closeTag = _outputToString(indentationLevel, "</div>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a table row and returns it as a heap-allocated string.
 */
static char* _generateRowAsString(const unsigned int indentationLevel, FilaTabla* tableRow) {
    char* openTag = _outputToString(indentationLevel, "<tr>\n");
    char* content = strdup("");
    
    if (tableRow->type == TABLA_FILA_TABLA) {
        free(content);
        content = _generateColumnAsString(indentationLevel + 1, tableRow->columnaTabla);
    }
    
    char* closeTag = _outputToString(indentationLevel, "</tr>\n");
    
    char* result = concatenate(3, openTag, content, closeTag);
    
    free(openTag);
    free(content);
    free(closeTag);
    
    return result;
}

/**
 * Generates the HTML for a navigation row and returns it as a heap-allocated string.
 */
static char* _generateRowNavAsString(const unsigned int indentationLevel, FilaNav* navRow) {
    if (!navRow) {
        return strdup("");
    }
    
    char* links = strdup("");
    FilaNav* currentRow = navRow;
    
    while (currentRow) {
        if (currentRow->id && currentRow->string) {
            char* linkTag = _outputToString(indentationLevel, "<a href=\"#%s\">\n%s</a>\n",
                                          currentRow->id, currentRow->string);
            
            char* tempLinks = links;
            links = concatenate(2, tempLinks, linkTag);
            
            free(tempLinks);
            free(linkTag);
        }
        
        if (currentRow->type == FILANAV_FILA_NAVEGADOR) {
            currentRow = currentRow->filaNav;
        } else {
            break;
        }
    }
    
    return links;
}

/**
 * Generates the HTML for punto-por-punto rows and returns it as a heap-allocated string.
 */
static char* _generateRowPPPAsString(const unsigned int indentationLevel, FilaPPP* pppRow) {
    if (!pppRow) {
        return strdup("");
    }
    
    char* result = NULL;
    
    if (pppRow->type == FILAPPP_EXPRESSION_FILAPPP) {
        char* openTag = _outputToString(indentationLevel, "<li>\n");
        char* content = _generateExpressionAsString(indentationLevel +1, pppRow->expressionFila);
        char* closeTag = _outputToString(indentationLevel, "</li>\n");
        char* nextRows = _generateRowPPPAsString(indentationLevel, pppRow->filaPPP);
        
        result = concatenate(4, openTag, content, closeTag, nextRows);
        
        free(openTag);
        free(content);
        free(closeTag);
        free(nextRows);
    } 
    else if (pppRow->type == FILAPPP_EXPRESSION) {
        char* openTag = _outputToString(indentationLevel, "<li>\n");
        char* content = _generateExpressionAsString(indentationLevel+1, pppRow->expression);
        char* closeTag = _outputToString(indentationLevel, "</li>\n");
        result = concatenate(3, openTag, content, closeTag);
        
        free(openTag);
        free(content);
        free(closeTag);
    }
    else {
        result = strdup("");
    }
    
    return result;
}

/**
 * Generates the HTML for a column and returns it as a heap-allocated string.
 */
static char* _generateColumnAsString(const unsigned int indentationLevel, ColumnaTabla* columnaTabla) {
    if (!columnaTabla) {
        return strdup("");
    }
    
    char* result = NULL;
    
    if (columnaTabla->type == COLUMNA_COL) {
        char* openTag = _outputToString(indentationLevel, "<td>\n");
        char* content = _generateSimpleExpressionAsString(indentationLevel + 1, columnaTabla->expression);
        char* closeTag = _outputToString(indentationLevel, "</td>\n");
        
        result = concatenate(3, openTag, content, closeTag);
        
        free(openTag);
        free(content);
        free(closeTag);
    }
    else if (columnaTabla->type == COLUMNA_FIN_FILA) {
        // No content for end of row
        result = strdup("");
    }
    
    return result;
}


/**
 * Creates the prologue of the generated output, an HTML document. 
 */
static void _generatePrologue(FILE* output) {
	_output(output,0, "%s",
		"<!DOCTYPE html>\n"
		"<html lang=\"es\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <link rel=\"stylesheet\" href=\"style.css\">\n"
		"    <title>HTML Generado</title>\n"
		"</head>\n"
		"<body>\n"
	);
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid HTML document.
 */
static void _generateEpilogue(FILE* output) {
	_output(output,0, "%s",
		"</body>\n"
		"</html>\n\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}



/**
 * Creates a formatted string with indentation and returns it as a heap-allocated string.
 * The caller is responsible for freeing the memory.
 */
static char* _outputToString(const unsigned int indentationLevel, const char* const format, ...) {
	char* indentation = _indentation(indentationLevel);
	char* effectiveFormat = concatenate(2, indentation, format);

	// get lenght
	va_list arguments;
	va_start(arguments, format);
	va_list argumentsCopy;
	va_copy(argumentsCopy, arguments);


	int size = vsnprintf(NULL, 0, effectiveFormat, arguments);
	va_end(arguments);

	if (size < 0) {
		free(effectiveFormat);
		free(indentation);
		va_end(argumentsCopy);
		return NULL;
	}

	//malloc
	char* result = malloc((size + 1) * sizeof(char));
	if (!result) {
		free(effectiveFormat);
		free(indentation);
		va_end(argumentsCopy);
		return NULL;
	}


	vsnprintf(result, size + 1, effectiveFormat, argumentsCopy);
	va_end(argumentsCopy);

	//limpio temporales
	free(effectiveFormat);
	free(indentation);

	return result;
}



/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(FILE* output,const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(output, effectiveFormat, arguments);
	fflush(output);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
    _currentCompilerState = compilerState;
    FILE *output = fopen("results/index.html", "w");
    if (!output) {
        logError(_logger, "Could not open output file for writing.");
        return;
    }
	_generatePrologue(output);
	_generateProgram(compilerState->abstractSyntaxtTree,output);
	_generateEpilogue(output);
	checkUndeclaredIdsAndLog(compilerState->unDeclaredSymbols);
    fclose(output);
	logDebugging(_logger, "Finished generation.");
}
