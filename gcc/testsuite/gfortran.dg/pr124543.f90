! { dg-do run )

! PR124543, test case derived from that in the PR with several
! corrections. Some of the logic which was inverted. Some INQUIRE
! statements were missing the EXISTS. Segfaults now fixed.

! Some of the INQUIRE statements were missing the EXISTS.


program inq_tests
  implicit none

! Test various INQUIRE return values
! Most of these tests are based on Table C.1 in Section C.6.5 of F95.
! F2003, F2018 and F2023 can optionally be enabled.

  character(16) :: access, action, blank, delim, direct, form, formatted
  character(16) :: name, pad, position, read, readwrite, sequential
  character(16) :: unformatted, write

  logical :: exist, named, opened
  integer :: iostat, nextrec, number, recl

  character(20) :: decimal, encoding, round, signc, stream
  integer :: pos
  character(16) :: async
  integer :: id
  logical :: pending
  character(16) :: leading_zero

  integer, parameter :: lun = 42
  character(*), parameter :: lfn = 'fort.42'

  integer :: fail_count = 0, pass_count = 0

! Make sure there is no file with the name we will be using
  open (lun, file=lfn, status='old', iostat=iostat)
  if (iostat == 0) then
    close (lun, status='delete')
  end if

! File not connected yet
  call init_vars ()
  inquire (file=lfn, access=access, action=action, blank=blank, delim=delim, exist=exist,  &
      direct=direct, form=form, formatted=formatted, pad=pad, position=position,  &
      read=read, readwrite=readwrite, sequential=sequential, unformatted=unformatted,  &
      write=write, named=named, name=name, number=number, opened=opened, iostat=iostat)
  !print *, 'Unconnected INQUIRE by file'
  if  (iostat /= 0) stop 49
  if  (access /= 'UNDEFINED') stop 50
  if  (action /= 'UNDEFINED') stop 51
  if  (blank  /= 'UNDEFINED') stop 52
  if  (delim  /= 'UNDEFINED') stop 53
  if  (direct /= 'UNKNOWN') stop 54
  if  (exist) stop 55
  if  (form   /= 'UNDEFINED') stop 56
  if  (formatted /= 'UNKNOWN') stop 57
  if  (name   /= lfn) stop 58
  if  (.not. named) stop 59
  if  (number /= -1) stop 60
  if  (opened) stop 61
  if  (pad    /= 'UNDEFINED') stop 62
  if  (position /= 'UNDEFINED') stop 63
  if  (read   /= 'UNKNOWN') stop 64 ! The file does not exist.
  if  (readwrite /= 'UNKNOWN') stop 65
  if  (sequential /= 'UNKNOWN') stop 66
  if  (unformatted /= 'UNKNOWN') stop 67
  if  (write   /= 'UNKNOWN') stop 68

  inquire (file=lfn, decimal=decimal, encoding=encoding, pos=pos,  &
      round=round, sign=signc, stream=stream)
  if  (decimal /= 'UNDEFINED') stop 72
  if  (encoding /= 'UNKNOWN') stop 73
  if  (round /= 'UNDEFINED') stop 74
  if  (signc /= 'UNDEFINED') stop 75
  if  (stream /= 'UNKNOWN') stop 76
  inquire (file=lfn, asynchronous=async)
  if  (async /= 'UNDEFINED') stop 78
  inquire (file=lfn, id=id, pending=pending, recl=recl)  ! SEGV at run-time w/gfortran
  if  (pending) stop 80
  if  (recl /= -1) stop 81
!  inquire (file=lfn, leading_zero=leading_zero) stop
!  print *, 'leading_zero =', leading_zero, pf (leading_zero /= 'UNDEFINED')

