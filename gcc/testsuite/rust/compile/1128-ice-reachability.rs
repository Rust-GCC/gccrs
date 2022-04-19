trait Hasher {
    fn write(&mut self, bytes: &[u8]);
    fn write_u8(&mut self, i: u8) {
        self.write(&[i])
    }
}

pub trait PubHasher {
    fn p_write(&mut self, bytes: &[u8]);
    fn p_write_u8(&mut self, i: u8) {
        self.write(&[i])
    }
}
