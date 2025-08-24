# Physically Based Rendering in OpenGL
A quick and easy way to get started using OpenGL 3.3 Core Profile together with [imgui](https://github.com/ocornut/imgui) in a CMake environment.

This template only contains a wrapper class for shaders, and one for the window with appropriate `Initialize/LoadContent/Update/Render` as a game loop.

The shader class allows for **hot-reloading** of the fragment shader, so whenever you modify the `testing.fs` file in the `build/resources/shaders/` directory, the shader will automagically be reloaded! 

**NOTE:** The shader files in `resources/shaders/` are copied into the `build/` directory upon build, so if you want to save your hot-reloaded changes, then you must also modify the shaders in `resources/shaders/` directory.

## Getting started

1. Clone repo.
2. Download some [GLFW pre-compiled binaries](https://www.glfw.org/download) and put the `libglfw3.a` file in `libs/glfw/`. 
3. Compile with CMake and then run it! Piece of cake!

## Remarks

* Uses [imgui version 1.83](https://github.com/ocornut/imgui/releases/tag/v1.83)
* Has only been tested on MingW64 compiler for Windows (so it may require some fixing for it to work for gcc or clang)

### The Rendering Equation
Also called the "light transport equation" in some literature:
$$
L_o = \int_\Omega{f(\omega_o, \omega_i, n)L_i(p, \omega_i)n \cdot \omega_i d\omega_i}
$$

Using [Lambertian](https://en.wikipedia.org/wiki/Lambertian_reflectance) and [Cook-Torrance](https://en.wikipedia.org/wiki/Specular_highlight#Cook%E2%80%93Torrance_model) for diffuse and specular reflection respectively where $k_d + k_s = 1$ and $c$ is the color :
$$
L_o = \int_\Omega{(k_d\frac{c}{\pi} + k_s\frac{DFG}{4(\omega_o \cdot n)(\omega_i \cdot n)})L_i(p, \omega_i)n \cdot \omega_i d\omega_i}
$$

By linearity
$$
L_o = k_d\frac{c}{\pi}\int_\Omega{L_i(p, \omega_i)n \cdot \omega_i d\omega_i} +

k_s\int\frac{DFG}{4(\omega_o \cdot n)(\omega_i \cdot n)}L_i(p, \omega_i)n \cdot \omega_i d\omega_i
$$
