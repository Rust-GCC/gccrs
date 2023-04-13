/* { dg-do compile } */
/* { dg-options "-fdiagnostics-format=sarif-file" } */

extern void inject_ice (void);

void test_inject_ice (void)
{
  inject_ice (); /* { dg-ice "" } */
  /* { dg-regexp "during GIMPLE pass: crash_test" } */
}

/* Verify that some JSON was written to a file with the expected name.  */
/* { dg-final { verify-sarif-file } } */

/* We expect various properties.
   The indentation here reflects the expected hierarchy, though these tests
   don't check for that, merely the string fragments we expect.

   { dg-final { scan-sarif-file "\"version\": \"2.1.0\"" } }
   { dg-final { scan-sarif-file "\"runs\": \\\[" } }
     { dg-final { scan-sarif-file "\"artifacts\": \\\[" } } 
       { dg-final { scan-sarif-file "\"location\": " } }
         { dg-final { scan-sarif-file "\"uri\": " } }

       { dg-final { scan-sarif-file "\"sourceLanguage\": \"c\"" { target c } } }
       { dg-final { scan-sarif-file "\"sourceLanguage\": \"cplusplus\"" { target c++ } } }

       { dg-final { scan-sarif-file "\"contents\": " } }
         { dg-final { scan-sarif-file "\"text\": " } }
     { dg-final { scan-sarif-file "\"tool\": " } }
       { dg-final { scan-sarif-file "\"driver\": " } }
         { dg-final { scan-sarif-file "\"name\": \"GNU C" } }
         { dg-final { scan-sarif-file "\"fullName\": \"GNU C" } }
         { dg-final { scan-sarif-file "\"informationUri\": \"" } }
       { dg-final { scan-sarif-file "\"extensions\": \\\[" } }
         { dg-final { scan-sarif-file "\"name\": \"crash_test_plugin\"" } }

     We expect no results:
     { dg-final { scan-sarif-file "\"results\": \\\[\\\]" } }

     but instead should have an invocations array...

     { dg-final { scan-sarif-file "\"invocations\": \\\[" } }

     ...containing this:
       { dg-final { scan-sarif-file "\"executionSuccessful\": false" } }
       { dg-final { scan-sarif-file "\"toolExecutionNotifications\": \\\[" } }

       ...containing this notification:
         { dg-final { scan-sarif-file "\"level\": \"error\"" } }
         { dg-final { scan-sarif-file "\"locations\": \\\[" } }
           { dg-final { scan-sarif-file "\"logicalLocations\": \\\[" } }
             { dg-final { scan-sarif-file "\"kind\": \"function\"" } }
             { dg-final { scan-sarif-file "\"name\": \"test_inject_ice\"" } }
           { dg-final { scan-sarif-file "\"physicalLocation\": " } }
             { dg-final { scan-sarif-file "\"contextRegion\": " } }
             { dg-final { scan-sarif-file "\"artifactLocation\": " } }
             { dg-final { scan-sarif-file "\"region\": " } }
               { dg-final { scan-sarif-file "\"startLine\": 8" } }
               { dg-final { scan-sarif-file "\"startColumn\": 3" } }
               { dg-final { scan-sarif-file "\"endColumn\": 16" } }
         { dg-final { scan-sarif-file "\"message\": " } }
           { dg-final { scan-sarif-file "\"text\": \"I'm sorry Dave, I'm afraid I can't do that\"" } } */
