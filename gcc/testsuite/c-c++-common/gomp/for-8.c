/* { dg-do compile } */
/* { dg-additional-options "-fdump-tree-ompexp" } */

/* Check that the static loop and distribute expanders fetch the thread/team
   id and count through a single GOMP_loop_static_worksharing /
   GOMP_distribute_static_worksharing call rather than separate
   omp_get_thread_num/omp_get_num_threads or omp_get_team_num/omp_get_num_teams
   calls.  */

void bar (int);

/* Static schedule without a chunk size goes through
   expand_omp_for_static_nochunk.  */

void
f1 (int n)
{
  int i;
  #pragma omp for schedule(static)
  for (i = 0; i < n; ++i)
    bar (i);
}

/* Static schedule with a chunk size goes through
   expand_omp_for_static_chunk.  */

void
f2 (int n)
{
  int i;
  #pragma omp for schedule(static, 4)
  for (i = 0; i < n; ++i)
    bar (i);
}

/* Distribute without a chunk size goes through
   expand_omp_for_static_nochunk.  */

void
f3 (int n)
{
  int i;
#pragma omp teams
#pragma omp distribute
  for (i = 0; i < n; ++i)
    bar (i);
}

/* Distribute with a chunk size goes through
   expand_omp_for_static_chunk.  */

void
f4 (int n)
{
  int i;
#pragma omp teams
#pragma omp distribute dist_schedule(static, 4)
  for (i = 0; i < n; ++i)
    bar (i);
}

/* { dg-final { scan-tree-dump "GOMP_loop_static_worksharing" "ompexp" } } */
/* { dg-final { scan-tree-dump "GOMP_distribute_static_worksharing" "ompexp" } } */
/* { dg-final { scan-tree-dump-not "omp_get_num_threads" "ompexp" } } */
/* { dg-final { scan-tree-dump-not "omp_get_thread_num" "ompexp" } } */
/* { dg-final { scan-tree-dump-not "omp_get_num_teams" "ompexp" } } */
/* { dg-final { scan-tree-dump-not "omp_get_team_num" "ompexp" } } */
