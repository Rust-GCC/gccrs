enum BtNode {
    Node(u32,Box<BtNode>,Box<BtNode>),
    Leaf(u32),
}

fn main() {
    let y = match 10 {
        Foo<T>::A(value) => value, // { dg-error "" "" { target *-*-* } }
        Foo<T>::B => 7,
    };
}

