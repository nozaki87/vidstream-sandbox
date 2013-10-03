vidstream-sandbox
=================
This is sandbox for AVB video streaming.

* Libraries you need
libjpeg
OpenCV-dev
Gstreamer 1.0

* How to build
mkdir build
cd build
cmake ../
make
Now you can find executable binaries under build/test/.

* Usage of sender03
sender03 outputs H264 ES (Elementary stream) as standard output.

** TCP
*** General webcam.
gst-launch-1.0 v4l2src ! video/x-raw-yuv,width=1920,height=1080,framerate=30/1 ! x264enc  ! h264parse ! mpegtsmux ! tcpserversink  host=127.0.0.1 port=5000
*** Logitech C920 (For HW Encoding of H.264)
./test/sender03 | gst-launch -v -e filesrc location=/dev/fd/0 ! h264parse ! mpegtsmux ! tcpserversink  host=127.0.0.1 port=5000

UDP
*** General webcam.
gst-launch-1.0 -v -e filesrc location=camera.h264 ! h264parse ! mpegtsmux ! rtpmp2tpay ! udpsink  host=127.0.0.1 port=4000
*** Logitech C920 (For HW Encoding of H.264)
gst-launch-1.0 -v udpsrc port=4000 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264' ! rtpmp2tdepay ! tsdemux ! h264parse ! avdec_h264 ! videoconvert ! videoscale ! ximagesink sync=false

*** General webcam.
gst-launch -v -e filesrc location=camera.h264 ! h264parse ! mpegtsmux ! rtpmp2tpay ! udpsink  host=127.0.0.1 port=4000
*** Logitech C920 (For HW Encoding of H.264)
gst-launch -v udpsrc port=4000 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264' ! rtpmp2tdepay ! mpegtsdemux ! queue ! ffdec_h264 !  ffmpegcolorspace ! deinterlace ! xvimagesink sync=false

