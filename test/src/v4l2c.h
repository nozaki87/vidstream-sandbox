#ifndef V4L2C_H
#define V4L2C_H

#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "v4l2w.h"

class V4L2C {
public:
    V4L2WHandler_t * mV4l2w;
    char mDevname[256];
    struct V4L2W_IMGPARAM mV4L2WImgParam;
    // int mWidth;
    // int mHeight;
    V4L2C() {
        mV4L2WImgParam.width = 1920;
        mV4L2WImgParam.height = 1080;
        mV4L2WImgParam.fps = 30;
        mV4L2WImgParam.img_fmt = V4L2W_IMGFORMAT_BGR24;
        mV4L2WImgParam.pix_fmt = V4L2_PIX_FMT_H264;
        mV4l2w = NULL;
    }
    V4L2C(int dev) {
        mV4L2WImgParam.width = 1920;
        mV4L2WImgParam.height = 1080;
        mV4L2WImgParam.fps = 30;
        mV4L2WImgParam.img_fmt = V4L2W_IMGFORMAT_BGR24;
        mV4L2WImgParam.pix_fmt = V4L2_PIX_FMT_H264;
        mV4l2w = NULL;
        open(dev);
    }
    ~V4L2C() {
        //release();
    }
    void open(int dev) {
        release();
    	sprintf(mDevname, "/dev/video%d", dev);
    }
    void release() {
        if (mV4l2w) V4L2W_finalize(mV4l2w);
    }
    void read(cv::Mat & image) {
        if (mV4l2w == NULL) {
            mV4l2w = V4L2W_init_w_param(mDevname, mV4L2WImgParam);
        }
        V4L2W_output_stream(mV4l2w, 1);
        // image = cv::Mat(mV4L2WImgParam.height, mV4L2WImgParam.width, CV_8UC3, V4L2W_capture(mV4l2w));
    }
    // int read_stream(unsigned char * data, int size) {
    //     if (mV4l2w == NULL) {
    //         mV4l2w = V4L2W_init_w_param(mDevname, mV4L2WImgParam);
    //     }
    //     image = cv::Mat(mV4L2WImgParam.height, mV4L2WImgParam.width, CV_8UC3, V4L2W_capture(mV4l2w));
    // }
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
