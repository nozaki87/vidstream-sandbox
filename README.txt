vidstream-sandbox
=================
This is sandbox for AVB video streaming.



* Usage of sender03
./test/sender03 | gst-launch -v -e filesrc location=/dev/fd/0 ! h264parse ! ffdec_h264 ! ffmpegcolorspace ! deinterlace ! xvimagesink
gst-launch -v -m filesrc location=camera.h264 ! h264parse ! decodebin ! xvimagesink sync=false
