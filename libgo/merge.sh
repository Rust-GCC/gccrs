#!/bin/sh

# Copyright 2009 The Go Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

# This script merges changes from the master copy of the Go library
# into the libgo library.  This does the easy stuff; the hard stuff is
# left to the user.

# The file MERGE should hold the Mercurial revision number of the last
# revision which was merged into these sources.  Given that, and given
# the current sources, we can run the usual diff3 algorithm to merge
# all changes into our sources.

set -e

TMPDIR=${TMPDIR:-/tmp}

OLDDIR=${TMPDIR}/libgo-merge-old
NEWDIR=${TMPDIR}/libgo-merge-new

if ! test -f MERGE; then
  echo 1>&2 "merge.sh: must be run in libgo source directory"
  exit 1
fi

if test $# -ne 1; then
  echo 1>&2 "merge.sh: Usage: merge.sh mercurial-repository"
  exit 1
fi

repository=$1

merge_rev=`sed 1q MERGE`

rm -rf ${OLDDIR}
hg clone -r ${merge_rev} ${repository} ${OLDDIR}

rm -rf ${NEWDIR}
hg clone ${repository} ${NEWDIR}

new_rev=`cd ${NEWDIR} && hg log | sed 1q | sed -e 's/.*://'`

(cd ${NEWDIR}/src/pkg && find . -name '*.go' -print) | while read f; do
  if test `dirname $f` = "./syscall"; then
    continue
  fi
  oldfile=${OLDDIR}/src/pkg/$f
  newfile=${NEWDIR}/src/pkg/$f
  libgofile=go/$f
  if test -f ${oldfile}; then
    # The file exists in the old version.
    if ! test -f ${libgofile}; then
      echo "merge.sh: $f: skipping: exists in old and new hg, but not in libgo"
      continue
    fi
    if cmp -s ${oldfile} ${libgofile}; then
      # The libgo file is unchanged from the old version.
      if cmp -s ${newfile} ${libgofile}; then
        # File is unchanged from old to new version.
        continue
      fi
      # Update file in libgo.
      echo "merge.sh: $f: updating"
      cp ${newfile} ${libgofile}
    else
      # The libgo file has local changes.
      set +e
      diff3 -m -E ${libgofile} ${oldfile} ${newfile} > ${libgofile}.tmp
      status=$?
      set -e
      case $status in
      0)
        echo "merge.sh: $f: updating"
        mv ${libgofile}.tmp ${libgofile}
        ;;
      1)
        echo "merge.sh: $f: CONFLICTS"
        mv ${libgofile}.tmp ${libgofile}
	hg resolve -u ${libgofile}
        ;;
      *)
        echo 1>&2 "merge.sh: $f: diff3 failure"
        exit 1
        ;;
      esac
    fi
  else
    # The file does not exist in the old version.
    if test -f ${libgofile}; then
      if ! cmp -s ${newfile} ${libgofile}; then
        echo 1>&2 "merge.sh: $1: in new and libgo but not old"
        exit 1
      fi
    else
      echo "merge.sh: $f: NEW"
      dir=`dirname ${libgofile}`
      if ! test -d ${dir}; then
        mkdir ${dir}
      fi
      cp ${newfile} ${libgofile}
      hg add ${libgofile}
    fi
  fi
done

(cd ${OLDDIR}/src/pkg && find . -name '*.go' -print) | while read f; do
  oldfile=${OLDDIR}/src/pkg/$f
  newfile=${NEWDIR}/src/pkg/$f
  libgofile=go/$f
  if test -f ${newfile}; then
    continue
  fi
  if ! test -f ${libgofile}; then
    continue
  fi
  echo "merge.sh: ${libgofile}: REMOVED"
  rm -f ${libgofile}
  hg rm ${libgofile}
done

(echo ${new_rev}; sed -ne '2,$p' MERGE) > MERGE.tmp
mv MERGE.tmp MERGE
