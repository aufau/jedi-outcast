This branch on top of faufixes contains modifications for demo viewing
and video rendering.

Features so far:

* Render demos to uncompressed avi files.
  Just set cl_aviFrameRate and type /demo XYZ; video <filename> You
  can write it straight to a named pipe and encode to any format you
  fancy:

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

* Render demos with motion blur.
  r_blurFrames tells how many frames are to be blended into a single
  frame.
  r_blurSkipFrames allows you to pass over a fixed ammount of frames
  after blurring.
  r_blurType can be set to uniform, gaussian or triangular. Frames
  will be blended with different weights depending on this setting

  Unfortunately jk2 engine is limited to rendering at most 1000 fps
  therefore (r_blurFrames + r_blurSkipFrames) * r_aviFramerate
  shouldn't be much greater than 1000

  Example: To achieve classic movie look use these settings.

  cl_aviFramerate 24
  r_blurFrames 21
  r_skipFrames 21
  r_blurType uniform

  Uniform blur is well suited for slow scenes, free camera takes and
  screenshots. Gaussian and triangular blur are better for fast, first
  persion actions.

Lesser fixes:

* "Connection Interrupted" message no longer appears when playing back
  a demo
