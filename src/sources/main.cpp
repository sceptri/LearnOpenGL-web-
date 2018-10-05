/*
    Just re-write it, it's a piece of junk which you can't navigate through
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <include/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/headers/shader_s.h"

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);
int main()
{
    //glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //load OpenGL function pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Console output of max number of VBOs in VAO (vertex attributes in vertex array object)
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    //enabling Depth Test
    glEnable(GL_DEPTH_TEST);

    /* ------------ Hello triangle stuff ------------ */

    //creating vertex array object and binding it
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //vertices, with colors and with texture coords
    float vertices[] =
    {
        -0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,0.5f,-0.5f,

        -0.5f,0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,

        0.5f,0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,

        -0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        -0.5f,0.5f,0.5f,

        -0.5f,0.5f,0.5f,
        -0.5f,0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        0.5f,0.5f,0.5f,

        -0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,0.5f

    };

    float textureCoords[] =
    {

        0.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f,
        0.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f,
        0.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f,
        0.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f,
        0.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f,
        0.0f,0.0f,
        0.0f,1.0f,
        1.0f,1.0f,
        1.0f,0.0f


    };

    int indices[] =
    {
        0,1,3,
        3,1,2,
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22

    };

    //creating, binding and loading EBO with data
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

    //creating, binding and loading VBO with data
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

    //specifing arrangment of values in our VBO for vertex shader, enabling 0th attrib array (layout (location = 0) in our shader)
    //Takes data from currently bound buffer to GL_ARRAY_BUFFER and "binds" them to according position in VAO
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int texVBO;
    glGenBuffers(1, &texVBO);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (textureCoords), textureCoords, GL_STATIC_DRAW);
    //"Binding" texture coordinates to third attrib array
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //generating and binding texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //set texture wrapping/filtering options only on the currently bound texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //set all images to be flipped to have origin in the bottom left corner
    stbi_set_flip_vertically_on_load(true);

    //load and generate texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("/home/sceptri/Dev/Dev/QtWorkspace/LearnOpenGLweb/res/images/container.jpg", &width, &height, &nrChannels, 0); //loading image into memory and then passing pointer to it
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //loading texture data into bound texture
        glGenerateMipmap(GL_TEXTURE_2D); //generating mipmap
    }
    else
    {
        std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
    }
    stbi_image_free(data);

    //same stuff for second texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    data = stbi_load("/home/sceptri/Dev/Dev/QtWorkspace/LearnOpenGLweb/res/images/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    //loading shaders, parsing them, linking them etc.
    Shader ourShader("/home/sceptri/Dev/Dev/QtWorkspace/LearnOpenGLweb/res/shaders/shader.vs", "/home/sceptri/Dev/Dev/QtWorkspace/LearnOpenGLweb/res/shaders/shader.fs"); //absolute path

    //"Binding" color coordinates to second attrib array
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof (float))); //second from the end is stride in bytes and last one is offset also in bytes

    //Enabling attrib array to be used
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //unbinding VBO and VAO
    glBindVertexArray(0);

    /* ------------ Here it ends ------------ */

    /* ------------ Matrix Transformations for Coordinates systems ----------- */
    //local coordinates of the model
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view; //converts local coordinates to global
    //we translate in reverse direction that we actually want -> we're moving whole world(camera is static)
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    //projection matrix, more info on: http://www.songho.ca/opengl/gl_projectionmatrix.html
    projection = glm::perspective(glm::radians(45.0f), (float)(8/6), 0.1f, 100.0f);

    /* ------------ Here it ends ------------ */

    //Draw in wireframe mode?
    //yes
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //no
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(!glfwWindowShouldClose(window))
    {        

        //matrix stuff
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        /*
        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        */

        //input
        processInput(window);

        //clearing window with color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //Drawing objects--------------
        //use shader program
        ourShader.use();
        ourShader.setInt("Texture2", 3);
        ourShader.setMatrix("projection", projection);
        ourShader.setMatrix("view", view);
        ourShader.setMatrix("model", model);


        //loading value for the un. var. to the uniform variable
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //sets model equal again to identity matrix modified to rotate as stated
        model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//resizing screen -> modifying viewport
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0,0, width, height);
}

//setting esc to close window
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)== GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
