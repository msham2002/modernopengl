# C++ Raycaster

A classic raycasting engine, built in C++ using modern OpenGL. This project renders a 2D map into a 3D perspective with texture mapped walls, floors, and ceilings.

---
## Dependencies

* **SDL2**
* **GLEW**
* **GLM**
* **stb_image**

All dependencies are included in the `vendor/` directory.

---
## How to Build

This project uses CMake to generate a build system.

**Prerequisites:**
* A C++ compiler
* CMake 3.10+

**Steps:**

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/msham2002/raycaster.git](https://github.com/msham2002/raycaster.git)
    cd modernopengl
    ```

2.  **Generate project files:**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Compile:**
    ```bash
    cmake --build .
    ```

The executable (`raycaster.exe`) will be located in the `build/Debug` or `build/Release` directory.
