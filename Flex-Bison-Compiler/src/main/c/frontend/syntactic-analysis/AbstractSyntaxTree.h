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
typedef enum TableType TableType;
typedef enum NavegadorType NavegadorType;
typedef enum FilaPPPType FilaPPPType;
typedef enum ImageType ImageType;
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
typedef struct Image Image;
typedef struct Title Title;
typedef struct Subtitle Subtitle;
typedef struct Link Link;
typedef struct Component Component;
typedef struct Modifier Modifier;
typedef struct ComplexExpression ComplexExpression;
typedef struct Href Href;
typedef struct Seccion Seccion;
typedef struct Table Table;
typedef struct Navegador Navegador;
typedef struct PuntoPorPunto PuntoPorPunto;
typedef struct FilaPPP FilaPPP;
typedef struct FilaNav FilaNav;
typedef struct FilaTabla FilaTabla;
typedef struct ColumnaTabla ColumnaTabla;


/**
 * Node types for the Abstract Syntax Tree (AST).
 */

 //NODENAME_TYPE1_TYPE2_TYPE3_...

enum BodyType {
	BODY_EXPRESSION,
	BODY_EXPRESSION_BODY,
	BODY_EMPTY
};

enum HeaderFooterType {
	HEADER_BODY,
	HEADER_EMPTY
};

enum ProgramType {
	PROGRAM_EMPTY,
	PROGRAM_HEADER, 
	PROGRAM_FOOTER,
	PROGRAM_BODY,
	PROGRAM_HEADER_FOOTER,
	PROGRAM_HEADER_BODY,
	PROGRAM_FOOTER_BODY,
	PROGRAM_HEADER_FOOTER_BODY
};

enum PuntoPorPuntoType {
	PPP_FILA_PUNTO_POR_PUNTO,
	PPP_EMPTY
};

enum ExpressionType {
	EXPRESSION_SIMPLE_EXPRESSION,
	EXPRESSION_COMPLEX_EXPRESSION,
	EXPRESSION_COMPONENTE,
	EXPRESSION_STRING,
	EXPRESSION_ID,
	EXPRESSION_ID_SIMPLEEXPRESSION,
	EXPRESSION_ID_COMPLEXEXPRESSION
};

enum SimpleExpressionType {
	SEXPRESSION_TEXT,
	SEXPRESSION_IMG,
	SEXPRESSION_TITLE,
	SEXPRESSION_SUBTITLE,
	SEXPRESSION_LINK
};

enum ComponentType {
	COMPONENT_COMPONENT,
	COMPONENT_EMPTY
};

enum ComplexExpressionType {
	CEXPRESSION_PUNTO_POR_PUNTO,
	CEXPRESSION_SECCION,
	CEXPRESSION_TABLA,
	CEXPRESSION_NAVEGADOR
};

enum ModifierType {
	MODIFIER_COLOR_MOD,
	MODIFIER_EMPTY,
	MODIFIER_MODIFIER
};

enum SeccionType {
	SECCION_EMPTY,
	SECCION_BODY
};

enum TableType {
	TABLA_EMPTY,
	TABLA_FILA_TABLA
};
enum FilaTablaType {
	FILA_COL,
	FILA_FILA_TABLA
};

enum NavegadorType {
	NAVEGADOR_EMPTY,
	NAVEGADOR_FILA_NAVEGADOR
};


enum ColumnaTablaType {
	COLUMNA_COL,
	COLUMNA_FIN_FILA
};

enum ColorType {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_PINK,
	COLOR_PURPLE,
	COLOR_BLACK,
	COLOR_WHITE
};
enum TextType {
	TEXT_MODIFIED_TEXT,
	TEXT_SIMPLE_TEXT
};

enum HrefType {
	HREF_URL,
	HREF_ID
};


enum FilaPPPType {
	FILAPPP_EXPRESSION_FILAPPP,
	FILAPPP_EXPRESSION
};

enum FilaNavType {
	FILANAV_FILA_NAVEGADOR,
	FILANAV_SIMPLE
};


///TODO esto esta incompleto, no se que hacer con las variabe.
struct Expression {
	union {
		char * string;
		// Previously declared component
		char * componentId;
		/*char * id; //Esto sería variable.*/
		struct {
			SimpleExpression * simpleExpressionId;
			char * simpleId; //Esto sería variable. 
		};
		struct{
			ComplexExpression * complexExpressionId;
			char * complexId;
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
		Image * img;
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
		Table * tabla;
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
			Expression * expressionFila;
		};
		Expression * expression;
	};
	FilaPPPType type;
};

struct Table{
	union{
		struct{};
		FilaTabla * filaTabla;
	};
	TableType type;
};

struct FilaTabla{
	union{
		struct {
			FilaTabla * filaTabla;
			ColumnaTabla * columnaTablaConFila;
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
	char * string;
	char * id;		
	union{
		struct{	
			FilaNav * filaNav;
		};
		struct{
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


struct Image {
	
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
		Href * href; //Variable
		SimpleExpression * simpleExpression;
};

struct Href{
		char * url;//variable*/
		char * id;//variable*/
	HrefType type;
};

struct Text {
			char * string;
			Modifier * modifier;
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
	Expression * expression;
	Body * body;

	BodyType type;
};

struct Modifier {
	union {
		struct {
			ColorType  color;
			Modifier * modifierWithColor;
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
			Header * headerFull;
			Body * bodyFull;
			Footer * footerFull;	
		};
		//Footer and Body (FAB)
		struct{
			Body * bodyFB;
			Footer * footerFB;
		};
		// Header and Body (HAB)
		struct{
			Header * headerHB;
			Body * bodyHB;
		};
		// Header and Footer (HF)
		struct{
			Header * headerHF;
			Footer * footerHF;
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
void releaseTabla(Table * tabla);
void releaseNavegador(Navegador * navegador);
void releasePuntoPorPunto(PuntoPorPunto * puntoPorPunto);
void releaseFilaPPP(FilaPPP * filaPPP);
void releaseFilaTabla(FilaTabla * filaTabla);
void releaseColumnaTabla(ColumnaTabla * columnaTabla);
void releaseFilaNav(FilaNav * filaNav);
void releaseComponent(Component * component);
void releaseImg(Image * img);
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
