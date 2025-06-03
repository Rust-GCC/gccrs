extern int i;

/* The OpenACC specification allows nested compute constructs, but we don't
   support that yet.  */

void
f_acc_parallel (void)
{
#pragma acc parallel
  {
#pragma acc parallel /* { dg-bogus ".parallel. construct inside of .parallel. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc kernels /* { dg-bogus ".kernels. construct inside of .parallel. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc serial /* { dg-bogus ".serial. construct inside of .parallel. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc data /* { dg-error ".data. construct inside of .parallel. region" } */
    ;
#pragma acc update host(i) /* { dg-error ".update. construct inside of .parallel. region" } */
#pragma acc enter data copyin(i) /* { dg-error ".enter data. construct inside of .parallel. region" } */
#pragma acc exit data delete(i) /* { dg-error ".exit data. construct inside of .parallel. region" } */
  }
}

void
f_acc_kernels (void)
{
#pragma acc kernels
  {
#pragma acc parallel /* { dg-bogus ".parallel. construct inside of .kernels. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc kernels /* { dg-bogus ".kernels. construct inside of .kernels. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc serial /* { dg-bogus ".serial. construct inside of .kernels. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc data /* { dg-error ".data. construct inside of .kernels. region" } */
    ;
#pragma acc update host(i) /* { dg-error ".update. construct inside of .kernels. region" } */
#pragma acc enter data copyin(i) /* { dg-error ".enter data. construct inside of .kernels. region" } */
#pragma acc exit data delete(i) /* { dg-error ".exit data. construct inside of .kernels. region" } */
  }
}

void
f_acc_serial (void)
{
#pragma acc serial
  {
#pragma acc parallel /* { dg-bogus ".parallel. construct inside of .serial. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc kernels /* { dg-bogus ".kernels. construct inside of .serial. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc serial /* { dg-bogus ".serial. construct inside of .serial. region" "not implemented" { xfail *-*-* } } */
    ;
#pragma acc data /* { dg-error ".data. construct inside of .serial. region" } */
    ;
#pragma acc update host(i) /* { dg-error ".update. construct inside of .serial. region" } */
#pragma acc enter data copyin(i) /* { dg-error ".enter data. construct inside of .serial. region" } */
#pragma acc exit data delete(i) /* { dg-error ".exit data. construct inside of .serial. region" } */
  }
}

void
f_acc_data (void)
{
  unsigned int i;
#pragma acc data
  {
#pragma acc loop /* { dg-error "loop directive must be associated with an OpenACC compute region" } */
    for (i = 0; i < 2; ++i)
      ;

#pragma acc data
    {
#pragma acc loop /* { dg-error "loop directive must be associated with an OpenACC compute region" } */
      for (i = 0; i < 2; ++i)
	;
    }
  }
}

#pragma acc routine
void
f_acc_routine (void)
{
#pragma acc parallel /* { dg-error "OpenACC region inside of OpenACC routine, nested parallelism not supported yet" } */
  ;
}

void
f (void)
{
  int i, v = 0;

#pragma acc loop gang reduction (+:v) /* { dg-error "loop directive must be associated with an OpenACC compute region" } */
  for (i = 0; i < 10; i++)
    v++;
}
