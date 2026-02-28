#include <cctype>
#include <iostream>
#include <string>

#include "rm_a0/a0_04_vowel_count.hpp"
namespace rm_a0{
  std::size_t CountVowels(const std::string &line){
    size_t n=0;
    for(int i=0;i<line.length();i++){
      if(std::tolower(line[i])=='a'||std::tolower(line[i])=='e'||std::tolower(line[i])=='i'||std::tolower(line[i])=='o'||std::tolower(line[i])=='u'){
        n++;
      }
    }
    return n;
  }
}
int main() {
  std::string line;
  if (!std::getline(std::cin, line)) {
    return 0;
  }

  std::cout << rm_a0::CountVowels(line) << "\n";

  return 0;
}
