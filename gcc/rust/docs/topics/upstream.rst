.. Copyright (C) 2025 Free Software Foundation, Inc.

   This is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see
   <https://www.gnu.org/licenses/>.

Upstream handling
=================

This document describes how the gccrs project synchronizes with upstream GCC. This include both directions:

* how to merge latest changes from GCC into GCCRS (upstream -> downstream)
* how to merge GCCRS changes into GCC (downstream -> upstream)

Main develpment branch anatomy
------------------------------

The main branch (called ``master`` in github, denoted ``gccrs-master`` here to
avoid confusion) is regularly updated with changes from upstream GCC ``master``
branch (denoted ``gcc-master``). For this purpose, a "rebased branch" is merged
in the ``gccrs-master`` branch, with the ``--strategy=ours``. This allows:

* to make sure only the version comming from the "rebased branch" is kept;
* to keep existing history and commits: we don't want every update to "rewrite
  the history" and make all forks incompatible.

Updating the "rebased branch"
-----------------------------

The "rebased branch" consists in::


                                 gccrs-master
        ┌────A-B-C-D──┬──────────────────────►
        │             │                    ▲
        │             └────────────────┐   │
        │                              │   │
   ──U0─@───U1─U2─U3───┬───────────────┼───┼─────────► gcc-master
                       │               │   │
                       │               │   │
                       │               ▼   │
                       └─A'-B'-C'-D'───M ──┴─────────► gccrs master-rebased

* the ``A..D`` range represents all the commits in the ``gccrs-master`` branch
  that are not upstreamed yet, based on the ``U0`` commit from the
  ``gcc-master`` branch.
* the ``A'..D'`` range represents the same commits, rebased onto the ``U3``
  commit from the ``gcc-master`` branch.
* ``M`` is a merge commit using the `ours strategy
  <https://git-scm.com/docs/merge-strategies#Documentation/merge-strategies.txt-ours>`_.

Receipes
--------

Common setup (adjust the actual git command to your setup)

- GCCRS github remote is ``gccrs``
- Upstream GCC remote is ``gcc``

.. code-block:: bash

   $ git remote add gccrs git@github.com:Rust-GCC/gccrs.git
   $ git remote add gcc git://gcc.gnu.org/git/gcc.git


Getting changes from upstream (GCC -> GCCRS)
++++++++++++++++++++++++++++++++++++++++++++

The idea is to create a new branch where GCCRS commits are rebased over a recent
GCC commit, and the result is merged back using the ``ours`` strategy (meaning
that only the changes from the rebased branch are kept, the other branch's
changes are discarded) in the main GCCRS develpment branch. The merging instead
of rewriting the history allows the existing branches (e.g. existing pull
request, or other work-in-progress) can be rebased over the develpment branch
without any particular issue.

Actual commands to do the update:

1. Switch to ``master-rebased``
2. Remove the Merge commit
3. Rebase onto a fresh GCC master
4. Merge with current ``master``
5. Update the gccrs ``master`` branch (either fast-forward or merge
   `master-rebased`).

.. code-block:: bash

   $ git switch master-rebased              #1
   $ git reset HEAD^                        #2
   $ git rebase gcc/master                  #3
   $ git merge --strategy=ours gccrs/master #4
   $ FIXME                                  #5

Upstreaming (GCCRS -> GCC)
++++++++++++++++++++++++++

To upstream GCCRS commit to GCC, we'll create a new branch from the develpment
branch and apply some cleanup to remove changes that must not be sent in GCC
(e.g. github actions, anything GCCRS specific in the project root). It's also
possible that some more cleanup is applied (not exhaustive):

- "commit A" followed by "revert A": both should be dropped.
- "commit B" followed by "fixup B" (any forms): these should be squashed.


Prior to upstreaming changes, you need to make sure the current base for
``gccrs-master`` is a (very) recent commit from the ``gcc-master``.

1. Update all remotes and local branches

   .. code-block:: bash

      $ git remote update
      $ git switch gccrs-master && git pull --ff-only
      $ git switch gcc-master && git pull --ff-only

2. Create a new branch ``upstream-$DATE`` from the ``gccrs-master``:

   .. code-block:: bash

      $ git switch -C "upstream-$DATE" gccrs-master

3. Apply some scripts to drop any commit touching GCCRS only files (mainly the
   github CI and some doc that has no use upstream). The following command can
   be used to list commits that contains changes that should not be sent
   upstream. By construction, we should be able to drop these commits, but we
   should double check before (e.g. some commit may touch both CI and
   compiler... should not happen, but we had some)

     .. code-block:: bash

        $ git rev-list $(git merge-base gcc/master upstream-$DATE)..upstream-$DATE \
              -- .github/ CODE_OF_CONDUCT.md CONTRIBUTING.md Dockerfile README.md logo.png

   You can combine it with ``rebase`` to drop commits:

     .. code-block:: bash

        $ git rev-list --abbrev-commit $(git merge-base gcc/master HEAD)..HEAD \
              -- .github/ CODE_OF_CONDUCT.md CONTRIBUTING.md Dockerfile README.md logo.png | \
           sed -e 's@\(.*\)@/\1/d@g' > s.sed
        $ GIT_SEQUENCE_EDITOR="sed -i -f s.sed" \
              git rebase -i $(git merge-base gcc-master HEAD)

4. Check the GNU ChangeLog. Use interactive rebase to edit failing commits.

   .. code-block:: bash

      $ ./contrib/gcc-changelog/git_check_commit.py gcc-master..gccrs-master

5. Bootstrap and run the testsuite for every commit:

   .. code-block:: bash

      $ mkdir build && cd build
      $ ../configure --enable-languages=rust
      $ git rebase -i gcc-master -x 'make -j $(nproc) && make -j $(nproc) check-rust'

6. Review one last time before pushing commits:

   .. code-block:: bash

      $ git log --oneline gcc-master..gccrs-master

7. Push commits

   .. code-block:: bash

      $ git push gcc "upsteam-$DATE:master"

   If the hook fails, it's probably because someone updated upstream ``master``
   branch. A simple rebase should be fine:

   .. code-block:: bash

      $ git pull --rebase gcc/master
      $ git push gcc "upsteam-$DATE:master"

7. Send emails and annotate:

   .. code-block:: bash

      $ GIT_EDITOR=FIXME-script git send-email --annotate \
             --subject-prefix=COMMITTED  \
             --to=gcc-patches@gcc.gnu.org gcc-master
