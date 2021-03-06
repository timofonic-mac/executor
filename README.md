Current Status
==============

Major hacking is happening here.
If you want to *use* Executor, go to the "original" version at github.com/ctm/executor.

I am trying here to modernize the code base (based on MaddTheSane's C++ port)
by inserting type-safe templates for all Big-Endian values.

This should have three benefits:

1. More type safety, which means fewer bugs once the bugs I add have been fixed again
2. 64 bit support
3. Easier-to-read code, because we can hopyfully automate most byte swapping.


Compilation Notes
=================

Current configure line:

```
CXX='g++ -m32' CC='gcc -m32' ../src/configure --with-front-end=x --with-sound=dummy --host=i686-unknown-linux
```

Original Readme
========================================================================

Executor is a Macintosh emulator that is able to run many ancient Mac
OS 680x0 binaries (System 6 era, early System 7) without using any
intellectual property from Apple Computer.

Executor has not been actively worked on for many years.  It is more
of a curiousity than anything practical; it does not run PowerPC
binaries, nor did it even run most System 7 applications.  Unless you
have a particular application that you know works with Executor, or
just like emulation a lot, you probably don't want to spend any time
with Executor.

Included are a bunch of auxiliarly files and miscellaneous front-ends.

The DOS port required various other libraries that we had permission
to use, but that we did not necessarily have the permission to publish
the source for, so the source to some of the packages that the DOS
port requires is not present in the github repository.

If you want to try to build a version of Executor, try this:

  cd src
  autoreconf --install
  cd ..
  mkdir build
  cd build
  ../src/configure
  make
  make install

That should leave you with a binary called "executor" (at least on
32-bit x86 Linux systems, or a Mac (Intel or PowerPC) with XCode 3
installed) and a directory called "skel".  By default, executor will
be installed in "/usr/local/bin" and skel will be installed in
"/usr/local/share/executor".

skel itself isn't meant to be written to; it's meant to be copied
somewhere and for the copy to be read/write.  For example,

  cp -Rp /usr/local/share/executor/skel/volume /tmp/ExecutorVolume
  export SystemFolder="/tmp/ExecutorVolume/System Folder"
  executor

The environment variables that drive Executor including where Executor
looks for the system folder are a bit arcane and meant to fit together
with installers that we no longer use.  It may make sense to make it
so that Executor automatically copies the contents of skel/volume to a
users home-directory the first time Executor is run, but that's an
exercise left to the reader.

If you're on an x86_64 based machine and you have the ability to compile
32-bit binaries and you have these packages installed (where the ? is 3 for
Fedora 10, 5 for Fedora 11 and 6 for Fedora 12):

  SDL-devel.i?86
  libX11-devel.i?86
  db4-devel.i?86
  libXext.i?86
  libXrender.i?86
  libXrandr.i?86

you should be able to build a copy of Executor by overriding CC and
OBJC to be the 32-bit version and explicitly stating that you're
building on a 32-bit system (which is a little bit untrue, but works):

  mkdir -p build/debug-linux
  cd build/debug-linux
  CC='gcc -m32' OBJC='gcc -m32' ../../src/configure --build=i686-pc-linux
  make

The above works on Fedora 10, 11 and 12 x86_64.

Yes, the above is pretty messy.  Since the x86_64 bit native version of
Executor doesn't build and is likely to anytime soon, it probably makes
sense to make it so that by default we build the ix86 version of Executor
when compiling on x86_64, and that we complain if the various 32-bit
libraries aren't available.

In the past it was possible to cross-compile a version of Executor for
Windows using mingw32.  So far that port hasn't been tried since
putting the code on Github.  It probably doesn't work, but probably
could be made to work without much effort.

At one time we had Executor running on the Alpha, a 64-bit
environment.  We actually modified gcc to support "pointer bit-fields"
and were able to get Executor to run.  Now that 64-bit machines are
more popular, I've begun an experiment in supporting 64-bit machines
without modifying the compiler.  See README.64 for more info.

As I continue Executor's transition to a recent gnu build system I may
inadvertantly push versions that don't build when using the above
commands.  Don't hesitate to ask me if you're having trouble
compiling, linking or running any of the above platforms.

My email address is still <ctm@ardi.com>, although ARDI itself is
defunct.  I get a ridiculous amount of spam and will quite possibly
not see email addressed to me.  I'm ctm on github (http://github.com)
and typically check my email there once a day.

--Cliff Matthews
