#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

typedef enum ProgramType ProgramType;
typedef enum HeaderFooterType HeaderFooterType;
typedef enum BodyType BodyType;
typedef enum ExpressionType ExpressionType;
typedef enum SimpleExpressionType SimpleExpressionType;
typedef enum ComplexExpressionType ComplexExpressionType;
typedef enum TextType TextType;
typedef enum ColorType ColorType;
typedef enum ModifierType ModifierType;
typedef enum HrefType HrefType;
typedef enum PuntoPorPuntoType PuntoPorPuntoType;
typedef enum SeccionType SeccionType;
typedef enum TablaType TablaType;
typedef enum NavegadorType NavegadorType;
typedef enum FilaPPPType FilaPPPType;
typedef enum ImgType ImgType;
typedef enum TitleType TitleType;
typedef enum SubtitleType SubtitleType;
typedef enum LinkType LinkType;
typedef enum FilaNavType FilaNavType;
typedef enum FilaTablaType FilaTablaType;
typedef enum ColumnaTablaType ColumnaTablaType;
typedef enum ComponentType ComponentType;


typedef struct Header Header;
typedef struct Footer Footer;
typedef struct Body Body;
typedef struct Program Program;
typedef struct Expression Expression;
typedef struct SimpleExpression SimpleExpression;
typedef struct Text Text;
typedef struct Img Img;
typedef struct Title Title;
typedef struct Subtitle Subtitle;
typedef struct Link Link;
typedef struct Component Component;
typedef struct Modifier Modifier;
typedef struct ComplexExpression ComplexExpression;
typedef struct Href Href;
typedef struct Seccion Seccion;
typedef struct Tabla Tabla;
typedef struct Navegador Navegador;
typedef struct PuntoPorPunto PuntoPorPunto;
typedef struct FilaPPP FilaPPP;
typedef struct FilaNav FilaNav;
typedef struct FilaTabla FilaTabla;
typedef struct ColumnaTabla ColumnaTabla;


/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum BodyType {
	EXPRESSION,
	EXPRESSION_BODY
};

enum HeaderFooterType {
	BODY,
	EMPTY
};

enum ProgramType {
	EMPTY,
	HEADER, 
	FOOTER,
	BODY,
	HEADER_FOOTER,
	HEADER_BODY,
	FOOTER_BODY,
	HEADER_FOOTER_BODY
};

enum PuntoPorPuntoType {
	FILA_PUNTO_POR_PUNTO,
	EMPTY
};

enum ExpressionType {
	SIMPLE_EXPRESSION,
	COMPLEX_EXPRESSION,
	COMPONENTE
};

enum SimpleExpressionType {
	TEXT,
	IMG,
	TITLE,
	SUBTITLE,
	LINK
};

enum ComponentType {
	COMPONENT,
	EMPTY
};

enum ComplexExpressionType {
	PUNTO_POR_PUNTO,
	SECCION,
	TABLA,
	NAVEGADOR
};

enum ModifierType {
	COLOR_MOD,
	EMPTY,
	MODIFIER
};

enum SeccionType {
	EMPTY,
	BODY
};

enum TablaType {
	EMPTY,
	FILA_TABLA
};
enum FilaTablaType {
	COL,
	FILA_TABLA
};

enum NavegadorType {
	EMPTY,
	FILA_NAVEGADOR
};

enum FilaNavType {
	FILA_NAVEGADOR,
	EMPTY
};

enum ColumnaTablaType {
	COL,
	FIN_FILA
};

enum ColorType {
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	PINK,
	PURPLE,
	BLACK,
	WHITE
};
enum TextType {
	MODIFIED_TEXT,
	SIMPLE_TEXT
};

enum HrefType {
	URL,
	ID
};
enum ComplexExpressionType {
	SECCION,
	TABLA,
	NAVEGADOR,
	PUNTO_POR_PUNTO
};

enum FilaPPPType {
	EXPRESSION_FILAPPP,
	EXPRESSION
};

enum FilaNavType {
	FILA_NAVEGADOR,
	SIMPLE
};


///TODO esto esta incompleto, no se que hacer con las variabe.
struct Expression {
	union {
		char * string;
		// Previously declared component
		Body * componentId;
		struct {
			SimpleExpression * simpleExpression;
			char * id; //Esto sería variable. 
		};
		struct{
			ComplexExpression * complexExpression;
			char * id;
		};
		SimpleExpression * simpleExpression;
		ComplexExpression * complexExpression;

		//Component declaration
		Component * component;
	};
	ExpressionType type;
};

