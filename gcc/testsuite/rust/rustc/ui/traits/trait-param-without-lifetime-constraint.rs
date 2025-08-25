struct Article {
    proof_reader: ProofReader,
}

struct ProofReader {
    name: String,
}

pub trait HaveRelationship<To> {
    fn get_relation(&self) -> To;
}

impl HaveRelationship<&ProofReader> for Article {
    fn get_relation(&self) -> &ProofReader {
// { dg-error "" "" { target *-*-* } .-1 }
        &self.proof_reader
    }
}

fn main() {}

