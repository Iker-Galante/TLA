#include "Generator.h"

#include "../../frontend/syntactic-analysis/SyntacticAnalyzer.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 2;
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

/*
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
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////


/** PRIVATE FUNCTIONS */

static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

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
	if(body->type == BODY_EXPRESSION_BODY){
		_generateExpression(indentationLevel, body->expressionB);
		_generateBody(indentationLevel, body->bodyB);
	}
	else if(body->type == BODY_EXPRESSION){
		_generateExpression(indentationLevel, body->expression);
	}
}

static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	switch (expression->type) {
		case EXPRESSION_SIMPLE_EXPRESSION:
			_generateSimpleExpression(indentationLevel, expression->simpleExpression);
			break;
		case EXPRESSION_COMPLEX_EXPRESSION:
			_generateComplexExpression(indentationLevel, expression->complexExpression);
			break;
		case EXPRESSION_COMPONENTE:
			_generateComponent(indentationLevel, expression->component);
			break;
		case EXPRESSION_STRING:
			_generateString(indentationLevel, expression->string); //Es así?
			break;
		case EXPRESSION_ID:
			_generateComponentId(indentationLevel, expression->componentId);
			break;
		case EXPRESSION_ID_SIMPLEEXPRESSION:
			_generateSimpleExpressionId(indentationLevel, expression->simpleExpression,expression->simpleId);
			break;
		case EXPRESSION_ID_COMPLEXEXPRESSION:
			_generateComplexExpressionId(indentationLevel, expression->complexExpression, expression->complexId);
			break;
		default:
			logError(_logger, "Unknown expression type: %d", expression->type);
			break;
	}
}

static void _generateSimpleExpression(const unsigned int indentationLevel, SimpleExpression * simpleExpression) {
	switch (simpleExpression->type) {
		case SEXPRESSION_TEXT:
			_generateText(indentationLevel, simpleExpression->text);
			break;
		case SEXPRESSION_IMG:
			_generateImage(indentationLevel, simpleExpression->img);
			break;
		case SEXPRESSION_TITLE:
			_generateTitle(indentationLevel, simpleExpression->title);
			break;
		case SEXPRESSION_SUBTITLE:
			_generateSubtitle(indentationLevel, simpleExpression->subtitle);
			break;
		case SEXPRESSION_LINK:
			_generateLink(indentationLevel, simpleExpression->link);
			break;
		default:
			logError(_logger, "Unknown simple expression type: %d", simpleExpression->type);
			break;
	}
}

static void _generateImage(const unsigned int indentationLevel, Image * img) {
	if(img->alternative == NULL){
    _output(indentationLevel, "<img src=\"%s\"/>\n", img->url);
	} else {
	_output(indentationLevel, "<img src=\"%s\" alt=\"%s\"/>\n", img->url, img->alternative);
	}
}

static void _generateTitle(const unsigned int indentationLevel, Title * title) {
	_output(indentationLevel, "<h1>%s</h1>\n", title->string);
}


static void _generateLink(const unsigned int indentationLevel, Link *link) {
    if (!link || !link->href) return;

    const char *hrefValue = "";
    if (link->href->type == HREF_URL) {
        hrefValue = link->href->url;
        _output(indentationLevel, "<a href=\"%s\">", hrefValue);
    } else if (link->href->type == HREF_ID) {
        _output(indentationLevel, "<a href=\"#%s\">", link->href->id);
    } else {
        _output(indentationLevel, "<a>");
    }
	
    _generateSimpleExpression(indentationLevel, link->simpleExpression);
    

    _output(indentationLevel, "</a>\n");
}

static void _generateSubtitle(const unsigned int indentationLevel, Subtitle * subtitle) {
	_output(indentationLevel, "<h2>%s</h2>\n", subtitle->string);
}


