static FOO2: () = {
    let x = [47; (1 << 47) - 1];
    // { dg-error "the type ..i32; 140737488355327.. is too big for the current architecture" "" { target *-*-* } .-1 }
};
