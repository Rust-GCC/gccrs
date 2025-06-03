/* Test erroneous cases of nested 'reduction' clauses.  */

/* See also 'gfortran.dg/goacc/nested-reductions-2-serial.f90'. */

/* { dg-additional-options -Wuninitialized } */

void acc_serial (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc serial
  /* implicit 'copy (sum, diff)'
     { dg-warning {'sum' is used uninitialized} TODO { xfail *-*-* } .-2 }
     { dg-warning {'diff' is used uninitialized} TODO { xfail *-*-* } .-3 } */
  {
    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." } 
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum) reduction(-:diff)
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum) // { dg-warning "nested loop in reduction needs reduction clause for .diff." }
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}

/* The same tests as above, but using a combined serial loop construct.  */

void acc_serial_loop (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc serial loop
  /* implicit 'copy (sum, diff)'
     { dg-warning {'sum' is used uninitialized} TODO { xfail *-*-* } .-2 }
     { dg-warning {'diff' is used uninitialized} TODO { xfail *-*-* } .-3 } */
  for (int h = 0; h < 10; ++h)
  {
    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." }
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum)
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(+:sum) reduction(-:diff)
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum) // { dg-warning "nested loop in reduction needs reduction clause for .diff." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}

/* The same tests as above, but now the outermost reduction clause is on
   the serial region, not the outermost loop.  */
void acc_serial_reduction (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc serial reduction(+:sum)
  /* implicit 'copy (sum, diff)'
     { dg-warning {'sum' is used uninitialized} TODO { xfail *-*-* } .-2 }
     { dg-warning {'diff' is used uninitialized} TODO { xfail *-*-* } .-3 } */
  {
    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." }
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(max:sum) // { dg-warning "conflicting reduction operations for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(max:sum) // { dg-warning "conflicting reduction operations for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      for (j = 0; j < 10; j++)
      #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." })
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
	for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(-:diff) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum) // { dg-warning "nested loop in reduction needs reduction clause for .diff." }
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}

/* The same tests as above, but using a combined serial loop construct, and
   the outermost reduction clause is on that one, not the outermost loop.  */
void acc_serial_loop_reduction (void)
{
  int i, j, k, l, sum, diff;

  #pragma acc serial loop reduction(+:sum)
  /* implicit 'copy (sum, diff)'
     { dg-warning {'sum' is used uninitialized} TODO { xfail *-*-* } .-2 }
     { dg-warning {'diff' is used uninitialized} TODO { xfail *-*-* } .-3 } */
  for (int h = 0; h < 10; ++h)
  {
    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop collapse(2) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
          #pragma acc loop reduction(+:sum)
          for (l = 0; l < 10; l++)
            sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." }
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop reduction(-:sum)
        for (k = 0; k < 10; k++)
          sum = 1;

    #pragma acc loop reduction(max:sum) // { dg-warning "conflicting reduction operations for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
        #pragma acc loop // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(max:sum) // { dg-warning "conflicting reduction operations for .sum." }
    for (i = 0; i < 10; i++)
      #pragma acc loop reduction(-:sum) // { dg-warning "conflicting reduction operations for .sum." }
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
      for (j = 0; j < 10; j++)
      #pragma acc loop reduction(+:sum) // { dg-warning "conflicting reduction operations for .sum." })
      // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (k = 0; k < 10; k++)
	  #pragma acc loop reduction(*:sum) // { dg-warning "conflicting reduction operations for .sum." }
	  for (l = 0; l < 10; l++)
	    sum = 1;

    #pragma acc loop reduction(-:diff) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
    for (i = 0; i < 10; i++)
      {
        #pragma acc loop reduction(-:diff) // { dg-warning "nested loop in reduction needs reduction clause for .sum." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(+:sum)
          for (k = 0; k < 10; k++)
            sum = 1;

        #pragma acc loop reduction(+:sum) // { dg-warning "nested loop in reduction needs reduction clause for .diff." }
        // { dg-warning "insufficient partitioning available to parallelize loop" "" { target *-*-* } .-1 } 
        for (j = 0; j < 10; j++)
          #pragma acc loop reduction(-:diff)
          for (k = 0; k < 10; k++)
            diff = 1;
      }
  }
}
