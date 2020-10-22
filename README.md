##  2D Batch Renderer


```
  2dc is a 2D renderer which uses renderer optimization - 2D batching,
  2dc is entirely written in pure C, based on Modern OpenGL API.
  It comes with simple Entity system and 2d Vector helper functions. it also uses
  stb_image.h and linmath.h as a dependency.
```

## Building

```bash
  # Install submodules 
  $ git submodule update --init --recursive

  # build glfw shared library
  $ make glfw

  $ make build # Build sandbox
```

### VBO & EBO Example
```c
    float vertices[] = {
       // positions          // colors           // texture coords
        0.2f,  0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,  // top right
        0.2f, -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,  // bottom right
       -0.2f, -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  // bottom left
       -0.2f,  0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  // top left
        
        0.1f,  0.3f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
        0.1f, -0.3f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // bottom right
       -0.1f, -0.3f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // bottom left
       -0.1f,  0.3f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
        
        0.1f,  0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  // top right
        0.1f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
       -0.1f, -0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,  // bottom left
       -0.1f,  0.1f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
    };

    unsigned int indices[] = {
      // first shape
      0, 1, 3, // first triangle
      1, 3, 2, // second triangle
      
      // Second shape
      4, 5, 7, // first triangle
      5, 7, 6,  // second triangle
        
      // Third shape
      8, 9, 11, // first triangle
      9, 11, 10,  // second triangle
    };
```
