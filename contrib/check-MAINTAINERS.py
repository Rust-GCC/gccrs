#!/usr/bin/env python3

# Copyright (C) 2022-2026 Free Software Foundation, Inc.
#
# This file is part of GCC.
#
# GCC is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GCC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING.  If not, write to
# the Free Software Foundation, 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

# Check that names in the file are sorted alphabetically by surname, name
# bugzilla handle and email (in this order).

import locale
import sys
import re
from itertools import groupby
from operator import itemgetter
from difflib import ndiff
import unidecode

locale.setlocale(locale.LC_ALL, 'en_US.utf8')

exit_code = 0

if len(sys.argv) != 2:
    print('Usage: ./check-MAINTAINERS.py path-to/MAINTAINERS')
    sys.exit(1)


def get_name_for_sort(name):
    parts = name.split()
    surname = parts[-1] + ", " + " ".join(parts[:-1])

    # Special-case some names
    if (name == 'Stefan Schulze Frielinghaus'
        or name == 'Kris Van Hees'):
        surname = " ".join(parts[1:]) + ", " + parts[0]
    if surname.startswith("d'"):
        surname = surname[2:]

    # Remove accents
    return unidecode.unidecode(surname)


def check_group(name, lines, columns, matcher, sort_by):
    global exit_code
    named_lines = []
    rex = re.compile (matcher)
    for line in lines:
        if line.startswith(' '):
            print(f'Line should not start with space: "{line}"')
            exit_code = 2
            continue

        if line.endswith(' '):
            print(f'Line should not end with space: "{line}"')
            exit_code = 3
            continue

        # Special-case some names
        if line == 'James Norris':
            named_lines.append((get_name_for_sort(line), "", "", line))
            continue

        fields = rex.match(line)
        if not fields:
            print(f'Could not parse line: "{line}"')
            exit_code = 3
            continue

        matched_keys = [
            name for name, idx in sorted (rex.groupindex.items(),
                                        key=lambda x: x[1])
            if fields.group(idx) is not None]

        pos = 0
        for i, k in enumerate (matched_keys):
            if i >= len (columns):
                break
            if pos < columns[i]:
                pos = columns[i]
            start = fields.start(k)
            if k == 'email':
                start -= 1  # Account for the leading '<'
            if start == pos:
                True
            elif (start > pos
                  and pos > 2
                  and k == 'email'
                  and line[start-1] == " "
                  and line[start-2] != " "):
                True
            elif (start > pos
                  and k != 'email'
                  and pos > 3
                  and line[start-2:start] == "  "
                  and line[start-3] != " "):
                True
            else:
                exit_code = 3
                print (line)
                print (f"{k} starts in the wrong column: expected: {pos}, actual {start}")
            pos += 1 + len (fields[k].rstrip())

        fields = fields.groupdict()
        if 'Team' in fields and fields['Team']:
            fields['User'] = fields['Team']
            fields['email'] = ""
        pieces = []
        for i, f in enumerate (sort_by):
            if f not in fields:
                print('Internal error: re mismatch')
                sys.exit(10)
            if f == 'User':
                pieces.append(get_name_for_sort(fields[f].rstrip()))
            elif f == 'component':
                pieces.append(fields[f].rstrip().lower())
            else:
                pieces.append(fields[f].rstrip())
        pieces.append (line)
        named_lines.append(pieces)

    order = []
    for i, _ in enumerate (sort_by):
        order.append(i)
    order = tuple(order)
    lines = [line + "\n" for line in lines]
    sorted_lines = [line[-1] + "\n"
                    for line in sorted(named_lines,
                                       key = itemgetter(*order))]
    if lines != sorted_lines:
        exit_code = 1
        diff = [line for line in ndiff(lines, sorted_lines)
                if not line.startswith('? ')]
        print(f'Wrong order for {name}:\n')
        print(''.join(diff))
    else:
        print(f'{name} are fine!')


text = open(sys.argv[1]).read()
if '\t' in text:
    print('The file should not contain tabs')
    exit_code = 9

sections = [
    # heading, paragraph index, column numbers, regex, sort order
    ('Global Reviewers',
     1,
     [0, 48],
     r'^(?P<User>.{47}) <(?P<email>.*)>$',
     ['User', 'email']),
    ('CPU Port Maintainers    (CPU alphabetical order)',
     1,
     [0, 24, 48],
     r'^(?P<component>.{23}) (?P<User>.*) <(?P<email>[^<>]*)>$',
     ['component', 'User', 'email']),
    ('OS Port Maintainers     (OS alphabetical order)',
     1,
     [0, 24, 48],
     r'^(?P<component>.{23}) (?P<User>.*) <(?P<email>[^<>]*)>$',
     ['component', 'User', 'email']),
    ('Language Front Ends Maintainer',
     1,
     [0, 24, 48],
     r'^(?P<component>.{23}) (?P<User>.*) <(?P<email>[^<>]*)>$',
     ['component', 'User', 'email']),
    ('Various Maintainers',
     1,
     [0, 24, 48],
     r'''(?x)
         ^(?P<component>(?:(?!\s\s).)*)\s\s+
         (?:(?P<Team>All.*maintainers)
          |(?P<User>[^\s].*)\s+<(?P<email>[^<>]*)>)$''',
     ['component', 'User', 'email']),
    ('Reviewers',
     1,
     [0, 24, 48],
     r'^(?P<component>(?:(?!  ).)*)  +(?P<User>[^\s].*) <(?P<email>[^<>]*)>$',
     ['component', 'User', 'email']),
    ('Write After Approval',
     2,
     [0, 32, 48],
     r'^(?P<User>.{31}) (?P<account>.{15}) <(?P<email>[^<>]*)>$',
     ['User', 'email', 'account']),
    ('Bug database only accounts',
     1,
     [0, 48],
     r'^(?P<User>.{47}) <(?P<email>[^<>]*)>$',
     ['User', 'email']),
    ('Contributing under the DCO',
     2,
     [0, 48],
     r'^(?P<User>.{47}) <(?P<email>[^<>]*)>$',
     ['User', 'email'])
]

i = 0
count = 0
for is_empty, lines in groupby(text.splitlines(), lambda x: not x):
    if is_empty:
        continue
    lines = list(lines)
    if count > 0:
        count -= 1
        if count == 0:
            check_group(sections[i][0], lines, sections[i][2], sections[i][3],
                        sections[i][4])
            i += 1
    elif len(lines) == 1 and i < len(sections) and sections[i][0] in lines[0]:
        count = sections[i][1]

if i < len(sections):
    print(f'Missing "{sections[i][0]}" section')
    exit_code = 10

sys.exit(exit_code)
