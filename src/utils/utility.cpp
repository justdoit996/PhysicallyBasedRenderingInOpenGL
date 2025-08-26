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

    glGenTextures(1, &textureID);
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

unsigned int loadHdrTexture(const std::string& path) {
  unsigned int hdrTexture;

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrComponents;
  float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT,
        data);  // note how we specify the texture's data value to be float

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Failed to load HDR image." << std::endl;
  }
  return hdrTexture;
}