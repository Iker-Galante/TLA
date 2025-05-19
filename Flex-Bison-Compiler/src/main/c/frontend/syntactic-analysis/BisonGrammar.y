%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	char * string;
  char * id;
	Token token;

	/** Non-terminals. */

	Program * program;
	Header * header;
	Body * body;
	Footer * footer;
	SimpleExpression * simple_expression;
       ComplexExpression * complex_expression;
       Expression * expression;
	Modifier * modifiers;
	Component * component;
	FilaPPP * row_ppp;
	FilaTabla * row_table;
	ColumnaTabla * column_table;
	FilaNav * row_nav;
	Seccion * section;
	Text * text;
	Image * image;
	Title * title;
	Subtitle * subtitle;
       Link * link;
       Navegador * navigator;
       Table * table;
       Href * href;
       PuntoPorPunto * puntoPorPunto;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

%destructor { releaseProgram($$); } <program>
%destructor { releaseHeader($$); } <header>
%destructor { releaseFooter($$); } <footer>
%destructor { releaseBody($$); } <body>
%destructor { releaseSimpleExpression($$); } <simple_expression>
%destructor { releaseComplexExpression($$); } <complex_expression>
%destructor { releaseComponent($$); } <component>
%destructor { releaseFilaPPP($$); } <row_ppp>
%destructor { releaseFilaTabla($$); } <row_table>
%destructor { releaseColumnaTabla($$); } <column_table>
%destructor { releaseFilaNav($$); } <row_nav>
%destructor { releaseSeccion($$); } <section>
%destructor { releaseText($$); } <text>
%destructor { releaseImg($$); } <image>
%destructor { releaseTitle($$); } <title>
%destructor { releaseSubtitle($$); } <subtitle>
%destructor { releaseLink($$); } <link>
%destructor { releaseNavegador($$); } <navigator>
%destructor { releaseTabla($$); } <table>
%destructor { releaseHref($$); } <href>
%destructor { releasePuntoPorPunto($$); } <puntoPorPunto>

/** Terminals. */
%token <string> STRING
%token <id> ID
%token <token> PRINCIPIO FIN ENCABEZADO PIE TEXTO IMAGEN TITULO SUBTITULO ENLACE COLOR SUBRAYADO ITALICA NEGRITA TAMANIO
%token <token> PUNTO_POR_PUNTO SECCION TABLA NAVEGADOR COMPONENTE FIN_NAVEGADOR
%token <token> INICIO_TABLA FIN_TABLA INICIO_SECCION FIN_SECCION FIN_ENCABEZADO FIN_PIE FIN_COMPONENTE FIN_FILA FIN_PPP FIN_NAV
%token <token> UNKNOWN NEW_LINE DOS_PUNTOS GUION PARENTESIS_IZQUIERDO PARENTESIS_DERECHO LLAVE_IZQUIERDA LLAVE_DERECHA NUMERAL

/** Non-terminals. */
%type <program> program
%type <header> header
%type <body> body
%type <footer> footer
%type <expression> expression
%type <simple_expression> simple_expression
%type <modifiers> modifiers
%type <component> component
%type <complex_expression> complex_expression
%type <row_ppp> row_ppp
%type <row_table> row_table
%type <column_table> column_table
%type <row_nav> row_nav
%type <section> section
%type <text> text
%type <image> image
%type <title> title
%type <subtitle> subtitle
%type <link> link
%type <navigator> navigator
%type <table> table
%type <href> href
%type <puntoPorPunto> puntoPorPunto

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
// TODO CHECK IF THIS IS OK!

program:
    PRINCIPIO NEW_LINE header body footer FIN        { $$ = ProgramSemanticAction($4, $3, $5, PROGRAM_HEADER_FOOTER_BODY,currentCompilerState()); }
  | PRINCIPIO NEW_LINE body footer FIN               { $$ = ProgramSemanticAction($3, NULL, $4, PROGRAM_FOOTER_BODY,currentCompilerState()); }
  | PRINCIPIO NEW_LINE header footer FIN             { $$ = ProgramSemanticAction(NULL, $3, $4, PROGRAM_HEADER_FOOTER,currentCompilerState()); }
  | PRINCIPIO NEW_LINE header body FIN               { $$ = ProgramSemanticAction($4, $3, NULL, PROGRAM_HEADER_BODY,currentCompilerState()); }
  | PRINCIPIO NEW_LINE footer FIN                    { $$ = ProgramSemanticAction(NULL, NULL, $3, PROGRAM_FOOTER,currentCompilerState()); }
  | PRINCIPIO NEW_LINE body FIN                      { $$ = ProgramSemanticAction($3, NULL, NULL, PROGRAM_BODY,currentCompilerState()); }
  | PRINCIPIO NEW_LINE header FIN                    { $$ = ProgramSemanticAction(NULL,$3, NULL, PROGRAM_HEADER,currentCompilerState()); }
  | PRINCIPIO NEW_LINE FIN                           { $$ = ProgramSemanticAction(NULL, NULL, NULL, PROGRAM_EMPTY,currentCompilerState()); }
  ;