static void _generateText(const unsigned int indentationLevel, Text * text) {
	switch (text->type) {
		case TEXT_MODIFIED_TEXT:
			_output(indentationLevel, "%s", "<span style=\""); // Open a span for modified text
			_generateModifiedText(indentationLevel, text->modifier);
			_output(indentationLevel, "%s", "\">"); // Close the style attribute
			_generateSimpleText(indentationLevel, text->string);
			_output(indentationLevel, "%s", "</span>\n"); // Close the span opened in _generateModifiedText
			break;
		case TEXT_SIMPLE_TEXT:
			_generateSimpleText(indentationLevel, text->string);
			break;
		default:
			logError(_logger, "Unknown text type: %d", text->type);
			break;
	}
}

static void _generateModifiedText(const unsigned int indentationLevel, Modifier * modifier) {
	if (modifier == NULL) {
		return;
	}
	switch (modifier->type) {
		case MODIFIER_COLOR_MOD:
			switch (modifier->color)
			{
			case COLOR_RED:
				_output(indentationLevel, "color: red;");
				break;
			case COLOR_GREEN:
				_output(indentationLevel, "color: green;");
				break;
			case COLOR_BLUE:
				_output(indentationLevel, "color: blue;");
				break;
			case COLOR_YELLOW:
				_output(indentationLevel, "color: yellow;");
				break;
			case COLOR_ORANGE:
				_output(indentationLevel, "color: orange;");
				break;
			default:
				break;
			}
			_generateModifiedText(indentationLevel, modifier->modifierWithColor);
			break;
		case MODIFIER_EMPTY:
			// No action needed for empty modifier
			break;
		case MODIFIER_MODIFIER:
			switch (modifier->style)
			{
			case UNDERLINE:
				_output(indentationLevel, "text-decoration: underline;");
				break;
			case BOLD:
				_output(indentationLevel, "font-weight: bold;");
				break;
			case ITALIC:
				_output(indentationLevel, "font-style: italic;");
				break;
			case BIG:
				_output(indentationLevel, "font-size: larger;");
				break;
			case TINY:
				_output(indentationLevel, "font-size: smaller;");
				break;
			case MEDIUM:
				_output(indentationLevel, "font-size: medium;");
				break;
			default:
				logError(_logger, "Unknown style type: %d", modifier->style);
				return; // Exit if an unknown style is encountered
			}
			_generateModifiedText(indentationLevel, modifier->modifier);
			break;
		default:
			logError(_logger, "Unknown modifier type: %d", modifier->type);
			break;
	}

}

static void _generateSimpleText(const unsigned int indentationLevel, const char * string) {
	_output(indentationLevel, "<p>%s</p>\n", string);
}

static void _generateComplexExpression(const unsigned int indentationLevel, ComplexExpression * complexExpression) {
	switch (complexExpression->type) {
		case CEXPRESSION_SECCION:
			_generateSection(indentationLevel, complexExpression->seccion);
			break;
		case CEXPRESSION_TABLA:
			_generateTable(indentationLevel, complexExpression->tabla);
			break;
		case CEXPRESSION_NAVEGADOR:
			_generateNavigator(indentationLevel, complexExpression->navegador);
			break;
		case CEXPRESSION_PUNTO_POR_PUNTO:
			_generatePPP(indentationLevel, complexExpression->puntoPorPunto);
			break;
		default:
			logError(_logger, "Unknown complex expression type: %d", complexExpression->type);
			break;
	}
}


static void _generateSection(const unsigned int indentationLevel, Seccion * section) {
	_output(indentationLevel, "%s", "<div>\n");
	if (section->type == SECCION_BODY) {
		_generateBody(1 + indentationLevel, section->body);
	}
	_output(indentationLevel, "%s", "</div>\n");
}


static void _generateTable(const unsigned int indentationLevel, Table * table) {
	_output(indentationLevel, "%s", "<table>\n");
	if (table->type == TABLA_FILA_TABLA) {
		_generateRow(1 + indentationLevel, table->filaTabla);
	}
	_output(indentationLevel, "%s", "</table>\n");
}


