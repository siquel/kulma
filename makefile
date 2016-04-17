UNAME := $(shell uname)
ifeq ($(UNAME), $(filter $(UNAME), Linux))
	OS=linux
else
	OS=windows
endif

GENIE=tools/bin/$(OS)/genie

.PHONY: help

help:
	@echo Insert something here

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
