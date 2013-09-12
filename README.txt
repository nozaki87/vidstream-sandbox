vidstream-sandbox
=================
This is sandbox for AVB video streaming.



* Usage of sender03
./test/sender03 | gst-launch -v -e filesrc location=/dev/fd/0 ! h264parse ! ffdec_h264 ! ffmpegcolorspace ! deinterlace ! xvimagesink
gst-launch -v -m filesrc location=camera.h264 ! h264parse ! decodebin ! xvimagesink sync=false

./test/sender03 | gst-launch -v -e filesrc location=/dev/fd/0 ! h264parse ! rtph264pay ! udpsink  host=127.0.0.1 port=4000
gst-launch -v udpsrc port=4000 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264' ! rtph264depay ! queue ! ffdec_h264 !  ffmpegcolorspace ! deinterlace ! xvimagesink sync=false

gst-launch v4l2src ! video/x-raw-yuv,width=1920,height=1080,framerate=20/1 ! xvimagesink
gst-launch v4l2src ! video/x-raw-yuv,width=1920,height=1080,framerate=20/1 ! x264enc ! rtph264pay ! udpsink  host=192.168.1.127 port=4000
