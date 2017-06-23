.PHONY: clean All

All:
	@echo "----------Building project:[ PokeFinder - Debug ]----------"
	@"$(MAKE)" -f  "PokeFinder.mk"
clean:
	@echo "----------Cleaning project:[ PokeFinder - Debug ]----------"
	@"$(MAKE)" -f  "PokeFinder.mk" clean
