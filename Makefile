
.PHONY: benchmark
benchmark: containerof_interface cpp_abstract_class embedded_vtable_interface fat_pointer_interface
	asdf

.PHONY: containerof_interface
containerof_interface:
	$(MAKE) -C src/containerof_interface

.PHONY: cpp_abstract_class
cpp_abstract_class:
	$(MAKE) -C src/cpp_abstract_class

.PHONY: embedded_vtable_interface
embedded_vtable_interface:
	$(MAKE) -C src/embedded_vtable_interface

.PHONY: fat_pointer_interface
fat_pointer_interface:
	$(MAKE) -C src/fat_pointer_interface

.PHONY: clean
clean:
	@rm -rf a.out
	@rm -rf $(shell find . -name '*.o')
	@rm -rf $(shell find . -name '*.d')
	@rm -rf $(shell find . -name '*.gch')
