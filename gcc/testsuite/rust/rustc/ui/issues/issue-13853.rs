trait Node {
    fn zomg();
}

trait Graph<N: Node> {
    fn nodes<'a, I: Iterator<Item=&'a N>>(&'a self) -> I
        where N: 'a;
}

impl<N: Node> Graph<N> for Vec<N> {
    fn nodes<'a, I: Iterator<Item=&'a N>>(&self) -> I
        where N: 'a
    {
        self.iter() // { dg-error ".E0308." "" { target *-*-* } }
    }
}

struct Stuff;

impl Node for Stuff {
    fn zomg() {
        println!("zomg");
    }
}

fn iterate<N: Node, G: Graph<N>>(graph: &G) {
    for node in graph.iter() { // { dg-error ".E0599." "" { target *-*-* } }
        node.zomg();
    }
}

pub fn main() {
    let graph = Vec::new();

    graph.push(Stuff);

    iterate(graph); // { dg-error ".E0308." "" { target *-*-* } }
}

