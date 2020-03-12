#################
### Functions ###
#################

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

#################
### Variables ###
#################
<% for var in $VARS %>
<%= var -%>
<% end %>
################
### Targets  ###
################
<% for target in $PHONY_TARGETS %>
<%= target -%>
<% end %>
-include $(DEPENDENCIES)
<% for target in $REAL_TARGETS %>
<%= target -%>
<% end %>

