#include <stdio.h>
#include <vector>
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
    NDPServer ndps;

    int port = 45678;
    ndps.set_port(port);
    while (1) {
        cv::Mat image(VIDEO_HEIGHT, VIDEO_WIDTH, CV_8UC3);
        printf("receiving...\n");
#if 1
        NDPFrame ndpframe = ndps.recv_frame();
        unsigned char * data = new unsigned char [ndpframe.get_size()];
        int size = ndpframe.read_data(data, ndpframe.get_size());
        std::vector<uchar> buf(data, data + size);
        cv::Mat recvimg = cv::imdecode(buf, 1);
//        cv::imshow("received image", image);
       cv::imshow("received image", recvimg);
        ndpframe.discard();
        if (check_break()) break;
#else
        std::string message;
        NDPFrame ndpframe = ndps.recv_frame();
        unsigned char tmpstr[1024];
        int size = ndpframe.read_data(tmpstr, 1024);
        tmpstr[size] = '\0';
        message = std::string((char *)tmpstr);
        printf("message:%s\n", message.c_str());
        ndpframe.discard();
#endif
    }

    return 0;
}


