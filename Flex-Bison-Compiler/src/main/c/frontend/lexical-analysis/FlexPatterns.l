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

%x IMPORT_EXPRESSION

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

/* Cambio de contexto: Comentarios */
"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
/* Fin de contexto */

/* Cambio de contexto: Import expressions */
"{"                                 { 
                                        printf("%s", "Context switch: Import expression start\n");
                                        BEGIN(IMPORT_EXPRESSION); 
                                    }
                                
<IMPORT_EXPRESSION>[^{}]+           {
                                        LexicalAnalizerContext * lexicalAnalizerContext = createLexicalAnalyzerContext();
                                        printf("Creating next buffer to consume from file: '%s'.\n" , lexicalAnalizerContext->lexeme);
                                        yyin = fopen(lexicalAnalizerContext->lexeme, "r");
                                        importedExpressionBufferState = yy_create_buffer(yyin, YY_BUF_SIZE);
                                        destroyLexicalAnalyzerContext(lexicalAnalizerContext);
                                    }

<IMPORT_EXPRESSION>"}"              { 
                                        printf("%s", "Loading next buffer from imported expression.\n");
                                        yypush_buffer_state(importedExpressionBufferState);
                                        BEGIN(INITIAL); 
                                    }

<IMPORT_EXPRESSION>.                {
                                        printf("%s", "Syntax error inside import expression.\n");
                                        return UnknownLexemeAction(createLexicalAnalyzerContext());
                                    }

<<EOF>>                             {
                                        printf("%s", "EOF inside import expression, popping buffer.\n");
                                        yypop_buffer_state();
                                        if (!YY_CURRENT_BUFFER) {
                                            // Avisa a Bison que no hay más entradas del buffer en el stack
                                            yyterminate();
                                        }
                                    }
/* Fin de contexto */


/*  Expresion regular  |  Simbolo de Σ  */
"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
