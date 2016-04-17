UNAME := $(shell uname)
ifeq ($(UNAME), $(filter $(UNAME), Linux))
	OS=linux
else
	OS=windows
endif

GENIE=tools/bin/$(OS)/genie

linux-build:
	$(GENIE) --file=build/genie.lua --compiler=linux-clang gmake
linux-debug64: linux-build
	make 
