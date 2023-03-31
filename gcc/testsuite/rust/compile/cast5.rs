fn main() {
    const A: char = 0x1F888 as char;
    //~^ ERROR only u8 can be cast into char
    const B: char = 129160 as char;
    //~^ ERROR only u8 can be cast into char
    const C: i32 = 42;
    const D: char = C as char;
    //~^ ERROR only u8 can be cast into char
    const E: char = '\u{01F888}';
    const F: u8 = 42; 
    const G: char= F as char;
}
