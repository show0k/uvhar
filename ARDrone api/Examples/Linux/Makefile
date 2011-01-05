
.PHONY:

all:
	@$(MAKE) -C Navigation/Build
	@$(MAKE) -C sdk_demo/Build

$(MAKECMDGOALS):
	@$(MAKE) -C Navigation/Build $(MAKECMDGOALS)
	@$(MAKE) -C sdk_demo/Build $(MAKECMDGOALS)

