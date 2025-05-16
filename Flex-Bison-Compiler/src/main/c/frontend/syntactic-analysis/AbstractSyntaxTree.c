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
			case HEADER_FOOTER_BODY:
				releaseHeader(program->header);
				releaseBody(program->body);
				releaseFooter(program->footer);
				break;
			case FOOTER_BODY:
				releaseBody(program->body);
				releaseFooter(program->footer);
				break;
			case HEADER_BODY:
				releaseHeader(program->header);
				releaseBody(program->body);
				break;
			case HEADER_FOOTER:
				releaseHeader(program->header);
				releaseFooter(program->footer);
				break;
			case BODY:
				releaseBody(program->body);
				break;
			case FOOTER:
				releaseFooter(program->footer);
				break;
			case HEADER:
				releaseHeader(program->header);
				break;
		}
		free(program);
	}
}


void releaseExpression(Expression * expression){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case SIMPLE_EXPRESSION:
				releaseSimpleExpression(expression->simpleExpression);
				break;
			case COMPLEX_EXPRESSION:
				releaseComplexExpression(expression->complexExpression);
				break;
			case COMPONENTE:
				releaseComponent(expression->component);
				break;
		}
		free(expression); //Asumo que esto libera todo el resto que queda...
	}
}

void releaseHeader(Header * header) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (header != NULL) {
        if (header->type == BODY) {
            releaseBody(header->body);
        }
        free(header);
    }
}

void releaseFooter(Footer * footer) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (footer != NULL) {
        if (footer->type == BODY) {
            releaseBody(footer->body);
        }
        free(footer);
    }
}

void releaseBody(Body * body) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (body != NULL) {
        if (body->type == EXPRESSION_BODY) {
            releaseExpression(body->expression);
            releaseBody(body->body);
        } else if (body->type == EXPRESSION) {
            releaseExpression(body->expression);
        }
        free(body);
    }
}

void releaseSimpleExpression(SimpleExpression * simpleExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (simpleExpression != NULL) {
        switch (simpleExpression->type) {
            case TEXT:
                releaseText(simpleExpression->text);
                break;
            case IMG:
                releaseImg(simpleExpression->img);
                break;
            case TITLE:
                releaseTitle(simpleExpression->title);
                break;
            case SUBTITLE:
                releaseSubtitle(simpleExpression->subtitle);
                break;
            case LINK:
                releaseLink(simpleExpression->link);
                break;
        }
        free(simpleExpression);
    }
}

void releaseModifiers(Modifier * modifiers) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (modifiers != NULL) {
		switch (modifiers->type) {
		case COLOR_MOD:
			free(modifiers->color);
			free(modifiers->modifier);
			break;
		case MODIFIER:
			free(modifiers->modifier);
			break;
		}
		free(modifiers);
	}
}


void releaseComponent(Component * component) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (component != NULL) {
        if(component->type == COMPONENT) {
			releaseComponent(component->body);
    }
}
		free(component->id);
		free(component);
}

void releaseComplexExpression(ComplexExpression * complexExpression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (complexExpression != NULL) {
        switch (complexExpression->type) {
            case PUNTO_POR_PUNTO:
                releaseRowPPP(complexExpression->puntoPorPunto);
                break;
            case SECCION:
                releaseSeccion(complexExpression->seccion);
                break;
            case TABLA:
                releaseTabla(complexExpression->tabla);
                break;
            case NAVEGADOR:
                releaseNavegador(complexExpression->navegador);
                break;
        }
        free(complexExpression);
    }
}

void releaseNavegador(Navegador * navegador) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (navegador != NULL) {
		if(navegador->type == FILA_NAVEGADOR){
			releaseRowNav(navegador->filaNav);
		}
		free(navegador);
	}
}

void releaseFilaPPP(FilaPPP * rowPPP) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (rowPPP != NULL) {
       if(rowPPP->type == FILA_PUNTO_POR_PUNTO){
			releaseFilaPPP(rowPPP->filaPPP);
    }
		releaseExpression(rowPPP->expression);
		free(rowPPP);
	}
}

void releaseFilaTabla(FilaTabla * rowTable) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (rowTable != NULL) {
        if(rowTable->type == FILA_TABLA) {
			releaseFilaTabla(rowTable->filaTabla);
			releaseColumnaTabla(rowTable->columnaTabla);
		} else if (rowTable->type == COL) {
			releaseColumnaTabla(rowTable->columnaTabla);
		}
    }
	free(rowTable);
}

void releaseColumnaTabla(ColumnaTabla * columnTable) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (columnTable != NULL) {
			if(columnTable->type == COL) {
				releaseColumnaTabla(columnTable->columnaTabla);
				releaseSimpleExpression(columnTable->expression);
			}
	}
	free(columnTable);
}

void releaseRowNav(FilaNav * rowNav) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (rowNav != NULL) {
		if(rowNav->type == FILA_NAVEGADOR){
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
		if(seccion->type == BODY){
			releaseBody(seccion->body);
		}
		free(seccion);
	}
}

void releaseText(Text * text){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if(text != NULL) {
		if(text->type == SIMPLE_TEXT){
			free(text->string);
		}
		else if(text->type == MODIFIED_TEXT){
			releaseModifiers(text->modifier);
			free(text->string);
		}
	}
	free(text);
}

void releaseImg(Img * img) {
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
    if (subtitle != NULL) {
        free(subtitle->string);
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
