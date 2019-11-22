Quick Start
============
.. highlight:: c++

You must have a compiler which can compile C++17. Thankfully, most
recent compilers release in the past couple of years (written late 2019)
will do a sufficient job for givr.


Step 1: Download givr.h
-----------------------
givr is distributed as two files, :code:`givr.h` which you include in your programs and a :code:`givr.cpp` which you must compile and link with your programs.

They can be downloaded here: 

 * `givr.h <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/build/givr.h>`_
 * `givr.cpp <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/build/givr.cpp>`_

Step 2: Setup windowing
-----------------------
You can use any windowing library with givr. For the quickstart, we will assume you have :code:`glfw3` installed and available to your compiler. There are many ways to do this. We recommend (vcpkg) <https://github.com/Microsoft/vcpkg>. You will also need a method for initalizing the OpenGL libraries. For this quickstart, we will use :code:`glad` which may also be installed via :code:`vcpkg`.

Finally, we will also use a set of helpers that wraps glad and glfw3 into a simpler API. This library is called :code:`givio` and is available here:

* `io.h <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/examples/libs/io.h>`_


Step 3: Your main program
-------------------------
You will need a main.cpp to run your program. You can write your own or start from this simple example of a triangle:

 * `triangle.cpp <https://gitlab.cpsc.ucalgary.ca/graphics-interaction-visualization/givr/raw/master/examples/bin-src/triangle.cpp>`_

Step 3: (Optional) Download an example project
----------------------------------------------
As a convenience, we have provided an example project which has all these things already included and setup.  You can download it here:

* `example-project <https://gitlab.cpsc.ucalgary.ca/lakin.wecker/givr-simple-project/-/archive/master/givr-simple-project-master.zip>`_


Step 4: Compilation
-------------------
There are many different compilers, build systems and IDEs that you can use to do graphics programming. Visual Studio, CLion, X-Code, cmake, g++, clang etc. etc.  Any version of these that is recent enough will work with givr.

More in depth compilation guidance is given in the :ref:`user-manual` section of the User Manual.  
If you use the above example project and you have cmake installed and available on the command line, then you can issue these commands from the root directory to build the example.

.. code-block:: bash
  :linenos:

    cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
    cmake --build build

    ./build/simple




