#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#define DBG(val) std::cerr << val << std::endl

const char *v_shader_src = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "void main() {\n"
                           "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                           "}\0";

const char *f_shader_src = "#version 330 core\n"
                           "out vec4 FragColor;\n"
                           "void main() {\n"
                           "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                           "}\0";

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int height, int width) {
  glViewport(0, 0, width, height);
}

uint init_program(GLFWwindow *window) {
  unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v_shader, 1, &v_shader_src, NULL);
  glCompileShader(v_shader);

  int success;
  glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoBuf[512];
    glGetShaderInfoLog(v_shader, 512, NULL, infoBuf);
    DBG("FAILED: could not compile v_shader: " << infoBuf);
  }

  unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_shader, 1, &f_shader_src, NULL);
  glCompileShader(f_shader);

  glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoBuf[512];
    glGetShaderInfoLog(f_shader, 512, NULL, infoBuf);
    DBG("FAILED: could not compile f_shader: " << infoBuf);
  }

  uint program = glCreateProgram();
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

  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
  };
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  int program = init_program(window);
  glUseProgram(program);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(.2f, 0.0f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
