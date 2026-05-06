MODULE printsize;

FROM SYSTEM IMPORT WORD, WORD32 ;
FROM libc IMPORT printf ;
	  
	  
BEGIN				(*   Module printsize Initialization Code  *)
     printf ("SIZE (WORD) = %d\n", SIZE (WORD)) ;
     printf ("SIZE (WORD32) = %d\n", SIZE (WORD32)) ;     
END printsize.
