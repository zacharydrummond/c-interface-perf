.PHONY: clean
clean:
	@rm -rf a.out
	@rm -rf $(shell find . -name '*.o')
	@rm -rf $(shell find . -name '*.d')
	@rm -rf $(shell find . -name '*.gch')
