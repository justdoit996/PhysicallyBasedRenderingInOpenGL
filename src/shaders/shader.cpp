#include "shaders/shader.h"

#include "utils/utility.h"

Shader::Shader() {}

void Shader::Use() {
  glUseProgram(program_id_);
}

void Shader::Unload() {
  // Delete the current shader and remove from memory
  glDeleteProgram(this->program_id_);
}

void Shader::ReloadFromFile() {
  // Get the current modified time for the fragment shader file
  long currentModTime = GetFileModTime(this->fragment_file_);

  // If the current modified time is LATER than the previously set modified time
  if (currentModTime > fragment_mod_time_on_load_) {
    // Unload current shader
    this->Unload();

    // Load new shader using the same files, however, the fragment file
    // will contain new code this time
    Shader s = Shader::LoadShader(this->vertex_file_, this->fragment_file_);

    // Discard newly loaded shader, but persist the shader program id it created
    // during loading
    this->program_id_ = s.program_id_;
    // Set the latest fragment file modified time to the current time
    this->fragment_mod_time_on_load_ = currentModTime;
  }
}

bool Shader::CompileShader(unsigned int shaderId, char (&infoLog)[512]) {
  // This assumes that you have already appended the source code to the shader
  // Attempts to compile the shader
  glCompileShader(shaderId);

  // Get the compile status from the shader
  int success;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    // If it didn't succeed, then fill infoLog with error msg.
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
  }

  // If the compilation succeeded, return true
  return success > 0;
}

bool Shader::LinkProgram(unsigned int program_id_, char (&infoLog)[512]) {
  // Assumes that all shaders are attached prior to linking
  glLinkProgram(program_id_);

  // Get the link status from the program
  int success;
  glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
  if (!success) {
    // If it didn't succeed in linking, get the error msg and put in infoLog
    glGetProgramInfoLog(program_id_, 512, NULL, infoLog);
  }

  // Return true if success
  return success > 0;
}

Shader Shader::LoadShader(std::string fileVertexShader,
                          std::string fileFragmentShader) {
  // Bool for checking if at any point during loading it failed
  bool anyError = false;

  // Reads the code from the shader files
  std::string vertexCode;
  if (!ReadFile(fileVertexShader, vertexCode, true)) {
    std::cout << "ERROR::SHADER::VERTEX(" << fileVertexShader
              << ")::FILE_NOT_FOUND" << std::endl;
    anyError = true;
  }
  std::string fragmentCode;
  if (!ReadFile(fileFragmentShader, fragmentCode, true)) {
    std::cout << "ERROR::SHADER::FRAGMENT(" << fileVertexShader
              << ")::FILE_NOT_FOUND" << std::endl;
    anyError = true;
  }

  if (anyError) {
    return Shader{};
  }

  // Turns them into c-strings
  const char* vertexCodeCstr = vertexCode.c_str();
  const char* fragmentCodeCstr = fragmentCode.c_str();

  // Create vertex and fragment shader
  unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  // Attach the shader code to its shader
  glShaderSource(vertexShaderId, 1, &vertexCodeCstr, NULL);
  glShaderSource(fragmentShaderId, 1, &fragmentCodeCstr, NULL);

  // Attempt to compile the vertex shader
  char infoLog[512];
  if (!Shader::CompileShader(vertexShaderId, infoLog)) {
    std::cout << "ERROR::SHADER::VERTEX(" << fileVertexShader
              << ")::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    anyError = true;
  }
  // Attempt to compile the fragment shader
  if (!Shader::CompileShader(fragmentShaderId, infoLog)) {
    std::cout << "ERROR::SHADER::FRAGMENT(" << fileFragmentShader
              << ")::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    anyError = true;
  }

  // Create a shader program
  unsigned int program_id_ = glCreateProgram();

  // Attach both the vertex and fragment shader to the program
  glAttachShader(program_id_, vertexShaderId);
  glAttachShader(program_id_, fragmentShaderId);

  // Attempt to link the vertex and fragment shaders
  if (!Shader::LinkProgram(program_id_, infoLog)) {
    std::cout << "ERROR::SHADER::LINKING(" << fileVertexShader << " + "
              << fileFragmentShader << ")::LINKING_FAILED\n"
              << infoLog << std::endl;
    anyError = true;
  }

  // After linking, we no longer need the individual shaders
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);

  // Create a shader instance and fill with newly created values
  Shader s;
  s.fragment_mod_time_on_load_ = GetFileModTime(fileFragmentShader);
  s.program_id_ = program_id_;
  s.vertex_file_ = fileVertexShader;
  s.fragment_file_ = fileFragmentShader;

  // If we at any point did NOT get an error, then we say that it loaded
  // successfully
  if (!anyError) {
    std::cout << "INFO::SHADER[" << s.program_id_ << "](" << fileVertexShader
              << " + " << fileFragmentShader << ")::SUCCESSFULLY_LOADED"
              << std::endl;
  }

  return s;
}
