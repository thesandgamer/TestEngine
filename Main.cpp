


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>

#include "Renderer.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


Renderer renderer;


bool firstMouse = true;
float lastMouseX = 0, lastMouseY = 0;


int main(int, char**)
{
    //On initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //Créer la fenêtre
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "TestEngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Set le contexte et initlise glad
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Pour pouvoir resize la fenêtre
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Enable le callback pour la souris: va écouter les inputs que va faire la souris et appeler la fonction mouse_callback avec la postion de la souris à l'écran
    glfwSetCursorPosCallback(window, mouse_callback);

    //Enable callback pour scroll de la souris
    glfwSetScrollCallback(window, scroll_callback);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
   // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    bool* rotationOpen= new bool(true);


    renderer.init();   
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last framec   


    //Set le mode de la souris pour ce focus sur dans l'écran
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        renderer.processInputs(window);

        renderer.update(deltaTime);

//--------------------------IMGUI
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGuiWindowFlags window_flags = 0;
            window_flags |= ImGuiWindowFlags_NoResize;

            ImGui::Begin("Rotation", rotationOpen, window_flags);
            ImGui::End();
        }
        //ImGui::ShowDemoWindow(); // Show demo window
//-------------------------

         //-------------------------DRAWING
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);   //Change la couleur de clean de l'écran

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //Clean l'écran avec la couleur et on clean le buffer de profondeur


        //Draw objects here
        renderer.draw();


//--------------------------IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
//--------------------------

        // (Your code calls glfwSwapBuffers() etc.)
        //-------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

   
    }

    renderer.end();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) 
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    //On va calculer la différence entre la position actuelle de la souris 
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // reversed since y-coordinates range from bottom to top
    lastMouseX = xpos;
    lastMouseY = ypos;

    //Multiplie l'offset (c'est à dire la différence entre où était la souris et là où elle est actuellement) par la sensibilité pour pas que ça aille trop vite
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    renderer.processMouse(xoffset, yoffset);

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    renderer.processScroll(yoffset);
}