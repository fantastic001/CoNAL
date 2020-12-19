%{
  #include <iostream>
  #include <node_spec/Specification.hpp>
  #include "parser.hpp"
  using namespace std;
  // %option yyclass="::conal::activity_manager::node_spec::Scanner"
// Tell Flex the lexer's prototype ...
# define YY_DECL \
  yy::parser::symbol_type yylex (::conal::activity_manager::node_spec::Specification**)
// ... and declare it for the parser's sake.
extern "C" YY_DECL;


%}



%%
\%  { return yy::parser::make_BROADCAST_OPERATOR(); }
\+  { return yy::parser::make_OR(); }
,   { return yy::parser::make_AND(); }
\(  { return yy::parser::make_OPEN_BRACKET(); }
\)  { return yy::parser::make_CLOSED_BRACKET(); }
=   { return yy::parser::make_EQUALS(); }
\[  { return yy::parser::make_OPEN_PARAM_BRACKET(); }
\]  { return yy::parser::make_CLOSED_PARAM_BRACKET(); }
[.a-zA-Z0-9]+   {
  //yylval.sval = strdup(yytext);
  return yy::parser::make_STRING(yytext);
}
!   { return yy::parser::make_NOT(); }
\*  { return yy::parser::make_BROADCAST_OPERATOR(); }
%%

