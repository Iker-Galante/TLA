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

"principio"                 { printf("TOKEN: PRINCIPIO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PRINCIPIO); }
"final"                             { printf("TOKEN: FIN\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN); }
"encabezado"						{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ENCABEZADO); }
"fin-encabezado"					{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_ENCABEZADO); }
"pie"							    { return KeywordLexemeAction(createLexicalAnalyzerContext(), PIE); }
"fin-pie"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_PIE); }
"texto"							    { printf("TOKEN: TESTO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), TEXTO); }
"imagen"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), IMAGEN); }
"titulo"							{ printf("TOKEN: Titulo\n");return KeywordLexemeAction(createLexicalAnalyzerContext(), TITULO); }
"subtitulo"						    { return KeywordLexemeAction(createLexicalAnalyzerContext(), SUBTITULO); }
"enlace"                            { return KeywordLexemeAction(createLexicalAnalyzerContext(), ENLACE); }
"color"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), COLOR); }
"subrayado"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), SUBRAYADO); }
"italica"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), ITALICA); }
"negrita"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), NEGRITA); }
"tamanio"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), TAMANIO); }
"punto-por-punto"                   { return KeywordLexemeAction(createLexicalAnalyzerContext(), PUNTO_POR_PUNTO); }
"seccion"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), SECCION); }
"tabla"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), TABLA); }
"navegador"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), NAVEGADOR); }
"componente"                        { return KeywordLexemeAction(createLexicalAnalyzerContext(), COMPONENTE); }
"fin-navegador"                     { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_NAVEGADOR); }
"inicio-tabla"                      { return KeywordLexemeAction(createLexicalAnalyzerContext(), INICIO_TABLA); }
"fin-tabla"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_TABLA); }
"inicio-seccion"                    { return KeywordLexemeAction(createLexicalAnalyzerContext(), INICIO_SECCION); }
"fin-seccion"                       { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_SECCION); }
"fin-componente"                    { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_COMPONENTE); }
"fin-fila"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_FILA); }
"fin-ppp"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_PPP); }
"fin-nav"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_NAV); }
":"                                { BEGIN(STRING_PROCESSING);printf("TOKEN: DOS PUNTOS\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(),DOS_PUNTOS); }
"-"                             { printf("TOKEN: MENOS\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), GUION); }
"("                               { printf("TOKEN: PARENTESIS_IZQUIERDO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_IZQUIERDO); }
")"                               { printf("TOKEN: PARENTESIS_DERECHO\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_DERECHO); }
"{"                               { printf("TOKEN: LLAVE_IZQUIERDA\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), LLAVE_IZQUIERDA); }
"}"                               { printf("TOKEN: LLAVE_DERECHA\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), LLAVE_DERECHA); }
"#"                               { printf("TOKEN: NUMERAL\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), NUMERAL); }
[\r]?\n                            { printf("TOKEN: NEWLINE WINDOWS\n"); return KeywordLexemeAction(createLexicalAnalyzerContext(), NEW_LINE); }
[a-zA-Z0-9_]+                       { printf("TOKEN: ESTRIN \n");return StringLexemeAction(createLexicalAnalyzerContext()); }
[[:space:]]+						{ ; IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<STRING_PROCESSING>[^\n\r]+        { printf("TOKEN: ESTRIN RARO \n"); return StringLexemeAction(createLexicalAnalyzerContext()); }
<STRING_PROCESSING>[\r]?\n         { printf("TOKEN: NEWLINE WINDOWS (STR)\n"); BEGIN(INITIAL); return KeywordLexemeAction(createLexicalAnalyzerContext(), NEW_LINE); }
.									{ printf("IGNORADO");return UnknownLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"
