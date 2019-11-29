.. _quick-start:
Quick Start
___________
.. highlight:: c++

You must have a compiler which can compile C++17. Thankfully, most
recent compilers release in the past couple of years (written late 2019)
will do a sufficient job for givr.


Building the Examples
=====================
.. highlight:: bash

There are some simple examples you can use to get started with givr.
Included within them are all necessary dependencies to compile except
for a compiler and CMake.

You will need a compiler capable of compiling C++17. We have tested these
projects on various recent distributions and it compiles and runs.

Download a recent version of the examples. They are included as part of
the givr distribution, which can be downloaded here:
https://github.com/lakinwecker/givr/archive/v0.0.8.zip
Unzip this file somewhere.  Then continue to your OS specific instructions.

You can also use git to grab a copy, the gitlab repository is here:
https://github.com/lakinwecker/givr/tree/v0.0.8


Windows
-------
On windows, you will need Visual Studio 2017. You may also need to update it
after installation so that your compiler is up to date (launch Visual Studio
and click on the flag in the top corner).  You will also need to download
and install cmake: https://cmake.org/download/

Create a :code:`build` directory which will contain all of the build files. You
can create this anywhere, but I most often create it next to or within the
directory that was created above.

Now run the cmake-gui command from your start menu. It requires that you
specify two directories.  The first is the directory which contains the
CMakeLists.txt you wish to build.  The second is the :code:`build` directory
that you created.

Once these have been selected, press the configure button. It will
run for a few seconds, produce some output in the bottom frame and then show
you some configuration entries that you can change if you would like. You do
not need to change any of these, despite the fact that they are coloured red.
Next, press the generate button. Finally, press the Open Project button.

Note that by default the project will not have a default startup file set. You
can choose one of the example programs as your default and then build and run it.

If you want more information on how to use cmake-gui on windows they have
instructions here: https://cmake.org/runningcmake/

Windows Video
*************

.. raw:: html

   <iframe width="696" height="392" src="https://www.youtube.com/embed/BuQvK3zlzwY" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


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


Using the example project
=========================

There is a sample project you can use as a basis for your project.
It is available at the link below and comes with a cmake file for
building your project. The build instructions follow those given
for the examples above.

Download the project here:

* `example-project <https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-simple-project/-/archive/v0.0.8/givr-simple-project-v0.0.8.zip

Including givr in an existing project.
======================================

We have also provided a simple way to get givr up and running for your own project.
If you already have a project and want to integrate it directly, then follow these steps

Step 1: Download givr.h
-----------------------
givr is distributed as two files, :code:`givr.h` which you include in your
programs and a :code:`givr.cpp` which you must compile and link with your
programs.

They can be downloaded here: 

 * `givr.h <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/build/givr.h>`_
 * `givr.cpp <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/build/givr.cpp>`_

Step 2: Obtain a windowing library 
----------------------------------
If you already have a windowing library setup, you may skip this step.
If not, then read on for a brief overview of how to setup windowing/opengl
in your program.

You can use any windowing library with givr. However, for this documentation,
we will use :code:`glfw3`. There are many ways to obtain and install
:code:`glfw3`. We recommend (vcpkg) <https://github.com/Microsoft/vcpkg>. You
will also need a method for initalizing the OpenGL libraries. We are using
:code:`glad` in the sample project and examples, which may also be installed
via :code:`vcpkg`.

Finally, we will also use a set of helpers that wraps :code:`glad` and
:code:`glfw3` into a simpler API. This library is called :code:`givio`
and is available here:

* `io.h <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/examples/libs/io.h>`_


Step 3: Your main program
-------------------------
You will need a main.cpp to run your program. You can write your own or
start from this simple example of a triangle:

 * `triangle.cpp <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/examples/bin-src/triangle.cpp>`_

