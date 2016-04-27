UNAME := $(shell uname)
ifeq ($(UNAME), $(filter $(UNAME), Linux))
	OS=linux
else
	OS=windows
endif

GENIE=kaluketju/tools/bin/$(OS)/genie

.PHONY: help

help:
	@echo Insert something here

projgen:
	$(GENIE) --file=build/genie.lua vs2013
	$(GENIE) --file=build/genie.lua vs2015
	$(GENIE) --gcc=linux-gcc --file=build/genie.lua gmake
	$(GENIE) --gcc=linux-clang --file=build/genie.lua gmake

linux-build:
	$(GENIE) --file=build/genie.lua --compiler=linux-clang gmake
linux-debug64: linux-build
	make -C .build/linux config=debug64 
linux-release64: linux-build
	make -C .build/linux config=release64
linux: linux-debug64 linux-release64

.PHONY: clean
clean:
	@echo Cleaning..
	-@rm -rf .build
