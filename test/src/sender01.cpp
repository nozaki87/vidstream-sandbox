#include <stdio.h>
#include <iostream>
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
    cv::VideoCapture VC;

    int port = 45678;
    SOCKET sock = udpsock_client();
    if (sock == -1) {
        deinit_udpsock(sock);
        return -1;
    }
    VC.open(0);
    VC.set(CV_CAP_PROP_FRAME_WIDTH, VIDEO_WIDTH);
    VC.set(CV_CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT);
    VC.set(CV_CAP_PROP_FPS, 30);

    int i = 0;
    while (1) {
        VC.read(inputimage);
	printf("input image, %d, %d\n", inputimage.cols, inputimage.rows);
        cv::imshow("Input Image", inputimage);
	char message[256];
	sprintf(message, "sender %d", i); // syntax is "commandnumber, totalnumber, x, y, z, x, y, z, ..."
	sendudp(message, "127.0.0.1", 45678, sock);
	i++;
        if (check_break()) break;
    }

    VC.release();

    deinit_udpsock(sock);

    return 0;
}


