! { dg-do compile }
! { dg-options "-w" }

program p
  select type (x%a) ! { dg-error "Unexpected '%'.*nonderived-type variable 'x'" }
  end select        ! { dg-error "Expecting END PROGRAM statement" }
end
