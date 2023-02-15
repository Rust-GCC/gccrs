#!/usr/bin/env python3

# Copyright (C) 2018-2023 Free Software Foundation, Inc.
#
# Find missing and extra parameters in documentation compared to
# output of: gcc --help=params.
#
# This file is part of GCC.
#
# GCC is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3, or (at your option) any later
# version.
#
# GCC is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.  */
#
#
#

import argparse
import sys
from itertools import dropwhile, takewhile


def get_param_tuple(line):
    line = line.strip().replace('--param=', '')
    i = line.find(' ')
    name = line[:i]
    if '=' in name:
        name = name[:name.find('=')]
    description = line[i:].strip()
    return (name, description)


parser = argparse.ArgumentParser()
parser.add_argument('texi_file')
parser.add_argument('params_output')

args = parser.parse_args()

ignored = {'logical-op-non-short-circuit'}
params = {}

for line in open(args.params_output).readlines():
    if line.startswith(' ' * 2) and not line.startswith(' ' * 8):
        r = get_param_tuple(line)
        params[r[0]] = r[1]

# Find section in .texi manual with parameters
texi = ([x.strip() for x in open(args.texi_file).readlines()])
texi = dropwhile(lambda x: 'item --param' not in x, texi)
texi = takewhile(lambda x: '@node Instrumentation Options' not in x, texi)
texi = list(texi)[1:]

texi_params = []
for line in texi:
    for token in ('@item ', '@itemx '):
        if line.startswith(token):
            texi_params.append(line[len(token):])
            break

# skip digits
texi_params = [x for x in texi_params if not x[0].isdigit()]
# skip aarch64 params
texi_params = [x for x in texi_params if not x.startswith('aarch64')]
sorted_params = sorted(texi_params)

texi_set = set(texi_params) - ignored
params_set = set(params.keys()) - ignored

success = True
extra = texi_set - params_set
if len(extra):
    print('Extra:')
    print(extra)
    success = False

missing = params_set - texi_set
if len(missing):
    print('Missing:')
    for m in missing:
        print('@item ' + m)
        print(params[m])
        print()
    success = False

sys.exit(0 if success else 1)
