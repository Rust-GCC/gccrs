package main

func f1(i int) bool {
  switch j := i; j {
  case 3: fallthrough
  case 1: return true
  case 2: return false
  default: return false
  case 4: return true
  }
}

func f2(i int) int {
  switch i {
    case i < 0: return -1
    case i > 0: return 1
    default: return 0
    case i != 0: return 1000
  }
}

func f3(i int) int {
 lab:
  switch i {
    case 1: break
    case 2: return 2
    case 3, 4:
      switch i {
        case 3: break lab
        case 4: break
      }
      return 4
  }
  return 1
}

func main() {
  if !f1(1) {
    panic();
  }
  if f1(2) {
    panic();
  }
  if !f1(3) {
    panic();
  }
  if !f1(4) {
    panic();
  }
  if f1(5) {
    panic();
  }

  if f2(-100) != -1 {
    panic();
  }
  if f2(1000) != 1 {
    panic();
  }
  if f2(0) != 0 {
    panic();
  }

  if f3(1) != 1 || f3(2) != 2 || f3(3) != 1 || f3(4) != 4 {
    panic();
  }
}
