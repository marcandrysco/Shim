HEADERS = \
	  src/defs.h \
	  \
	  src/debug/exception.h \
	  src/debug/res.h \
	  \
	  src/mem/manage.h \
	  \
	  src/io/defs.h \
	  src/io/chunk.h \
	  src/io/device.h \
	  src/io/output.h \
	  src/io/print.h \
	  \
	  src/string/base.h \
	  src/string/io.h \
	  src/string/locale.h \
	  \
	  src/sys/proc.h \
	  src/sys/sync.h \
	  src/sys/time.h \
	  \
	  src/thread/base.h \
	  src/thread/cond.h \
	  src/thread/local.h \
	  src/thread/lock.h \
	  \
	  src/types/defs.h \
	  src/types/avltree.h \
	  src/types/compare.h \
	  src/types/llist.h \
	  src/types/strbuf.h \

ifeq ($(bmake_HOST),windows)
else
	EXTRA = src/thread/posix/defs.h
endif

all: shim.h

shim.h: $(HEADERS) user.mk
	printf '#ifndef LIBSCR_H\n#define LIBSCR_H\n' > shim.h
	for file in $(HEADERS) $(EXTRA) ; do sed -e '1,/%shim.h%/d' -e '/%~shim.h%/,$$d' $$file >> shim.h ; done
	printf '#endif\n' >> shim.h

install: shim_h_install

shim_h_install: shim.h
	install --mode 0644 -D shim.h "$(PREFIX)/include/shim.h"

clean: shim_h_clean

shim_h_clean:
	rm -f shim.h

.PHONY: shim_h_clean