! Unit not connected yet
  call init_vars ()
  inquire (unit=lun, access=access, action=action, blank=blank, delim=delim,  &
      direct=direct, form=form, formatted=formatted, pad=pad, position=position,  &
      read=read, readwrite=readwrite, sequential=sequential, unformatted=unformatted,  &
      write=write, named=named, name=name, number=number, opened=opened, iostat=iostat)
  !print *
  !print *, 'Unconnected INQUIRE by unit'
  if  (iostat /= 0) stop 93
  if  (access /= 'UNDEFINED') stop 94
  if  (action /= 'UNDEFINED') stop 95
  if  (blank  /= 'UNDEFINED') stop 96
  if  (delim  /= 'UNDEFINED') stop 97
  if  (direct /= 'UNKNOWN') stop 98
  if  (exist) stop 99
  if  (form   /= 'UNDEFINED') stop 100
  if  (formatted /= 'UNKNOWN') stop 101
  if  (name   /= 'xxx') stop 102
  if  (named) stop 103
  if  (number /= -1) stop 104
  if  (opened) stop 105
  if  (pad    /= 'UNDEFINED') stop 106
  if  (position /= 'UNDEFINED') stop 107
  if  (read   /= 'UNKNOWN') stop 108
  if  (readwrite /= 'UNKNOWN') stop 109
  if  (sequential /= 'UNKNOWN') stop 110
  if  (unformatted /= 'UNKNOWN') stop 111
  if  (write   /= 'UNKNOWN') stop 112
  iostat = 99
  inquire (unit=lun, decimal=decimal, encoding=encoding, pos=pos,  &
      round=round, sign=signc, stream=stream, asynchronous=async, iostat=iostat)
  if  (decimal /= 'UNDEFINED' .or. (iostat /= 0)) stop 116
  if  (encoding /= 'UNKNOWN') stop 117
  if  (round /= 'UNDEFINED') stop 118
  if  (signc /= 'UNDEFINED') stop 119
  if  (stream /= 'UNKNOWN') stop 120
  if  (async /= 'UNDEFINED') stop 122
  iostat = 99
  inquire (unit=lun, id=id, pending=pending, recl=recl, iostat=iostat)  ! SEGV at run-time w/gfortran
  if  (pending .or. (iostat /= 0)) stop 124
  if  (recl   /= -1) stop 125
!  inquire (unit=lun, leading_zero=leading_zero)
!  print *, 'leading_zero =', leading_zero, pf (leading_zero /= 'UNDEFINED')

! Open the file to connect it to the unit - sequential access

  open (lun, file=lfn, status='new', access='sequential', form='formatted', recl=80)
  write (lun,*)'hello world!'
  rewind (lun)

  call init_vars ()
  inquire (file=lfn, access=access, action=action, blank=blank, delim=delim, recl=recl,  &
      direct=direct, form=form, formatted=formatted, pad=pad, position=position, exist=exist, &
      read=read, readwrite=readwrite, sequential=sequential, unformatted=unformatted,  &
      write=write, named=named, name=name, number=number, opened=opened, iostat=iostat)
  !print *
  !print *, 'Connected INQUIRE by file (sequential)'
  if  (iostat /= 0) stop 142
  if  (access /= 'SEQUENTIAL') stop 143
  if  (action /= 'READWRITE') stop 144
  if  (blank  /= 'NULL') stop 145
  if  (delim  /= 'NONE') stop 146
  if  (direct /= 'NO') stop 147
  if  (.not. exist) stop 148
  if  (form   /= 'FORMATTED') stop 149
  if  (formatted /= 'YES') stop 150
  if  (name   /= lfn) stop 151
  if  (.not. named) stop 152
  if  (number /= 42) stop 153
  if  (.not. opened) stop 154
  if  (pad    /= 'YES') stop 155
  if  (position /= 'REWIND') stop 156
  if  (read   /= 'YES') stop 157
  if  (readwrite /= 'YES') stop 158
  if  (recl   /= 80) stop 159
  if  (sequential /= 'YES') stop 160
  if  (unformatted /= 'NO') stop 161
  if  (write   /= 'YES') stop 162
  iostat = 99
  inquire (file=lfn, decimal=decimal, encoding=encoding, pos=pos,  &
      round=round, sign=signc, stream=stream, iostat=iostat)
  if  (decimal /= 'POINT' .or. (iostat /= 0)) stop 166
  if  (encoding /= 'UNKNOWN') stop 167
  if  (round /= 'PROCESSOR_DEFINED') stop 168
  if  (signc /= 'PROCESSOR_DEFINED') stop 169
  if  (stream /= 'NO') stop 170

