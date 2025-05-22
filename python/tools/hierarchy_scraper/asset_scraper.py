import unreal

asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()

for untyped_asset in asset_registry.get_all_assets():
  asset: unreal.AssetData = untyped_asset
  parent = asset.get_tag_value('ParentClass')
  if parent is not None:
    unreal.log(f"Parent of {asset.asset_name}: {parent}")
