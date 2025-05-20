%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x STRING_PROCESSING
%x IDENTIFICADOR
%x PPP
%x COMPONENT
%x DUMMY
%x LINK
%x INNERLINK
/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
[\r]?\n                            { printf("TOKEN: NEWLINE WINDOWS\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), NEW_LINE); }
"principio"                 { printf("TOKEN: PRINCIPIO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PRINCIPIO); }
"final"                             { printf("TOKEN: FIN\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN); }
"encabezado"						{ yy_push_state(DUMMY); return KeywordLexemeAction(createLexicalAnalyzerContext(), ENCABEZADO); }
"fin-encabezado"					{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_ENCABEZADO); }
"pie"							    {yy_push_state(DUMMY); return KeywordLexemeAction(createLexicalAnalyzerContext(), PIE); }
"fin-pie"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_PIE); }
"texto"							    { printf("TOKEN: TESTO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), TEXTO); }
"imagen"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), IMAGEN); }
"titulo"							{ printf("TOKEN: Titulo\n");return KeywordLexemeAction(createLexicalAnalyzerContext(), TITULO); }
"subtitulo"						    { return KeywordLexemeAction(createLexicalAnalyzerContext(), SUBTITULO); }
"enlace"                            {yy_push_state(LINK); printf("TOKEN: ENLACE\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), ENLACE); }
"color"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), COLOR); }
"subrayado"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), SUBRAYADO); }
"italica"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), ITALICA); }
"negrita"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), NEGRITA); }
"tamanio"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), TAMANIO); }
"punto-por-punto"                   {  yy_push_state(PPP);printf("TOKEN: PPP\n");return KeywordLexemeAction(createLexicalAnalyzerContext(), PUNTO_POR_PUNTO); }
"seccion"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), SECCION); }
"tabla"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), TABLA); }
"navegador"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), NAVEGADOR); }
"componente"                        {printf("COMPONENT\n");  yy_push_state(COMPONENT); return KeywordLexemeAction(createLexicalAnalyzerContext(), COMPONENTE); }
"fin-navegador"                     { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_NAVEGADOR); }
"inicio-tabla"                      { return KeywordLexemeAction(createLexicalAnalyzerContext(), INICIO_TABLA); }
"fin-tabla"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_TABLA); }
"inicio-seccion"                    { return KeywordLexemeAction(createLexicalAnalyzerContext(), INICIO_SECCION); }
"fin-seccion"                       { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_SECCION); }
"fin-componente"                    { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_COMPONENTE); }
"fin-fila"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_FILA); }
"fin-ppp"                           {printf("TOKEN: FINPPP\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_PPP); }
":"                                { yy_push_state(STRING_PROCESSING);printf("TOKEN: DOS PUNTOS\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(),DOS_PUNTOS); }
<STRING_PROCESSING>[\r]?\n         { printf("TOKEN: NEWLINE WINDOWS (STR)\n"); yy_pop_state(); return KeywordLexemeAction(createLexicalAnalyzerContext(), NEW_LINE); }
<PPP,COMPONENT,DUMMY,LINK>[\r]?\n         { printf("TOKEN: NEWLINE COMP O PPP \n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), NEW_LINE); }
"-"[[:space:]]*                             { printf("TOKEN: MENOS\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), GUION); }
<*>[[:space:]]+						{ ; IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<INITIAL,COMPONENT>"("                               { yy_push_state(IDENTIFICADOR);printf("TOKEN: PARENTESIS_IZQUIERDO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_IZQUIERDO); }
<IDENTIFICADOR>")"[[:space:]]*	                               { yy_pop_state();printf("TOKEN: PARENTESIS_DERECHO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_DERECHO); }
"{"                               { yy_push_state(IDENTIFICADOR);printf("TOKEN: LLAVE_IZQUIERDA\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), LLAVE_IZQUIERDA); }
<IDENTIFICADOR>"}"                               { yy_pop_state();printf("TOKEN: LLAVE_DERECHA\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), LLAVE_DERECHA); }
<IDENTIFICADOR>"#"                               { printf("TOKEN: NUMERAL\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), NUMERAL); }
<IDENTIFICADOR>[a-zA-Z_][a-zA-Z0-9_.]*         { printf("TOKEN: IDENTIFICADOR\n"); return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
[a-zA-Z0-9_ ]+                       { printf("TOKEN: ESTRIN \n");return StringLexemeAction(createLexicalAnalyzerContext()); }
<STRING_PROCESSING>[^\n\r]+        { printf("TOKEN: ESTRIN RARO \n"); return StringLexemeAction(createLexicalAnalyzerContext()); }
<COMPONENT,DUMMY,PPP>":"                               {yy_pop_state(); printf("TOKEN: DOS PUNTOS COMPLEJO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(),DOS_PUNTOS); }
<LINK>":"                               {printf("TOKEN: DOS PUNTOS COMPLEJO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(),DOS_PUNTOS); }
<LINK>"("                               {printf("TOKEN: PARENTESIS_IZQUIERDO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_IZQUIERDO); }
<LINK>")"[[:space:]]*	                { yy_pop_state();printf("TOKEN: PARENTESIS_DERECHO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_DERECHO); }
<LINK>"#"                              { yy_push_state(INNERLINK);printf("TOKEN: NUMERAL\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), NUMERAL); }
<LINK>[a-zA-Z_][a-zA-Z0-9_.]*         { printf("TOKEN: IDENTIFICADOR\n"); return StringLexemeAction(createLexicalAnalyzerContext()); }
<INNERLINK>[a-zA-Z_][a-zA-Z0-9_.]*         {yy_pop_state(); printf("TOKEN: IDENTIFICADOR\n"); return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
%%

#include "FlexExport.h"
