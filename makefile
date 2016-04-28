UNAME := $(shell uname)
ifeq ($(UNAME), $(filter $(UNAME), Linux))
	OS=linux
else
	OS=windows
endif

GENIE=kaluketju/tools/bin/$(OS)/genie

projgen:
	$(GENIE) --file=build/genie.lua vs2013
	$(GENIE) --file=build/genie.lua vs2015
	$(GENIE) --gcc=linux-gcc --file=build/genie.lua gmake
	$(GENIE) --gcc=linux-clang --file=build/genie.lua gmake

linux-build-gcc:
	$(GENIE) --gcc=linux-gcc --file=build/genie.lua gmake
linux-debug64-gcc: linux-build-gcc
	make -C .build/projects/gmake-linux config=debug64 
linux-release64-gcc: linux-build-gcc
	make -C .build/projects/gmake-linux config=release64
linux-gcc: linux-debug64-gcc linux-release64-gcc

linux-build-clang:
	$(GENIE) --gcc=linux-clang --file=build/genie.lua gmake
linux-debug64-clang: linux-build-clang
	make -C .build/projects/gmake-linux-clang config=debug64
linux-release64-clang: linux-build-clang
	make -C .build/projects/gmake-linux-clang config=release64
linux-clang: linux-debug64-clang linux-release64-clang

windows-build: 
	$(GENIE) --file=build/genie.lua vs2013
	$(GENIE) --file=build/genie.lua vs2015

.PHONY: clean
clean:
	@echo Cleaning..
	-@rm -rf .build
