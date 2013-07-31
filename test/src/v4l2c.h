#ifndef V4L2C_H
#define V4L2C_H

#include "opencv2/core/core.hpp"
#include "v4l2w.h"

class V4L2C {
public:
    V4L2WHandler_t * mV4l2w;
    int mWidth;
    int mHeight;
    V4L2C() {
        mWidth = 640;
	mHeight = 480;
    }
    V4L2C(int dev) {
        mWidth = 640;
	mHeight = 480;
        open(dev);
    }
    ~V4L2C() {
        //release();
    }
    void open(int dev) {
        char devname[256];
	sprintf(devname, "/dev/video%d", dev);
        mV4l2w = V4L2W_initialize(devname, mWidth, mHeight, 30, V4L2W_IMGFORMAT_BGR24);
    }
    void release() {
        V4L2W_finalize(mV4l2w);
    }
    void read(cv::Mat & image) {
        image = cv::Mat(mHeight, mWidth, CV_8UC3, V4L2W_capture(mV4l2w));
    }
    void set(int idx, int val) {

    }
    int  get(int idx) {

    }
    int grab() {

    }
    int retrieve(cv::Mat & image) {
        read(image);
    }
    void set_v4l2p(int idx, int val){
	V4L2W_setctrl(mV4l2w, idx, val);
    }
    int get_v4l2p(int idx){
        return V4L2W_getctrl(mV4l2w, idx);
    }
    void suspend_v4l2(){
        V4L2W_suspend(mV4l2w);
    }
    void resume_v4l2(){
	V4L2W_resume(mV4l2w);
    }
};

#endif // V4L2C_H
