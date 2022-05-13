// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"


// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacionBajo();
void animacionPuerta();
void animacionReproductor();
void animacionMarceline();
void animacionArania();



// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
//bool esta_abajo = true;
bool rotar = false;
float posicionBajo= 6.353f;
bool animBajo = false;
bool sentido = false;

bool sentidoPuerta = true;
float abrePuerta = 0.0f;
bool animPuerta = false;


bool animRueda = false;
float rotacionRueda = 0.0f;


bool animMarceline = false;
float posxmarceline = 7.9f;
float posymarceline = 0.78f;
float poszmarceline = -12.164f;
float rotmarceline = -90.0f;
bool recorridoMar1 = true;
bool recorridoMar2 = false;
bool recorridoMar3 = false;
bool recorridoMar4 = false;


bool animArania = false;
float posxarania = 1.19f;
float posyarania = 7.0f;
float poszarania = -4.636f;
bool recorridoAr1 = true;
bool recorridoAr2 = false;
bool recorridoAr3 = false;
float rotarania = 90.0;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/lamp.vs", "Shaders/lamp.frag");




    // Load models
   /* Model Pizza((char*)"Models/Pizza/PizzaSteve.obj");
    Model brader((char*)"Models/Pizza/brader.obj");*/
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    GLfloat vertices[] =
    {
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

    };

    //GLuint indices[] =
    //{  // Note that we start from 0!
    //    0,1,3,
    //    1,2,3

    //};

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load textures
    //Model pokeArriba((char*)"Models/Pokeball/pokearriba.obj");
    //Model pokeAbajo((char*)"Models/Pokeball/pokeabajo.obj");
    //Model objetoProyecto((char*)"Models/reproductor/reproductor.obj");
    Model casa((char*)"Models/casa/casa.obj");
    Model puerta((char*)"Models/casa/puerta.obj");
    Model alfombra1((char*)"Models/alfombra1/alfombra1.obj");
    Model alfombra2((char*)"Models/alfombra2/alfombra2.obj");
    Model lampara((char*)"Models/lampara/lampara.obj");
    Model cama((char*)"Models/cama/cama.obj");
    Model reproductor((char*)"Models/reproductor/reproductor.obj");
    Model bajo((char*)"Models/MarcelineBass/MarcelineBass.obj");
    Model sillon((char*)"Models/sillon/sillon.obj");
    Model tele((char*)"Models/tele/tele.obj");
    Model cuadro((char*)"Models/casa/2DOCUARTO/cuadro.obj");
    Model escalera((char*)"Models/casa/2DOCUARTO/escalera.obj");
    Model lampara_2((char*)"Models/casa/2DOCUARTO/lampara_2.obj");
    Model vela((char*)"Models/casa/2DOCUARTO/vela.obj");
    Model vela2((char*)"Models/casa/2DOCUARTO/vela.obj");
    Model sillon2((char*)"Models/casa/2DOCUARTO/sillon.obj");
    Model rueda((char*)"Models/reproductor/rueda.obj");
    Model finn((char*)"Models/Finn/finn.obj");
    Model marceline((char*)"Models/Marceline/marceline.obj");
    Model arania((char*)"Models/arania/arania.obj");


    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);



    ////Cambiar lo de alfa para PNG----------------------------------------------------------------------------------
    //image = stbi_load("images/goku.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //if (image)
    //{
    //    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    //    //glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(image);


    // Game loop

    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        animacionBajo();
        animacionPuerta();
        animacionReproductor();
        animacionMarceline();
        animacionArania();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //glDrawElements(GL_TRIANGLES, 6, GL_FLAT, 0);
        //Pizza.Draw(shader);

        //Casa
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        casa.Draw(shader);
        
        //Puerta 3.926   2.727     -6.871
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.1f, 2.754f, -6.894f));
        model = glm::rotate(model, glm::radians(abrePuerta), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        puerta.Draw(shader);


        //Alfombra1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(13.824f, 6.283f, -5.119f));
        model = glm::translate(model, glm::vec3(-12.7f, 0.075f, 0.0f));
        model = glm::scale(model, glm::vec3(0.31f, 0.313f, 0.265f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        alfombra1.Draw(shader);

        //Alfombra 2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.051f, 6.358f, -5.63f));
        model = glm::scale(model, glm::vec3(0.385f, 1.0f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        alfombra2.Draw(shader);

        //Lampara
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(14.282f, 6.192f, -5.717f));
        model = glm::translate(model, glm::vec3(-12.7f, 0.075f, 0.0f));
        model = glm::scale(model, glm::vec3(0.181f, 0.181f, 0.181f));
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lampara.Draw(shader);


        //Arania
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posxarania, posyarania, poszarania));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::rotate(model, glm::radians(rotarania), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arania.Draw(shader);

        //cama
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(14.041f, 6.283f, -6.364f));
        model = glm::translate(model, glm::vec3(-12.7f, 0.07f, 0.0f));
        model = glm::scale(model, glm::vec3(0.197f, 0.197f, 0.197f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cama.Draw(shader);
        
        //reproductor
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(14.26f, 6.26f, -6.948f));
        model = glm::translate(model, glm::vec3(-12.7f, 0.06f, 0.0f));
        model = glm::scale(model, glm::vec3(0.186f, 0.186f, 0.186f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        reproductor.Draw(shader);

        //rueda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.51f, 6.805f, -7.141f));      
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(rotacionRueda), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.195f, 0.195f, 0.195f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        rueda.Draw(shader);

        //rueda2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.63f, 6.805f, -7.141f));        
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(rotacionRueda), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.195f, 0.195f, 0.195f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        rueda.Draw(shader);        

        //bajo
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.026f, posicionBajo, -7.291f));        
        model = glm::scale(model, glm::vec3(0.083f, 0.083f, 0.083f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bajo.Draw(shader);        


        //sillon
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(11.521f, 6.283f, -5.118f));
        model = glm::translate(model, glm::vec3(-12.7f, 0.075f, 0.0f));
        model = glm::scale(model, glm::vec3(0.409f, 0.409f, 0.409f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sillon.Draw(shader);


        //tele
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(11.904f, 6.223f, -6.084f));
        model = glm::translate(model, glm::vec3(-12.7f, 0.075f, 0.0f));
        model = glm::scale(model, glm::vec3(0.233f, 0.233f, 0.233f));
        model = glm::rotate(model, glm::radians(-128.43f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tele.Draw(shader);

        //cuadro
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.182f, 1.72f, -10.159f));
        model = glm::scale(model, glm::vec3(0.389f, 0.389f, 0.389f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cuadro.Draw(shader);

        //escalera
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.221f, 1.267f, -1.209f));
        model = glm::scale(model, glm::vec3(0.39f, 0.358f, 0.39f));    
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        escalera.Draw(shader);

        //vela
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 2.841f, -10.636f));
        model = glm::scale(model, glm::vec3(0.115f, 0.115f, 0.115f));        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        vela.Draw(shader);

        //vela2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.244f, 2.841f, -10.636f));
        model = glm::scale(model, glm::vec3(0.115f, 0.115f, 0.115f));        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        vela2.Draw(shader);


        //sillon
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, 1.148f, -4.825f));
        model = glm::scale(model, glm::vec3(0.433f, 0.433f, 0.433f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sillon2.Draw(shader);

        //lampaara2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.276f, -7.475f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lampara_2.Draw(shader);

        //finn
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.12f, 1.327f, -3.28f));
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        finn.Draw(shader);

        //Marceline
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(posxmarceline, posymarceline, poszmarceline));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians(rotmarceline), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        marceline.Draw(shader);


        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        lampshader.Use();
        //glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (keys[GLFW_KEY_O])
    {
        rotar = true;
    }

}

