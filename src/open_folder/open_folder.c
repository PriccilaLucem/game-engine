#include "open_folder.h"
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include "../logs/log.h"

void open_file_dialog(){
    OPENFILENAME ofn;       
    char szFile[260] = {0}; 

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box
    if (GetOpenFileName(&ofn) == TRUE) {
        log_message(LOG_LEVEL_INFO, ("Selected file: " + std::string(ofn.lpstrFile)).c_str(), __FILE__);
    }
    else{
        log_message(LOG_LEVEL_ERROR, "File selection canceled or failed.", __FILE__);
    }
}

void open_folder_dialog(){
    BROWSEINFO bi = {0};
    bi.lpszTitle = "Select a folder";
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != 0) {
        char path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path)) {
            log_message(LOG_LEVEL_INFO, ("Selected folder: " + std::string(path)).c_str(), __FILE__);
        }
        CoTaskMemFree(pidl);
    }
    else{
        log_message(LOG_LEVEL_ERROR, "Folder selection canceled or failed.", __FILE__);
    }
}

