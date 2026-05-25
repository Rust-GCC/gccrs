/* PR c/125377 */

static int omp_is_initial_device(void);
// { dg-warning "'omp_is_initial_device' used but never defined" "" { target c } .-1 }
#pragma omp begin declare variant match(device={kind(host)})
    static inline int omp_is_initial_device(void) { return 1; }  // { dg-note "previous variant declaration here" }
#pragma omp end declare variant

#pragma omp begin declare variant match(device={kind(host)})
    static inline int omp_is_initial_device(void) { return 1; } // { dg-error "multiple definitions of variants with the same context selector violate the one-definition rule" }
#pragma omp end declare variant

int main()
{
  __builtin_printf("%d\n", omp_is_initial_device());
}
