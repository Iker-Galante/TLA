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
[\r]?\n                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), NEW_LINE); }
"principio"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), PRINCIPIO); }
"final"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN); }
"encabezado"						{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ENCABEZADO); }
"fin-encabezado"					{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_ENCABEZADO); }
"pie"							    { return KeywordLexemeAction(createLexicalAnalyzerContext(), PIE); }
"fin-pie"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FIN_PIE); }
"texto"							    { return KeywordLexemeAction(createLexicalAnalyzerContext(), TEXTO); }
"imagen"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), IMAGEN); }
"titulo"							{ return KeywordLexemeAction(createLexicalAnalyzerContext(), TITULO); }
"subtitulo"						    { return KeywordLexemeAction(createLexicalAnalyzerContext(), SUBTITULO); }
"enlace"                            { return KeywordLexemeAction(createLexicalAnalyzerContext(), ENLACE); }
"color"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), COLOR); }
"subrayado"                         { return KeywordLexemeAction(createLexicalAnalyzerContext(), SUBRAYADO); }
"italica"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), ITALICA); }
"negrita"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), NEGRITA); }
"tamanio"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), TAMANIO); }
"punto-por-punto"                   { return KeywordLexemeAction(createLexicalAnalyzerContext(), PUNTO_POR_PUNTO); }
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
<*>":"                              { return KeywordLexemeAction(createLexicalAnalyzerContext(),DOS_PUNTOS); }
"grande"                            { return KeywordLexemeAction(createLexicalAnalyzerContext(), GRANDE); }
"pequeno"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), PEQUENIO); }
"normal"                            { return KeywordLexemeAction(createLexicalAnalyzerContext(), NORMAL); }
"rojo"                              { return KeywordLexemeAction(createLexicalAnalyzerContext(), ROJO); }
"verde"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), VERDE); }
"azul"                              { return KeywordLexemeAction(createLexicalAnalyzerContext(), AZUL); }
"amarillo"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), AMARILLO); }
"naranja"                           { return KeywordLexemeAction(createLexicalAnalyzerContext(), NARANJA); }



"\""                                             { BEGIN(STRING_PROCESSING); IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<STRING_PROCESSING>[^"]+                         { return StringLexemeAction(createLexicalAnalyzerContext()); }
<STRING_PROCESSING>"\""                          { BEGIN(INITIAL);IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"-"[[:space:]]*                                  { return KeywordLexemeAction(createLexicalAnalyzerContext(), GUION); }
"(\""                                            { BEGIN(STRING_PROCESSING); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_IZQUIERDO); }
"("                                              { BEGIN(IDENTIFICADOR); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_IZQUIERDO); }
<IDENTIFICADOR>")"                               { BEGIN(INITIAL); return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_DERECHO); }
"{"                                              { BEGIN(IDENTIFICADOR);  return KeywordLexemeAction(createLexicalAnalyzerContext(), LLAVE_IZQUIERDA); }
<IDENTIFICADOR>"}"                               { BEGIN(INITIAL); return KeywordLexemeAction(createLexicalAnalyzerContext(), LLAVE_DERECHA); }
<IDENTIFICADOR>"#"                               { return KeywordLexemeAction(createLexicalAnalyzerContext(), NUMERAL); }
<IDENTIFICADOR>[a-zA-Z_][a-zA-Z0-9_.]*           { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }

<*>[[:space:]]+						             { ; IgnoredLexemeAction(createLexicalAnalyzerContext()); }
")"                                              { return KeywordLexemeAction(createLexicalAnalyzerContext(), PARENTESIS_DERECHO); }
.									             { return UnknownLexemeAction(createLexicalAnalyzerContext()); }
%%

#include "FlexExport.h"
