# <%= derive_autogen_commands(options) %>

#################
### Functions ###
#################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

#################
### Variables ###
#################
<% for var in all_vars %>
<%= var -%>
<% end %>
################
### Targets  ###
################
<% for target in phony_targets %>
<%= target -%>
<% end %>
-include $(DEPENDENCIES)
<% for target in real_targets %>
<%= target -%>
<% end %>

