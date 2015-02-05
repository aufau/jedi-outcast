Description
===========

This is a fork of ioquake/jedi-outcast github repository. Apparently
the development is dead over there but I would be happy to merge back
my commits when it comes back alive.

The main goal of this project is to fix bugs and clean up the code
base so as to provide a reliable, maintainable and cross-platform Jedi
Outcast multiplayer client for myself and other players. You may be
surprised, but there is still an active multiplayer community 12 years
after the release.

Most changes originate from either ioq3_ or OpenJK_ projects. For me
it's a great learning opportunity and I'll be happy to merge useful
contributions. Adding support for new architectures/platforms should
be pretty straight-forward now.

Building
========

Currently the only supported platform is GNU/Linux x86. 32bit binaries
build and run just fine on amd64 multilib systems.

Dependencies
------------

* libSDL2
* OpenGL
* OpenAL

Compiling
---------

::

   mkdir build && cd build
   cmake ../CODE-mp/
   make

Installing
----------

Copy jk2mp and base/ to the directory containing game files.

~/.jkii is the directory where all configs, screenshots, logs and
demos will be stored. You can also put pk3s and executables there.

Related projects
================

As far as I know there are no projects developing jk2 online
multiplayer client. Please let me know if you stumble upon any.

If you are interested in capturing videos from jk2 demo files then
check out the joMME_ project and mme branch of this repo.

If you play jk2 online on linux then you may also be interested in the
recent revival of the xqf_ game server browser. I've pushed patches for
jk2 support.

.. _ioq3: https://github.com/ioquake/ioq3/
.. _OpenJK: https://github.com/JACoders/OpenJK
.. _joMME: https://github.com/entdark/jk2mp/
.. _xqf: https://github.com/XQF/xqf