header:
    ENCABEZADO DOS_PUNTOS NEW_LINE body FIN_ENCABEZADO NEW_LINE     { $$ = HeaderSemanticAction($4, HEADER_BODY); }
  | ENCABEZADO DOS_PUNTOS NEW_LINE FIN_ENCABEZADO NEW_LINE           { $$ = HeaderSemanticAction(NULL, HEADER_EMPTY); }
  ;

footer:
    PIE DOS_PUNTOS NEW_LINE body FIN_PIE NEW_LINE                    { $$ = FooterSemanticAction($4, HEADER_BODY); }
  | PIE DOS_PUNTOS NEW_LINE FIN_PIE NEW_LINE                         { $$ = FooterSemanticAction(NULL, HEADER_EMPTY); }
  ;

body:
    expression body                                   { $$ = BodySemanticAction($1, $2, BODY_EXPRESSION_BODY); }
  | expression                                        { $$ = BodySemanticAction($1, NULL, BODY_EXPRESSION); }
  ;

expression:
    PARENTESIS_IZQUIERDO ID PARENTESIS_DERECHO simple_expression                      { $$ = ExpressionSemanticAction($2, NULL, NULL, $4, NULL, EXPRESSION_ID_SIMPLEEXPRESSION); }
  | PARENTESIS_IZQUIERDO ID PARENTESIS_DERECHO complex_expression                     { $$ = ExpressionSemanticAction($2, NULL, $4, NULL, NULL, EXPRESSION_ID_COMPLEXEXPRESSION); }
  | STRING                                            { $$ = ExpressionSemanticAction(NULL, $1, NULL, NULL, NULL, EXPRESSION_STRING); }
  | simple_expression                                 { $$ = ExpressionSemanticAction(NULL, NULL, NULL, $1, NULL, EXPRESSION_SIMPLE_EXPRESSION); }
  | LLAVE_IZQUIERDA ID LLAVE_DERECHA                                        { $$ = ExpressionSemanticAction($2, NULL, NULL, NULL, NULL, EXPRESSION_ID); }
  | component                                         { $$ = ExpressionSemanticAction(NULL, NULL, NULL, NULL, $1, EXPRESSION_COMPONENTE); }
  | complex_expression                                { $$ = ExpressionSemanticAction(NULL, NULL, $1, NULL, NULL, EXPRESSION_COMPLEX_EXPRESSION); }
  ;

simple_expression:
    text                                              { $$ = SimpleExpressionSemanticAction($1, NULL, NULL, NULL, NULL, SEXPRESSION_TEXT); }
  | image                                             { $$ = SimpleExpressionSemanticAction(NULL, $1, NULL, NULL, NULL, SEXPRESSION_IMG); }
  | title                                             { $$ = SimpleExpressionSemanticAction(NULL, NULL, $1, NULL, NULL, SEXPRESSION_TITLE); }
  | subtitle                                          { $$ = SimpleExpressionSemanticAction(NULL, NULL, NULL, $1, NULL, SEXPRESSION_SUBTITLE); }
  | link                                              { $$ = SimpleExpressionSemanticAction(NULL, NULL, NULL, NULL, $1, SEXPRESSION_LINK); }
  ;

modifiers:
    modifiers COLOR                                   { $$ = ModifierSemanticAction($1, COLOR_RED, MODIFIER_COLOR_MOD); }
  | modifiers SUBRAYADO                               { $$ = ModifierSemanticAction($1, COLOR_GREEN, MODIFIER_COLOR_MOD); }
  | modifiers ITALICA                                 { $$ = ModifierSemanticAction($1, COLOR_BLUE, MODIFIER_COLOR_MOD); }
  | modifiers NEGRITA                                 { $$ = ModifierSemanticAction($1, COLOR_YELLOW, MODIFIER_COLOR_MOD); }
  | modifiers TAMANIO                                 { $$ = ModifierSemanticAction($1, COLOR_ORANGE, MODIFIER_COLOR_MOD); }
  | COLOR                                             { $$ = ModifierSemanticAction(NULL, COLOR_RED, MODIFIER_COLOR_MOD); }
  | SUBRAYADO                                         { $$ = ModifierSemanticAction(NULL, COLOR_GREEN, MODIFIER_COLOR_MOD); }
  | ITALICA                                           { $$ = ModifierSemanticAction(NULL, COLOR_BLUE, MODIFIER_COLOR_MOD); }
  | NEGRITA                                           { $$ = ModifierSemanticAction(NULL, COLOR_YELLOW, MODIFIER_COLOR_MOD); }
  | TAMANIO                                           { $$ = ModifierSemanticAction(NULL, COLOR_ORANGE, MODIFIER_COLOR_MOD); }
  ;

component:
    COMPONENTE PARENTESIS_IZQUIERDO ID PARENTESIS_DERECHO NEW_LINE body FIN_COMPONENTE NEW_LINE     { $$ = ComponentSemanticAction($3, $6, COMPONENT_COMPONENT); }
  | COMPONENTE PARENTESIS_IZQUIERDO ID PARENTESIS_DERECHO NEW_LINE FIN_COMPONENTE NEW_LINE          { $$ = ComponentSemanticAction($3, NULL, COMPONENT_EMPTY); }
  ;

