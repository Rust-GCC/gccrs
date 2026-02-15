#![deny(unreachable_patterns)]

fn main() {
    let x: Vec<(isize, isize)> = Vec::new();
    let x: &[(isize, isize)] = &x;
    match *x {
        [a, (2, 3), _] => (),
        [(1, 2), (2, 3), b] => (), // { dg-error "" "" { target *-*-* } }
        _ => ()
    }

    let x: Vec<String> = vec!["foo".to_string(),
                              "bar".to_string(),
                              "baz".to_string()];
    let x: &[String] = &x;
    match *x {
        [ref a, _, _, ..] => { println!("{}", a); }
        [_, _, _, _, _] => { } // { dg-error "" "" { target *-*-* } }
        _ => { }
    }

    let x: Vec<char> = vec!['a', 'b', 'c'];
    let x: &[char] = &x;
    match *x {
        ['a', 'b', 'c', ref _tail @ ..] => {}
        ['a', 'b', 'c'] => {} // { dg-error "" "" { target *-*-* } }
        _ => {}
    }
}

