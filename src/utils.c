#include "utils.h"
unsigned long set_last_n_bits(unsigned long value, unsigned long nBits) {
  return value | ((1U << nBits) - 1U);
}
