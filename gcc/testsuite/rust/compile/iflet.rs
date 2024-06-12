pub fn f() -> i32 {
    let mut res = 0;

    enum E {
        X(i32),
    }
    let v = E::X(4);
    
    if let E::X(n) = v {
        res = n;
    }

    res
}
