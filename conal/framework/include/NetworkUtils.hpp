#pragma once

namespace conal {
    namespace utilities {
        typedef uint32_t packet_length_t;
        
        ssize_t send_all(int sockfd, const void* buf, size_t len, int flags);

        /**
         * Taken from https://stackoverflow.com/questions/12691882/how-to-send-and-receive-data-socket-tcp-c-c#12696176
         */
        ssize_t recv_all(int sockfd, void *buf, size_t len, int flags);
    }
}