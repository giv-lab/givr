.. GIVR documentation master file, created by
   sphinx-quickstart on Fri Jan 18 12:39:20 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to GIVR's documentation!
================================

givr is an MIT licensed, type-safe API for rendering geometry.  It was
primarily developed by Lakin Wecker, Jeremy Hart and Andrew Owens for use
in CPSC 587 - Computer Animation at the University of Calgary.

givr provides an API for getting basic geometry onto the screen with few
lines of code and with a type-safe API that prevents many common errors.
It is written using C++-17 and provides syntax that allows you to quickly
specify geometry details, style parameters and the camera/projection (view)
such that the geometry appears on screen.

It makes use sof C++ templates and type checking to ensure that only compatible
instances of each are used together and that all required parameters are
specified.

givr requires that you provide an OpenGL context and allows you to use
whichever windowing and user input library you prefer. It also requires that
you provide the main loop of the program.

This documentation is split into two parts. The first part is a user manual
which guides you through downloading, building and running your first givr
based program.  The second is a reference manual that covers all of the API
and their parameters.


.. toctree::
   :maxdepth: 2
   :caption: User Manual:

   user-manual

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
