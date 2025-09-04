# Physically Based Rendering in OpenGL


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
L_o = \int_\Omega{f_r(\omega_o, \omega_i, n)L_i(p, \omega_i)n \cdot \omega_i d\omega_i}
$$

Using [Lambertian](https://en.wikipedia.org/wiki/Lambertian_reflectance) and [Cook-Torrance](https://en.wikipedia.org/wiki/Specular_highlight#Cook%E2%80%93Torrance_model) for diffuse and specular reflection respectively where $k_d + k_s = 1$ and $c$ is the color :
$$
L_o = \int_\Omega{(k_d\frac{c}{\pi} + k_s\frac{DFG}{4(\omega_o \cdot n)(\omega_i \cdot n)})L_i(p, \omega_i)n \cdot \omega_i d\omega_i}
$$

By linearity:
$$
L_o = k_d\frac{c}{\pi}\int_\Omega{L_i(p, \omega_i)n \cdot \omega_i d\omega_i} +

k_s\int_\Omega\frac{DFG}{4(\omega_o \cdot n)(\omega_i \cdot n)}L_i(p, \omega_i)n \cdot \omega_i d\omega_i
$$

In essence:
$$
L_o = \int_\Omega{diffuse} + \int_\Omega{specular}

$$
