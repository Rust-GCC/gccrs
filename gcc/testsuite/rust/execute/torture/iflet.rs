enum Res {
    OK,
    BAD,
}

enum LOption {
    Some(i32),
    None,
}

fn test(v: LOption) -> Res {
    if let LOption::Some(v) = v {
        return Res::OK;
    }
    return Res::BAD;
}


fn test2(v: LOption) -> Res {
    if let LOption::Some(v) = v {
        return match v {
            100 => Res::OK,
            _   => Res::BAD,
        }
    }
    return Res::BAD;
}


fn main() -> i32 {

    match test(LOption::None) {
        Res::OK => return 1,
        Res::BAD => (),
    }

    match test(LOption::Some(1)) {
        Res::OK => (),
        Res::BAD => return 1,
    }

    match test2(LOption::Some(100)) {
        Res::OK => return 0,
        Res::BAD => return 1,
    }

}
