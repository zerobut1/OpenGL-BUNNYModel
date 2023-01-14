#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "head/shader.h"
#include "head/camera.h"
#include "head/stb_image.h"
#include "head/model.h"

#include <iostream>

// ���鶥������
float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f};

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// ���ڴ�С����
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// ���������
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ʱ���
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ��Դλ��
glm::vec3 lightPos(-10.0f, 3.0f, 10.0f);

// ʰȡ�ĵ��λ��
glm::vec3 pickPos(2.696138f, 2.284638f, 9.227790f);

// ģ��ָ��
Model *m_Model = NULL;

// �߿�ģʽ
bool line_mode = false;

int main()
{
    // ��ʼ��
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    Shader ourShader("shader/model_loading/model_loading.vs", "shader/model_loading/model_loading.fs");
    Shader lightCubeShader("shader/light_cube/light_cube.vs", "shader/light_cube/light_cube.fs");
    Shader pickShader("shader/pick_point/pickpoint.vs", "shader/pick_point/pickpoint.fs");

    // ����ģ��
    Model ourModel("model/bunny_iH.ply");
    m_Model = &ourModel;

    //-----���Դ��VAO-----
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    unsigned int lightCubeVBO;
    glGenBuffers(1, &lightCubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //----------

    //-----ʰȡ���VAO-----
    unsigned int pickPointVAO;
    glGenVertexArrays(1, &pickPointVAO);
    glBindVertexArray(pickPointVAO);
    unsigned int pickPointVBO;
    glGenBuffers(1, &pickPointVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pickPointVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, pickPointVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //----------

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ʱ���
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // �������
        processInput(window);

        // �����ɫ����
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ͶӰ������۲����
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //-----��Ⱦģ��-----
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
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
        model = glm::translate(model, pickPos);
        model = glm::scale(model, glm::vec3(0.03f)); // Make it a smaller cube
        lightCubeShader.setMat4("model", model);
        // ����
        glBindVertexArray(pickPointVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //----------

        // ����¼�����������
        glfwSwapBuffers(window);
        glfwPollEvents();
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
            lastX = xpos;
            lastY = ypos;
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
    // ������Ļ�ռ�����
    double MouseX, MouseY;
    glfwGetCursorPos(window, &MouseX, &MouseY);
    switch (button)
    {
        // �������ʱ
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS)
        {
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);

            // ʰȡ����
            double mindis = 50.0;
            double maxZ = 10.0;
            bool flag = 0;
            int idx = -1;

            // ����ģ�����е����Ļ�ռ�����
            for (int i = 0; i < m_Model->meshes[0].vertices.size(); i++)
            {
                glm::vec4 pointPosition = projection * view * model * glm::vec4(m_Model->meshes[0].vertices[i].Position, 1.0);

                double screenX = (pointPosition.x + pointPosition.w) / (2 * pointPosition.w) * SCR_WIDTH;
                double screenY = (pointPosition.w - pointPosition.y) / (2 * pointPosition.w) * SCR_HEIGHT;
                // ���������ľ���
                double dis = glm::distance(glm::vec2(MouseX, MouseY), glm::vec2(screenX, screenY));
                // ѡ�����������ڷ�Χ�ڵ����е�����ӽ�������ĵ�
                if (dis < mindis && pointPosition.z < maxZ)
                {
                    maxZ = pointPosition.z;
                    flag = 1;
                    idx = i;
                }
            }
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