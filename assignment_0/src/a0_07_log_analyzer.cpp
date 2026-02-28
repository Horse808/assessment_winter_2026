#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include "rm_a0/a0_07_log_analyzer.hpp"
namespace rm_a0{
  LogStats AnalyzeLogFile(const std::string &path, bool &ok){
    LogStats s;
    ok=true;
    std::ifstream f(path);
    if(!f.is_open()){
      ok=false;
      return s;
    }
    std::string l,mlevel;
    long long mtime=-1;
    int n=0;
    while(std::getline(f,l)){
      std::istringstream iss(l);
      std::string level;
      long long time,ptime;
      if(!(iss>>level>>time)){
        ok=false;
        return s;
      }
      n+=1;
      if(level=="INFO"){
        s.info+=time;
        ptime=s.info;
      }
      else if(level=="WARN"){
        s.warn+=time;
        ptime=s.warn;
      }
      else{
        s.error+=time;
        ptime=s.error;
      }
      s.avg_ms+=time;
      if(ptime>mtime){
        mtime=ptime;
        mlevel=level;
      }
    }
    s.avg_ms=s.avg_ms/n;
    s.max_level=mlevel;
    s.max_ms=mtime;
    return s;
  }
  std::string SolveLogAnalyzer(const std::string &input, bool &ok){
    ok=true;
    std::string path=input;
    LogStats result=AnalyzeLogFile(path,ok);
    if(!ok){
      return "";
    }
    std::ostringstream oss;
    oss<<"INFO="<<result.info<<"\n";
    oss<<"WARN="<<result.warn<<"\n";
    oss<<"ERROR="<<result.error<<"\n";
    oss<<std::fixed<<std::setprecision(2)<<"avg="<<result.avg_ms<<"\n";
    oss<<"max="<<result.max_level<<" "<<result.max_ms<<"\n";
    return oss.str();
  }
}
int main() {
  std::string path;
  if (!std::getline(std::cin, path)) {
    return 0;
  }

  bool ok = false;
  std::string out = rm_a0::SolveLogAnalyzer(path + "\n", ok);
  std::cout << out;

  return 0;
}