static void _generateNavigator(const unsigned int indentationLevel, Navegador * nav) {
	_output(indentationLevel, "%s", "<nav>\n");
	if (nav->type == NAVEGADOR_FILA_NAVEGADOR) {
		_generateRowNav(1 + indentationLevel, nav->filaNav);
	}
	_output(indentationLevel, "%s", "</nav>\n");
}


static void _generatePPP(const unsigned int indentationLevel, PuntoPorPunto * ppp) {
	_output(indentationLevel, "%s", "<ul>\n");
	if (ppp->type == PPP_FILA_PUNTO_POR_PUNTO) {
		_generateRowPPP(1 + indentationLevel, ppp->filaPPP);
	}
	_output(indentationLevel, "%s", "</ul>\n");
}


static void _generateRow(const unsigned int indentationLevel, FilaTabla * tableRow) {
	_output(indentationLevel, "%s", "<tr>\n");
	if (tableRow->type == TABLA_FILA_TABLA) {
		_generateColumn(indentationLevel, tableRow->columnaTabla);
	}
	_output(indentationLevel, "%s", "</tr>\n");
}


static void _generateRowNav(const unsigned int indentationLevel, FilaNav * navRow) {
    while (navRow) {
        if (navRow->id && navRow->string) {
            _output(indentationLevel, "<a href=\"#%s\">%s</a>\n", navRow->id, navRow->string);
        }
        if (navRow->type == FILANAV_FILA_NAVEGADOR) {
            navRow = navRow->filaNav;
        } else {
            break;
        }
    }
}


static void _generateRowPPP(const unsigned int indentationLevel, FilaPPP * pppRow) {
	_output(indentationLevel, "<li>");
	if (pppRow->type == FILAPPP_EXPRESSION_FILAPPP) {
		_generateExpression(0, pppRow->expressionFila);
		_output(1, "%s", "</li>\n");
		_generateRowPPP(indentationLevel , pppRow->filaPPP);
	} else if (pppRow->type == FILAPPP_EXPRESSION) {
		_generateExpression(0, pppRow->expression);
		_output(1, "%s", "</li>\n");
	}
}



static void _generateColumn(const unsigned int indentationLevel, ColumnaTabla * columnaTabla) {
	_output(indentationLevel, "%s", "<td>\n");
	if (columnaTabla->type == COLUMNA_COL) {
		_generateSimpleExpression(1 + indentationLevel, columnaTabla->expression);
	} else if (columnaTabla->type == COLUMNA_FIN_FILA) {
		// No action needed for end of row
	}
	_output(indentationLevel, "%s", "</td>\n");
}

//TODO DUDAS DE COMO HACER ESTE. LATER TALK WITH MANCIO
/*
 * ESTA TE CREA UN NUEVO COMPONENTE
 */
static void _generateComponent(const unsigned int indentationLevel, Component * component) {

	SymbolTableEntry * entry = g_hash_table_lookup(currentCompilerState()->symbolTable, component->id);
	char * componentHtml;

    if (component->id) {
        _output(indentationLevel, "<div id=\"%s\">\n", component->id);
    } else {
        _output(indentationLevel, "<div>\n");
    }
    if (component->type == COMPONENT_COMPONENT) {
        _generateBody(1 + indentationLevel, component->body);
    }
    _output(indentationLevel, "%s", "</div>\n");
}


//TODO como "spawneo" al componente (?)
/*
 * ESTA TE SPAWNEA UN COMPONENTE YA CREADO
 */
static void _generateComponentId(const unsigned int indentationLevel, const char * componentId) {
	if (!g_hash_table_contains(currentCompilerState()->symbolTable, componentId)) {
		logError(_logger, "Component with id '%s' is not defined.", componentId);
		return;
	}
	if (componentId) {
		_output(indentationLevel, "<div id=%s>\n",componentId );

	} else {
		_output(indentationLevel, "%s", "<div>\n");
	}
	_output(indentationLevel, "%s", "</div>\n");
}

