enum Res {
    OK,
    BAD,
}

enum LOption {
    Some(i32),
    None,
}

fn test_can_destructure_Some(v: LOption) -> Res {
    if let LOption::Some(v) = v {
        return Res::OK;
    }
    return Res::BAD;
}


fn test_inner_value_is_100(v: LOption) -> Res {
    if let LOption::Some(v) = v {
        return match v {
            100 => Res::OK,
            _   => Res::BAD,
        }
    }
    return Res::BAD;
}

fn test_if_else(v: LOption) -> Res {
    if let LOption::Some(v) = v {
        return Res::BAD;
    } else {
        return Res::OK;
    }
}

fn main() -> i32 {

    match test_can_destructure_Some(LOption::None) {
        Res::OK => return 1,
        Res::BAD => (),
    }

    match test_can_destructure_Some(LOption::Some(1)) {
        Res::OK => (),
        Res::BAD => return 1,
    }

    match test_inner_value_is_100(LOption::Some(101)) {
        Res::OK => (),
        Res::BAD => return 1,
    }

    match test_if_else(LOption::None) {
        Res::OK => (),
        Res::BAD => return 1,
    }

    0
}
