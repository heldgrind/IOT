all: build

VERSION := $(shell yotta --version 2> /dev/null)

check:
ifeq ($(VERSION),)
	@echo you should use before trying anything: source /sync/Module_Dev_app_mobile/yotta/bin/activate
	@false
endif

build: check
	@yotta build

install: check
	@cp build/bbc-microbit-classic-gcc/source/microbit-samples-combined.hex /media/syllicium/MICROBIT/
	@echo "Install done"

clean: check
	@yotta clean
	@echo "Cleaning done"
