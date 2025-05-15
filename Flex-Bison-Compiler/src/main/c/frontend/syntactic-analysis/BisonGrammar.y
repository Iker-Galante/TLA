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
	Modifiers * modifiers;
	Component * component;
	ComplexExpression * complex_expression;
	RowPPP * row_ppp;
	RowTable * row_table;
	ColumnTable * column_table;
	RowNav * row_nav;
	Section * section;
	Expression * expression;
	Text * text;
	Image * image;
	Title * title;
	Subtitle * subtitle;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>

/** Terminals. */
%token <string> STRING
%token <id> ID
%token <token> PRINCIPIO FIN ENCABEZADO PIE TEXTO IMAGEN TITULO SUBTITULO ENLACE COLOR SUBRAYADO ITALICA NEGRITA TAMANIO
%token <token> PUNTO_POR_PUNTO SECCION TABLA NAVEGADOR COMPONENTE
%token <token> INICIO_TABLA FIN_TABLA INICIO_SECCION FIN_SECCION FIN_ENCABEZADO FIN_PIE FIN_COMPONENTE FIN_FILA FIN_PPP FIN_NAV
%token <token> UNKNOWN

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

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.


program: PRINCIPIO header body footer FIN { $$ = ProgramSemanticAction($2, $3, $4); }
       | PRINCIPIO body footer FIN { $$ = ProgramSemanticAction(NULL, $2, $3); }
       | PRINCIPIO header footer FIN { $$ = ProgramSemanticAction($2, NULL, $3); }
       | PRINCIPIO header body FIN { $$ = ProgramSemanticAction($2, $3, NULL); }
       | PRINCIPIO footer FIN { $$ = ProgramSemanticAction(NULL, NULL, $2); }
       | PRINCIPIO body FIN { $$ = ProgramSemanticAction(NULL, $2, NULL); }
       | PRINCIPIO header FIN { $$ = ProgramSemanticAction($2, NULL, NULL); }
       | PRINCIPIO FIN { $$ = ProgramSemanticAction(NULL, NULL, NULL); }
       ;

header: ENCABEZADO ':' '\n' body FIN_ENCABEZADO '\n' { $$ = HeaderSemanticAction($4); }
      | ENCABEZADO ':' '\n' FIN_ENCABEZADO '\n' { $$ = HeaderSemanticAction(NULL); }
      ;

footer: PIE ':' '\n' body FIN_PIE '\n' { $$ = FooterSemanticAction($4); }
      | PIE ':' '\n' FIN_PIE '\n' { $$ = FooterSemanticAction(NULL); }
      ;

body: expression body { $$ = BodySemanticAction($1, $2); }
    | expression { $$ = BodySemanticAction($1, NULL); }
    ;

expression: '(' ID ')' simple_expression { $$ = IdSimpleExpressionSemanticAction($2, $4); }
          | '(' ID ')' complex_expression { $$ = IdComplexExpressionSemanticAction($2, $4); }
          | STRING { $$ = StringExpressionSemanticAction($1); }
          | simple_expression { $$ = SimpleExpressionSemanticAction($1); }
          | '{' ID '}' { $$ = IdExpressionSemanticAction($2); }
          | component { $$ = ComponentExpressionSemanticAction($1); }
          | complex_expression { $$ = ComplexExpressionSemanticAction($1); }
          ;

simple_expression: TEXTO { $$ = TextSemanticAction($1); }
                 | IMAGEN { $$ = ImageSemanticAction($1); }
                 | TITULO { $$ = TitleSemanticAction($1); }
                 | SUBTITULO { $$ = SubtitleSemanticAction($1); }
                 | ENLACE { $$ = LinkSemanticAction($1); }
                 ;

modifiers: modifiers COLOR { $$ = AddModifierSemanticAction($1, COLOR); }
         | modifiers SUBRAYADO { $$ = AddModifierSemanticAction($1, SUBRAYADO); }
         | modifiers ITALICA { $$ = AddModifierSemanticAction($1, ITALICA); }
         | modifiers NEGRITA { $$ = AddModifierSemanticAction($1, NEGRITA); }
         | modifiers TAMANIO { $$ = AddModifierSemanticAction($1, TAMANIO); }
         | COLOR { $$ = SingleModifierSemanticAction(COLOR); }
         | SUBRAYADO { $$ = SingleModifierSemanticAction(SUBRAYADO); }
         | ITALICA { $$ = SingleModifierSemanticAction(ITALICA); }
         | NEGRITA { $$ = SingleModifierSemanticAction(NEGRITA); }
         | TAMANIO { $$ = SingleModifierSemanticAction(TAMANIO); }
         ;

component: COMPONENTE '(' ID ')' '\n' body FIN_COMPONENTE '\n' { $$ = ComponentSemanticAction($3, $6); }
         | COMPONENTE '(' ID ')' '\n' FIN_COMPONENTE '\n' { $$ = ComponentSemanticAction($3, NULL); }
         ;

complex_expression: PUNTO_POR_PUNTO { $$ = PuntoPorPuntoSemanticAction(); }
                  | SECCION { $$ = SectionSemanticAction(); }
                  | TABLA { $$ = TableSemanticAction(); }
                  | NAVEGADOR { $$ = NavigatorSemanticAction(); }
                  ;

row_ppp: '-' expression row_ppp                  { $$ = RowPPPSemanticAction($2, $3); }
       | '-' expression                          { $$ = RowPPPSemanticAction($2, NULL); }
       | '-' row_ppp                                    { $$ = RowPPPSemanticAction(NULL, $2); }
       ;

row_table: column_table row_table                       { $$ = RowTableSemanticAction($1, $2); }
         | column_table                                 { $$ = RowTableSemanticAction($1, NULL); }
         ;

column_table: simple_expression column_table { $$ = ColumnTableSemanticAction($1, $2); }
            | FIN_FILA { $$ = ColumnTableSemanticAction(NULL, NULL); }
            ;

row_nav: '-' '(' ID ')' STRING '\n' row_nav { $$ = RowNavSemanticAction($3, $5, $7); }
       | '-' '(' ID ')' STRING '\n' { $$ = RowNavSemanticAction($3, $5, NULL); }
       ;

section: INICIO_SECCION ':' '\n' body FIN_SECCION '\n' { $$ = SectionSemanticAction($4); }
       | INICIO_SECCION ':' '\n' FIN_SECCION '\n' { $$ = SectionSemanticAction(NULL); }
       ;

%%
