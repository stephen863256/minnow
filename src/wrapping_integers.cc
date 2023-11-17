#include "wrapping_integers.hh"
#include <cstdint>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return zero_point + n;
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.
  static constexpr uint64_t TWO31 = 1UL << 31;
  static constexpr uint64_t TWO32 = 1UL << 32;

  auto const ckpt = Wrap32::wrap(checkpoint, zero_point);
  uint64_t bias = raw_value_ - ckpt.raw_value_;
  if(bias <= TWO31 || checkpoint + bias < TWO32)
  {
    return checkpoint + bias; 
  }
  return checkpoint + bias - TWO32;
}
