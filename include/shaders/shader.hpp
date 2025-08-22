#pragma once

#include <iostream>
#include <string>
#include "glad.h"
#include "glfw3.h"

class Shader {
 public:
  unsigned int programID;
  std::string vertexFile;
  std::string fragmentFile;

  long fragmentModTimeOnLoad;

  Shader();
  void Unload();
  void ReloadFromFile();
  static Shader LoadShader(std::string fileVertexShader,
                           std::string fileFragmentShader);
  void set_bool(const std::string& name, bool value) const;

 private:
  static bool CompileShader(unsigned int shaderId, char (&infoLog)[512]);
  static bool LinkProgram(unsigned int programID, char (&infoLog)[512]);
};