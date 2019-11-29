.. GIVR documentation master file, created by
   sphinx-quickstart on Fri Jan 18 12:39:20 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to GIVR's documentation!
================================

givr is a GPLv3 licensed, type-safe API for rendering geometry.  It was
primarily developed by Lakin Wecker, Jeremy Hart, Andrew Owens, Kathleen Ang
and Dr. Przemyslaw Prusinkiewicz for use in CPSC 587 - Computer Animation
at the University of Calgary.

Computer graphics programs are incredibly rewarding. Not only do you get
immediate feedback about your program when you run it, the results are
enjoyable to watch and can be shown to your friends and family. However, modern
Computer Graphics programming environments are complicated.
They are designed to allow for maximum flexibility and performance and leave
you to do much of the verification yourself.  It is very easy to make a program
that compiles, runs and produces no output on the screen.

givr provides an API for getting basic geometry onto the screen with few
lines of code and with a type-safe API that prevents many common errors.
It is written using C++-17 and provides syntax that allows you to quickly
specify geometry details, style parameters and the camera/projection (view)
such that the geometry appears on screen.

It makes use of C++ templates and type checking to ensure that only compatible
instances of each are used together and that all required parameters are
specified.

givr requires that you provide an OpenGL context and allows you to use
any windowing and user input library. It also requires that you provide the
main loop for your program.

This documentation is split into three parts. The first part is a general
introduction to graphics programming and will cover how graphics programs
are often organized and which concepts are necessary.  The second part is
a user manual, which guides you through downloading, building and running
your first givr based program.  The third is a reference manual that covers
all of the API and their parameters.

.. toctree::
   :maxdepth: 2
   :caption: Quick Start

   quick-start/quick-start

.. toctree::
   :maxdepth: 3
   :caption: User Manual:

   user-manual/introduction
   user-manual/windowing
   user-manual/givr

.. toctree::
   :maxdepth: 2
   :caption: Reference Manual:

   reference-manual/camera_and_projection
   reference-manual/geometry
   reference-manual/styles


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
