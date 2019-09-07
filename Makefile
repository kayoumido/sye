
.PHONY: so3 libc usr clean

all:	libc usr so3
	echo "Compilation done"

so3:
	cd so3 && make

libc:
	cd usr/libc/ && make

usr:
	@echo Compiling usr and deploying in the root filesystem ...
	cd usr && make && ./deploy.sh


clean:
	cd so3 && make clean
	cd usr/libc && make clean
	cd usr && make clean
