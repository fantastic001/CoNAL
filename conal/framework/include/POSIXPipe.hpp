#pragma once

#include <ext/stdio_filebuf.h>
#include <iostream>

namespace conal {
    namespace framework {
        class POSIXPipe {
                __gnu_cxx::stdio_filebuf<char> filebuf;
                std::iostream* stream;
            public:

                POSIXPipe(int posix_handle, std::ios::openmode mode) : filebuf(posix_handle, mode)
                {
                    stream = new std::iostream(&filebuf);
                }
                ~POSIXPipe() {
                    delete stream;
                }

                std::iostream& getStream() {
                    return *stream;
                }
        };
    }
}