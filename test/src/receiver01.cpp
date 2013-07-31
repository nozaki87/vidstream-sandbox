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
//         VC.read(inputimage);
// 	printf("input image, %d, %d\n", inputimage.cols, inputimage.rows);
//         cv::imshow("Input Image", inputimage);
//         if (check_break()) break;
        std::string sender_addr;
        printf("receiving...\n");
        std::string message;
        udpsock_recvstr(sock, sender_addr, 2048, message);
        printf("message:%s (%s)\n", message.c_str(), sender_addr.c_str());
    }

    deinit_udpsock(sock);

    return 0;
}


