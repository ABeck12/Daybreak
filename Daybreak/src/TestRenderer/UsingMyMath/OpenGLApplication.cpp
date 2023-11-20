//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_glfw.h>
//#include <imgui/imgui_impl_opengl3.h>
//
//#include "ErrorTest.h"
//#include "VecToString.h"
//
//#include "Renderer.h"
//#include "Texture.h"
//#include "VertexArray.h"
//#include "IndexBuffer.h"
//#include "VertexBuffer.h"
//#include "Shader.h"
//
//#include <iostream>
//#include <string>
//
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//const unsigned int PIX_PER_UNIT = 20;
//
//
//
//
//int main()
//{
//
//#pragma region Init
//    glfwInit();
//    //glEnable(GL_DEBUG_OUTPUT);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    glfwSwapInterval(1); // matches opengl swap timing to match display refresh rate
//
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    ImGui::CreateContext();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui::StyleColorsDark();
//    ImGui_ImplOpenGL3_Init("#version 330 core");
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//#pragma endregion
//
//    Shader shader("../Daybreak/src/TestRenderer/res/TextureShader.glsl");
//    //Shader shader("../Daybreak/src/TestRenderer/res/RainbowTest.glsl");
//    shader.Bind();
//
//
//    //float vertices[] = {
//    //     0.5f,  0.5f, 0.0f,  // top right
//    //     0.5f, -0.5f, 0.0f,  // bottom right
//    //    -0.5f, -0.5f, 0.0f,  // bottom left
//    //    -0.5f,  0.5f, 0.0f   // top left 
//    //};
//    //float vertices[] = {
//    //    100.0f, 100.0f, 0.0f, 0.0f,
//    //    200.0f, 100.0f, 1.0f, 0.0f,
//    //    200.0f, 200.0f, 1.0f, 1.0f,
//    //    100.0f, 200.0f, 0.0f, 1.0f  
//    //};
//   
//    //float vertices[] = {
//    //    -50.0f, -50.0f, 0.0f, 0.0f,
//    //     50.0f, -50.0f, 1.0f, 0.0f,
//    //     50.0f, 50.0f,  1.0f, 1.0f,
//    //    -50.0f, 50.0f,  0.0f, 1.0f
//    //};
//    float vertices[] = {
//    -50.0f, -50.0f, 0.0f, 0.0f, 0.0f,
//     50.0f, -50.0f, 0.0f, 1.0f, 0.0f,
//     50.0f,  50.0f, 0.0f, 1.0f, 1.0f,
//    -50.0f,  50.0f, 0.0f, 0.0f, 1.0f
//    };
//    
//
//    unsigned int indices[] = {  // note that we start from 0!
//        0, 1, 3,  // first Triangle
//        1, 2, 3   // second Triangle
//    };
//
//  
//    //float cubeVert[] = {
//    //    -.5f, -.5f,  .5f, 0.0f, 0.0f,
//    //     .5f, -.5f,  .5f, 1.0f, 0.0f,
//    //     .5f,  .5f,  .5f, 1.0f, 1.0f,
//    //    -.5f,  .5f,  .5f, 0.0f, 1.0f,
//    //    -.5f, -.5f, -.5f, 0.0f, 0.0f,
//    //     .5f, -.5f, -.5f, 1.0f, 0.0f,
//    //     .5f,  .5f, -.5f, 1.0f, 1.0f,
//    //    -.5f,  .5f, -.5f, 0.0f, 1.0f
//    //};
//    //float cubeIndices[] = {
//    //    0, 1, 3, //Front 1
//    //    1, 2, 3, //Front 2
//    //}
//
//
//    VertexArray va;
//    VertexBuffer vb(vertices, sizeof(vertices));
//
//    VertexBufferLayout layout;
//    layout.Push<float>(3);
//    layout.Push<float>(2);
//    va.AddBuffer(vb, layout);
//
//
//
//    glm::mat4 proj;
//    //glm::mat4 proj = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -100.0f, 100.0f); //transforms from screen space to NDC
//    
//    
//    //glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH/(float)SCR_HEIGHT, -1.0f, 100.0f); //transforms from screen space to NDC
//    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); //
//     //
//    
//    
//
//    IndexBuffer ib(indices, 6);
//    
//
//    Texture texture("C:/dev/Daybreak/Daybreak/src/TestRenderer/res/Textures/Test.png");
//    texture.Bind();
//    shader.SetUniform1i("u_Texture", 0);
//
//    Renderer renderer;
//
//    shader.Unbind();
//    va.Unbind();
//    vb.Unbind();
//    ib.Unbind();
//    
//    // uncomment this call to draw in wireframe polygons.
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    
//    float r = 0.0f;
//    float increment = 0.05f;
//
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//
//    glm::vec3 viewTranslation((float)SCR_WIDTH / 2, (float)SCR_HEIGHT / 2, 0);// this is a vec3 from screen corner to middle
//    //glm::vec3 viewTranslation(200, 200, 0); 
//
//
//    glm::vec3 CameraPosition(0.0f, 0.0f, 10.0f);
//
//    glm::vec3 modelLocation(0.0f);
//    glm::vec3 modelScale(1.0f);
//    float modelZRotation = 0.0f;
//
//    float fov = 90.0f;
//    float near = 0.0f;
//    float far = 10.0f;
//    bool usePerspective = false;
//    bool addOffset = false;
//
//    float theta = 0.0f;
//    float radius = 75.0f;
//    glm::vec3 imageoffsetvec(0.0f);
//#pragma region Loop
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        renderer.Clear();
//        
//        if (theta >= (6.0* 3.14159f))
//            theta = 0.0f;
//        theta += 0.05f;
//
//        imageoffsetvec.x = radius* glm::cos(theta);
//        imageoffsetvec.y = radius* glm::sin(theta);
//        imageoffsetvec.z = -glm::abs(5.0f * radius * glm::sin(theta / 3.0f) * glm::sin(theta / 3.0f));
//
//
//
//        
//        {
//            ImGui_ImplOpenGL3_NewFrame();
//            ImGui_ImplGlfw_NewFrame();
//            ImGui::NewFrame();
//
//
//            ImGui::Begin("Camera Debug");
//            ImGui::Checkbox("Use perspective", &usePerspective);
//            //ImGui::Checkbox("Add offset", &addOffset);
//            ImGui::SliderFloat("Fov", &fov, 0.f, 180.0f);
//            //ImGui::SliderFloat("Near", &near, -30.0f, 0.0);
//            //ImGui::SliderFloat("Far Bound", &far, 0.f, 10.0f);
//            ImGui::SliderFloat2("Pos", &CameraPosition.x, -900.0f, 900.0f);
//            ImGui::SliderFloat("Pos Z", &CameraPosition.z, 0.0f, 1000.0f);
//            //ImGui::SliderFloat2("View Translation", &viewTranslation.x, 0.0f, 800.0f);
//            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//            ImGui::End();
//
//
//
//            ImGui::Begin("Image Debug");
//            //ImGui::SliderFloat3("test", &test.x, -100.0f, 100.0f);
//
//            ImGui::SliderFloat3("Scale", &modelScale.x, 0.0f, 50.0f);
//            ImGui::SliderFloat2("Pos XY", &modelLocation.x, 0.0f, 900.f);
//            ImGui::SliderFloat("Pos Z", &modelLocation.z, -10.0f, 100.0f);
//            ImGui::SliderFloat("Radius", &radius, 0.0f, 1000.0f);
//            ImGui::End();
//        }
//        // This is for the rainbow shader
//        //shader.Bind();
//        //texture.Bind();
//        //if (r > 1.0f)
//        //    increment = -0.05f;
//        //else if (r < 0.0f)
//        //    increment = 0.05f;
//        //r += increment;
//        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
//        //shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
//        //glm::mat4 model(1.0f);
//        if (usePerspective)
//            proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.0f, far); //transforms from screen space to NDC
//        else
//            proj = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -100000.0f, 10000.0f);
//        
//        glm::mat4 view;
//        if (!usePerspective)
//            view = glm::translate(glm::mat4(1.0f), viewTranslation-CameraPosition);
//        else
//            view = glm::translate(glm::mat4(1.0f), -CameraPosition);
//
//        
//        //glm::mat4 model = glm::translate(glm::mat4(1.0f), viewTranslation);
//        glm::mat4 model;
//        model = glm::translate(glm::mat4(1.0f), modelLocation+ imageoffsetvec);
//        model = glm::rotate(model, glm::radians(modelZRotation), glm::vec3(0.0f, 0.0f, 1.0f));
//        model = glm::scale(model, modelScale);
//
//        //glm::mat4 toWorldPos= glm::scale(glm::mat4(1.0f), glm::vec3((float)PIX_PER_UNIT));
//        //glm::mat4 mvp = proj * view * worldScale * model;
//        glm::mat4 mvp = proj * view * model;
//
//        
//        //shader.SetUniformMat4f("u_MVP", mvp);
//
//        //LOG(mvp * glm::vec4(50.f, 50.f, 0.f, 0.f));
//
//        GLCall(renderer.Draw(va, ib, shader));
//        
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//#pragma endregion
//
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwTerminate();
//    return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}