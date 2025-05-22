from typing import Callable

import CppHeaderParser
import os
import sys

def is_cpp(path: str) -> bool:
  return path.endswith(".h") or path.endswith(".hpp") or path.endswith(".cpp")

def for_each_file(path: str, on_file: Callable[[str], None]):
  for root, subdirs, files in os.walk(path):
    files = filter(is_cpp, files)
    for file in files:
      full_file_path = os.path.join(root, file)
      on_file(full_file_path)
    for subdir in subdirs:
      full_dir_path = os.path.join(root, subdir)
      for_each_file(full_dir_path, on_file)

def on_file(path: str):
  header = CppHeaderParser.CppHeader(path)
  for name, untyped_cls in header.classes.items():
    cls: CppHeaderParser.CppHeaderParser.CppClass = untyped_cls
    parents = cls["inherits"]
    parent_names = list(map(lambda p: p["class"], parents))
    print(",".join([name] + parent_names))


dirs = sys.argv[1:]

for dir in dirs:
  for_each_file(dir, on_file)
