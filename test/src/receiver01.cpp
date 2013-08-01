#include <stdio.h>
#include <vector>
#include "udp_wrapper.h"
#include "opencv2/highgui/highgui.hpp"

static int check_break (void)
{
    unsigned char c;
    c = cvWaitKey (3);
    if (c == '\x20') c = cv::waitKey(0);
    if (c == '\x1b') return 1;
    else return 0;
}

#define VIDEO_WIDTH  640
#define VIDEO_HEIGHT 480

int main(int argc, char ** argv) 
{
    cv::Mat inputimage;

    int port = 45678;
    SOCKET sock = udpsock_server(port, NULL);
    if (sock == -1) {
        deinit_udpsock(sock);
        return -1;
    }
    while (1) {
        cv::Mat image(VIDEO_HEIGHT, VIDEO_WIDTH, CV_8UC3);
        std::string sender_addr;
        printf("receiving...\n");
        std::string message;
#if 0
        udpsock_recvdata(sock, sender_addr, image.step * image.rows, image.data);
        cv::imshow("received image", image);
        if (check_break()) break;
#else
        udpsock_recvstr(sock, sender_addr, 2048, message);
        printf("message:%s (%s)\n", message.c_str(), sender_addr.c_str());
#endif
    }

    deinit_udpsock(sock);

    return 0;
}


