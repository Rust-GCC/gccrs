fn main() {
    1.create_a_type_error[ // { dg-error ".E0610." "" { target *-*-* } }
        ()+() // { dg-error ".E0369." "" { target *-*-* } }
              //   ^ ensure that we typeck the inner expression ^
    ];
}

