Building the Examples
=====================
.. highlight:: bash

There are some simple examples you can use to get you started with givr.
Included within them are all necessary dependencies to compile except
for a compiler and CMake.

You will need a compiler capable of compiling C++17. We don't use all
of the features from C++17, and have tested these projects on various
recent distributions and it compiles and runs.

Download a recent version of the examples. The most recent version is:
https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-examples/-/archive/master/givr-examples-master.zip
Unzip this file somewhere.  Then continue to your OS specific instructions.

You can also use git to grab a copy, the gitlab repository is here:
https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-examples


Windows
-------
On windows, you will need Visual Studio 2017. You will also need to
download and install cmake: https://cmake.org/download/

Next to the directory that was created when you unzipped the above file,
create a `build` directory.

Now run the cmake-gui command from your start menu. It requires two directories
the first is the directory which contains the CMakeLists.txt from the file you
unzipped above.  The second is the  `build` directory that you created.

Once these have been selected, you can press the configure button. It will
run for a few seconds, produce some output in the bottom frame and then show
you some configuration entries that you can change if you would like. You do 
not need to change any of these, despite the fact that they are coloured red.
Next, press the generate button. Finally, press the Open Project button.

Note that by default the project will not have a default startup file set. You
can choose one of the example programs as your default and then build and run it.

If you want more information on how to use cmake-gui on windows they have
instructions here: https://cmake.org/runningcmake/


Linux
-----
In linux you will need to install cmake. All modern distributions come
with a way to install it that is pretty simple or straightforward. In
Ubuntu you can use::

   sudo apt-get install cmake build-essential

In arch linux you can use::

   pacman -Sy cmake

Once cmake is installed, navigate to the directory which contains the
`CMakeLists.txt` file and run these commands::

    cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
    cmake --build build

This will create a `build` directory and build the project within it.
You can then run the examples afterwards with::

   ./build/sph
   ./build/pbd

OSX
---
I don't have a mac on which to test these instructions, so I'm going to depend
on someone else to figure this out and tell me what they are. Ostensibly, you 
should be able to use the same commands as the linux version to build.

