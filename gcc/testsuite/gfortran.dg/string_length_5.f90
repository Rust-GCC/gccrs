! { dg-do run }
! PR fortran/85547 - string length for array constructors with type-spec
!
! Reported by Walter Spector

program p
  implicit none
  integer, parameter :: k = 16
  integer            :: m = k
  integer, volatile  :: n = k
  character(10)      :: path  = 'xyz/'
  character(8)       :: path2 = 'abc/'
  character(*), parameter :: s = 'ijk/'
  if (k /= len ( [ character(k) :: path ]        )) stop 1
  if (k /= len ( [ character(m) :: path ]        )) stop 2
  if (k /= len ( [ character(n) :: path ]        )) stop 3
  if (k /= len ( [ character(k) :: path ] ,kind=2)) stop 4
  if (k /= len ( [ character(m) :: path ] ,kind=2)) stop 5
  if (k /= len ( [ character(n) :: path ] ,kind=2)) stop 6

  if (k /= len ( [ character(k) ::      ]        )) stop 7
  if (k /= len ( [ character(m) ::      ]        )) stop 8
  if (k /= len ( [ character(n) ::      ]        )) stop 9
  if (k /= len ( [ character(k) ::      ] ,kind=2)) stop 10
  if (k /= len ( [ character(m) ::      ] ,kind=2)) stop 11
  if (k /= len ( [ character(n) ::      ] ,kind=2)) stop 12
  if (k /= len ( [ character(2*n/2) ::  ]        )) stop 13
  if (k /= len ( [ character(2*n/2) ::  ] ,kind=2)) stop 14
  if (k /= len ( [ character((m+n)/2) ::] ,kind=2)) stop 15
  if (k /= len ( [ character((m+n)/2) ::] ,kind=2)) stop 16
  if (k /= len ([[ character(k) ::      ]],kind=2)) stop 17
  if (k /= len ([[ character(m) ::      ]],kind=2)) stop 18
  if (k /= len ([[ character(n) ::      ]],kind=2)) stop 19
  if (k /= len ([[ character((m+n)/2) ::]],kind=2)) stop 20

  if (k /= len ( [ character(k)       :: path,path2 ] ,kind=2)) stop 21
  if (k /= len ( [ character(m)       :: path,path2 ] ,kind=2)) stop 22
  if (k /= len ( [ character(n)       :: path,path2 ] ,kind=2)) stop 23
  if (k /= len ( [ character((m+n)/2) :: path,path2 ] ,kind=2)) stop 24
  if (k /= len ([[ character(k)       :: path,path2 ]],kind=2)) stop 25
  if (k /= len ([[ character(m)       :: path,path2 ]],kind=2)) stop 26
  if (k /= len ([[ character(n)       :: path,path2 ]],kind=2)) stop 27
  if (k /= len ([[ character((m+n)/2) :: path,path2 ]],kind=2)) stop 28

  call sub ()
contains
  subroutine sub ()
!   call print_string (31, [ character(k)       :: ] )
!   call print_string (32, [ character(m)       :: ] )
!   call print_string (33, [ character(n)       :: ] )
!   call print_string (34, [ character((m+n)/2) :: ] )
!   call print_string (35, [ character(k)       :: path ] )
!   call print_string (36, [ character(m)       :: path ] )
!   call print_string (37, [ character(n)       :: path ] )
!   call print_string (38, [ character((m+n)/2) :: path ] )
!   call print_string (39, [ character(k)       :: path,path2 ] )
!   call print_string (40, [ character(m)       :: path,path2 ] )
!   call print_string (41, [ character(n)       :: path,path2 ] )
!   call print_string (42, [ character((m+n)/2) :: path,path2 ] )
!
!   call print_string (51,[[ character(k)       :: ]])
!   call print_string (52,[[ character(m)       :: ]])
!   call print_string (53,[[ character(n)       :: ]])
!   call print_string (54,[[ character((m+n)/2) :: ]])
!   call print_string (55,[[ character(k)       :: path ]])
!   call print_string (56,[[ character(m)       :: path ]])
!   call print_string (57,[[ character(n)       :: path ]])
!   call print_string (58,[[ character((m+n)/2) :: path ]])
!   call print_string (59,[[ character(k)       :: path,path2 ]])
!   call print_string (60,[[ character(m)       :: path,path2 ]])
!   call print_string (61,[[ character(n)       :: path,path2 ]])
!   call print_string (62,[[ character((m+n)/2) :: path,path2 ]])

!   call print_string (70, [ character(k)       ::     ] )
    call print_string (71, [ character(k)       :: s   ] )
    call print_string (72, [ character(k)       :: s,s ] )

  end subroutine sub

  subroutine print_string (i, s)
    integer,      intent(in) :: i
    character(*), intent(in) :: s(:)
    if (len(s) /= k) then
       print *, i, len(s), len(s)==k, size (s), s(:)
       stop i
    end if
  end subroutine
end program
