#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "debug.hpp"

class Shader {
public:
  unsigned int ID; // program id

  Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexSource;
    std::string fragmentSource;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      // open the files
      vertexFile.open(vertexPath);
      fragmentFile.open(fragmentPath);
      std::stringstream vertexStream, fragmentStream;

      // read the file buffer into a stream
      vertexStream << vertexFile.rdbuf();
      fragmentStream << fragmentFile.rdbuf();

      // close the files
      vertexFile.close();
      fragmentFile.close();

      // convert the stream into a string
      vertexSource = vertexStream.str();
      fragmentSource = fragmentStream.str();
    } catch (std::ifstream::failure e) {
      DBG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.code() << e.what());
    }

    // turn the std::string into c strings
    const char *vShaderCode = vertexSource.c_str();
    const char *fShaderCode = fragmentSource.c_str();

    // compile the shaders
    int success;
    char infoLog[512];

    unsigned int vShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderHandle, 1, &vShaderCode, NULL);
    glCompileShader(vShaderHandle);

    glGetShaderiv(vShaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vShaderHandle, 512, NULL, infoLog);
      DBG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog);
    }

    unsigned int fShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderHandle, 1, &fShaderCode, NULL);
    glCompileShader(fShaderHandle);

    glGetShaderiv(fShaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fShaderHandle, 512, NULL, infoLog);
      DBG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog);
    }

	// attch and link the program
    ID = glCreateProgram();
    glAttachShader(ID, vShaderHandle);
    glAttachShader(ID, fShaderHandle);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      DBG("FAILED: could not link program: " << infoLog);
    }

	glDeleteShader(vShaderHandle);
	glDeleteShader(fShaderHandle);
  };

  void use() 
  {
	glUseProgram(ID);  
  };

  void setBool(const std::string &name, bool value) const
  {
	  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const
  {
	  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const
  {
	  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
};



