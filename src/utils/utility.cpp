#include "utils/utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad.h"

bool ReadFile(std::string file,
              std::string& fileContents,
              bool addLineTerminator) {
  std::ifstream f(file);
  std::string currentLine;
  std::string allFileContent;

  if (f.is_open()) {
    while (getline(f, currentLine)) {
      if (addLineTerminator) {
        currentLine += "\n";
      }
      allFileContent += currentLine;
    }
    f.close();
    fileContents = allFileContent;
    return true;
  } else {
    return false;
  }
}

// Found how to do this from
// https://www.oreilly.com/library/view/c-cookbook/0596007612/ch10s07.html
long GetFileModTime(std::string file) {
  struct stat fileInfo;
  stat(file.c_str(), &fileInfo);
  return fileInfo.st_mtime;
}

unsigned int loadTexture(const std::string& path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }
  return textureID;
}