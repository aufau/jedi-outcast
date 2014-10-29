This branch on top of faufixes contains modifications for demo viewing
and video rendering.

Features so far:

* Render a demo to an uncompressed avi file.
  Just set cl_aviFrameRate and type /demo XYZ; video <filename> You
  can write it straight to a named pipe and encode to any format you
  fancy:

  $ cd .jkii/base/videos
  $ mkfifo video.avi
  $ ffmpeg -i clip.avi -c:v libx264 -crf 20 -pix_fmt yuv420p clip.mp4

  Then run the game and type:

  /cl_aviFramerate 60
  /demo XYZ; video clip

  IMPORTANT
  r_aviFetchMode can assume one of the following values: gl_rgb,
  gl_bgr, gl_rgba and gl_bgra. Finding the correct value for your GPU
  can yield a huge performance improvement. On my hw it's ~1000% more
  fps.

* Render a demo with motion blur.
  r_blurFrames defines how many game frames are to be blended into a
  single output frame.
  r_blurSkipFrames allows you to pass over a fixed ammount of game
  frames after blending.
  r_blurType can be set to uniform, gaussian or triangular. Game frames
  will be blended with different weights depending on this setting

  Unfortunately jk2 engine is limited to rendering at most at 1000fps
  therefore (r_blurFrames + r_blurSkipFrames) * r_aviFramerate
  shouldn't be much greater than 1000

  Example: To achieve the classic cinema movie look use these
  settings.

  cl_aviFramerate 24
  r_blurFrames 21
  r_skipFrames 21
  r_blurType uniform

  Uniform blur is well suited for slow scenes, free camera takes and
  screenshots. Gaussian and triangular blur are better for fast paced
  first person action.

Fixes:

* "Connection Interrupted" message no longer appears during the demo
  playback.
* It is possible to capture a video with r_overbrightbits in the
  windowed mode now. Image on your screen will be dimm but the
  recorded video should have propper brightness.
