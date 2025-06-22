#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */
void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		switch (program->type) {
			case PROGRAM_HEADER_FOOTER_BODY:
				releaseHeader(program->headerFull);
				releaseBody(program->bodyFull);
				releaseFooter(program->footerFull);
				break;
			case PROGRAM_FOOTER_BODY:
				releaseBody(program->bodyFB);
				releaseFooter(program->footerFB);
				break;
			case PROGRAM_HEADER_BODY:
				releaseHeader(program->headerHB);
				releaseBody(program->bodyHB);
				break;
			case PROGRAM_HEADER_FOOTER:
				releaseHeader(program->headerHF);
				releaseFooter(program->footerHF);
				break;
			case PROGRAM_BODY:
				releaseBody(program->body);
				break;
			case PROGRAM_FOOTER:
				releaseFooter(program->footer);
				break;
			case PROGRAM_HEADER:
				releaseHeader(program->header);
				break;
			case PROGRAM_EMPTY:
				break;
			default:
				logError(_logger, "Unknown program type: %d", program->type);
				break;
		}
		free(program);
	}
}


void releaseExpression(Expression * expression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case EXPRESSION_STRING:
				free(expression->string);
				break;
			case EXPRESSION_ID:
				free(expression->componentId);
				break;
			case EXPRESSION_ID_SIMPLEEXPRESSION:
				releaseSimpleExpression(expression->simpleExpressionId);
				free(expression->simpleId);
				break;
			case EXPRESSION_ID_COMPLEXEXPRESSION:
				releaseComplexExpression(expression->complexExpressionId);
				free(expression->complexId);
				break;
			case EXPRESSION_SIMPLE_EXPRESSION:
				releaseSimpleExpression(expression->simpleExpression);
				break;
			case EXPRESSION_COMPLEX_EXPRESSION:
				releaseComplexExpression(expression->complexExpression);
				break;
			case EXPRESSION_COMPONENTE:
				releaseComponent(expression->component);
				break;
			default:
				logError(_logger, "Unknown expression type: %d", expression->type);
				break;
		}
		free(expression); //Asumo que esto libera todo el resto que queda...
	}
}

void releaseHeader(Header * header) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (header != NULL) {
        if (header->type == HEADER_BODY) {
            releaseBody(header->body);
        }
        free(header);
    }
}

void releaseFooter(Footer * footer) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (footer != NULL) {
        if (footer->type == HEADER_BODY) {
            releaseBody(footer->body);
        }
        free(footer);
    }
}

void releaseBody(Body * body) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (body != NULL) {
        if (body->type == BODY_EXPRESSION_BODY) {
            releaseExpression(body->expressionB);
            releaseBody(body->bodyB);
        } else if (body->type == BODY_EXPRESSION) {
            releaseExpression(body->expression);
        }
        free(body);
    }
}

void releaseSimpleExpression(SimpleExpression * simpleExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (simpleExpression != NULL) {
        switch (simpleExpression->type) {
            case SEXPRESSION_TEXT:
                releaseText(simpleExpression->text);
                break;
            case SEXPRESSION_IMG:
                releaseImg(simpleExpression->img);
                break;
            case SEXPRESSION_TITLE:
                releaseTitle(simpleExpression->title);
                break;
            case SEXPRESSION_SUBTITLE:
                releaseSubtitle(simpleExpression->subtitle);
                break;
            case SEXPRESSION_LINK:
                releaseLink(simpleExpression->link);
                break;
			default:
				logError(_logger, "Unknown simple expression type: %d", simpleExpression->type);
				break;
        }
        free(simpleExpression);
    }
}

void releaseModifiers(Modifier * modifiers) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (modifiers != NULL) {
		switch (modifiers->type) {
		case MODIFIER_COLOR_MOD:
			/*free(modifiers->color);*/
			releaseModifiers(modifiers->modifierWithColor);
			break;
		case MODIFIER_MODIFIER:
			releaseModifiers(modifiers->modifier);
			break;
		case MODIFIER_EMPTY:
			break;
		default:
			logError(_logger, "Unknown modifier type: %d", modifiers->type);
			break;
		}
		free(modifiers);
	}
}


void releaseComponent(Component * component) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (component != NULL) {
        if(component->type == COMPONENT_COMPONENT) {
			releaseBody(component->body);
    }
	free(component->id);
	free(component);
}

}

