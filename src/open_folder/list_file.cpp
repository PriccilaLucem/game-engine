#include "tinyxml2.h"
#include "open_folder.h"
#include "../logs/log.h"
#include <filesystem>
#include <iostream>

static void list_directory_icons(const std::string& folder_path, std::vector<Archives>& archives) {
    namespace fs = std::filesystem;

    try {
        for (const auto& entry : fs::directory_iterator(folder_path)) {
            Archives archive;
            archive.name = entry.path().filename().string();
            archive.path = entry.path().string();
            archive.isDir = entry.is_directory();
            archive.isFile = entry.is_regular_file();
            archives.push_back(archive);
        }
    } catch (const fs::filesystem_error& e) {
        log_message(LOG_LEVEL_ERROR, ("Error accessing directory: " + std::string(e.what())).c_str(), __FILE__);
    }
}

std::vector<Archives> open_folder_and_list(std::string folder_path) {
    std::vector<Archives> archives;
    list_directory_icons(folder_path, archives);
    return archives;
}
static void show_directory_contents(const std::string& folder_path){
    std::vector<Archives> archives = open_folder_and_list(folder_path);
    for (const auto& archive : archives) {
        std::cout << (archive.isDir ? "[DIR] " : "[FILE] ") << archive.name << " - " << archive.path << std::endl;
    }
}


