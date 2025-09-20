#include "ui_to_scene_data.h"

#include "utils/constants.h"

namespace pbr_utils {

std::vector<std::string> getFileNamesInDirectory(
    const std::string& directoryPath) {
  std::vector<std::string> fileNames;
  try {
    // Create a directory_iterator for the given path
    for (const auto& entry :
         std::filesystem::directory_iterator(directoryPath)) {
      // Check if the entry is a regular file (not a directory)
      // Add the file name to the vector
      fileNames.push_back(entry.path().filename().string());
    }
  } catch (const std::filesystem::filesystem_error& e) {
    std::cerr << "Filesystem error: " << e.what() << std::endl;
  }
  return fileNames;
}

std::vector<std::string> material_names =
    getFileNamesInDirectory("resources/assets/textures/pbr/");

std::vector<std::string> environment_names =
    getFileNamesInDirectory(constants::ui_defaults::environment_path_prefix);

};  // namespace pbr_utils