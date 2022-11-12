# Configuration file for the Sphinx documentation builder.

import sys
sys.path.append('../..//doc')

from baseconf import *

name = 'libgomp'
project = 'GNU Offloading and Multi Processing Runtime Library'
copyright = '2006-2022 Free Software Foundation, Inc.'
authors = 'GCC Developer Community'

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
latex_documents = [
  ('index', f'{name}.tex', project, authors, 'manual'),
]

texinfo_documents = [
  ('index', name, project, authors, None, None, None, True)
]

set_common(name, globals())
