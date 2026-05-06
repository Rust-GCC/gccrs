int
get_count_bits_in_reg(int bit_start, int bit_count, int i_reg)
{
  int ret_count = 0;
  int num_reg = 0;
  int last_used_bit = (bit_start + bit_count - 1);
  for (int num_bit = bit_start; num_bit <= last_used_bit; ++num_bit) {
    ++ret_count;
    if ((((num_bit + 1) % 32) == 0) || (num_bit == last_used_bit)) {
      if (i_reg == num_reg++) {
	return ret_count;
      }
      ret_count = 0;
    }
  }
  return 0;
}