void animacionBajo() {
    if (animBajo) {
        if (posicionBajo >= 6.5) {
            sentido = false;
        }
        if (posicionBajo < 6.353) {
            sentido = true;
        }
        if (sentido) {
            posicionBajo += 0.0005f;
        }
        else
            posicionBajo -= 0.0005f;

    }
}

void animacionPuerta() {
    if (animPuerta) {
        if (sentidoPuerta) {//Se tiene que abrir la puerta
            if (abrePuerta < 80.0) {
                abrePuerta += 0.5;
            }
            if (abrePuerta >= 80.0) {
                sentidoPuerta = false;
                animPuerta = false;
            }
        }
        else { //Se tiene que cerrar la puerta
            if (abrePuerta > 0.0) {
                abrePuerta -= 0.5;
            }
            if (abrePuerta <= 0.0) {
                sentidoPuerta = true;
                animPuerta = false;
            }
        }
    }
}

void animacionReproductor() {
    if (animRueda) {
        rotacionRueda += 0.45f;
    }
    else {
        rotacionRueda = rotacionRueda;
    }
}

void animacionMarceline() {
    if (animMarceline) {
        if (recorridoMar1) {
            rotmarceline = -90.0f;
            posymarceline += 0.031f;
            poszmarceline += 0.1f;
            if (posymarceline > 4.5 && poszmarceline > -1.3) {
                recorridoMar1 = false;
                recorridoMar2 = true;
            }
        }
        if (recorridoMar2) {
            rotmarceline = 180.0f;
            posxmarceline -= 0.1f;
            posymarceline -= 0.021;
            if (posxmarceline < -5.77 && posymarceline < 1.3) {
                recorridoMar2 = false;
                recorridoMar3 = true;
            }
        }
        if (recorridoMar3) {
            rotmarceline = 90.0f;
            posymarceline += 0.034;
            poszmarceline -= 0.1;
            if (posymarceline > 5.06 && poszmarceline < -12.164) {
                recorridoMar3 = false;
                recorridoMar4 = true;
            }
        }
        if (recorridoMar4) {
            rotmarceline = 0.0f;
            posxmarceline += 0.1f;
            posymarceline -= 0.031;
            if (posxmarceline > 7.9 && posymarceline < 0.78) {
                recorridoMar4 = false;
                recorridoMar1 = true;
            }
        }
    }
}

void animacionArania() {
    if (animArania) {
        if (recorridoAr1) {
            float v = 8.0f;
            float rad = 30.0 / 57.3;
            poszarania -= 0.008f;
            posyarania = 12 + (tan(rad) * poszarania - (9.8 / (2 * v * v * cos(rad) * cos(rad))) * poszarania * poszarania);            
            if (posyarania < 6.38) {
                recorridoAr1 = false;
                recorridoAr2 = true;
            }
        }
        if (recorridoAr2) {
            rotarania = 0;
            posxarania += 0.001;
            if (posxarania > 1.7) {
                recorridoAr2 = false;
                recorridoAr3 = true;
            }
        }
        if (recorridoAr3) {
            rotarania = 90;
            poszarania -= 0.001f;            
            if (poszarania < -5.3) {               
                recorridoAr3 = false;
            }
        }
    }
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (keys[GLFW_KEY_R]) {
        animRueda = !animRueda;
    }
    if (keys[GLFW_KEY_B]) {
        animBajo = !animBajo;
    }
    if (keys[GLFW_KEY_P]) {
        animPuerta = !animPuerta;
    }
    if (keys[GLFW_KEY_M]) {
        animMarceline = !animMarceline;
    }
    if (keys[GLFW_KEY_T]) {
        animArania = !animArania;
    }

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }




}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}