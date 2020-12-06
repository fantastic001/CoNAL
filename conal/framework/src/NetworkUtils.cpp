#include <sys/socket.h>

#include <cstdio>

namespace conal {
    namespace utilities {
        ssize_t send_all(int sockfd, const void* buf, size_t len, int flags)
        {
            // printf("<< ");
            // for (int i = 0; i < len; i++) {
            //     printf("%02x ", ((const char*)buf)[i]);
            // }
            // printf("\n");
            
            size_t toread = len;
            char  *bufptr = (char*) buf;

            while (toread > 0)
            {
                ssize_t rsz = send(sockfd, bufptr, toread, flags);
                if (rsz <= 0)
                    return rsz;  /* Error or other end closed cnnection */

                toread -= rsz;  /* Read less next time */
                bufptr += rsz;  /* Next buffer position to read into */
            }

            return len;
        }

        /**
         * Taken from https://stackoverflow.com/questions/12691882/how-to-send-and-receive-data-socket-tcp-c-c#12696176
         */
        ssize_t recv_all(int sockfd, void *buf, size_t len, int flags)
        {
            size_t toread = len;
            char  *bufptr = (char*) buf;

            while (toread > 0)
            {
                ssize_t rsz = recv(sockfd, bufptr, toread, flags);
                if (rsz <= 0)
                    return rsz;  /* Error or other end closed cnnection */

                toread -= rsz;  /* Read less next time */
                bufptr += rsz;  /* Next buffer position to read into */
            }
            
            // printf(">> ");
            // for (int i = 0; i < len; i++) {
            //     printf("%02x ", ((const char*)buf)[i]);
            // }
            // printf("\n");

            return len;
        }
    }
}