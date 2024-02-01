#include <glad/glad.h>
#include <GLFW/glfw3.h>
//Import the following to load shaders
#include <iostream>
#include <string>

//GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//Modifier for the model's x Position
float x_mod = 0;
float y_mod = 0;
float theta_mod = 0;
float scale_mod = 1;


void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    //When use presses D
    if (key == GLFW_KEY_D &&
        action == GLFW_PRESS) {
        //Move bunny to right
        x_mod += 0.1f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_A &&
        action == GLFW_PRESS) {
        //Move bunny to right
        x_mod -= 0.1f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_W &&
        action == GLFW_PRESS) {
        //Move bunny to right
        y_mod += 0.1f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_S &&
        action == GLFW_PRESS) {
        //Move bunny to right
        y_mod -= 0.1f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_G) {
        //Move bunny to right
        theta_mod += 4.0f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_H) {
        //Move bunny to right
        theta_mod -= 4.0f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_P) {
        //Move bunny to right
        scale_mod += 4.0f;
        //std::cout << x_mod; check the values
    }

    if (key == GLFW_KEY_L) {
        //Move bunny to right
        scale_mod -= 1.0f;
        //std::cout << x_mod; check the values
    }



}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Rafael Ira Rambano Villanueva", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //Set the callback function to the window
    glfwSetKeyCallback(window, Key_Callback);

    //Load the shader file into a string stream
    std::fstream vertSrc("Shaders/Raputa.vert");
    std::stringstream vertBuff;
    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //Load the shader file into a string stream
    std::fstream fragSrc("Shaders/Raputa.frag");
    std::stringstream fragBuff;
    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0]; //show the error

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertexShader); // Don't leak the shader.
        
    }
    

    //Create a Fragment Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);

        std::cout << &errorLog[0]; //show the error

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragShader); // Don't leak the shader.
        //return;
    }


    //Create the Shader Program
    GLuint shaderProg = glCreateProgram();
    //Attach the compiled Vertex Shader
    glAttachShader(shaderProg, vertexShader);
    //Attach the compiled Fragment Shader
    glAttachShader(shaderProg, fragShader);

    //Finalize the compilation process
    glLinkProgram(shaderProg);




    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
            );
    }

    GLfloat vertices[]
    {
        0.f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        0.5f, -0.5f, 0.f
    };

    GLuint indices[]{
        0,1,2
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * attributes.vertices.size(),
        attributes.vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    
    float x = 0, y = 0, z = 0;
    float scale_x = 1, scale_y = 1, scale_z = 1;
    float theta = 90;
    float axis_x = 0, axis_y = 1, axis_z = 0;

 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        x = x_mod;
        y = y_mod;
        theta = theta_mod;
        scale_x = scale_mod;


        //Start with the translation matrix
        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x, y, z)
        );

        //Multiply the resulting matrix with the scale matrix
        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        //Finally multiply it with the rotation matrix
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );

        //Get the location of the transform variable in the shader
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        //Assign the matrix
        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE, //transpose?
            glm::value_ptr(transformation_matrix)); //Pointer to the matrix



        //Tell open GL to use this foler
        //for the VAO/s below
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}






/*
glBegin(GL_POLYGON);
glVertex2f(0.2167, 0.6656); // top
glVertex2f(0.7000, 0.0000); // right
glVertex2f(0.2146, -0.6663); // bottom right
glVertex2f(-0.5671, -0.4104); // bottom left
glVertex2f(-0.5658, 0.4122); // left 
glEnd(); */