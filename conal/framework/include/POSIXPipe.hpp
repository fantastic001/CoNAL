#pragma once

#include <ext/stdio_filebuf.h>
#include <iostream>

namespace conal {
    namespace framework {
        /*
        POSIXPipe is special class representing wrapper around reading and writing of posix
        pipes (FIFO). It gives way to get stream object which can be used to write and read
        to unix pipes. 
        */
        class POSIXPipe {
                __gnu_cxx::stdio_filebuf<char> filebuf;
                std::iostream* stream;
            public:
                /*
                Constructor for POSIXPipe object. 

                \param posix_handle file descriptor which can be obtained using open(...)
                \param mode how to open given file for stream creation. Usually, flags
                            are set the same as how one opens file with open(...).
                */
                POSIXPipe(int posix_handle, std::ios::openmode mode) : filebuf(posix_handle, mode)
                {
                    stream = new std::iostream(&filebuf);
                }
                ~POSIXPipe() {
                    delete stream;
                }
                // get stream object which can be used for reading and writing 
                std::iostream& getStream() {
                    return *stream;
                }
        };
    }
}