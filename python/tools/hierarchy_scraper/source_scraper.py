from typing import Callable

import CppHeaderParser
import os
import sys


def for_each_file(path: str, on_file: Callable[[str], None]):
  for root, subdirs, files in os.walk(path):
    for file in files:
      full_file_path = os.path.join(root, file)
      on_file(full_file_path)
    for subdir in subdirs:
      full_dir_path = os.path.join(root, subdir)
      for_each_file(full_dir_path, on_file)

def on_file(path: str):
  header = CppHeaderParser.CppHeader(path)
  for k, v in header.classes.items():
    cls: CppHeaderParser.CppHeaderParser.CppClass = v
    parents = cls['inherits']
    parent_names = set(map(lambda p: p["class"], parents))
    if "AActor" in parent_names:
      print(f"class {k} inherits from AActor")

dirs = sys.argv[1:]

for dir in dirs:
  for_each_file(dir, on_file)