struct SimpleExpression{
	union{
		Text * text;
		Img * img;
		Title * title;
		Subtitle * subtitle;
		Link * link;
	};
	SimpleExpressionType type;
};

struct ComplexExpression{
	union{
		PuntoPorPunto * puntoPorPunto;
		Seccion * seccion;
		Tabla * tabla;
		Navegador * navegador;
	};
	ComplexExpressionType type;
};

struct Seccion{
	union {
		Body * body;
		struct{};
	};
	SeccionType type;
};

struct Navegador{
	union {
		FilaNav * filaNav;
		struct{};
	};
	NavegadorType type;
};

struct PuntoPorPunto{
	union{
		FilaPPP * filaPPP;
		struct{};
	};
	PuntoPorPuntoType type;
};

struct FilaPPP{
	union{
		struct {
			FilaPPP * filaPPP;
			Expression * expression;
		};
		Expression * expression;
	};
	FilaPPPType type;
};

struct Tabla{
	union{
		struct{};
		FilaTabla * filaTabla;
	};
	TablaType type;
};

struct FilaTabla{
	union{
		struct {
			FilaTabla * filaTabla;
			ColumnaTabla * columnaTabla;
		};
		ColumnaTabla * columnaTabla;
	};
	FilaTablaType type;
};

struct ColumnaTabla{
	union{
		struct {
			ColumnaTabla * columnaTabla;
			SimpleExpression * expression;
		};
		struct{};
	};
	ColumnaTablaType type;
};

struct FilaNav{
	union{
		struct{
			char * string;
			char * id;		
			FilaNav * filaNav;
		};
		struct{
			char * string;
			char * id;
		};
	};
	FilaNavType type;
};

//Ahhh creo que lo que habias hechoe staba bien. En el FilaNav

struct Component{
	union{
		Body * body;
		struct{};
	};
	char * id;
	ComponentType type;
};


struct Img {
	
		char * url; //Variable
		char * alternative;
	
};
struct Title {
		char * string;
};

struct Subtitle {
		char * string;
};



struct Link {
		char * href; //Variable
		SimpleExpression * simpleExpression;
};

struct Href{
		char * url;//variable*/
		char * id;//variable*/
	HrefType type;
};

struct Text {
	union {
		struct {
			char * string;
			Modifier * modifier;
		};
		char * string;
	};
	TextType type;
};


struct Header {
	union {
		Body * body;
		struct{};
	};
	HeaderFooterType type;
};

struct Footer {
	union {
		Body * body;
		struct{};
	};
	HeaderFooterType type;
};

struct Body {
	union {
		Expression * expression;
		struct {
			Expression * expression;
			Body * body;
		};
	};
	BodyType type;
};

struct Modifier {
	union {
		struct {
			ColorType * color;
			Modifier * modifier;
		};
		struct {
			Modifier * modifier;
		};
		struct{};
	};
	ModifierType type;
};


struct Program {
	union{
		// Header, Body and Footer
		struct {
			Header * header;
			Body * body;
			Footer * footer;	
		};
		//Footer and Body
		struct{
			Body * body;
			Footer * footer;
		};
		// Header and Body
		struct{
			Header * header;
			Body * body;
		};
		// Header and Footer
		struct{
			Header * header;
			Footer * footer;
		};
		// Header
		struct{
			Header * header;
		};
		// Body
		struct{
			Body * body;
		};
		//Footer
		struct{
			Footer * footer;
		};
		// Empty
		struct{};
	};
	ProgramType type;
};

/**
 * Node recursive destructors.
 */
void releaseExpression(Expression * expression);
void releaseSimpleExpression(SimpleExpression * simpleExpression);
void releaseComplexExpression(ComplexExpression * complexExpression);
void releaseSeccion(Seccion * seccion);
void releaseTabla(Tabla * tabla);
void releaseNavegador(Navegador * navegador);
void releasePuntoPorPunto(PuntoPorPunto * puntoPorPunto);
void releaseFilaPPP(FilaPPP * filaPPP);
void releaseFilaTabla(FilaTabla * filaTabla);
void releaseColumnaTabla(ColumnaTabla * columnaTabla);
void releaseFilaNav(FilaNav * filaNav);
void releaseComponent(Component * component);
void releaseImg(Img * img);
void releaseTitle(Title * title);
void releaseSubtitle(Subtitle * subtitle);
void releaseLink(Link * link);
void releaseHref(Href * href);
void releaseText(Text * text);
void releaseModifiers(Modifier * modifiers);
void releaseHeader(Header * header);
void releaseFooter(Footer * footer);
void releaseBody(Body * body);
void releaseProgram(Program * program);


#endif
