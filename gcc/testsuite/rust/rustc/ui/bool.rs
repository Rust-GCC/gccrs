// run-pass
// Basic boolean tests


use std::cmp::Ordering::{Equal, Greater, Less};
use std::ops::{BitAnd, BitOr, BitXor};

fn main() {
    assert_eq!(false.eq(&true), false);
    assert_eq!(false == false, true);
    assert_eq!(false != true, true);
    assert_eq!(false.ne(&false), false);

    assert_eq!(false.bitand(false), false);
    assert_eq!(true.bitand(false), false);
    assert_eq!(false.bitand(true), false);
    assert_eq!(true.bitand(true), true);

    assert_eq!(false & false, false);
    assert_eq!(true & false, false);
    assert_eq!(false & true, false);
    assert_eq!(true & true, true);

    assert_eq!(false.bitor(false), false);
    assert_eq!(true.bitor(false), true);
    assert_eq!(false.bitor(true), true);
    assert_eq!(true.bitor(true), true);

    assert_eq!(false | false, false);
    assert_eq!(true | false, true);
    assert_eq!(false | true, true);
    assert_eq!(true | true, true);

    assert_eq!(false.bitxor(false), false);
    assert_eq!(true.bitxor(false), true);
    assert_eq!(false.bitxor(true), true);
    assert_eq!(true.bitxor(true), false);

    assert_eq!(false ^ false, false);
    assert_eq!(true ^ false, true);
    assert_eq!(false ^ true, true);
    assert_eq!(true ^ true, false);

    assert_eq!(!true, false);
    assert_eq!(!false, true);

    let s = false.to_string();
    assert_eq!(s, "false");
    let s = true.to_string();
    assert_eq!(s, "true");

    assert!(true > false);
    assert!(!(false > true));

    assert!(false < true);
    assert!(!(true < false));

    assert!(false <= false);
    assert!(false >= false);
    assert!(true <= true);
    assert!(true >= true);

    assert!(false <= true);
    assert!(!(false >= true));
    assert!(true >= false);
    assert!(!(true <= false));

    assert_eq!(true.cmp(&true), Equal);
    assert_eq!(false.cmp(&false), Equal);
    assert_eq!(true.cmp(&false), Greater);
    assert_eq!(false.cmp(&true), Less);
}