! Unit is connected
  call init_vars ()
  inquire (unit=lun, access=access, action=action, blank=blank, recl=recl, delim=delim,  &
      direct=direct, form=form, formatted=formatted, pad=pad, position=position,  &
      read=read, readwrite=readwrite, sequential=sequential, unformatted=unformatted,  &
      write=write, named=named, name=name, number=number, opened=opened, iostat=iostat)
  !print *
  !print *, 'Connected INQUIRE by unit (sequential)'
  if  (iostat /= 0) stop 180
  if  (access /= 'SEQUENTIAL') stop 181
  if  (action /= 'READWRITE') stop 182
  if  (blank  /= 'NULL') stop 183
  if  (delim  /= 'NONE') stop 184
  if  (direct /= 'NO') stop 185
  if  (.not. exist) stop 186
  if  (form   /= 'FORMATTED') stop 187
  if  (formatted /= 'YES') stop 188
  if  (name   /= lfn) stop 189
  if  (.not. named) stop 190
  if  (number /= 42) stop 191
  if  (.not. opened) stop 192
  if  (pad    /= 'YES') stop 193
  if  (position /= 'REWIND') stop 194
  if  (read   /= 'YES') stop 195
  if  (readwrite /= 'YES') stop 196
  if  (recl   /= 80) stop 197
  if  (sequential /= 'YES') stop 198
  if  (unformatted /= 'NO') stop 199
  if  (write   /= 'YES') stop 200
  iostat = 99
  inquire (unit=lun, decimal=decimal, encoding=encoding, pos=pos,  &
      round=round, sign=signc, stream=stream, iostat=iostat)
  if  (decimal /= 'POINT' .or. (iostat /= 0)) stop 204
  if  (encoding /= 'UNKNOWN') stop 205
  if  (round /= 'PROCESSOR_DEFINED') stop 206
  if  (signc /= 'PROCESSOR_DEFINED') stop 207
  if  (stream /= 'NO') stop 208

  close (lun, status='delete')

! Open the file to connect it to the unit - sequential stream access

  open (lun, file=lfn, status='new', access='stream', form='unformatted')
  write (lun) 'hello world!'
  rewind (lun)

  call init_vars ()
  inquire (file=lfn, access=access, action=action, blank=blank, delim=delim,  &
      direct=direct, form=form, formatted=formatted, pad=pad, position=position,  &
      read=read, readwrite=readwrite, sequential=sequential, unformatted=unformatted,  &
      write=write, opened=opened, name=name, number=number, named=named, iostat=iostat)
  !print *
  !print *, 'Connected INQUIRE by file (stream, unformatted)'
  if  (iostat /= 0) stop 225
  if  (access /= 'STREAM') stop 226
  if  (action /= 'READWRITE') stop 227
  if  (blank  /= 'UNDEFINED') stop 228
  if  (delim  /= 'UNDEFINED') stop 229
  if  (direct /= 'NO') stop 230
  if  (.not. exist) stop 231
  if  (form   /= 'UNFORMATTED') stop 232
  if  (formatted /= 'NO') stop 233
  if  (name   /= lfn) stop 234
  if  (.not. named) stop 235
  if  (number /= 42) stop 236
  if  (.not. opened) stop 237
  if  (pad    /= 'UNDEFINED') stop 238
  if  (position /= 'REWIND') stop 239
  if  (read   /= 'YES') stop 240
  if  (readwrite /= 'YES') stop 241
  if  (sequential /= 'NO') stop 242
  if  (unformatted /= 'YES') stop 243
  if  (write   /= 'YES') stop 244
  iostat = 99
  inquire (file=lfn, decimal=decimal, encoding=encoding, pos=pos,  &
      round=round, sign=signc, stream=stream, recl=recl, iostat=iostat)
  if  (decimal /= 'UNDEFINED' .or. (iostat /= 0)) stop 248
  if  (encoding /= 'UNKNOWN') stop 249
  if  (round /= 'PROCESSOR_DEFINED') stop 250
  if  (signc /= 'PROCESSOR_DEFINED') stop 251
  if  (stream /= 'YES') stop 252
  if  (recl   /= -2) stop 253

! Unit is connected
  call init_vars ()
  inquire (unit=lun, access=access, action=action, blank=blank, delim=delim, opened=opened,  &
      direct=direct, form=form, formatted=formatted, pad=pad, position=position,  &
      read=read, readwrite=readwrite, sequential=sequential, unformatted=unformatted,  &
      write=write, name=name, named=named, number=number, recl=recl, iostat=iostat)
  !print *
  !print *, 'Connected INQUIRE by unit (stream, unformatted)'
  if  (iostat /= 0) stop 263
  if  (access /= 'STREAM') stop 264
  if  (action /= 'READWRITE') stop 265
  if  (blank  /= 'UNDEFINED') stop 266
  if  (delim  /= 'UNDEFINED') stop 267
  if  (direct /= 'NO') stop 268
  if  (.not. exist) stop 269
  if  (form   /= 'UNFORMATTED') stop 270
  if  (formatted /= 'NO') stop 271
  if  (name   /= lfn) stop 272
  if  (.not. named) stop 273
  if  (number /= 42) stop 274
  if  (.not. opened) stop 275
  if  (pad    /= 'UNDEFINED') stop 276
  if  (position /= 'REWIND') stop 277
  if  (read   /= 'YES') stop 278
  if  (readwrite /= 'YES') stop 279
  if  (sequential /= 'NO') stop  280
  if  (unformatted /= 'YES') stop 281
  if  (write   /= 'YES') stop 282
  iostat = 99
  inquire (unit=lun, decimal=decimal, encoding=encoding, pos=pos,  &
      round=round, sign=signc, stream=stream, iostat=iostat)
  if  (decimal /= 'UNDEFINED' .or. (iostat /= 0)) stop 286
  if  (encoding /= 'UNKNOWN') stop 287
  if  (round /= 'PROCESSOR_DEFINED') stop 288
  if  (signc /= 'PROCESSOR_DEFINED') stop 289
  if  (stream /= 'YES') stop 290
  if  (recl   /= -2) stop 291
  close (lun, status='delete')

