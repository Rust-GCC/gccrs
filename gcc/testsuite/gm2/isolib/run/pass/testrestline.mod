MODULE testrestline ;

IMPORT SeqFile, TextIO ;

VAR
   chan    : SeqFile.ChanId ;
   line    : ARRAY [0..5] OF CHAR ;
   results : SeqFile.OpenResults ;
BEGIN
   SeqFile.OpenWrite (chan, "test.input", SeqFile.write, results) ;
   TextIO.WriteString (chan, "a line of text exceeding 6 chars") ;
   TextIO.WriteLn (chan) ;
   TextIO.WriteString (chan, "a second lineline of text exceeding 6 chars") ;
   TextIO.WriteLn (chan) ;
   SeqFile.Close (chan) ;

   (* Now see if we can read the first line.  *)
   SeqFile.OpenRead (chan, "test.input", SeqFile.read, results) ;
   TextIO.ReadRestLine (chan, line)
END testrestline.