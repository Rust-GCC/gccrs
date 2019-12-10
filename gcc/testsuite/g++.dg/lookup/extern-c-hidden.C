// Make sure unhidding an extern-c still checks it is compatible

extern "C" float fabsf (float);  // { dg-message "previous declaration" }

namespace Bob 
{
  extern "C" float fabsf (float, float); // { dg-error "20:conflicting C language" }
  extern "C" double fabs (double, double); // { dg-message "previous declaration" }
}

extern "C" double fabs (double); // { dg-error "19:conflicting C language" }
