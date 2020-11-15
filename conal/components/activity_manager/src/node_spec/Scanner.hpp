#pragma once 

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class Scanner : public yyFlexLexer{
                virtual int yylex();
            };
        }
    }
}