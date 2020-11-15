%{
  #include <cstdio>
  #include <iostream>
  #include <node_spec/AndSpecification.hpp>
  #include <node_spec/OrSpecification.hpp>
  #include <node_spec/NameSpecification.hpp>
  #include <node_spec/BroadcastSpecification.hpp>
  #include <node_spec/Specification.hpp>
  #include <node_spec/Parser.hpp>

  #include <memory>
  #include <utility>
  #include <iostream>


  #include "parser.hpp"
  using namespace conal::activity_manager::node_spec;

  using namespace std;

  void yyerror(const char *s);
  Specification* parse(const char*);
  int parse_string(const char*);
  extern "C" yy::parser::symbol_type yylex (::conal::activity_manager::node_spec::Specification**);
  typedef struct yy_buffer_state * YY_BUFFER_STATE;
  YY_BUFFER_STATE yy_scan_string(const char *str);

%}
%skeleton "lalr1.cc"
%param {::conal::activity_manager::node_spec::Specification** result }
%language "C++"
%define api.value.type variant
%define api.token.constructor


// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  Initially (by default), yystype
// is merely a typedef of "int", but for non-trivial projects, tokens could
// be of any arbitrary data type.  So, to deal with that, the idea is to
// override yystype's default typedef to be a C union instead.  Unions can
// hold all of the types of tokens that Flex could return, and this this means
// we can return ints or floats or strings cleanly.  Bison implements this
// mechanism with the %union directive:

//%parse-param {std::shared_ptr<Specification>& result}
// Define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the %union:
%token BROADCAST_OPERATOR
%token OR
%token AND 
%token OPEN_BRACKET
%token CLOSED_BRACKET
%token EQUALS 
%token OPEN_PARAM_BRACKET
%token CLOSED_PARAM_BRACKET
%token NOT
%token MASTER
%token <int> INT
%token <float> FLOAT
%token <std::string> STRING

%nterm <::conal::activity_manager::node_spec::Specification*> spec
%nterm <::conal::activity_manager::node_spec::Specification*> and_spec
%nterm <::conal::activity_manager::node_spec::Specification*> or_spec
%nterm <::conal::activity_manager::node_spec::Specification*> master_spec
%nterm <::conal::activity_manager::node_spec::Specification*> not_spec
%nterm <::conal::activity_manager::node_spec::Specification*> factor_spec
%nterm <::conal::activity_manager::node_spec::Specification*> broadcast_spec
%nterm <::conal::activity_manager::node_spec::Specification*> name_spec
%nterm <::conal::activity_manager::node_spec::Specification*> param_spec

%%
// This is the actual grammar that bison will parse, but for right now it's just
// something silly to echo to the screen what bison gets from flex.  We'll
// make a real one shortly:


start : spec  {  *result = $1; }

spec : and_spec | 
  or_spec | 
  broadcast_spec | 
  factor_spec | 
  name_spec | 
  param_spec | 
  not_spec | 
  master_spec;
and_spec : spec AND spec {
  $$ = new ::conal::activity_manager::node_spec::AndSpecification($1, $3);
}

or_spec : spec OR spec {
  //$$ = OrSpecification($1, $3);
}

broadcast_spec : BROADCAST_OPERATOR {
  $$ = new BroadcastSpecification();
}

factor_spec : OPEN_BRACKET spec CLOSED_BRACKET {
  $$ = $2;
}

name_spec : STRING EQUALS STRING {
  //if (std::string($1) == "name" ) $$ = NameSpecification($3);
  //else yyerror("Only name as parameter is supported. If you have another parameter to check, use [...]=...");
}

param_spec : OPEN_PARAM_BRACKET STRING CLOSED_PARAM_BRACKET EQUALS STRING {
  
}

not_spec : NOT spec {

}

master_spec : MASTER {

}

%%

Specification* conal::activity_manager::node_spec::Parser::parse(const char* text) {
  yy_scan_string(text);
  Specification* result;
  yy::parser parse(&result);
  int ret_code = parse ();
  if (ret_code == 0) return result; 
  return nullptr;
}

Specification* parse(const char* text) {
  return NULL;
}

void yy::parser::error(const std::string& err) {
  std::cerr << err; 
}
