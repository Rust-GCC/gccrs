! { dg-do compile }
! { dg-options "-Os -fdump-tree-original-raw" }
! { dg-final { scan-tree-dump {(?s)identifier_node  strg: _gfortran_spread_char_scalar.*?function_type.*?prms: @[0-9]+.*?tree_list[^\n]*chan: @[0-9]+.*?tree_list[^\n]*chan: @[0-9]+.*?tree_list[^\n]*chan: @[0-9]+.*?tree_list[^\n]*chan: @[0-9]+.*?tree_list[^\n]*chan: @[0-9]+.*?tree_list[^\n]*chan: @8} "original" } }

character(3) :: a = 'abc'

associate (y => spread(trim(a), 1, 2) // 'd')
  if (size(y) /= 2) stop 1
end associate
end
