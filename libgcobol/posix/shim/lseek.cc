#include <sys/types.h>
#include <unistd.h>

#include <cassert>

extern "C" {

off_t
posix_lseek(int fd, off_t offset, int whence) {
  /*
   * Map valid input whence value onto C standard library value.
   * Invalid values are passed through and rejected by lseek(2) per its documentation.
   * (The caller always needs to check for errors anyway.)
   */
  switch( whence ) {
  case 2:
    whence = SEEK_SET;
    break;
  case 4:
    whence = SEEK_CUR;
    break;
  case 8:
    whence = SEEK_END;
    break;
  } 
  return lseek(fd, offset, whence);
}

} // extern "C"
