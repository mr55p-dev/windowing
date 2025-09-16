#include <cmath>
#include <filesystem>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>

#define DBG(val) std::cerr << val << std::endl

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int height, int width) {
  glViewport(0, 0, width, height);
}

std::string read_file(std::string name) {
  std::ifstream vert(name);
  std::string str;
  std::string file_contents;
  while (std::getline(vert, str)) {
    file_contents += str;
    file_contents.push_back('\n');
  }
  vert.close();
  return file_contents;
}

unsigned int init_program(GLFWwindow *window) {
  std::string v_shader_src = read_file("../src/shader.vert");
  const char *v_shader_data = v_shader_src.c_str();
  unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v_shader, 1, &v_shader_data, NULL);
  glCompileShader(v_shader);

  int success;
  glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoBuf[512];
    glGetShaderInfoLog(v_shader, 512, NULL, infoBuf);
    DBG("FAILED: could not compile v_shader: " << infoBuf);
  }

  std::string f_shader_src = read_file("../src/shader.frag");
  const char *f_shader_data = f_shader_src.c_str();
  unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_shader, 1, &f_shader_data, NULL);
  glCompileShader(f_shader);

  glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoBuf[512];
    glGetShaderInfoLog(f_shader, 512, NULL, infoBuf);
    DBG("FAILED: could not compile f_shader: " << infoBuf);
  }

  unsigned int program = glCreateProgram();
  glAttachShader(program, v_shader);
  glAttachShader(program, f_shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoBuf[512];
    glGetProgramInfoLog(program, 512, NULL, infoBuf);
    DBG("FAILED: could not link program: " << infoBuf);
  }

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
  return program;
}

unsigned int triangle_vao() {
  // Setup the vertex array object
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // First a vertex buffer to store the vertices
  float vertices[] = {
      -0.5f, 0.5f,  0.0f, 0.5f,  0.5f,  0.0f,
      0.5f,  -0.5f, 0.0f, -0.5f, -0.5f, 0.0f,
  };
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Then an element buffer to store the element indices
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Declares the locations for use with shader (i think)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Re-bind the default VAO
  glBindVertexArray(0);

  return VAO;
}

int main() {
  int height = 600, width = 800;

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
  int program = init_program(window);
  glUseProgram(program);

  // Get the triangle VAO
  unsigned int vao_default = 0;
  unsigned int vao_rect = triangle_vao();

  int vertexLoc = glGetUniformLocation(program, "VertexColor");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(.2f, 0.0f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float time = glfwGetTime();
    float green = (sin(time) / 2.0f) + 0.5f;
	glUniform4f(vertexLoc, 0.0f, green, 1.0f, 1.0f);
    glBindVertexArray(vao_rect);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(vao_default);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
