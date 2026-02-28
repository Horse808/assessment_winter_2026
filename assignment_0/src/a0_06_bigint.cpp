#include <iostream>
#include <sstream>
#include <string>
#include "rm_a0/a0_06_bigint.hpp"
namespace rm_a0{
  BigInt::BigInt():digits_(1,0){}
  BigInt::BigInt(const std::string &s){
    for(int i=s.length()-1;i>-1;i--){
      digits_.push_back(s[i]-'0');
    }
  }
  BigInt operator+(const BigInt &a, const BigInt &b){
    BigInt c;
    int p=0;
    size_t max=std::max(a.digits_.size(),b.digits_.size());
    for(int i=0;i<max||p>0;i++){
      int s=p;
      if(i<a.digits_.size()){
        s+=a.digits_[i];
      }
      if(i<b.digits_.size()){
        s+=b.digits_[i];
      }
      c.digits_[i]=s%10;
      p=s/10;
    }
    return c;
  }
  std::ostream &operator<<(std::ostream &os, const BigInt &x){
    for(size_t i=x.digits_.size()-1;i>-1;i--){
      os<<x.digits_[i];
    }
    return os;
  }
  std::string SolveBigIntAdd(const std::string &input, bool &ok){
    ok=true;
    std::istringstream iss(input);
    std::string l1,l2;
    if(!std::getline(iss,l1)||!std::getline(iss,l2)){
      ok=false;
      return "";
    }
    BigInt a(l1);
    BigInt b{l2};
    BigInt s=a+b;
    std::ostringstream oss;
    oss<<s<<"\n";
    return oss.str();
  }
}
int main() {
  std::ostringstream oss;
  oss << std::cin.rdbuf();

  bool ok = false;
  std::string out = rm_a0::SolveBigIntAdd(oss.str(), ok);
  if (!ok) {
    return 0;
  }

  std::cout << out;
  return 0;
}
