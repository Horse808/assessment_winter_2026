#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "rm_a0/a0_05_score_stats.hpp"
namespace rm_a0{
  ScoreStatsResult ComputeScoreStats(const std::string &input, bool &ok){
    ScoreStatsResult result;
    ok=true;
    int n;
    std::istringstream p(input);
    if(!(p>>n)){
      ok=false;
      return result;
    }
    if(n<=0){
      ok=false;
      return result;
    }
    double ts=0;
    int max=-1,s;
    std::string name,tn;
    for(int i=0;i<n;i++){
      if(!(p>>name>>s)){
        ok=false;
        return result;
      }
      if(s>max){
        max=s;
        tn=name;
      }
      ts+=s;
    }
    result.avg=ts/n;
    result.top_name=tn;
    result.top_score=max;
    return result;
  }
  std::string SolveScoreStats(const std::string &input, bool &ok){
    ok=true;
    ScoreStatsResult Result=ComputeScoreStats(input,ok);
    if(!ok){
      return "";
    }
    std::ostringstream oss;
    oss<<"top="<<Result.top_name<<" "<<Result.top_score<<"\n";
    oss<<std::fixed<<std::setprecision(2)<<"avg="<<Result.avg<<"\n";
    return oss.str();
  }
}
int main() {
  std::ostringstream oss;
  oss << std::cin.rdbuf();

  bool ok = false;
  std::string out = rm_a0::SolveScoreStats(oss.str(), ok);
  if (!ok) {
    return 0;
  }

  std::cout << out;

  return 0;
}
