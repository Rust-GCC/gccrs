/* { dg-skip-if "requires hosted libstdc++ for stdlib malloc" { ! hostedlib } } */

/* Example adapted from https://cwe.mitre.org/data/definitions/415.html
   which states "Copyright © 2006–2022, The MITRE Corporation. CWE, CWSS, CWRAF, and the CWE logo are trademarks of The MITRE Corporation."
   and which has this on:
     https://cwe.mitre.org/about/termsofuse.html

   Terms of Use

   CWE™ is free to use by any organization or individual for any research, development, and/or commercial purposes, per these CWE Terms of Use. The MITRE Corporation ("MITRE") has copyrighted the CWE List, Top 25, CWSS, and CWRAF for the benefit of the community in order to ensure each remains a free and open standard, as well as to legally protect the ongoing use of it and any resulting content by government, vendors, and/or users. CWE is a trademark of MITRE. Please contact cwe@mitre.org if you require further clarification on this issue.

   LICENSE

   CWE Submissions: By submitting materials to The MITRE Corporation’s ("MITRE") Common Weakness Enumeration Program (CWE™), you hereby grant to MITRE a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable copyright license to use, reproduce, prepare derivative works of, publicly display, publicly perform, sublicense, and distribute your submitted materials and derivative works. Unless otherwise required by applicable law or agreed to in writing, it is understood that you are providing such materials on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied, including, without limitation, any warranties or conditions of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A PARTICULAR PURPOSE.

   CWE Usage: MITRE hereby grants you a non-exclusive, royalty-free license to use CWE for research, development, and commercial purposes. Any copy you make for such purposes is authorized on the condition that you reproduce MITRE’s copyright designation and this license in any such copy.

   DISCLAIMERS

   ALL DOCUMENTS AND THE INFORMATION CONTAINED IN THE CWE ARE PROVIDED ON AN "AS IS" BASIS AND THE CONTRIBUTOR, THE ORGANIZATION HE/SHE REPRESENTS OR IS SPONSORED BY (IF ANY), THE MITRE CORPORATION, ITS BOARD OF TRUSTEES, OFFICERS, AGENTS, AND EMPLOYEES, DISCLAIM ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY WARRANTY THAT THE USE OF THE INFORMATION THEREIN WILL NOT INFRINGE ANY RIGHTS OR ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

   IN NO EVENT SHALL THE CONTRIBUTOR, THE ORGANIZATION HE/SHE REPRESENTS OR IS SPONSORED BY (IF ANY), THE MITRE CORPORATION, ITS BOARD OF TRUSTEES, OFFICERS, AGENTS, AND EMPLOYEES BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE INFORMATION OR THE USE OR OTHER DEALINGS IN THE CWE.  */

#include <stdlib.h>
#include <string.h>

#define SIZE 1024

void example_1 (int abrt)
{
  char* ptr = (char*)malloc (SIZE);
  /* ... */
  if (abrt) {
    free(ptr);
  }
  /* ... */
  free(ptr); /* { dg-warning "double-'free' of 'ptr'" } */
}

#define BUFSIZE1 512
#define BUFSIZE2 ((BUFSIZE1/2) - 8)

int main(int argc, char **argv) {
  char *buf1R1;
  char *buf2R1;
  char *buf1R2;
  buf1R1 = (char *) malloc(BUFSIZE2);
  buf2R1 = (char *) malloc(BUFSIZE2);
  free(buf1R1);
  free(buf2R1);
  buf1R2 = (char *) malloc(BUFSIZE1);
  strncpy(buf1R2, argv[1], BUFSIZE1-1); /* { dg-warning "use of possibly-NULL 'buf1R2'" } */
  free(buf2R1); /* { dg-warning "double-'free' of 'buf2R1'" } */
  free(buf1R2);
}
