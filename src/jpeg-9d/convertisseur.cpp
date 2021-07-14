#define cimg_use_libjpeg
#include "CImg.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

using namespace cimg_library;

int main() {
    char outputName[50];
    std::string dir_path = "Database";
    std::string image = "/mask", imageBank="imageResult/";
    std::string currentDatabase,database_image_path,database_mask_path,file_path;
    std::string outputFile;
    for (const auto & entry : fs::directory_iterator(dir_path)){
        
        database_image_path=entry.path().string()+image; 
        currentDatabase=entry.path().string().substr(dir_path.size()+1);
        for (const auto & entry : fs::directory_iterator(database_image_path)){
            file_path=entry.path().string();
            outputFile=imageBank+currentDatabase+file_path.substr(dir_path.size()+1+currentDatabase.size()+image.size());
            //if(outputFile[outputFile.size()-3]=='j')
              //  continue;
            outputFile[outputFile.size()-1]='m';
            outputFile[outputFile.size()-2]='g';
            outputFile[outputFile.size()-3]='p';
            std::cout << outputFile <<std::endl; 
            CImg<unsigned char> im(file_path.c_str());
            im.save(outputFile.c_str());
            
        }
    }
 

} 