complex_expression:
    section                                     { $$ = ComplexExpressionSemanticAction($1, NULL, NULL, NULL, CEXPRESSION_SECCION); }
  | table                                       { $$ = ComplexExpressionSemanticAction(NULL, $1, NULL, NULL, CEXPRESSION_TABLA); }
  | navigator                                   { $$ = ComplexExpressionSemanticAction(NULL, NULL, $1, NULL, CEXPRESSION_NAVEGADOR); }
  | puntoPorPunto                               { $$ = ComplexExpressionSemanticAction(NULL, NULL, NULL, $1, CEXPRESSION_PUNTO_POR_PUNTO); }
  ;

row_ppp:
    GUION expression NEW_LINE row_ppp                  { $$ = FilaPPPSemanticAction($2, $4, FILAPPP_EXPRESSION_FILAPPP); }
  | GUION expression NEW_LINE                         { $$ = FilaPPPSemanticAction($2, NULL, FILAPPP_EXPRESSION); }
  ;

row_table:
    column_table row_table                  { $$ = FilaTablaSemanticAction($1, $2, FILA_FILA_TABLA); }
  | column_table                            { $$ = FilaTablaSemanticAction($1, NULL, FILA_COL); }
  ;

column_table:
    simple_expression column_table          { $$ = ColumnaTablaSemanticAction($1, $2, COLUMNA_COL); }
  | FIN_FILA                                { $$ = ColumnaTablaSemanticAction(NULL, NULL, COLUMNA_FIN_FILA); }
  ;

row_nav:
    GUION PARENTESIS_IZQUIERDO ID PARENTESIS_DERECHO STRING NEW_LINE row_nav  { $$ = FilaNavSemanticAction($3, $5, $7, FILANAV_FILA_NAVEGADOR); }
  | GUION PARENTESIS_IZQUIERDO ID PARENTESIS_DERECHO STRING NEW_LINE          { $$ = FilaNavSemanticAction($3, $5, NULL, FILANAV_SIMPLE); }
  ;

section:
    INICIO_SECCION DOS_PUNTOS NEW_LINE body FIN_SECCION NEW_LINE      { $$ = SeccionSemanticAction($4, SECCION_BODY); }
  | INICIO_SECCION DOS_PUNTOS NEW_LINE FIN_SECCION NEW_LINE           { $$ = SeccionSemanticAction(NULL, SECCION_EMPTY); }
  ;

text:
       TEXTO DOS_PUNTOS STRING NEW_LINE                               { $$ = TextSemanticAction($3,NULL, TEXT_SIMPLE_TEXT); }
       | TEXTO DOS_PUNTOS STRING modifiers NEW_LINE                   { $$ = TextSemanticAction($3, $4, TEXT_MODIFIED_TEXT); }

image:
       IMAGEN DOS_PUNTOS STRING NEW_LINE                              { $$ = ImgSemanticAction($4,NULL); };

title:
       TITULO DOS_PUNTOS STRING NEW_LINE                              { $$ = TitleSemanticAction($3); }

subtitle:
       SUBTITULO DOS_PUNTOS STRING NEW_LINE                           { $$ = SubtitleSemanticAction($4); }

link:
       ENLACE DOS_PUNTOS PARENTESIS_IZQUIERDO href PARENTESIS_DERECHO simple_expression NEW_LINE  { $$ = LinkSemanticAction($4, $6); }

navigator:
    NAVEGADOR DOS_PUNTOS NEW_LINE row_nav FIN_NAVEGADOR NEW_LINE      { $$ = NavegadorSemanticAction($4, NAVEGADOR_FILA_NAVEGADOR); }
  | NAVEGADOR DOS_PUNTOS NEW_LINE FIN_NAVEGADOR NEW_LINE              { $$ = NavegadorSemanticAction(NULL, NAVEGADOR_EMPTY); }
  ;

table:
    INICIO_TABLA DOS_PUNTOS NEW_LINE row_table FIN_TABLA NEW_LINE     { $$ = TablaSemanticAction($4, TABLA_FILA_TABLA); }
  | INICIO_TABLA DOS_PUNTOS NEW_LINE FIN_TABLA NEW_LINE               { $$ = TablaSemanticAction(NULL, TABLA_EMPTY); }
  ;

href:
    STRING                    { $$ = HrefSemanticAction($1, NULL, HREF_URL); }
  | NUMERAL ID                { $$ = HrefSemanticAction(NULL, $2, HREF_ID); }
  ;

puntoPorPunto:
    PUNTO_POR_PUNTO DOS_PUNTOS NEW_LINE row_ppp FIN_PPP NEW_LINE      { $$ = PuntoPorPuntoSemanticAction($4, PPP_FILA_PUNTO_POR_PUNTO); }
  | PUNTO_POR_PUNTO DOS_PUNTOS NEW_LINE FIN_PPP NEW_LINE              { $$ = PuntoPorPuntoSemanticAction(NULL, PPP_EMPTY); }
  ;


%%
