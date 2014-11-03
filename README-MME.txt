This branch on top of faufixes contains modifications for demo viewing
and video rendering.

Features so far:

* Render a demo to an uncompressed avi file.
  Just set cl_aviFrameRate and type /demo XYZ; video <filename> You
  can write it straight to a named pipe and encode to any format you
  fancy on the fly to avoid trashing your disk:

  $ cd .jkii/base/videos
  $ mkfifo video.avi
  $ ffmpeg -i clip.avi -c:v libx264 -crf 20 -pix_fmt yuv420p clip.mp4

  Then run the game and type:

  /cl_aviFramerate 60
  /demo XYZ; video clip

  IMPORTANT
  r_aviFetchMode can assume one of the following values: GL_RGB,
  GL_BGR, GL_RGBA and GL_BGRA. Finding the correct value for your GPU
  can yield a huge performance improvement. On my hw it's ~1000% more
  fps.

* Render a demo with motion blur.
  r_blurFrames defines how many game frames are to be blended into a
  single output frame.
  r_blurOverlap makes blur patterns overlap or have gaps in between
  them. The unit is a frame. Negative values will result in gaps. 0 is
  the default.
  r_blurType can be set to uniform, gaussian or triangular. Game frames
  will be blended with different weights depending on this setting
  r_blurQuality affects only gaussian and triangular blur types. It
  allows you to increase performance by cutting off less significant
  frames at the edge of the blur patter. You can set it to low,
  medium, high, best or a percentage. Don't use percentage unless you
  know what you are doing.

  Unfortunately jk2 engine cannot run at more than 1000fps, therefore
  (r_blurFrames - r_blurOverlap) * r_aviFramerate must be less than
  1000.

  Example: To achieve the classic cinema movie look use these
  settings.

  cl_aviFramerate 24
  r_blurFrames 21
  r_blurOverlap -21
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

Q&A:

Q: How do I improve quality?
A: Start with increasing cl_aviFramerate. Nowadays youtube accepts
   videos up to 60fps. Next comes r_blurFrames. Crank it up until you
   hit the 1000fps formula. Only then you should think of using high
   or best r_blurQuality.
