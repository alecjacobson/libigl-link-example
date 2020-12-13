# 🌱 Linking to prebuilt libigl libraries 🌱

Suppose you have a new project that will use libigl. You could
compile it in header only mode (`IGL_STATIC_LIBRARY` _not defined_), but every
compilation of your project will be slow. Or you could compile it in static
library mode by adding libigl as a subproject your projects cmake
(`include(libigl.cmake)`). However, the first compile of your project will take
a very long time. This also scales badly. If you have _n_ libigl projects,
you'll end up compiling _n_ copies of libigl. 

This project demonstrates how to link against prebuilt libigl static libraries.
The intended use case is for rapid prototyping many small libigl projects (each
within their own cmake environment) on a machine with a common libigl build
directory. The `FindLIBIGL.cmake` defined here is _compatible_ with the standard
libigl setup and controlled by the option `LIBIGL_USE_PREBUILT_LIBRARIES`).

## Example setup:

Assume that you've downloaded and compiled libigl _somewhere else_. For example,
maybe you ran:

    git clone https://github.com/libigl/libigl.git
    cd libigl
    mkdir build
    cd build
    cmake ../ -DLIBIGL_USE_STATIC_LIBRARY=ON -DLIBIGL_WITH_CGAL=ON -DLIBIGL_WITH_MATLAB=ON -DLIBIGL_WITH_MOSEK=ON
    make -j8

This `make` will take quite a while, but once it's done, the `libigl/build/`
director will contain static libraries:

    libigl/build/libCoMISo.a
    libigl/build/libglad.a
    libigl/build/libglfw3.a
    libigl/build/libigl.a
    libigl/build/libigl_cgal.a
    libigl/build/libigl_comiso.a
    libigl/build/libigl_embree.a
    libigl/build/libigl_matlab.a
    libigl/build/libigl_mosek.a
    libigl/build/libigl_opengl.a
    libigl/build/libigl_opengl_glfw.a
    libigl/build/libigl_opengl_glfw_imgui.a
    libigl/build/libigl_png.a
    libigl/build/libigl_predicates.a
    libigl/build/libigl_stb_image.a
    libigl/build/libigl_tetgen.a
    libigl/build/libigl_triangle.a
    libigl/build/libigl_xml.a
    libigl/build/libimgui.a
    libigl/build/libimguizmo.a
    libigl/build/libpredicates.a
    libigl/build/libtetgen.a
    libigl/build/libtinyxml2.a
    libigl/build/libtriangle.a

**_Now_**, consider another project _somewhere else_. For example,

    git clone https://alecjacobson/libigl-link-example
    cd libigl-link-example
    mkdir build
    cd build
    # replace ../../libigl with path to your libigl directory above
    cmake ../ -DLIBIGL_DIR=../../libigl/ -DLIBIGL_USE_PREBUILT_LIBRARIES=ON
    make -j8

This will simply compile your project and link to the externally located libigl
libraries (and – if all goes well – all necessary dependency libraries).

## 🐞 Debuggin'

It may be useful to build a second copy of libigl in debug mode. For example,
you might do:

    cd libigl
    mkdir debug
    cd debug
    cmake ../ -DCMAKE_BUILD_TYPE=DEBUG -DLIBIGL_USE_STATIC_LIBRARY=ON -DLIBIGL_WITH_CGAL=ON -DLIBIGL_WITH_MATLAB=ON -DLIBIGL_WITH_MOSEK=ON
    make -j8

If you similar build _your project_ in debug mode, then you can tell cmake to
use the debug version of libigl:

    cd libigl-link
    mkdir debug
    cd debug
    # replace ../../libigl with path to your libigl directory above
    cmake ../ -DLIBIGL_DIR=../../libigl/  -DLIBIGL_BUILD_DIR=../../libigl/debug -DCMAKE_BUILD_TYPE=DEBUG -DLIBIGL_USE_PREBUILT_LIBRARIES=ON
    make -j8
