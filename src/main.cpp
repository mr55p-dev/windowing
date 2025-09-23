#include <cmath>
#include <glad/glad.h>

#include "debug.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int height, int width) {
  glViewport(0, 0, width, height);
}

unsigned int triangle_vao() {
  // Setup the vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // First a vertex buffer to store the vertices
  float vertices[] = {
      // clang-format off
	  //    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 
	  // 0.0f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f, 
	  // 0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,
      // clang-format on
      // clang-format off
	  -0.5f, 0.5f,  0.0f,   1.0f, 0.0f, 0.0f,
	  0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,
	  0.5f,  -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,
      // clang-format on
  };
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Declares the first set of vertex attributes (and sends to shader at loc 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Declares the second set of vertex attributes (and sends to shader at loc 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Then an element buffer to store the element indices
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Re-bind the default VAO
  glBindVertexArray(0);

  return VAO;
}

int main() {
  int height = 800, width = 800;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(width, height, "MY WINDOW", NULL, NULL);
  if (window == NULL) {
    DBG("GLFW Widnow was null");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    DBG("Failed to load GLAD");
    return -1;
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // Configure the shader program
  //

  Shader shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");

  // Get the triangle VAO
  unsigned int vao_default = 0;
  unsigned int vao_rect = triangle_vao();

  while (!glfwWindowShouldClose(window)) {
    glClearColor(.2f, 0.0f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set the shader green value
    float time = glfwGetTime();
	// float time = 0;
    shader.use();
	shader.setFloat("cosTheta", cos(time));
	shader.setFloat("sinTheta", sin(time));

    glBindVertexArray(vao_rect);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // re-bind the default vertex array
    glBindVertexArray(vao_default);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
