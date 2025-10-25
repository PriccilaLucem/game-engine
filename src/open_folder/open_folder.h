#ifndef OPEN_FOLDER_H
#define OPEN_FOLDER_H

#include <string>
#include <vector>
#include <stdbool.h>

typedef struct Archives{
    std::string name;
    std::string path;
    bool isDir;
    bool isFile;
} Archives;


std::vector<Archives> open_folder_and_list(std::string folder_path);
void open_folder_dialog();
void open_file_dialog();



#endif