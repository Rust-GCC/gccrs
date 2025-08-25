fn main() {
    'test_1: while break 'test_1 {}
    while break {}
// { dg-error ".E0590." "" { target *-*-* } .-1 }

    'test_2: while let true = break 'test_2 {}
    while let true = break {}
// { dg-error ".E0590." "" { target *-*-* } .-1 }

    loop { 'test_3: while break 'test_3 {} }
    loop { while break {} }
// { dg-error ".E0590." "" { target *-*-* } .-1 }

    loop {
        'test_4: while break 'test_4 {}
        break;
    }
    loop {
        while break {}
// { dg-error ".E0590." "" { target *-*-* } .-1 }
        break;
    }

    'test_5: while continue 'test_5 {}
    while continue {}
// { dg-error ".E0590." "" { target *-*-* } .-1 }

    'test_6: while let true = continue 'test_6 {}
    while let true = continue {}
// { dg-error ".E0590." "" { target *-*-* } .-1 }

    loop { 'test_7: while continue 'test_7 {} }
    loop { while continue {} }
// { dg-error ".E0590." "" { target *-*-* } .-1 }

    loop {
        'test_8: while continue 'test_8 {}
        continue;
    }
    loop {
        while continue {}
// { dg-error ".E0590." "" { target *-*-* } .-1 }
        continue;
    }
}

