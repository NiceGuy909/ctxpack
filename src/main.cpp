#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <optional>
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

bool isTextFile(const fs::path& path){
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open())
        return false;
    char c;
    while (f.get(c)) {
        if(!c) return false;
    }
    return true;
}

std::optional<int> fileLineCount(const fs::path& path){
    if(!isTextFile(path)) return std::nullopt;
    int lineCnt = 0;
    std::ifstream f(path);
    if(!f.is_open()) return std::nullopt;
    std::string line;
    while(std::getline(f,line)){
        lineCnt++;
    }
    return lineCnt;
}

std::optional<std::string> readFile(const fs::path& path){
    if(!isTextFile(path)) return std::nullopt;
    std::ifstream f(path);
    if(!f.is_open()) return std::nullopt;
    std::string content(
    (std::istreambuf_iterator<char>(f)),
    std::istreambuf_iterator<char>()
    );
    return content;
    
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
            
            std::cout<<"Path: "<<it<<std::endl;
            std::cout<<"Extention: "<<it.extension()<<std::endl;
            std::cout<<"Size: "<<fs::file_size(it)<<" Bytes"<<std::endl;
            auto count = fileLineCount(it);

            if (count) {
                std::cout << "Line Count: " << *count << std::endl;
            }
            
            std::cout<<"text/binary: "<<isTextFile(it)<<std::endl;

            auto content = readFile(it);

            if(content){
                std::cout<<"==== "<<it<<" ===="<<std::endl;
                std::cout<<content.value()<<std::endl;
            }
        }
        std::cout<<"Files Found: "<<files.size()<<std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cout<<e.what()<<std::endl;
        return 1;

    }


    return 0;
}