void releaseComplexExpression(ComplexExpression * complexExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (complexExpression != NULL) {
        switch (complexExpression->type) {
            case CEXPRESSION_PUNTO_POR_PUNTO:
                releasePuntoPorPunto(complexExpression->puntoPorPunto);
                break;
            case CEXPRESSION_SECCION:
                releaseSeccion(complexExpression->seccion);
                break;
            case CEXPRESSION_TABLA:
                releaseTabla(complexExpression->tabla);
                break;
            case CEXPRESSION_NAVEGADOR:
                releaseNavegador(complexExpression->navegador);
                break;
        }
        free(complexExpression);
    }
}

void releasePuntoPorPunto(PuntoPorPunto * puntoPorPunto) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (puntoPorPunto != NULL) {
		if(puntoPorPunto->type == PPP_FILA_PUNTO_POR_PUNTO){
			releaseFilaPPP(puntoPorPunto->filaPPP);
		}
		free(puntoPorPunto);
	}
}

void releaseNavegador(Navegador * navegador) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (navegador != NULL) {
		if(navegador->type == NAVEGADOR_FILA_NAVEGADOR){
			releaseFilaNav(navegador->filaNav);
		}
		free(navegador);
	}
}

void releaseFilaPPP(FilaPPP * rowPPP) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (rowPPP != NULL) {
		switch (rowPPP->type) {
		{
		case FILAPPP_EXPRESSION_FILAPPP:
			releaseFilaPPP(rowPPP->filaPPP);
			releaseExpression(rowPPP->expressionFila);
			break;
		case FILAPPP_EXPRESSION:
			releaseExpression(rowPPP->expression);
			break;
		default:
			logError(_logger, "Unknown row PPP type: %d", rowPPP->type);
			break;
		}
	}
	free(rowPPP);
	}
}

void releaseFilaTabla(FilaTabla * rowTable) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (rowTable != NULL) {
        if(rowTable->type == TABLA_FILA_TABLA) {
			releaseFilaTabla(rowTable->filaTabla);
        	releaseColumnaTabla(rowTable->columnaTablaConFila);
		}
    	else {
    		releaseColumnaTabla(rowTable->columnaTabla);
    	}
	free(rowTable);
	}
}

void releaseColumnaTabla(ColumnaTabla * columnTable) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (columnTable != NULL) {
			if(columnTable->type == COLUMNA_COL) {
				releaseColumnaTabla(columnTable->columnaTabla);
				releaseSimpleExpression(columnTable->expression);
			}
			free(columnTable);
	}
}

void releaseRowNav(FilaNav * rowNav) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (rowNav != NULL) {
		if(rowNav->type == NAVEGADOR_FILA_NAVEGADOR){
			releaseRowNav(rowNav->filaNav);
    }
		free(rowNav->id);
		free(rowNav->string);
		free(rowNav);
	}
}

void releaseSeccion(Seccion * seccion) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (seccion != NULL) {
		if(seccion->type == SECCION_BODY){
			releaseBody(seccion->body);
		}
		free(seccion);
	}
}

void releaseText(Text * text){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(text != NULL) {
		if(text->type == TEXT_SIMPLE_TEXT){
		}
		else if(text->type == TEXT_MODIFIED_TEXT){
			releaseModifiers(text->modifier);
		}
		printf("RELEASING TEXT STRING");
		free(text->string);
		free(text);
	}
	
}

void releaseImg(Image * img) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (img != NULL) {
        free(img->url);
        free(img->alternative);
        free(img);
    }
}

void releaseTitle(Title * title) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (title != NULL) {
        free(title->string);
        free(title);
    }
}

void releaseSubtitle(Subtitle * subtitle) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	printf("RELEASING SUBTITLE STRING");
	if (subtitle != NULL) {
		free(subtitle->string);
		subtitle->string = NULL;
		free(subtitle);
	}
}

void releaseLink(Link * link) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (link != NULL) {
        releaseHref(link->href);
        releaseSimpleExpression(link->simpleExpression);
        free(link);
    }
}

void releaseHref(Href * href) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (href != NULL) {
		free(href->url);
		free(href->id);
		free(href);
	}
}

void releaseTabla(Table * tabla) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (tabla != NULL) {
        if (tabla->type == TABLA_FILA_TABLA) {
            releaseFilaTabla(tabla->filaTabla);
        }
        free(tabla);
    }
}

void releaseFilaNav(FilaNav * filaNav) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (filaNav != NULL) {
        if (filaNav->type == NAVEGADOR_FILA_NAVEGADOR) {
            releaseFilaNav(filaNav->filaNav);
        }
        free(filaNav->id);
        free(filaNav->string);
        free(filaNav);
    }
}



/*   PARA LOGUEAR NADA MAS */

// Helper function for indentation
static void _printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

// Forward declarations for AST logging functions
static void _logAST_Body(Body* body, int indent);
static void _logAST_Expression(Expression* expr, int indent);
static void _logAST_SimpleExpression(SimpleExpression* simpleExpr, int indent);
static void _logAST_ComplexExpression(ComplexExpression* complexExpr, int indent);

