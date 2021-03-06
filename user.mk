HEADERS = \
	src/defs.h \
	src/io/defs.h \
	src/types/defs.h \
	\
	src/app/path.h \
	\
	src/debug/exception.h \
	src/debug/res.h \
	\
	src/fs/base.h \
	src/fs/path.h \
	\
	src/math/algo.h \
	src/math/float.h \
	src/math/func.h \
	src/math/rand.h \
	\
	src/mem/base.h \
	src/mem/manage.h \
	\
	src/io/chunk.h \
	src/io/conf.h \
	src/io/device.h \
	src/io/file.h \
	src/io/filter.h \
	src/io/input.h \
	src/io/iter.h \
	src/io/object.h \
	src/io/output.h \
	src/io/print.h \
	src/io/reader.h \
	src/io/scan.h \
	src/io/writer.h \
	\
	src/os/env.h \
	src/os/path.h \
	\
	src/string/base.h \
	src/string/io.h \
	src/string/locale.h \
	src/string/scan.h \
	\
	src/sys/lib.h \
	src/sys/proc.h \
	src/sys/sync.h \
	src/sys/time.h \
	\
	src/thread/base.h \
	src/thread/cond.h \
	src/thread/local.h \
	src/thread/lock.h \
	\
	src/types/avltree.h \
	src/types/avlitree.h \
	src/types/avljtree.h \
	src/types/compare.h \
	src/types/enum.h \
	src/types/filter.h \
	src/types/float.h \
	src/types/integer.h \
	src/types/iter.h \
	src/types/llist.h \
	src/types/queue.h \
	src/types/strbuf.h \
	src/types/type.h \
	src/types/value.h \

ifeq ($(bmake_HOST),windows)
else
	EXTRA = src/thread/posix/defs.h
endif

all: shim.h

shim.h: $(HEADERS) user.mk
	printf '#ifndef LIBSHIM_H\n#define LIBSHIM_H\n' > shim.h
	for file in $(EXTRA) $(HEADERS) ; do sed -e '1,/%shim.h%/d' -e '/%~shim.h%/,$$d' $$file >> shim.h ; done
	printf '#endif\n' >> shim.h

install: shim_h_install pkgconfig_install

shim_h_install: shim.h
	install --mode 0644 -D shim.h "$(PREFIX)/include/shim.h"

pkgconfig_install: shim.pc
	install --mode 0644 -D shim.pc "$(bmake_PATH_LIB)/pkgconfig/shim.pc"

clean: shim_h_clean

shim_h_clean:
	rm -f shim.h

.PHONY: shim_h_clean
