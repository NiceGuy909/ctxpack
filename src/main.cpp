#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

bool shouldSkipDirectory(const fs::path& it){
    if(
        it.filename()==".git" || 
        it.filename()=="node_modules" ||
        it.filename()=="dist" ||
        it.filename()=="build" 
    ){
        return true;
    }else{
        return false;
    }
}

std::vector<fs::path> scanRepo(fs::path path){
    std::vector<fs::path> filesFound;
    auto it = fs::recursive_directory_iterator(path);
    while(it != fs::recursive_directory_iterator{} ){
        if (
            it->is_directory() && shouldSkipDirectory(it->path())
        ) {
            it.disable_recursion_pending();
        }
        if(it->is_regular_file()){
            filesFound.push_back(it->path());
        }
        ++it;
    }
    return filesFound;
}

int main(int argc,char** argv) {
    if(argc<2){
        std::cout<<"Error: Specify Path to Repo"<<std::endl;
        return 1;
    }
    fs::path path = argv[1] ;
    

    try{
        auto files = scanRepo(path);
        for(const auto& it: files){
            std::cout<<it<<std::endl;
        }
        std::cout<<"Files Found: "<<files.size()<<std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cout<<e.what()<<std::endl;
        return 1;

    }


    return 0;
}