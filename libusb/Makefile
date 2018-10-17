SUBDIRS = blink controlusb

all:
	for dir in $(SUBDIRS) ; do make -C $$dir all ; done

clean:
	for dir in $(SUBDIRS) ; do make -C $$dir clean ; done

clobber:
	for dir in $(SUBDIRS) ; do make -C $$dir clobber ; done
	find . -name '*.t' -exec rm -f {} \;

