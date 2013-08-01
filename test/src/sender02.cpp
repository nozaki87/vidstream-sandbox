#include <stdio.h>
#include <iostream>
#include "ndp.h"
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
    NDPClient ndpc;

    int port = 45678;
    ndpc.set_port(port);
    // std::string addr = std::string("128.32.33.48");
    std::string addr = std::string("127.0.0.1");
    VC.open(0);
    VC.set(CV_CAP_PROP_FRAME_WIDTH, VIDEO_WIDTH);
    VC.set(CV_CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT);
    VC.set(CV_CAP_PROP_FPS, 30);

    int i = 0;
    while (1) {
#if 1
        VC.read(inputimage);
        printf("input image, %d, %d\n", inputimage.cols, inputimage.rows);
        cv::imshow("Input Image", inputimage);
        std::vector<uchar> buf;
        cv::imencode(".jpg", inputimage, buf);
        // printf("%ld\n", inputimage.step * inputimage.rows);
        ndpc.send_frame(addr, (unsigned char *)&buf[0], buf.size());
#else
        char message[256];
        sprintf(message, "%d-123456789ThisIsData.ThisIsData.0123456789Yeah!!!!", i); // syntax is "commandnumber, totalnumber, x, y, z, x, y, z, ..."
        std::string mes = std::string(message);
        ndpc.send_frame(addr, (unsigned char *)mes.c_str(), mes.size());
#endif
        i++;
        if (check_break()) break;
    }

    VC.release();

    return 0;
}


