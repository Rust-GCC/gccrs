       *> { dg-do run }
       *> { dg-output-file "group2/floating-point_FORMAT_2.out" }
       identification division.
       program-id. prog.

       data division.
       working-storage section.
       01  cmp1a                        comp-1.
       01  cmp1b                        comp-1.
       01  cmp1c                        comp-1.

       01  cmp2a                        comp-2.
       01  cmp2b                        comp-2.
       01  cmp2c                        comp-2.

       01  cmp3a                        float-extended.
       01  cmp3b                        float-extended.
       01  cmp3c                        float-extended.

       procedure division.
            display "--- COMP-1 FORMAT 2 ---"
            move 10 to cmp1a
            move  7 to cmp1b
            add      cmp1b to    cmp1a giving cmp1c
            display  "add      " cmp1a space cmp1b space  cmp1c
            subtract cmp1b from  cmp1a giving cmp1c
            display  "subtract " cmp1a space cmp1b space  cmp1c
            multiply cmp1b by    cmp1a giving cmp1c
            display  "multiply " cmp1a space cmp1b space  cmp1c
            divide   cmp1a by    cmp1b giving cmp1c
            display  "divide   " cmp1a space cmp1b space  cmp1c

            display "--- COMP-2 FORMAT 2 ---"
            move 10 to cmp2a
            move  7 to cmp2b
            add      cmp2b to    cmp2a giving cmp2c
            display  "add      " cmp2a space cmp2b space  cmp2c
            subtract cmp2b from  cmp2a giving cmp2c
            display  "subtract " cmp2a space cmp2b space  cmp2c
            multiply cmp2b by    cmp2a giving cmp2c
            display  "multiply " cmp2a space cmp2b space  cmp2c
            divide   cmp2a by    cmp2b giving cmp2c
            display  "divide   " cmp2a space cmp2b space  cmp2c

            display "--- FLOAT-EXTENDED FORMAT 2 ---"
            move 10 to cmp3a
            move  7 to cmp3b
            add      cmp3b to    cmp3a giving cmp3c
            display  "add      " cmp3a space cmp3b space  cmp3c
            subtract cmp3b from  cmp3a giving cmp3c
            display  "subtract " cmp3a space cmp3b space  cmp3c
            multiply cmp3b by    cmp3a giving cmp3c
            display  "multiply " cmp3a space cmp3b space  cmp3c
            divide   cmp3a by    cmp3b giving cmp3c
            display  "divide   " cmp3a space cmp3b space  cmp3c

           goback.
       end program prog.

