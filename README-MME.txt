This branch on top of faufixes contains modifications for demo viewing
and video rendering.

Features so far:

* Render demos to uncompressed avi files. Just set cl_aviFrameRate and
  type /demo XYZ; video <filename> You can write it straight to a
  named pipe and encode to any format you fancy:

  $ cd .jkii/base/videos
  $ mkfifo video.avi
  $ ffmpeg -i video.avi -r 60 -c:v libx264 -crf 20 -pix_fmt yuv420p -f mp4 -an -y video.mp4

  Then run the game and type:

  /cl_aviFramerate 60
  /demo XYZ; video video
