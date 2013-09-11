#include <stdio.h>
#include <iostream>
#include "udp_wrapper.h"
#include "opencv2/highgui/highgui.hpp"
#if !defined(WIN32) && !defined(__APPLE__)
# include "v4l2c.h"
#endif

static unsigned char latest_key;

static int check_break (void)
{
    unsigned char c;
    c = cvWaitKey (3);
    if (c == '\x20') c = cv::waitKey(0);
    latest_key = c;
    if (c == '\x1b') return 1;
    else return 0;
}

#define USE_SOKET_OUTPUT
#define VIDEO_WIDTH  640
#define VIDEO_HEIGHT 480

int main(int argc, char ** argv) 
{
    cv::Mat inputimage;
#if defined(WIN32) || defined(__APPLE__)
    cv::VideoCapture VC;
#else
    V4L2C VC;
#endif

    VC.open(0);
    VC.set(CV_CAP_PROP_FRAME_WIDTH, VIDEO_WIDTH);
    VC.set(CV_CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT);
    VC.set(CV_CAP_PROP_FPS, 30);

    while (1) {
        VC.read(inputimage);
        cv::imshow("Input Image", inputimage);
        if (check_break()) break;
    }

    VC.release();

    return 0;
}

