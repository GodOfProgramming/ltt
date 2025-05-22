import unreal

for cls in unreal.ClassIterator(unreal.Object):
  print(cls.get_class_path_name())
