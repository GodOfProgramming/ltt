import os
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath(os.path.dirname(__file__))

flags = [
    '-Wall',
    '-Wextra',
    '-x',
    'c'
]

def Settings( **kwargs):
    return {
        'flags': flags,
        'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT
    }

