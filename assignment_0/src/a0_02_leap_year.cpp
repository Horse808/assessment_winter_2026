#include <iostream>

#include "rm_a0/a0_02_leap_year.hpp"
namespace rm_a0{
  bool IsLeapYear(int year){
    if(year%400==0||(year%4==0&&year%100!=0)){
      return true;
    }
    else{
      return false;
    }
  }
  std::string FormatLeapYearAnswer(bool is_leap_year){
    if(is_leap_year){
      return "YES\n";
    }
    else{
      return "NO\n";
    }
  }
  std::string SolveLeapYear(const std::string &input, bool &ok){
    ok=true;
    try{
      int year=std::stoi(input);
      return FormatLeapYearAnswer(IsLeapYear(year));
    }
    catch(...){
      ok=false;
      return "";
    }
  }
}
int main() {
  int year = 0;
  if (!(std::cin >> year)) {
    return 0;
  }

  std::cout << rm_a0::FormatLeapYearAnswer(rm_a0::IsLeapYear(year));

  return 0;
}
