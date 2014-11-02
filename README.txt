This is a fork of ioquake/jedi-outcast github repository. Apparently
the development is dead over there but I would be happy to merge back
my improvements when it comes back alive.

The main goal of this project is to fix bugs and clean up the code
base so as to provide a reliable Jedi Outcast multiplayer client for myself
and other players. You may be surprised, but there is still an active
multiplayer community 12 years after the release.

Currently there is no way to build 64bit binaries. The build system
will try to compile 32bit binaries on amd64 systems.

	Dependencies:

* libSDL
* OpenGL
* OpenAL

	How to build multiplayer:

mkdir build-mp && cd build-mp
cmake ../CODE-mp/
make

Copy jk2mp and jk2mpded to the directory containing base.
Copy *.so to your base directory.
~/.jkii is the default directory where all configs, screenshots
and pk3 files will be stored.

	Related projects:

As far as I know there is only one other project developing
multiplayer jk2 client: mike-pm/jedi-outcast at github. Please let me
know if you stumble upon any other.

If you are interested in capturing videos from jk2 demo files then
check out the mme branch of this repo.

If you play jk2 online on linux then you may also be interested in the
recent revival of the xqf game serwer browser. I've pushed patches for
jk2 support. See XQF/xqf @ github.
