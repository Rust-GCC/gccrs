! { dg-do run }
! PR90374 "5.5 d0.d, e0.d, es0.d, en0.d, g0.d and ew.d edit descriptors
program pr90374
  real(4) :: rn
  character(32) :: afmt, aresult
  real(8) :: one = 1.0D0, zero = 0.0D0, nan, pinf, minf

  nan = zero/zero
  rn = 0.00314_4
  afmt = "(D0.3)"
  write (aresult,fmt=afmt) rn
  if (aresult /= "0.314D-02") stop 12
  afmt = "(E0.10)"
  write (aresult,fmt=afmt) rn
  if (aresult /= "0.3139999928E-02") stop 15
  afmt = "(ES0.10)"
  write (aresult,fmt=afmt) rn
  if (aresult /= "3.1399999280E-03") stop 18
  afmt = "(EN0.10)"
  write (aresult,fmt=afmt) rn
  if (aresult /= "3.1399999280E-03") stop 21
  afmt = "(G0.10)"
  write (aresult,fmt=afmt) rn
  if (aresult /= "0.3139999928E-02") stop 24
  write (aresult,fmt="(D0.3)") rn
  if (aresult /= "0.314D-02") stop 26
  write (aresult,fmt="(E0.10)") rn
  if (aresult /= "0.3139999928E-02") stop 28
  write (aresult,fmt="(ES0.10)") rn
  if (aresult /= "3.1399999280E-03") stop 30
  write (aresult,fmt="(EN0.10)") rn
  if (aresult /= "3.1399999280E-03") stop 32
  write (aresult,fmt="(G0.10)") rn
  if (aresult /= "0.3139999928E-02") stop 34

end
