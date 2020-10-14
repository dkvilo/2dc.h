//
//  main.c
//  2dc - sandbox
//
//  Created by David Kviloria on 10/11/20.
//  Copyright © 2020 David Kviloria. All rights reserved.
//

#include <stdio.h>
#include <math.h> // cos, sin

#include <GLFW/glfw3.h>

#include "../2dc.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, const char * argv[]) {

    GLFWwindow* window;
    int mvp_location, uTime_location;

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1080, 720, "2dc.h Batch renderer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    struct Entity player = {
        .name = "Player",
        .index = 0,
        .position = {
            0.f,
            0.f
        },
    };

    register_entity(&player, manager);
    
    VertexBuffer vbo;
    gen_vertex_buffer(&vbo);
    bind_vertex_buffer(&vbo);
    vertex_buffer_data(sizeof(vertices), vertices);
    
    IndiceBuffer ebo;
    gen_indice_buffer(&ebo);
    bind_indice_buffer(&ebo);

    VertexArray vao;
    gen_and_bind_vertex_array(&vao);
        
    bind_vertex_buffer(&vbo);
    vertex_buffer_data(sizeof(vertices), vertices);

    bind_indice_buffer(&ebo);
    indice_buffer_data(sizeof(indices), indices);
    
    ShaderProgram program;
    craete_program(
       &program,
       compile_shader(
        read_file_content("./shaders/vertex_shader.vert"),
        GL_VERTEX_SHADER
       ),
       compile_shader(
        read_file_content("./shaders/fragment_shader.frag"),
        GL_FRAGMENT_SHADER
       )
    );

    mvp_location = glGetUniformLocation(program.m_program_id, "MVP");
    uTime_location = glGetUniformLocation(program.m_program_id, "uTime");

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    float ratio;
    int width, height;
    
    mat4x4 model, projection, mvp;
    double deltaTime, previousTime, elapsedTime;
    
    float p_speed = 0.05f;
    struct Entity *ent_player = get_entity("Player", manager);
    if (ent_player == NULL) {
        printf("Player Not Found");
        exit(-1);
    }
    
    while (!glfwWindowShouldClose(window)) {
        
        deltaTime = glfwGetTime();
        previousTime = deltaTime;
        elapsedTime = deltaTime - previousTime;
        
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 0, 1);
        
        mat4x4_identity(model);
        mat4x4_ortho(projection, -ratio, ratio, 1.f, -1.f, 1.f, -100.f);
        mat4x4_translate(model, ent_player->position[0], ent_player->position[1], 0.f);
        mat4x4_mul(mvp, projection, model);

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glUniform1f(uTime_location, deltaTime);
        
        // Use Shader Program
        use_program(&program);
        
        // Render
        draw_traingles(sizeof(vertices) / sizeof(vertices[0]));
        
        
        // Player Controlls
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            ent_player->position[0] = ent_player->position[0] += p_speed;
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            ent_player->position[0] = ent_player->position[0] -= p_speed;
    
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            ent_player->position[1] = ent_player->position[1] -= p_speed;
                
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            ent_player->position[1] = ent_player->position[1] += p_speed;
            

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
