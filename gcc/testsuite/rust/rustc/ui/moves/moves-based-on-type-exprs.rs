// Tests that references to move-by-default values trigger moves when
// they occur as part of various kinds of expressions.


struct Foo<A> { f: A }
fn guard(_s: String) -> bool {panic!()}
fn touch<A>(_a: &A) {}

fn f10() {
    let x = "hi".to_string();
    let _y = Foo { f:x };
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f20() {
    let x = "hi".to_string();
    let _y = (x, 3);
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f21() {
    let x = vec![1, 2, 3];
    let _y = (x[0], 3);
    touch(&x);
}

fn f30(cond: bool) {
    let x = "hi".to_string();
    let y = "ho".to_string();
    let _y = if cond {
        x
    } else {
        y
    };
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
    touch(&y); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f40(cond: bool) {
    let x = "hi".to_string();
    let y = "ho".to_string();
    let _y = match cond {
        true => x,
        false => y
    };
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
    touch(&y); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f50(cond: bool) {
    let x = "hi".to_string();
    let y = "ho".to_string();
    let _y = match cond {
        _ if guard(x) => 10,
        true => 10,
        false => 20,
    };
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
    touch(&y);
}

fn f70() {
    let x = "hi".to_string();
    let _y = [x];
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f80() {
    let x = "hi".to_string();
    let _y = vec![x];
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f100() {
    let x = vec!["hi".to_string()];
    let _y = x.into_iter().next().unwrap();
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f110() {
    let x = vec!["hi".to_string()];
    let _y = [x.into_iter().next().unwrap(); 1];
    touch(&x); // { dg-error ".E0382." "" { target *-*-* } }
}

fn f120() {
    let mut x = vec!["hi".to_string(), "ho".to_string()];
    x.swap(0, 1);
    touch(&x[0]);
    touch(&x[1]);
}

fn main() {}