! Open the file to connect it to the unit - direct access

  open (lun, file=lfn, status='new', access='direct', recl=12, form='formatted')
  write (lun,rec=1, fmt='(a)') 'hello world!'

  call init_vars ()
  inquire (file=lfn, access=access, action=action, blank=blank, delim=delim,  &
      direct=direct, form=form, formatted=formatted, nextrec=nextrec, pad=pad, opened=opened, &
      position=position, read=read, readwrite=readwrite, recl=recl, name=name, named=named,  &
      sequential=sequential, unformatted=unformatted, write=write, number=number, iostat=iostat)
  !print *
  !print *, 'Connected INQUIRE by file (direct, formatted)'
  if  (iostat /= 0) stop 306
  if  (access /= 'DIRECT') stop 307
  if  (action /= 'READWRITE') stop 308
  if  (blank  /= 'NULL') stop 309
  if  (delim  /= 'NONE') stop 310
  if  (direct /= 'YES') stop 311
  if  (.not. exist) stop 312
  if  (form   /= 'FORMATTED') stop 313
  if  (formatted /= 'YES') stop 314
  if  (name   /= lfn) stop 315
  if  (.not. named) stop 316
  if  (nextrec /= 2) stop 317
  if  (number /= 42) stop 318
  if  (.not. opened) stop 319
  if  (pad    /= 'YES') stop 320
  if  (position /= 'UNDEFINED') stop 31
  if  (read   /= 'YES') stop 322
  if  (readwrite /= 'YES') stop 323
  if  (recl   /= 12) stop 324
  if  (sequential /= 'NO') stop 325
  if  (unformatted /= 'NO') stop 326
  if  (write   /= 'YES') stop 327

! Unit is connected
  call init_vars ()
  inquire (unit=lun, access=access, action=action, blank=blank, delim=delim,  &
      direct=direct, form=form, formatted=formatted, nextrec=nextrec, pad=pad, opened=opened,  &
      position=position, read=read, readwrite=readwrite, recl=recl, name=name, named=named, &
      sequential=sequential, unformatted=unformatted, write=write, number=number, iostat=iostat)
  !print *
  !print *, 'Connected INQUIRE by unit (direct, formatted)'
  if  (iostat /= 0) stop 337
  if  (access /= 'DIRECT') stop 338
  if  (action /= 'READWRITE') stop 339
  if  (blank  /= 'NULL') stop 340
  if  (delim  /= 'NONE') stop 341
  if  (direct /= 'YES') stop 342
  if  (.not. exist) stop 343
  if  (form   /= 'FORMATTED') stop 344
  if  (formatted /= 'YES') stop 345
  if  (name   /= lfn) stop 346
  if  (.not. named) stop 347
  if  (nextrec /= 2) stop 348
  if  (number /= 42) stop 349
  if  (.not. opened) stop 350
  if  (pad    /= 'YES') stop 351
  if  (position /= 'UNDEFINED') stop 352
  if  (read   /= 'YES') stop 353
  if  (readwrite /= 'YES') stop 354
  if  (recl   /= 12) stop 355
  if  (sequential /= 'NO') stop 356
  if  (unformatted /= 'NO') stop 357
  if  (write   /= 'YES') stop 358

  close (lun, status="delete")
  print *, "all pass"
contains

  subroutine init_vars ()

    iostat = 99
    access = 'xxx'
    action = 'xxx'
    blank = 'xxx'
    delim = 'xxx'
    direct = 'xxx'
    form = 'xxx'
    formatted = 'xxx'
    name = 'xxx'
    nextrec = -41
    number = -42
    pad = 'xxx'
    position = 'xxx'
    read = 'xxx'
    readwrite = 'xxx'
    recl = -43
    sequential = 'xxx'
    unformatted = 'xxx'
    write = 'xxx'
    decimal = 'xxx'
    encoding = 'xxx'
    round = 'xxx'
    pos = -44
    signc = 'xxx'
    stream = 'xxx'
    async = 'xxx'
    id = -45
    leading_zero = 'xxx'

  end subroutine

end program
