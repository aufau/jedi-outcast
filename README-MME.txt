This branch on top of faufixes contains modifications for demo viewing
and video rendering.

Features so far:

* Render demos to uncompressed avi files. Just set cl_aviFrameRate and
  type /demo XYZ; video <filename> You can write it straight to a
  named pipe and encode to any format you fancy:

  $ cd .jkii/base/videos
  $ mkfifo video.avi
  $ ffmpeg -i video.avi -c:v libx264 -crf 20 -pix_fmt yuv420p video.mp4

  Then run the game and type:

  /cl_aviFramerate 60
  /demo XYZ; video video

  IMPORTANT
  r_aviFetchMode can assume on of the following values: gl_rgb,
  gl_bgr, gl_rgba and gl_bgra. Finding correct value for your GPU can
  yield a huge performance improvement. On my hw it's ~1000% more fps.
