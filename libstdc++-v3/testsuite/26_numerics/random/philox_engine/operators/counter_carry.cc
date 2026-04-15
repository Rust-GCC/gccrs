// { dg-do run { target c++26 } }
// N5014 29.5.4.5 Class Template philox_engine

#include <random>
#include <array>

#include <testsuite_hooks.h>

// Helper: set counter, skip one block (n outputs), then compare
// the next block against a reference engine at the expected counter.
template<typename Engine, std::size_t N>
void
verify_carry(const std::array<typename Engine::result_type, N>& current_counter,
	     const std::array<typename Engine::result_type, N>& expected_incremented_counter)
{
  Engine eng_a;
  eng_a.set_counter(current_counter);
  // Skip the block produced from current_counter
  eng_a.discard(N);

  Engine eng_b;
  eng_b.set_counter(expected_incremented_counter);
  // Both engines should now produce identical output
  for (std::size_t i = 0; i < N; ++i)
    VERIFY( eng_a() == eng_b() );
}

int main()
{
  // philox4x32 (n=4, w=32)
  verify_carry<std::philox4x32, 4>(std::array<std::philox4x32::result_type, 4>{0, 0, 0, 5},
				   std::array<std::philox4x32::result_type, 4>{0, 0, 0, 6});
  verify_carry<std::philox4x32, 4>(std::array<std::philox4x32::result_type, 4>{0, 0, 0, 0xFFFFFFFF},
				   std::array<std::philox4x32::result_type, 4>{0, 0, 1, 0});
  verify_carry<std::philox4x32, 4>(std::array<std::philox4x32::result_type, 4>{0, 0, 1, 0xFFFFFFFF},
				   std::array<std::philox4x32::result_type, 4>{0, 0, 2, 0});
  verify_carry<std::philox4x32, 4>(std::array<std::philox4x32::result_type, 4>{0, 0, 0xFFFFFFFF, 0xFFFFFFFF},
				   std::array<std::philox4x32::result_type, 4>{0, 1, 0, 0});
  verify_carry<std::philox4x32, 4>(std::array<std::philox4x32::result_type, 4>{0, 1, 0xFFFFFFFF, 0xFFFFFFFF},
				   std::array<std::philox4x32::result_type, 4>{0, 2, 0, 0});
  verify_carry<std::philox4x32, 4>(std::array<std::philox4x32::result_type, 4>{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF},
				   std::array<std::philox4x32::result_type, 4>{0, 0, 0, 0});

  // philox4x64 (n=4, w=64)
  constexpr auto philox4x64_max = std::philox4x64::max();
  verify_carry<std::philox4x64, 4>(std::array<std::philox4x64::result_type, 4>{0, 0, 0, philox4x64_max},
				   std::array<std::philox4x64::result_type, 4>{0, 0, 1, 0});
  verify_carry<std::philox4x64, 4>(std::array<std::philox4x64::result_type, 4>{0, 0, 1, philox4x64_max},
				   std::array<std::philox4x64::result_type, 4>{0, 0, 2, 0});
  verify_carry<std::philox4x64, 4>(std::array<std::philox4x64::result_type, 4>{0, 0, philox4x64_max, philox4x64_max},
				   std::array<std::philox4x64::result_type, 4>{0, 1, 0, 0});
  verify_carry<std::philox4x64, 4>(std::array<std::philox4x64::result_type, 4>{0, 1, philox4x64_max, philox4x64_max},
				   std::array<std::philox4x64::result_type, 4>{0, 2, 0, 0});
  verify_carry<std::philox4x64, 4>(std::array<std::philox4x64::result_type, 4>{philox4x64_max, philox4x64_max, philox4x64_max, philox4x64_max},
				   std::array<std::philox4x64::result_type, 4>{0, 0, 0, 0});

  // philox2x32 (n=2, w=32)
  using philox2x32 = std::philox_engine<std::uint_fast32_t,
        32, 2, 10, 0xCD9E8D57, 0x9E3779B9>;
  verify_carry<philox2x32, 2>(std::array<philox2x32::result_type, 2>{0, 5},
			      std::array<philox2x32::result_type, 2>{0, 6});
  verify_carry<philox2x32, 2>(std::array<philox2x32::result_type, 2>{0, 0xFFFFFFFF},
			      std::array<philox2x32::result_type, 2>{1, 0});
  verify_carry<philox2x32, 2>(std::array<philox2x32::result_type, 2>{1, 0xFFFFFFFF},
			      std::array<philox2x32::result_type, 2>{2, 0});
  verify_carry<philox2x32, 2>(std::array<philox2x32::result_type, 2>{0xFFFFFFFF, 0xFFFFFFFF},
			      std::array<philox2x32::result_type, 2>{0, 0});

  // philox2x64 (n=2, w=64)
  using philox2x64 = std::philox_engine<std::uint_fast64_t,
        64, 2, 10, 0xCD9E8D57, 0x9E3779B9>;
  constexpr auto philox2x64_max = philox2x64::max();
  verify_carry<philox2x64, 2>(std::array<philox2x64::result_type, 2>{0, 5},
			      std::array<philox2x64::result_type, 2>{0, 6});
  verify_carry<philox2x64, 2>(std::array<philox2x64::result_type, 2>{0, philox2x64_max},
			      std::array<philox2x64::result_type, 2>{1, 0});
  verify_carry<philox2x64, 2>(std::array<philox2x64::result_type, 2>{1, philox2x64_max},
			      std::array<philox2x64::result_type, 2>{2, 0});
  verify_carry<philox2x64, 2>(std::array<philox2x64::result_type, 2>{philox2x64_max, philox2x64_max},
			      std::array<philox2x64::result_type, 2>{0, 0});
}
