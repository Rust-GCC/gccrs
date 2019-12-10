/* Test valid use of the OpenACC 'declare' directive.  */


int v0;
#pragma acc declare create(v0)

int v1;
#pragma acc declare copyin(v1)

int *v2;
#pragma acc declare deviceptr(v2)

int v3;
#pragma acc declare device_resident(v3)

int v4;
#pragma acc declare link(v4)

int v5, v6, v7, v8;
#pragma acc declare create(v5, v6) copyin(v7, v8)

int v9;
#pragma acc declare present_or_copyin(v9)

int v10;
#pragma acc declare present_or_create(v10)


void
f (void)
{
  int va0;
#pragma acc declare create(va0)

  int va1;
#pragma acc declare copyin(va1)

  int *va2;
#pragma acc declare deviceptr(va2)

  int va3;
#pragma acc declare device_resident(va3)

  extern int ve0;
#pragma acc declare create(ve0)

  extern int ve1;
#pragma acc declare copyin(ve1)

  extern int *ve2;
#pragma acc declare deviceptr(ve2)

  extern int ve3;
#pragma acc declare device_resident(ve3)

  extern int ve4;
#pragma acc declare link(ve4)

  extern int ve5;
#pragma acc declare present_or_copyin(ve5)
 
  extern int ve6;
#pragma acc declare present_or_create(ve6)

  int va5;
#pragma acc declare copy(va5)

  int va6;
#pragma acc declare copyout(va6)

  int va7;
#pragma acc declare present(va7)

  int va8;
#pragma acc declare present_or_copy(va8)

  int va9;
#pragma acc declare present_or_copyin(va9)

  int va10;
#pragma acc declare present_or_copyout(va10)

  int va11;
#pragma acc declare present_or_create(va11)

 a:
  {
    int va0;
#pragma acc declare create(va0)
    if (v1)
      goto a;
    else
      goto b;
  }
 b:;
}


/* The same as 'f'.  */

void
f_2 (void)
{
  int va0;
#pragma acc declare create(va0)

  int va1;
#pragma acc declare copyin(va1)

  int *va2;
#pragma acc declare deviceptr(va2)

  int va3;
#pragma acc declare device_resident(va3)

#ifndef __cplusplus
  /* TODO PR90868

     C: "error: variable '[...]' used more than once with '#pragma acc declare'".  */
#else
  extern int ve0;
#pragma acc declare create(ve0)

  extern int ve1;
#pragma acc declare copyin(ve1)

  extern int *ve2;
#pragma acc declare deviceptr(ve2)

  extern int ve3;
#pragma acc declare device_resident(ve3)

  extern int ve4;
#pragma acc declare link(ve4)

  extern int ve5;
#pragma acc declare present_or_copyin(ve5)
 
  extern int ve6;
#pragma acc declare present_or_create(ve6)
#endif

  int va5;
#pragma acc declare copy(va5)

  int va6;
#pragma acc declare copyout(va6)

  int va7;
#pragma acc declare present(va7)

  int va8;
#pragma acc declare present_or_copy(va8)

  int va9;
#pragma acc declare present_or_copyin(va9)

  int va10;
#pragma acc declare present_or_copyout(va10)

  int va11;
#pragma acc declare present_or_create(va11)

 a:
  {
    int va0;
#pragma acc declare create(va0)
    if (v1)
      goto a;
    else
      goto b;
  }
 b:;
}


/* The same as 'f' but everything contained in an OpenACC 'data' construct.  */

void
f_data (void)
{
#pragma acc data
  {
    int va0;
# pragma acc declare create(va0)

    int va1;
# pragma acc declare copyin(va1)

    int *va2;
# pragma acc declare deviceptr(va2)

    int va3;
# pragma acc declare device_resident(va3)

#if 0
    /* TODO PR90868

       C: "error: variable '[...]' used more than once with '#pragma acc declare'".
       C++: ICE during gimplification.  */

    extern int ve0;
# pragma acc declare create(ve0)

    extern int ve1;
# pragma acc declare copyin(ve1)

    extern int *ve2;
# pragma acc declare deviceptr(ve2)

    extern int ve3;
# pragma acc declare device_resident(ve3)

    extern int ve4;
# pragma acc declare link(ve4)

    extern int ve5;
# pragma acc declare present_or_copyin(ve5)
 
    extern int ve6;
# pragma acc declare present_or_create(ve6)
#endif

    int va5;
# pragma acc declare copy(va5)

    int va6;
# pragma acc declare copyout(va6)

    int va7;
# pragma acc declare present(va7)

    int va8;
# pragma acc declare present_or_copy(va8)

    int va9;
# pragma acc declare present_or_copyin(va9)

    int va10;
# pragma acc declare present_or_copyout(va10)

    int va11;
# pragma acc declare present_or_create(va11)

  a:
    {
      int va0;
# pragma acc declare create(va0)
      if (v1)
	goto a;
      else
	goto b;
    }
  b:;
  }
}
