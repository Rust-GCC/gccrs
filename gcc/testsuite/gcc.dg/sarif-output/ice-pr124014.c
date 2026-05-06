/* { dg-options "-fdiagnostics-add-output=sarif -fbranch-probabilities" } */
/* { dg-additional-options "-flto" { target lto } }  */

void main() {} /* { dg-warning "missing-profile" } */
