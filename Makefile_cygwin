#UNAME := $(shell uname)
#ifeq ($(UNAME), SunOS)
all:
	mkdir -p tmp
	cp src/* tmp
	for file in $(shell ls tmp) ; do \
		sed -i -e 's///g' tmp/$$file ; \
	done
	mkdir -p bin
	g++ -Wall -o bin/UCC -DUNIX -DCYGWIN tmp/*.cpp
	rm -rf tmp
#endif
#notSunOS:
	#mkdir -p bin
	#g++ -Wall -o bin/UCC -DUNIX src/*.cpp
clean:
	-rm -f bin/UCC bin/UCC.exe
	-rm -rf tmp
	-rmdir bin