static void _generateString(const unsigned int indentationLevel, const char * string) {
	_output(1 + indentationLevel, "%s", string);
}


static void _generateSimpleExpressionId(const unsigned int indentationLevel, SimpleExpression * simpleExpression, const char * simpleId) {
	if (simpleId) {
		_output(indentationLevel, "%s", "<div id=\"");
		_output(indentationLevel, "%s", simpleId);
		_output(indentationLevel, "%s", "\">\n");
	} else {
		_output(indentationLevel, "%s", "<div>\n");
	}
	_generateSimpleExpression(1 + indentationLevel, simpleExpression);
	_output(indentationLevel, "%s", "</div>\n");
}

static void _generateComplexExpressionId(const unsigned int indentationLevel, ComplexExpression * complexExpression, const char * complexId) {
	if (complexId) {
		_output(indentationLevel, "%s", "<div id=\"");
		_output(indentationLevel, "%s", complexId);
		_output(indentationLevel, "%s", "\">\n");
	} else {
		_output(indentationLevel, "%s", "<div>\n");
	}
	_generateComplexExpression(1 + indentationLevel, complexExpression);
	_output(indentationLevel, "%s", "</div>\n");
}

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
		_generateHeader(1, program->headerHF);
		_generateFooter(1, program->footerHF);
		break;
	case PROGRAM_HEADER_BODY:
		_generateHeader(1, program->headerHB);
		_generateBody(1, program->bodyHB);
		break;
	case PROGRAM_HEADER:
		_generateHeader(1, program->header);
		break;
	case PROGRAM_FOOTER_BODY:
		_generateBody(1, program->bodyFB);
		_generateFooter(1, program->footerFB);
		break;
	case PROGRAM_FOOTER:
		_generateFooter(1, program->footer);
		break;
	case PROGRAM_BODY:
		_generateBody(1, program->body);
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
            char* openSpan = _outputToString(indentationLevel, "<span style=\"");
            char* style = _generateModifiedTextAsString(indentationLevel, text->modifier);
            char* closeStyle = strdup("\">");
            char* content = _generateSimpleTextAsString(0, text->string);
            char* closeSpan = strdup("</span>\n");
            
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
        openTag = _outputToString(indentationLevel, "<a href=\"%s\">", link->href->url);
    } else if (link->href->type == HREF_ID) {
        openTag = _outputToString(indentationLevel, "<a href=\"#%s\">", link->href->id);
    } else {
        openTag = _outputToString(indentationLevel, "<a>");
    }
    
    char* content = _generateSimpleExpressionAsString(0, link->simpleExpression);
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
    
    char* closeTag = _outputToString(indentationLevel, "</div>\n");
    
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
            char* linkTag = _outputToString(indentationLevel, "<a href=\"#%s\">%s</a>\n", 
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
        char* openTag = _outputToString(indentationLevel, "<li>");
        char* content = _generateExpressionAsString(0, pppRow->expressionFila); 
        char* closeTag = strdup("</li>\n");
        char* nextRows = _generateRowPPPAsString(indentationLevel, pppRow->filaPPP);
        
        result = concatenate(4, openTag, content, closeTag, nextRows);
        
        free(openTag);
        free(content);
        free(closeTag);
        free(nextRows);
    } 
    else if (pppRow->type == FILAPPP_EXPRESSION) {
        char* openTag = _outputToString(indentationLevel, "<li>");
        char* content = _generateExpressionAsString(0, pppRow->expression);
        char* closeTag = strdup("</li>\n");
        
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
static void _generatePrologue(void) {
	_output(0, "%s",
		"<!DOCTYPE html>\n"
		"<html lang=\"es\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"    <title>HTML Generado</title>\n"
		"</head>\n"
		"<body>\n"
	);
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid HTML document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s",
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
