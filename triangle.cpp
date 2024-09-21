#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <iostream>

//g++ triangle.cpp glad.c -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processSpace(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

bool space = false;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.3f, -0.8f, 0.0f, // left  
         0.3f, -0.8f, 0.0f, // right 
         0.0f,  0.0f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    // -----------
    const float pi = 3.1415f;
		float w = (sqrt(9.8f/0.7f));
    float t = glfwGetTime();
    float phi = (pi/2)*sin(w*t);
    float deltaphi = 0;
    float lastphi = 0;
    float x1 = vertices[0];
    float x2 = vertices[3];
    float x3 = vertices[6];
    float y1 = vertices[1];
    float y2 = vertices[4];
    float y3 = vertices[7];
    float xx1 = vertices[0];
    float xx2 = vertices[3];
    float xx3 = vertices[6];
    float yy1 = vertices[1];
    float yy2 = vertices[4];
    float yy3 = vertices[7];
    float lastt = 0;
    int i = 0;
    float s[2] = {(vertices[0]+vertices[3]+vertices[6])/3,(vertices[1]+vertices[4]+vertices[7]/3)};
    float f[2] = {0,0};
    while (!glfwWindowShouldClose(window))
    {
        t = glfwGetTime();

        // input
        // -----
        processInput(window);

        processSpace(window);
        
        if(!space){
            phi = (pi/2)*sin(w*t);
            deltaphi = phi - lastphi;
            lastphi = phi;
            
            /* Para un reloj
            phi = (int)glfwGetTime();
            if(phi!=lastphi){
                deltaphi=-((2*pi)/12); 
                lastphi=phi;
            }else{
                deltaphi=0;
            }
            */
            x1 = vertices[0];
            x2 = vertices[3];
            y1 = vertices[1];
            y2 = vertices[4];
            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shaderProgram);
            vertices[0]= vertices[6]+ ((x1-vertices[6])*cos(deltaphi) - (y1-vertices[7])*sin(deltaphi));
            vertices[1]= vertices[7]+ ((x1-vertices[6])*sin(deltaphi) + (y1-vertices[7])*cos(deltaphi));
            vertices[3]= vertices[6]+ ((x2-vertices[6])*cos(deltaphi) - (y2-vertices[7])*sin(deltaphi));
            vertices[4]= vertices[7]+ ((x2-vertices[6])*sin(deltaphi) + (y2-vertices[7])*cos(deltaphi));
            
            s[0] = (vertices[0]+vertices[3]+vertices[6])/3;
            s[1] = (vertices[1]+vertices[4]+vertices[7])/3;
            lastt = t;
            xx1 = x1;
            xx2 = x2;
            xx3 = x3;
            yy1 = y1;
            yy2 = y2;
            yy3 = y3;
            t = 0;           
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }else{
            t = (glfwGetTime()-lastt);
            x1 = vertices[0];
            x2 = vertices[3];
            x3 = vertices[6];
            y1 = vertices[1];
            y2 = vertices[4];
            y3 = vertices[7];

            s[0] = (vertices[0]+vertices[3]+vertices[6])/3;
            s[1] = (vertices[1]+vertices[4]+vertices[7])/3;
            
            if(s[0] > 0){
                f[0] = (s[0]+((x2-x1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2))));
                f[1] = (s[1]+((y2-y1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2))));
            }else{
                f[0] = (s[0]+((x1-x2)/sqrt(pow(x1-x2,2)+pow(y1-y2,2))));
                f[1] = (s[1]+((y1-y2)/sqrt(pow(x1-x2,2)+pow(y1-y2,2))));
            }
                        
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shaderProgram);
            
            vertices[0] = xx1+t*f[0];
            vertices[1] = yy1 + t*f[1] + ((pow(t,2)*-9.8f+f[1])/2);// + ((9.8f*pow(2,t))/2);
            vertices[3] = xx2+t*f[0];
            vertices[4] = yy2 + t*f[1] + ((pow(t,2)*-9.8f+f[1])/2);// + ((9.8f*pow(2,t))/2);
            vertices[6] = xx3+t*f[0];
            vertices[7] = yy3 + t*f[1] + ((pow(t,2)*-9.8f+f[1])/2);// + ((9.8f*pow(2,t))/2);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }

				
    		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    		glBindBuffer(GL_ARRAY_BUFFER, 0); 

    		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    		glBindVertexArray(0); 
				
				glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processSpace(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        space = true; 
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
