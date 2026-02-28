#include <iostream>

#include "rm_a0/a0_03_range_sum.hpp"
namespace rm_a0{
  long long RangeSum(long long l, long long r){
    long long s=0;
    for(int i=l;i++;i<=r){
      s+=i;
    }
    return s;
  }
}
int main() {
  long long l = 0;
  long long r = 0;
  if (!(std::cin >> l >> r)) {
    return 0;
  }

  std::cout << rm_a0::RangeSum(l, r) << "\n";

  return 0;
}
