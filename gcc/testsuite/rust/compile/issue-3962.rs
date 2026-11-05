static FOO2: () = {
    let x = [47; (1 << 47) - 1];
    // { dg-error "the type ..i32; 140737488355327.. is too big for the current architecture" "" { target x86_64-*-* } .-1 }
    // { dg-error "left shift count >= width of type" "" { target x86_64-*-* } .-2 }
};
