//
//  2dc.h
//  2dc
//
//  Created by David Kviloria on 10/14/20.
//  Copyright © 2020 David Kviloria. All rights reserved.
//

#ifndef _dc_h
#define _dc_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <strings.h>
#include "GLFW/glfw3.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "linmath/linmath.h"

#ifdef TWO_D_C_NO_STATIC
#define TWO_D_C_FUNC extern inline
#else
#define TWO_D_C_FUNC static inline
#endif

//
// Vector2d
//
#define PI 3.14159265
#define DEG PI / 180.0f

typedef struct Vector2df
{
float x, y;
} Vec2df;

TWO_D_C_FUNC float magnitude_vec2df(Vec2df *vec) {
    return (float) sqrt((vec->x * vec->x) + (vec->y * vec->y));
}

TWO_D_C_FUNC Vec2df normalize_vec2df(Vec2df *vec) {
    float len = magnitude_vec2df(vec);
    return (struct Vector2df) {
        .x = vec->x / len,
        .y = vec->y / len
    };
}

TWO_D_C_FUNC Vec2df add_vec2df(Vec2df *a, Vec2df *b) {
    return (struct Vector2df) {
        .x = a->x + b->x,
        .y = a->y + b->y
    };
}

TWO_D_C_FUNC Vec2df subtract_vec2df(Vec2df *a, Vec2df *b) {
    return (struct Vector2df) {
        .x = a->x - b->x,
        .y = a->y - b->y
    };
}

TWO_D_C_FUNC Vec2df divide_vec2df(Vec2df *vec, float angle) {
    float radius = (float) angle * DEG;
    return (struct Vector2df) {
        .x = (float) (vec->x * cos(radius) - vec->y * sin(radius)),
        .y = (float) (vec->x * sin(radius) + vec->y * cos(radius))
    };
}


//
// Entity
//
struct Entity
{
    const char *name;
    unsigned int index;
    vec2 position;
    vec2 rotation;
};

#define M_SIZE 512

struct Entity *manager[M_SIZE];

TWO_D_C_FUNC
struct Entity *get_entity(const char *name, struct Entity *_manager[]) {
    for (unsigned int i = 0; i <= M_SIZE; i++) {
        if (_manager[i] == NULL) continue;
        if (strcmp(name, _manager[i]->name) == 0) return _manager[i];
    }
    return NULL;
}

TWO_D_C_FUNC void
register_entity(struct Entity *entity, struct Entity *_manager[]) {
    _manager[*&entity->index] = entity;
}

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


//
// Utility functions
//
TWO_D_C_FUNC
const char *read_file_content(const char *path) {
    
    FILE *infile;
    char *buffer;
    long numbytes;
    
    infile = fopen(path, "r");
    if (infile == NULL) {
        perror("fopen\t");
        exit(-1);
    }
    // TODO: check the fucking here
    
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
 
    fseek(infile, 0L, SEEK_SET);
    
    buffer = (char*) calloc(numbytes, sizeof(char));
    if(buffer == NULL) exit(-1);

    fread(buffer, sizeof(char), numbytes, infile); fclose(infile);
    return buffer;
}

//
// GL Texture
//
TWO_D_C_FUNC
unsigned int load_and_generate_texture(char const *path, unsigned short slot) {

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(0, &texture);

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return texture;
}

//
// VertexArray
//

typedef struct{
unsigned int m_renderer_id;
} VertexArray;

TWO_D_C_FUNC
void gen_and_bind_vertex_array(VertexArray *vao) {
    glGenVertexArraysAPPLE(1, &vao->m_renderer_id);
    glBindVertexArrayAPPLE(vao->m_renderer_id);
}

TWO_D_C_FUNC
void unbind_vertex_array(VertexArray *vao) {
    glBindVertexArrayAPPLE(0);
}

TWO_D_C_FUNC
void free_vertex_array(VertexArray *vao) {
    glDeleteVertexArraysAPPLE(1, &vao->m_renderer_id);
}

//
// VertexBuffer
//

typedef struct {
unsigned int m_renderer_id;
} VertexBuffer;


TWO_D_C_FUNC
void gen_vertex_buffer(VertexBuffer *vbo) {
    glGenBuffers(1, &vbo->m_renderer_id);
}

TWO_D_C_FUNC
void bind_vertex_buffer(VertexBuffer *vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->m_renderer_id);
}

TWO_D_C_FUNC
void vertex_buffer_data(intptr_t size, float *data) {
    glBufferData(GL_ARRAY_BUFFER, size, *&data, GL_STATIC_DRAW);
}

TWO_D_C_FUNC
void unbind_vertex_buffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TWO_D_C_FUNC
void free_vertex_buffer(VertexBuffer *vbo) {
    glDeleteBuffers(1, &vbo->m_renderer_id);
}


//
// IndiceBuffer
//

typedef struct {
unsigned int m_renderer_id;
}IndiceBuffer;


TWO_D_C_FUNC
void gen_indice_buffer(IndiceBuffer *ebo) {
    glGenBuffers(1, &ebo->m_renderer_id);
}

TWO_D_C_FUNC
void bind_indice_buffer(IndiceBuffer *ebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->m_renderer_id);
}


TWO_D_C_FUNC
void indice_buffer_data(intptr_t size, unsigned int *data) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, *&data, GL_STATIC_DRAW);
}

TWO_D_C_FUNC
void free_indice_buffer(VertexBuffer *ebo) {
    glDeleteBuffers(1, &ebo->m_renderer_id);
}

//
// ShaderProgram
//

typedef struct {
unsigned int m_program_id;
} ShaderProgram;

TWO_D_C_FUNC
unsigned int compile_shader(const char *content, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &content, NULL);
    glCompileShader(shader);
    return shader;
}

TWO_D_C_FUNC
void craete_program(ShaderProgram *p, unsigned int fsh, unsigned int vsh) {
    p->m_program_id = glCreateProgram();
    
    glAttachShader(p->m_program_id, fsh);
    glAttachShader(p->m_program_id, vsh);
    glLinkProgram(p->m_program_id);
}

TWO_D_C_FUNC
void use_program(ShaderProgram *p) {
    glUseProgram(p->m_program_id);
}

TWO_D_C_FUNC
void draw_traingles(int32_t count) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

#ifdef __cplusplus
};
#endif

#endif /* _dc_h */