/**
 * Log the full Abstract Syntax Tree starting from the program root
 */
void logAbstractSyntaxTree(Program* program) {
    if (program == NULL) {
        printf("AST is NULL\n");
        return;
    }
    
    printf("==== AST BEGIN ====\n");
    printf("Program (type=%d)\n", program->type);
    
    switch (program->type) {
        case PROGRAM_HEADER_FOOTER_BODY:
            printf("  [Header + Body + Footer]\n");
            _logAST_Body(program->bodyFull, 2);
            break;
        case PROGRAM_FOOTER_BODY:
            printf("  [Body + Footer]\n");
            _logAST_Body(program->bodyFB, 2);
            break;
        case PROGRAM_HEADER_BODY:
            printf("  [Header + Body]\n");
            _logAST_Body(program->bodyHB, 2);
            break;
        case PROGRAM_BODY:
            printf("  [Body only]\n");
            _logAST_Body(program->body, 2);
            break;
        default:
            printf("  [Other program structure]\n");
    }
    printf("==== AST END ====\n");
}

static void _logAST_Body(Body* body, int indent) {
    if (body == NULL) {
        _printIndent(indent);
        printf("Body: NULL\n");
        return;
    }
    
    _printIndent(indent);
    printf("Body (type=%d)\n", body->type);
    
    switch (body->type) {
        case BODY_EXPRESSION_BODY:
            _printIndent(indent);
            printf("Has EXPRESSION + BODY\n");
            _logAST_Expression(body->expressionB, indent + 1);
            _logAST_Body(body->bodyB, indent + 1);
            break;
        case BODY_EXPRESSION:
            _printIndent(indent);
            printf("Has EXPRESSION only\n");
            _logAST_Expression(body->expression, indent + 1);
            break;
        default:
            _printIndent(indent + 1);
            printf("Empty body\n");
    }
}

static void _logAST_Expression(Expression* expr, int indent) {
    if (expr == NULL) {
        _printIndent(indent);
        printf("Expression: NULL\n");
        return;
    }
    
    _printIndent(indent);
    printf("Expression (type=%d): ", expr->type);
    
    switch (expr->type) {
        case EXPRESSION_SIMPLE_EXPRESSION:
            printf("SimpleExpression\n");
            _logAST_SimpleExpression(expr->simpleExpression, indent + 1);
            break;
        case EXPRESSION_COMPLEX_EXPRESSION:
            printf("ComplexExpression\n");
            _logAST_ComplexExpression(expr->complexExpression, indent + 1);
            break;
        case EXPRESSION_STRING:
            printf("String: \"%s\"\n", expr->string);
            break;
        case EXPRESSION_ID:
            printf("Component ID: \"%s\"\n", expr->componentId);
            break;
        default:
            printf("Other expression type\n");
    }
}

static void _logAST_SimpleExpression(SimpleExpression* simpleExpr, int indent) {
    if (simpleExpr == NULL) {
        _printIndent(indent);
        printf("SimpleExpression: NULL\n");
        return;
    }
    
    _printIndent(indent);
    printf("SimpleExpression (type=%d): ", simpleExpr->type);
    
    switch (simpleExpr->type) {
        case SEXPRESSION_TEXT:
            printf("Text: \"%s\"\n", simpleExpr->text->string);
            break;
        case SEXPRESSION_TITLE:
            printf("Title: \"%s\"\n", simpleExpr->title->string);
            break;
        case SEXPRESSION_SUBTITLE:
            printf("Subtitle: \"%s\"\n", simpleExpr->subtitle->string);
            break;
        case SEXPRESSION_IMG:
            printf("Image: \"%s\"\n", simpleExpr->img->url);
            break;
        default:
            printf("Other simple expression type\n");
    }
}

static void _logAST_ComplexExpression(ComplexExpression* complexExpr, int indent) {
    if (complexExpr == NULL) {
        _printIndent(indent);
        printf("ComplexExpression: NULL\n");
        return;
    }
    
    _printIndent(indent);
    printf("ComplexExpression (type=%d): ", complexExpr->type);
    
    switch (complexExpr->type) {
        case CEXPRESSION_PUNTO_POR_PUNTO:
            printf("Punto Por Punto\n");
            break;
        case CEXPRESSION_SECCION:
            printf("Section\n");
            break;
        case CEXPRESSION_TABLA:
            printf("Table\n");
            break;
        case CEXPRESSION_NAVEGADOR:
            printf("Navigator\n");
            break;
        default:
            printf("Other complex expression type\n");
    }
}