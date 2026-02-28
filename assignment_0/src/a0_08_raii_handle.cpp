#include <iostream>
#include <string>
#include <vector>
#include "rm_a0/a0_08_raii_handle.hpp"
namespace rm_a0{
  FileHandle::FileHandle(const char *path, const char *mode){
    fp_=std::fopen(path,mode);
  }
  FileHandle::~FileHandle(){
    if(fp_){
      std::fclose(fp_);
      fp_=nullptr;
    }
  }
  FileHandle::FileHandle(FileHandle &&other)noexcept
  :fp_(other.fp_){
    other.fp_=nullptr;
  }
  FileHandle&FileHandle::operator=(FileHandle &&other) noexcept{
    if(fp_){
      fclose(fp_);
    }
    fp_=other.fp_;
    other.fp_=nullptr;
    return *this;
  }
  bool FileHandle::valid() const{
    return (fp_!=nullptr);
  }
  FILE *FileHandle::get() const{
    return fp_;
  }

  
  bool CopyFile(const std::string &in_path, const std::string &out_path){
    FileHandle in_file(in_path.c_str(),"rb");
    if(!in_file.valid()){
      return false;
    }
    FileHandle out_file(out_path.c_str(),"wb");
    if(!out_file.valid()){
      return false;
    }
    std::vector<char> v(8192);
    size_t b_read;
    while((b_read=std::fread(v.data(),1,v.size(),in_file.get()))>0){
      if(std::fwrite(v.data(),1,v.size(),out_file.get())!=b_read){
        return false;
      }
    }
    return true;
  }
}
int main() {
  std::string in_path, out_path;
  if (!(std::cin >> in_path >> out_path))
    return 0;

  if (rm_a0::CopyFile(in_path, out_path)) {
    std::cout << "OK\n";
  } else {
    std::cout << "FAIL\n";
  }

  return 0;
}
