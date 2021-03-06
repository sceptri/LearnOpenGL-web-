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

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

float deltaTime = 0.0f;

float lastX = 400;//one half of app window width
float lastY = 300;//one half of app window height //cursor coords

float pitch = 0;
float yaw = 0;

float fov = 45.0f;

bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    //stbi_set_flip_vertically_on_load(true);

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
    //local coordinates of the mode
    /*
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    */


    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::mat4();
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


    glm::mat4 projection;
    //projection matrix, more info on: http://www.songho.ca/opengl/gl_projectionmatrix.html
    projection = glm::perspective(glm::radians(45.0f), (float)(8/6), 0.1f, 100.0f);

    /* ------------ Here it ends ------------ */

    //Draw in wireframe mode?
    //yes
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //no
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    float lastTime = 0.0f;
    float currentTime = 0.0f;

    while(!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;

        /*
        float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        */
        //lookAt() - first is cam pos, second is target pos, third is up direction of our global space

        /*
        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        */

        //input
        processInput(window);

        cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront.y = sin(glm::radians(pitch));
        cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(cameraFront);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        projection = glm::perspective(glm::radians(fov), (float)(8/6), 0.1f, 100.0f);

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


        //loading value for the un. var. to the uniform variable
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
          glm::mat4 model;
          model = glm::translate(model, cubePositions[i]);
          float angle = 20.0f * i;
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          ourShader.setMatrix("model", model);

          glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        //sets model equal again to identity matrix modified to rotate as stated

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;
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

    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
       cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //reversed
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if(fov <= 1.0f)
    fov = 1.0f;
  if(fov >= 45.0f)
    fov = 45.0f;
}
