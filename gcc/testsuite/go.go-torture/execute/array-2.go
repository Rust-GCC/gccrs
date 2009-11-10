package main

func fn(a []int) int {
  alen := len(a);
  for i := 0; i < alen; i++ {
    a[i] = i
  }
  return alen;
}

func main() {
  var a [2]int;
  if fn(&a) != 2 {
    panic();
  }
  if a[0] != 0 || a[1] != 1 {
    panic();
  }
}
