#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "learnopengl/camera.h"
#include "learnopengl/model.h"
#include "learnopengl/shader.h"

#include <iostream>

// ���鶥������
// clang-format off
float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f, 
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f, 
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f, 
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, 
    -0.5f, -0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f, 
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,  
    -0.5f, -0.5f,  0.5f, 
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f, 
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f, 
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f};
// clang-format on

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// ���ڴ�С����
const unsigned int SCR_WIDTH  = 1920;
const unsigned int SCR_HEIGHT = 1080;

// ���������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX     = SCR_WIDTH / 2.0f;
float lastY     = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ʱ���
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ��Դλ��
glm::vec3 lightPos(5.0f, 5.0f, -5.0f);

// ʰȡ�ĵ��λ��
glm::vec3 pickPos(0.0f, 0.0f, -4.0f);

// ģ��ָ��
Model *m_Model = NULL;

// �߿�ģʽ
bool line_mode = false;

int main()
{
    // ��ʼ��
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ��������
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // �󶨺���
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, callback);

    // ��ʼ��glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ��תy��
    stbi_set_flip_vertically_on_load(true);

    // ��z����
    glEnable(GL_DEPTH_TEST);

    // ����shader����
    Shader ourShader("../src/shader/model_loading/model_loading.vs", "../src/shader/model_loading/model_loading.fs");
    Shader lightCubeShader("../src/shader/light_cube/light_cube.vs", "../src/shader/light_cube/light_cube.fs");
    Shader pickShader("../src/shader/pick_point/pickpoint.vs", "../src/shader/pick_point/pickpoint.fs");

    // ����ģ��
    Model ourModel("../resources/objects/bunny/bunny_iH.ply");
    m_Model = &ourModel;

    //-----�������VBO-----
    unsigned int CubeVBO;
    glGenBuffers(1, &CubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    //----------

    //-----���Դ��VAO-----
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //----------

    //-----ʰȡ���VAO-----
    unsigned int pickPointVAO;
    glGenVertexArrays(1, &pickPointVAO);
    glBindVertexArray(pickPointVAO);
    //----------

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ʱ���
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        // �������
        processInput(window);

        // �����ɫ����
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ͶӰ������۲����
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view       = camera.GetViewMatrix();

        //-----��Ⱦģ��-----
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);
        // ������ͭ�Ĳ���
        ourShader.setVec3("material.ambient", 0.229500f, 0.088250f, 0.027500f);
        ourShader.setVec3("material.diffuse", 0.550800f, 0.211800f, 0.066000f);
        ourShader.setVec3("material.specular", 0.580594f, 0.223257f, 0.069570f);
        ourShader.setFloat("material.shininess", 51.200001f);
        // ���ö����
        ourShader.setVec3("dirLight.direction", 0.2f, -1.0f, 0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // ���õ��Դ
        ourShader.setVec3("pointLights.position", lightPos);
        ourShader.setVec3("pointLights.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights.diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights.constant", 1.0f);
        ourShader.setFloat("pointLights.linear", 0.09f);
        ourShader.setFloat("pointLights.quadratic", 0.032f);
        // ���þ۹�
        ourShader.setVec3("spotLight.position", camera.Position);
        ourShader.setVec3("spotLight.direction", camera.Front);
        ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.09f);
        ourShader.setFloat("spotLight.quadratic", 0.032f);
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        // ����
        // �Ƿ�Ϊ�߿�ģʽ
        if (line_mode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ourModel.Draw(ourShader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //----------

        //-----���Դ-----
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);
        // ����
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //----------

        //-----ʰȡ��-----
        pickShader.use();
        pickShader.setMat4("projection", projection);
        pickShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, pickPos);
        model = glm::scale(model, glm::vec3(0.03f)); // Make it a smaller cube
        pickShader.setMat4("model", model);
        // ����
        glBindVertexArray(pickPointVAO);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, 1);
        //----------

        // ����¼�����������
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    // ��ESC�˳�
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // WSAD������������
    // �ո����������shift�½�
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // tab�л��Ƿ�Ϊ�߿�ģʽ����
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        line_mode = !line_mode;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // �Ҽ����ʱ�ƶ��������ӽ�
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX      = xpos;
            lastY      = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// ʵ��ʰȡ�����ĺ���
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    int scr_width, scr_height;
    glfwGetWindowSize(window, &scr_width, &scr_height);

    // �õ��������Ļ������
    // mouse_xֵΪ0��SCR_WIDTH��һ����
    double mouse_x,
        mouse_y;
    float mouse_z;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    // ת��Ϊ������Ļ�ռ�����
    // mouse_xxֵΪ-1��1��һ��������
    float mouse_xx = (mouse_x * 2) / scr_width - 1;
    float mouse_yy = 1 - (mouse_y * 2) / scr_height;

    switch (button)
    {
        // �������ʱ
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS)
        {
            // �õ����������ֵ
            // ���ֵmouse_zΪ0��1��һ��������
            glReadBuffer(GL_FRONT);
            glReadPixels(int(mouse_x), scr_height - int(mouse_y) - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_z);

            // �������
            glm::mat4 projection =
                glm::perspective(glm::radians(camera.Zoom), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
            glm::mat4 view        = camera.GetViewMatrix();
            glm::mat4 model       = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 _projection = glm::inverse(projection);
            glm::mat4 _view       = glm::inverse(view);
            glm::mat4 _model      = glm::inverse(model);

            // �������ľֲ��ռ�����
            // ��mouse_z��ֵӳ�䵽-1��1�ķ�Χ
            glm::vec4 pointPosition =
                _model * _view * _projection * glm::vec4(mouse_xx, mouse_yy, 2 * mouse_z - 1.0f, 1.0f);
            glm::vec3 clickPoint(pointPosition.x / pointPosition.w,
                                 pointPosition.y / pointPosition.w,
                                 pointPosition.z / pointPosition.w);

            // ʰȡ����
            float mindis = 50.0;
            bool flag    = 0;
            int idx      = -1;

            // ����ģ�����е㵽������ľ���
            // ѡ������ĵ�
            for (int i = 0; i < m_Model->meshes[0].vertices.size(); i++)
            {
                float dis = glm::distance(clickPoint, m_Model->meshes[0].vertices[i].Position);
                // ѡ�����������ڷ�Χ�ڵ����е�����ӽ�������ĵ�
                if (dis < mindis)
                {
                    mindis = dis;
                    flag   = 1;
                    idx    = i;
                }
            }
            // ���ʰȡ����
            if (flag)
            {
                // ���ʰȡ�ĵ������
                pickPos = m_Model->meshes[0].vertices[idx].Position;
                cout << "����:( " << pickPos.x << " , " << pickPos.y << " , " << pickPos.z << " )" << endl;
            }
        }
        break;
    }
}