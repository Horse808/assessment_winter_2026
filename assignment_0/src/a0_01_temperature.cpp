#include <iostream>
#include <iomanip>

#include "rm_a0/a0_01_temperature.hpp"
namespace rm_a0{
  double CelsiusToFahrenheit(double celsius){
    return celsius*1.8+32;
  }
  std::string FormatFahrenheit(double fahrenheit){
    std::stringstream s;
    s<<std::fixed<<std::setprecision(2)<<fahrenheit<<'\n';
    return s.str();
  }
  std::string SolveTemperature(const std::string &input, bool &ok){
    ok=true;
    try
    {
      double c=std::stod(input);
      double f=CelsiusToFahrenheit(c);
      return FormatFahrenheit(f);
    }
    catch(...)
    {
      ok=false;
      return "";
    }
    
  }
}
int main() {
  double celsius = 0.0;
  if (!(std::cin >> celsius)) {
    return 0;
  }
  std::cout << rm_a0::FormatFahrenheit(rm_a0::CelsiusToFahrenheit(celsius));
  return 0;
}
