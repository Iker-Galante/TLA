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

//TODO CHEQUEAR TODO ESTO PORQUE ME QUEME MAL Y DEJE ERRORES. 

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
			releaseModifiers(modifiers->modifier);
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
