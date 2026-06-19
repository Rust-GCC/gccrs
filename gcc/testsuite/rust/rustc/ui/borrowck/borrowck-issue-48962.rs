struct Node {
    elem: i32,
    next: Option<Box<Node>>,
}

fn a() {
    let mut node = Node {
        elem: 5,
        next: None,
    };

    let mut src = &mut node;
    {src};
    src.next = None; // { dg-error ".E0382." "" { target *-*-* } }
}

fn b() {
    let mut src = &mut (22, 44);
    {src};
    src.0 = 66; // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {
    a();
    b();
}

