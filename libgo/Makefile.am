# Makefile.am -- Go library Makefile.

# Copyright 2009 The Go Authors. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

# Process this file with autoreconf to produce Makefile.in.

# Go support.
SUFFIXES = .c .go .gox .o .obj .lo .a

MAINT_CHARSET = latin1

mkinstalldirs = $(SHELL) $(toplevel_srcdir)/mkinstalldirs
PWD_COMMAND = $${PWDCMD-pwd}
STAMP = echo timestamp >

toolexecdir = $(glibgo_toolexecdir)
toolexeclibdir = $(glibgo_toolexeclibdir)

LIBFFI = @LIBFFI@
LIBFFIINCS = @LIBFFIINCS@

WARN_CFLAGS = $(WARN_FLAGS) $(WERROR)

# -I/-D flags to pass when compiling.
AM_CPPFLAGS = -I $(srcdir)/runtime $(LIBFFIINCS)

ACLOCAL_AMFLAGS = -I . -I .. -I ../config

AM_CFLAGS = -fplan9-extensions -fsplit-stack $(WARN_CFLAGS)

# Multilib support.
MAKEOVERRIDES=

# Work around what appears to be a GNU make  handling MAKEFLAGS
# values defined in terms of make variables, as is the case for CC and
# friends when we are called from the top level Makefile.
AM_MAKEFLAGS = \
	"AR_FLAGS=$(AR_FLAGS)" \
	"CC_FOR_BUILD=$(CC_FOR_BUILD)" \
	"CC_FOR_TARGET=$(CC_FOR_TARGET)" \
	"CFLAGS=$(CFLAGS)" \
	"CXXFLAGS=$(CXXFLAGS)" \
	"CFLAGS_FOR_BUILD=$(CFLAGS_FOR_BUILD)" \
	"CFLAGS_FOR_TARGET=$(CFLAGS_FOR_TARGET)" \
	"GCCGO_FOR_TARGET=$(GCCGO_FOR_TARGET)" \
	"GOFLAGS=$(GOFLAGS)" \
	"INSTALL=$(INSTALL)" \
	"INSTALL_DATA=$(INSTALL_DATA)" \
	"INSTALL_PROGRAM=$(INSTALL_PROGRAM)" \
	"INSTALL_SCRIPT=$(INSTALL_SCRIPT)" \
	"LDFLAGS=$(LDFLAGS)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"LIBCFLAGS_FOR_TARGET=$(LIBCFLAGS_FOR_TARGET)" \
	"MAKE=$(MAKE)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)" \
	"PICFLAG=$(PICFLAG)" \
	"PICFLAG_FOR_TARGET=$(PICFLAG_FOR_TARGET)" \
	"SHELL=$(SHELL)" \
	"RUNTESTFLAGS=$(RUNTESTFLAGS)" \
	"exec_prefix=$(exec_prefix)" \
	"infodir=$(infodir)" \
	"libdir=$(libdir)" \
	"includedir=$(includedir)" \
	"prefix=$(prefix)" \
	"tooldir=$(tooldir)" \
	"gxx_include_dir=$(gxx_include_dir)" \
	"AR=$(AR)" \
	"AS=$(AS)" \
	"LD=$(LD)" \
	"RANLIB=$(RANLIB)" \
	"NM=$(NM)" \
	"NM_FOR_BUILD=$(NM_FOR_BUILD)" \
	"NM_FOR_TARGET=$(NM_FOR_TARGET)" \
	"DESTDIR=$(DESTDIR)" \
	"WERROR=$(WERROR)"

# Subdir rules rely on $(FLAGS_TO_PASS)
FLAGS_TO_PASS = $(AM_MAKEFLAGS)

toolexeclib_LTLIBRARIES = libgo.la libgotesting.la
toolexeclib_LIBRARIES = libgobegin.a

toolexeclib_DATA = \
	asn1.gox \
	big.gox \
	bignum.gox \
	bufio.gox \
	bytes.gox \
	ebnf.gox \
	exec.gox \
	expvar.gox \
	flag.gox \
	fmt.gox \
	gob.gox \
	hash.gox \
	http.gox \
	image.gox \
	io.gox \
	json.gox \
	log.gox \
	malloc.gox \
	math.gox \
	net.gox \
	once.gox \
	os.gox \
	path.gox \
	rand.gox \
	reflect.gox \
	regexp.gox \
	rpc.gox \
	runtime.gox \
	sort.gox \
	strconv.gox \
	strings.gox \
	sync.gox \
	syscall.gox \
	tabwriter.gox \
	template.gox \
	testing.gox \
	time.gox \
	unicode.gox \
	utf8.gox \
	xml.gox

toolexeclibarchivedir = $(toolexeclibdir)/archive

toolexeclibarchive_DATA = \
	archive/tar.gox

toolexeclibcompressdir = $(toolexeclibdir)/compress

toolexeclibcompress_DATA = \
	compress/flate.gox \
	compress/gzip.gox \
	compress/zlib.gox

toolexeclibcontainerdir = $(toolexeclibdir)/container

toolexeclibcontainer_DATA = \
	container/heap.gox \
	container/list.gox \
	container/ring.gox \
	container/vector.gox

toolexeclibcryptodir = $(toolexeclibdir)/crypto

toolexeclibcrypto_DATA = \
	crypto/aes.gox \
	crypto/block.gox \
	crypto/hmac.gox \
	crypto/md5.gox \
	crypto/rc4.gox \
	crypto/rsa.gox \
	crypto/sha1.gox \
	crypto/subtle.gox \
	crypto/tls.gox \
	crypto/x509.gox

toolexeclibdebugdir = $(toolexeclibdir)/debug

toolexeclibdebug_DATA = \
	debug/dwarf.gox \
	debug/elf.gox \
	debug/gosym.gox \
	debug/macho.gox \
	debug/proc.gox

toolexeclibencodingdir = $(toolexeclibdir)/encoding

toolexeclibencoding_DATA = \
	encoding/ascii85.gox \
	encoding/base64.gox \
	encoding/binary.gox \
	encoding/git85.gox \
	encoding/hex.gox \
	encoding/pem.gox

toolexeclibexpdir = $(toolexeclibdir)/exp

toolexeclibexp_DATA = \
	exp/datafmt.gox \
	exp/eval.gox \
	exp/iterable.gox

toolexeclibgodir = $(toolexeclibdir)/go

toolexeclibgo_DATA = \
	go/ast.gox \
	go/doc.gox \
	go/parser.gox \
	go/printer.gox \
	go/scanner.gox \
	go/token.gox

toolexeclibhashdir = $(toolexeclibdir)/hash

toolexeclibhash_DATA = \
	hash/adler32.gox \
	hash/crc32.gox

toolexeclibimagedir = $(toolexeclibdir)/image

toolexeclibimage_DATA = \
	image/png.gox

toolexeclibtestingdir = $(toolexeclibdir)/testing
toolexeclibtesting_DATA = \
	testing/iotest.gox \
	testing/quick.gox \
	testing/script.gox

runtime_files = \
	runtime/go-bad-index.c \
	runtime/go-byte-array-to-string.c \
	runtime/go-breakpoint.c \
	runtime/go-caller.c \
	runtime/go-can-convert-interface.c \
	runtime/go-chan-cap.c \
	runtime/go-chan-len.c \
	runtime/go-close.c \
	runtime/go-closed.c \
	runtime/go-construct-map.c \
	runtime/go-convert-interface.c \
	runtime/go-defer.c \
	runtime/go-go.c \
	runtime/go-goexit.c \
	runtime/go-int-array-to-string.c \
	runtime/go-int-to-string.c \
	runtime/go-interface-compare.c \
	runtime/go-interface-to-object.c \
	runtime/go-interface-to-pointer.c \
	runtime/go-lock-os-thread.c \
	runtime/go-map-delete.c \
	runtime/go-map-index.c \
	runtime/go-map-len.c \
	runtime/go-map-range.c \
	runtime/go-new-channel.c \
	runtime/go-new-interface-object.c \
	runtime/go-new-interface-pointer.c \
	runtime/go-new-map.c \
	runtime/go-new.c \
	runtime/go-panic.c \
	runtime/go-print.c \
	runtime/go-rec-big.c \
	runtime/go-rec-nb-big.c \
	runtime/go-rec-nb-small.c \
	runtime/go-rec-small.c \
	runtime/go-refcount-decrement.c \
	runtime/go-refcount-flush.c \
	runtime/go-refcount-increment.c \
	runtime/go-refcount-msg.c \
	runtime/go-reflect.c \
	runtime/go-reflect-call.c \
	runtime/go-reflect-chan.c \
	runtime/go-reflect-map.c \
	runtime/go-rune.c \
	runtime/go-sched.c \
	runtime/go-select.c \
	runtime/go-semacquire.c \
	runtime/go-send-big.c \
	runtime/go-send-nb-big.c \
	runtime/go-send-nb-small.c \
	runtime/go-send-small.c \
	runtime/go-strcmp.c \
	runtime/go-string-range.c \
	runtime/go-strplus.c \
	runtime/go-strslice.c \
	runtime/go-trampoline.c \
	runtime/go-type-error.c \
	runtime/go-type-identity.c \
	runtime/go-type-interface.c \
	runtime/go-type-string.c \
	runtime/go-typedesc-equal.c \
	runtime/go-unreflect.c \
	runtime/go-unsafe-pointer.c \
	runtime/malloc.c \
	runtime/mcache.c \
	runtime/mcentral.c \
	runtime/mfixalloc.c \
	runtime/mheap.c \
	runtime/msize.c \
	runtime/proc.c \
	malloc_go.c

cgo2c.o: runtime/cgo2c.c
	$(CC_FOR_BUILD) -c $(CFLAGS_FOR_BUILD) $<

cgo2c: cgo2c.o
	$(CC_FOR_BUILD) $(CFLAGS_FOR_BUILD) $(LDFLAGS_FOR_BUILD) -o $@ $<

%.c: $(srcdir)/runtime/%.cgo cgo2c
	./cgo2c --gcc < $< > $@.tmp
	mv -f $@.tmp $@

BUILT_SOURCES = malloc_go.c

go_asn1_files = \
	go/asn1/asn1.go

go_big_files = \
	go/big/arith.go \
	go/big/int.go \
	go/big/nat.go

go_bignum_files = \
	go/bignum/arith.go \
	go/bignum/bignum.go \
	go/bignum/integer.go \
	go/bignum/rational.go

go_bufio_files = \
	go/bufio/bufio.go

go_bytes_files = \
	go/bytes/buffer.go \
	go/bytes/bytes.go

go_ebnf_files = \
	go/ebnf/ebnf.go \
	go/ebnf/parser.go

go_exec_files = \
	go/exec/exec.go

go_expvar_files = \
	go/expvar/expvar.go

go_flag_files = \
	go/flag/flag.go

go_fmt_files = \
	go/fmt/format.go \
	go/fmt/print.go

go_gob_files = \
	go/gob/decode.go \
	go/gob/decoder.go \
	go/gob/encode.go \
	go/gob/encoder.go \
	go/gob/type.go

go_hash_files = \
	go/hash/hash.go

go_http_files = \
	go/http/client.go \
	go/http/fs.go \
	go/http/request.go \
	go/http/server.go \
	go/http/status.go \
	go/http/url.go

go_image_files = \
	go/image/color.go \
	go/image/image.go

go_io_files = \
	go/io/io.go \
	go/io/pipe.go \
	go/io/utils.go

go_json_files = \
	go/json/generic.go \
	go/json/parse.go \
	go/json/struct.go

go_log_files = \
	go/log/log.go

go_malloc_files = \
	go/malloc/malloc.go

go_math_files = \
	go/math/asin.go \
	go/math/atan.go \
	go/math/atan2.go \
	go/math/bits.go \
	go/math/const.go \
	go/math/exp.go \
	go/math/fabs.go \
	go/math/floor.go \
	go/math/fmod.go \
	go/math/hypot.go \
	go/math/log.go \
	go/math/pow.go \
	go/math/pow10.go \
	go/math/sin.go \
	go/math/sinh.go \
	go/math/sqrt.go \
	go/math/tan.go \
	go/math/tanh.go \
	go/math/unsafe.go

go_net_files = \
	go/net/dnsclient.go \
	go/net/dnsconfig.go \
	go/net/dnsmsg.go \
	go/net/fd.go \
	go/net/fd_linux.go \
	go/net/ip.go \
	go/net/ipsock.go \
	go/net/net.go \
	go/net/parse.go \
	go/net/port.go \
	go/net/sock.go \
	go/net/tcpsock.go \
	go/net/udpsock.go \
	go/net/unixsock.go

go_once_files = \
	go/once/once.go

go_os_files = \
	go/os/dir.go \
	go/os/env.go \
	go/os/error.go \
	go/os/exec.go \
	go/os/file.go \
	go/os/getwd.go \
	go/os/path.go \
	go/os/proc.go \
	go/os/stat.go \
	go/os/sys_linux.go \
	go/os/time.go \
	go/os/types.go

go_path_files = \
	go/path/path.go

go_rand_files = \
	go/rand/exp.go \
	go/rand/normal.go \
	go/rand/rand.go \
	go/rand/rng.go

go_reflect_files = \
	go/reflect/deepequal.go \
	go/reflect/type.go \
	go/reflect/value.go

go_regexp_files = \
	go/regexp/regexp.go

go_rpc_files = \
	go/rpc/client.go \
	go/rpc/debug.go \
	go/rpc/server.go

go_runtime_files = \
	go/runtime/extern.go \
	go/runtime/type.go

go_sort_files = \
	go/sort/sort.go

go_strconv_files = \
	go/strconv/atof.go \
	go/strconv/atoi.go \
	go/strconv/decimal.go \
	go/strconv/ftoa.go \
	go/strconv/itoa.go \
	go/strconv/quote.go

go_strings_files = \
	go/strings/reader.go \
	go/strings/strings.go

go_sync_files = \
	go/sync/cas.c \
	go/sync/mutex.go

go_tabwriter_files = \
	go/tabwriter/tabwriter.go

go_template_files = \
	go/template/format.go \
	go/template/template.go

go_testing_files = \
	go/testing/regexp.go \
	go/testing/testing.go

go_time_files = \
	go/time/sleep.go \
	go/time/tick.go \
	go/time/time.go \
	go/time/zoneinfo.go

go_unicode_files = \
	go/unicode/digit.go \
	go/unicode/letter.go \
	go/unicode/tables.go

go_utf8_files = \
	go/utf8/utf8.go

go_xml_files = \
	go/xml/read.go \
	go/xml/xml.go

go_archive_tar_files = \
	go/archive/tar/common.go \
	go/archive/tar/reader.go \
	go/archive/tar/writer.go

go_compress_flate_files = \
	go/compress/flate/deflate.go \
	go/compress/flate/huffman_bit_writer.go \
	go/compress/flate/huffman_code.go \
	go/compress/flate/inflate.go \
	go/compress/flate/reverse_bits.go \
	go/compress/flate/token.go \
	go/compress/flate/util.go

go_compress_gzip_files = \
	go/compress/gzip/gunzip.go

go_compress_zlib_files = \
	go/compress/zlib/reader.go \
	go/compress/zlib/writer.go

go_container_heap_files = \
	go/container/heap/heap.go

go_container_list_files = \
	go/container/list/list.go

go_container_ring_files = \
	go/container/ring/ring.go

go_container_vector_files = \
	go/container/vector/intvector.go \
	go/container/vector/stringvector.go \
	go/container/vector/vector.go

go_crypto_aes_files = \
	go/crypto/aes/block.go \
	go/crypto/aes/cipher.go \
	go/crypto/aes/const.go
go_crypto_block_files = \
	go/crypto/block/cbc.go \
	go/crypto/block/cfb.go \
	go/crypto/block/cmac.go \
	go/crypto/block/cipher.go \
	go/crypto/block/ctr.go \
	go/crypto/block/eax.go \
	go/crypto/block/ecb.go \
	go/crypto/block/ofb.go \
	go/crypto/block/xor.go
go_crypto_hmac_files = \
	go/crypto/hmac/hmac.go
go_crypto_md5_files = \
	go/crypto/md5/md5.go \
	go/crypto/md5/md5block.go
go_crypto_rc4_files = \
	go/crypto/rc4/rc4.go
go_crypto_rsa_files = \
	go/crypto/rsa/pkcs1v15.go \
	go/crypto/rsa/rsa.go
go_crypto_sha1_files = \
	go/crypto/sha1/sha1.go \
	go/crypto/sha1/sha1block.go
go_crypto_subtle_files = \
	go/crypto/subtle/constant_time.go
go_crypto_tls_files = \
	go/crypto/tls/alert.go \
	go/crypto/tls/common.go \
	go/crypto/tls/handshake_messages.go \
	go/crypto/tls/handshake_server.go \
	go/crypto/tls/prf.go \
	go/crypto/tls/record_process.go \
	go/crypto/tls/record_read.go \
	go/crypto/tls/record_write.go \
	go/crypto/tls/tls.go
go_crypto_x509_files = \
	go/crypto/x509/x509.go

go_debug_dwarf_files = \
	go/debug/dwarf/buf.go \
	go/debug/dwarf/const.go \
	go/debug/dwarf/entry.go \
	go/debug/dwarf/open.go \
	go/debug/dwarf/type.go \
	go/debug/dwarf/unit.go
go_debug_elf_files = \
	go/debug/elf/elf.go \
	go/debug/elf/file.go
go_debug_gosym_files = \
	go/debug/gosym/pclntab.go \
	go/debug/gosym/symtab.go
go_debug_macho_files = \
	go/debug/macho/file.go \
	go/debug/macho/macho.go

if LIBGO_IS_LINUX
proc_file = go/debug/proc/proc_linux.go
if LIBGO_IS_386
regs_file = go/debug/proc/regs_linux_386.go
else
if LIBGO_IS_X86_64
regs_file = go/debug/proc/regs_linux_amd64.go
else
regs_file =
endif
endif
else
if LIBGO_IS_DARWIN
proc_file = go/debug/proc/proc_darwin.go
if LIBGO_IS_386
regs_file = go/debug/proc/regs_darwin_386.go
else
if LIBGO_IS_X86_64
regs_file = go/debug/proc/regs_darwin_amd64.go
else
regs_file =
endif
endif
else
proc_file =
regs_file =
endif
endif

go_debug_proc_files = \
	go/debug/proc/proc.go \
	$(proc_file) \
	$(regs_file)

go_encoding_ascii85_files = \
	go/encoding/ascii85/ascii85.go
go_encoding_base64_files = \
	go/encoding/base64/base64.go
go_encoding_binary_files = \
	go/encoding/binary/binary.go
go_encoding_git85_files = \
	go/encoding/git85/git.go
go_encoding_hex_files = \
	go/encoding/hex/hex.go
go_encoding_pem_files = \
	go/encoding/pem/pem.go

go_exp_datafmt_files = \
	go/exp/datafmt/datafmt.go \
	go/exp/datafmt/parser.go
go_exp_eval_files = \
	go/exp/eval/abort.go \
	go/exp/eval/bridge.go \
	go/exp/eval/compiler.go \
	go/exp/eval/expr.go \
	go/exp/eval/expr1.go \
	go/exp/eval/func.go \
	go/exp/eval/scope.go \
	go/exp/eval/stmt.go \
	go/exp/eval/type.go \
	go/exp/eval/typec.go \
	go/exp/eval/util.go \
	go/exp/eval/value.go \
	go/exp/eval/world.go
go_exp_iterable_files = \
	go/exp/iterable/iterable.go

go_go_ast_files = \
	go/go/ast/ast.go \
	go/go/ast/filter.go \
	go/go/ast/scope.go \
	go/go/ast/walk.go
go_go_doc_files = \
	go/go/doc/comment.go \
	go/go/doc/doc.go
go_go_parser_files = \
	go/go/parser/interface.go \
	go/go/parser/parser.go
go_go_printer_files = \
	go/go/printer/nodes.go \
	go/go/printer/printer.go
go_go_scanner_files = \
	go/go/scanner/errors.go \
	go/go/scanner/scanner.go
go_go_token_files = \
	go/go/token/token.go

go_hash_adler32_files = \
	go/hash/adler32/adler32.go
go_hash_crc32_files = \
	go/hash/crc32/crc32.go

go_image_png_files = \
	go/image/png/reader.go \
	go/image/png/writer.go

go_testing_iotest_files = \
	go/testing/iotest/logger.go \
	go/testing/iotest/reader.go \
	go/testing/iotest/writer.go
go_testing_quick_files = \
	go/testing/quick/quick.go
go_testing_script_files = \
	go/testing/script/script.go

if LIBGO_IS_LINUX
syscall_os_file = syscalls/syscall_linux.go
if LIBGO_IS_386
syscall_arch_file = syscalls/syscall_linux_386.go
else
if LIBGO_IS_X86_64
syscall_arch_file = syscalls/syscall_linux_amd64.go
else
syscall_arch_file =
endif
endif
else
syscall_os_file =
syscall_arch_file =
endif

go_syscall_files = \
	syscalls/errstr.go \
	syscalls/exec.go \
	syscalls/socket.go \
	syscalls/socket_epoll.go \
	syscalls/syscall.go \
	$(syscall_os_file) \
	$(syscall_arch_file) \
	syscalls/sysfile.go \
	sysinfo.go

libgo_go_objs = \
	asn1/libasn1.la \
	big/libbig.la \
	bignum/libbignum.la \
	bufio/libbufio.la \
	bytes/libbytes.la \
	ebnf/libebnf.la \
	exec/libexec.la \
	expvar/libexpvar.la \
	flag/libflag.la \
	fmt/libfmt.la \
	gob/libgob.la \
	hash/libhash.la \
	http/libhttp.la \
	image/libimage.la \
	io/libio.la \
	json/libjson.la \
	log/liblog.la \
	malloc/libmalloc.la \
	math/libmath.la \
	net/libnet.la \
	once/libonce.la \
	os/libos.la \
	path/libpath.la \
	rand/librand.la \
	reflect/libreflect.la \
	regexp/libregexp.la \
	rpc/librpc.la \
	runtime/libruntime.la \
	sort/libsort.la \
	strconv/libstrconv.la \
	strings/libstrings.la \
	sync/libsync.la \
	tabwriter/libtabwriter.la \
	template/libtemplate.la \
	time/libtime.la \
	unicode/libunicode.la \
	utf8/libutf8.la \
	xml/libxml.la \
	archive/libtar.la \
	compress/libflate.la \
	compress/libgzip.la \
	compress/libzlib.la \
	container/libheap.la \
	container/liblist.la \
	container/libring.la \
	container/libvector.la \
	crypto/libaes.la \
	crypto/libblock.la \
	crypto/libhmac.la \
	crypto/libmd5.la \
	crypto/librc4.la \
	crypto/librsa.la \
	crypto/libsha1.la \
	crypto/libsubtle.la \
	crypto/libtls.la \
	crypto/libx509.la \
	debug/libdwarf.la \
	debug/libelf.la \
	debug/libgosym.la \
	debug/libmacho.la \
	debug/libproc.la \
	encoding/libascii85.la \
	encoding/libbase64.la \
	encoding/libbinary.la \
	encoding/libgit85.la \
	encoding/libhex.la \
	encoding/libpem.la \
	exp/libdatafmt.la \
	exp/libeval.la \
	exp/libiterable.la \
	go/libast.la \
	go/libdoc.la \
	go/libparser.la \
	go/libprinter.la \
	go/libscanner.la \
	go/libtoken.la \
	hash/libadler32.la \
	hash/libcrc32.la \
	image/libpng.la \
	libsyscall.la

libgo_la_SOURCES = $(runtime_files)

libgo_la_LIBADD = $(libgo_go_objs) $(LIBFFI) -lpthread

libgobegin_a_SOURCES = \
	runtime/go-main.c

libgotesting_la_SOURCES =
libgotesting_la_LIBADD = \
	testing/libtesting.la \
	testing/libiotest.la \
	testing/libquick.la \
	testing/libscript.la

LTLDFLAGS = $(shell $(SHELL) $(top_srcdir)/../libtool-ldflags $(LDFLAGS))

GOFLAGS = $(CFLAGS)
GOCOMPILE = $(GCCGO) $(DEFAULT_INCLUDES) $(INCLUDES) $(GOFLAGS)

LTGOCOMPILE = $(LIBTOOL) --tag GO --mode=compile $(GCCGO) $(INCLUDES) \
	$(AM_GOFLAGS) $(GOFLAGS)

GOLINK = $(LIBTOOL) --tag GO --mode-link $(GCCGO) \
	$(OPT_LDFLAGS) $(SECTION_LDFLAGS) $(AM_GOFLAGS) $(LTLDFLAGS) \
	-lpthread -o $@

GOBUILD_FLAGS = \
	--go-compiler $(LTGOCOMPILE) -L $$dir -c -o \; \
	--c-compiler $(LTCOMPILE) -c -o \; \
	--packager $(AR) rc \; \
	--output-ext ".$(OBJEXT)"

# Build a package.
BUILDARCHIVE = \
	rm -f `echo $@ | sed -e 's|/lib|/|' -e 's/\.a/.gox/'`; \
	test -d $(@D) || mkdir -p $(@D); \
	rm -f $@; \
	files=`echo $^ | sed -e 's/[^ ]*\.gox//g'`; \
	if $(LTGOCOMPILE) -c -o $@.o $$files; then \
	  $(AR) rc $@ $@.o; \
	else exit 1; fi

# Build a .la file from a .a file.
.a.la:
	$(LINK) $<.lo

asn1/libasn1.a: $(go_asn1_files) fmt.gox os.gox reflect.gox strconv.gox \
		strings.gox time.gox
	$(BUILDARCHIVE)
asn1/libasn1.la: asn1/libasn1.a

big/libbig.a: $(go_big_files)
	$(BUILDARCHIVE)
big/libbig.la: big/libbig.a

bignum/libbignum.a: $(go_bignum_files) fmt.gox
	$(BUILDARCHIVE)
bignum/libbignum.la: bignum/libbignum.a

bufio/libbufio.a: $(go_bufio_files) io.gox os.gox strconv.gox utf8.gox
	$(BUILDARCHIVE)
bufio/libbufio.la: bufio/libbufio.a

bytes/libbytes.a: $(go_bytes_files) os.gox utf8.gox
	$(BUILDARCHIVE)
bytes/libbytes.la: bytes/libbytes.a

ebnf/libebnf.a: $(go_ebnf_files) container/vector.gox go/scanner.gox \
		go/token.gox os.gox strconv.gox unicode.gox utf8.gox
	$(BUILDARCHIVE)
ebnf/libebnf.la: ebnf/libebnf.a

exec/libexec.a: $(go_exec_files) os.gox strings.gox
	$(BUILDARCHIVE)
exec/libexec.la: exec/libexec.a

expvar/libexpvar.a: $(go_expvar_files) bytes.gox fmt.gox http.gox log.gox \
		strconv.gox sync.gox
	$(BUILDARCHIVE)
expvar/libexpvar.la: expvar/libexpvar.a

flag/libflag.a: $(go_flag_files) fmt.gox os.gox strconv.gox
	$(BUILDARCHIVE)
flag/libflag.al: flag/libflag.a

fmt/libfmt.a: $(go_fmt_files) io.gox os.gox reflect.gox strconv.gox
	$(BUILDARCHIVE)
fmt/libfmt.la: fmt/libfmt.a

gob/libgob.a: $(go_gob_files) bytes.gox fmt.gox io.gox math.gox os.gox \
		reflect.gox strings.gox sync.gox unicode.gox
	$(BUILDARCHIVE)
gob/libgob.la: gob/libgob.a

hash/libhash.a: $(go_hash_files) io.gox
	$(BUILDARCHIVE)
hash/libhash.la: hash/libhash.a

http/libhttp.a: $(go_http_files) bufio.gox bytes.gox container/vector.gox \
		fmt.gox io.gox log.gox net.gox os.gox path.gox strconv.gox \
		strings.gox utf8.gox
	$(BUILDARCHIVE)
http/libhttp.la: http/libhttp.a

image/libimage.a: $(go_image_files)
	$(BUILDARCHIVE)
image/libimage.la: image/libimage.a

io/libio.a: $(go_io_files) bytes.gox os.gox strings.gox sync.gox
	$(BUILDARCHIVE)
io/libio.la: io/libio.a

json/libjson.a: $(go_json_files) bytes.gox fmt.gox math.gox reflect.gox \
		strconv.gox strings.gox utf8.gox container/vector.gox
	$(BUILDARCHIVE)
json/libjson.la: json/libjson.a

log/liblog.a: $(go_log_files) fmt.gox io.gox runtime.gox os.gox time.gox
	$(BUILDARCHIVE)
log/liblog.la: log/liblog.a

malloc/libmalloc.a: $(go_malloc_files)
	$(BUILDARCHIVE)
malloc/libmalloc.la: malloc/libmalloc.a

math/libmath.a: $(go_math_files)
	$(BUILDARCHIVE)
math/libmath.la: math/libmath.a

net/libnet.a: $(go_net_files) fmt.gox io.gox once.gox os.gox reflect.gox \
		strconv.gox strings.gox sync.gox syscall.gox
	$(BUILDARCHIVE)
net/libnet.la: net/libnet.a

once/libonce.a: $(go_once_files) sync.gox
	$(BUILDARCHIVE)
once/libonce.la:once/libonce.a

os/libos.a: $(go_os_files) once.gox syscall.gox
	$(BUILDARCHIVE)
os/libos.la: os/libos.a

path/libpath.a: $(go_path_files) io.gox os.gox strings.gox
	$(BUILDARCHIVE)
path/libpath.la: path/libpath.a

rand/librand.a: $(go_rand_files) math.gox
	$(BUILDARCHIVE)
rand/librand.la: rand/librand.a

reflect/libreflect.a: $(go_reflect_files) runtime.gox strconv.gox
	$(BUILDARCHIVE)
reflect/libreflect.la: reflect/libreflect.a

regexp/libregexp.a: $(go_regexp_files) bytes.gox container/vector.gox io.gox \
		os.gox utf8.gox
	$(BUILDARCHIVE)
regexp/libregexp.la: regexp/libregexp.a

rpc/librpc.a: $(go_rpc_files) bufio.gox fmt.gox gob.gox http.gox io.gox \
		log.gox net.gox os.gox reflect.gox sort.gox strings.gox \
		strconv.gox sync.gox template.gox unicode.gox utf8.gox
	$(BUILDARCHIVE)
rpc/librpc.la: rpc/librpc.a

runtime/libruntime.a: $(go_runtime_files)
	$(BUILDARCHIVE)
runtime/libruntime.la: runtime/libruntime.a

sort/libsort.a: $(go_sort_files)
	$(BUILDARCHIVE)
sort/libsort.la: sort/libsort.a

strconv/libstrconv.a: $(go_strconv_files) bytes.gox math.gox os.gox \
		strings.gox unicode.gox utf8.gox
	$(BUILDARCHIVE)
strconv/libstrconv.la: strconv/libstrconv.a

strings/libstrings.a: $(go_strings_files) os.gox unicode.gox utf8.gox
	$(BUILDARCHIVE)
strings/libstrings.la: strings/libstrings.a

sync/libsync.a: $(go_sync_files) runtime.gox
	test -d sync || mkdir -p sync
	$(LTGOCOMPILE) -c -o sync/mutex.o $(srcdir)/go/sync/mutex.go
	$(LTCOMPILE) -c -o sync/cas.o $(srcdir)/go/sync/cas.c
	rm -f $@
	$(AR) rc $@ sync/mutex.o sync/cas.o
sync/libsync.la: sync/libsync.a
	$(LINK) sync/mutex.lo sync/cas.lo

tabwriter/libtabwriter.a: $(go_tabwriter_files) bytes.gox io.gox os.gox \
		utf8.gox container/vector.gox
	$(BUILDARCHIVE)
tabwriter/libtabwriter.la: tabwriter/libtabwriter.a

template/libtemplate.a: $(go_template_files) bytes.gox fmt.gox io.gox os.gox \
		reflect.gox runtime.gox strings.gox container/vector.gox
	$(BUILDARCHIVE)
template/libtemplate.la: template/libtemplate.a

testing/libtesting.a: $(go_testing_files) flag.gox fmt.gox os.gox regexp.gox \
		runtime.gox
	$(BUILDARCHIVE)
testing/libtesting.la: testing/libtesting.a

time/libtime.a: $(go_time_files) io.gox once.gox os.gox syscall.gox
	$(BUILDARCHIVE)
time/libtime.la: time/libtime.a

unicode/libunicode.a: $(go_unicode_files)
	$(BUILDARCHIVE)
unicode/libunicode.la: unicode/libunicode.a

utf8/libutf8.a: $(go_utf8_files) unicode.gox
	$(BUILDARCHIVE)
utf8/libutf8.la: utf8/libutf8.a

xml/libxml.a: $(go_xml_files) bufio.gox bytes.gox io.gox os.gox reflect.gox \
		strconv.gox strings.gox unicode.gox utf8.gox
	$(BUILDARCHIVE)
xml/libxml.la: xml/libxml.a

archive/libtar.a: $(go_archive_tar_files) bytes.gox io.gox os.gox strconv.gox \
		strings.gox
	$(BUILDARCHIVE)
archive/libtar.la: archive/libtar.a

compress/libflate.a: $(go_compress_flate_files) bufio.gox bytes.gox io.gox \
		math.gox os.gox sort.gox strconv.gox
	$(BUILDARCHIVE)
compress/libflate.la: compress/libflate.a

compress/libgzip.a: $(go_compress_gzip_files) bufio.gox compress/flate.gox \
		hash.gox hash/crc32.gox io.gox os.gox
	$(BUILDARCHIVE)
compress/libgzip.la: compress/libgzip.a

compress/libzlib.a: $(go_compress_zlib_files) bufio.gox compress/flate.gox \
		hash.gox hash/adler32.gox io.gox os.gox
	$(BUILDARCHIVE)
compress/libzlib.al: compress/libzlib.a

container/libheap.a: $(go_container_heap_files) sort.gox
	$(BUILDARCHIVE)
container/libheap.la: container/libheap.a

container/liblist.a: $(go_container_list_files)
	$(BUILDARCHIVE)
container/liblist.la: container/liblist.a

container/libring.a: $(go_container_ring_files)
	$(BUILDARCHIVE)
container/libring.la: container/libring.a

container/libvector.a: $(go_container_vector_files)
	$(BUILDARCHIVE)
container/libvector.la: container/libvector.a

crypto/libaes.a: $(go_crypto_aes_files) os.gox strconv.gox
	$(BUILDARCHIVE)
crypto/libaes.la: crypto/libaes.a

crypto/libblock.a: $(go_crypto_block_files) fmt.gox io.gox os.gox strconv.gox
	$(BUILDARCHIVE)
crypto/libblock.la: crypto/libblock.a

crypto/libhmac.a: $(go_crypto_hmac_files) crypto/md5.gox crypto/sha1.gox \
		hash.gox os.gox
	$(BUILDARCHIVE)
crypto/libhmac.la: crypto/libhmac.a

crypto/libmd5.a: $(go_crypto_md5_files) hash.gox os.gox
	$(BUILDARCHIVE)
crypto/libmd5.la: crypto/libmd5.a

crypto/librc4.a: $(go_crypto_rc4_files) os.gox strconv.gox
	$(BUILDARCHIVE)
crypto/librc4.la: crypto/librc4.a

crypto/librsa.a: $(go_crypto_rsa_files) big.gox bytes.gox crypto/subtle.gox \
		hash.gox io.gox os.gox
	$(BUILDARCHIVE)
crypto/librsa.la: crypto/librsa.a

crypto/libsha1.a: $(go_crypto_sha1_files) hash.gox os.gox
	$(BUILDARCHIVE)
crypto/libsha1.la: crypto/libsha1.a

crypto/libsubtle.a: $(go_crypto_subtle_files)
	$(BUILDARCHIVE)
crypto/libsubtle.la: crypto/libsubtle.a

crypto/libtls.a: $(go_crypto_tls_files) bufio.gox bytes.gox \
		container/list.gox crypto/hmac.gox crypto/md5.gox \
		crypto/rc4.gox crypto/rsa.gox crypto/sha1.gox \
		crypto/subtle.gox crypto/rsa.gox fmt.gox \
		hash.gox io.gox net.gox os.gox strings.gox time.gox
	$(BUILDARCHIVE)
crypto/libtls.la: crypto/libtls.a

crypto/libx509.a: $(go_crypto_x509_files) asn1.gox big.gox crypto/rsa.gox \
		os.gox
	$(BUILDARCHIVE)
crypto/libx509.la: crypto/libx509.a

debug/libdwarf.a: $(go_debug_dwarf_files) encoding/binary.gox os.gox \
		strconv.gox
	$(BUILDARCHIVE)
debug/libdwarf.la: debug/libdwarf.a

debug/libelf.a: $(go_debug_elf_files) bytes.gox debug/dwarf.gox \
		encoding/binary.gox fmt.gox io.gox os.gox strconv.gox
	$(BUILDARCHIVE)
debug/libelf.la: debug/libelf.a

debug/libgosym.a: $(go_debug_gosym_files) encoding/binary.gox fmt.gox os.gox \
		strconv.gox strings.gox
	$(BUILDARCHIVE)
debug/libgosym.la: debug/libgosym.a

debug/libmacho.a: $(go_debug_macho_files) bytes.gox debug/dwarf.gox \
		encoding/binary.gox fmt.gox io.gox os.gox strconv.gox
	$(BUILDARCHIVE)
debug/libmacho.la: debug/libmacho.a

debug/libproc.a: $(go_debug_proc_files) container/vector.gox fmt.gox io.gox \
		os.gox runtime.gox strconv.gox strings.gox sync.gox \
		syscall.gox
	$(BUILDARCHIVE)
debug/libproc.la: debug/libproc.a

encoding/libascii85.a: $(go_encoding_ascii85_files) bytes.gox io.gox os.gox \
		strconv.gox
	$(BUILDARCHIVE)
encoding/libascii85.la: encoding/libascii85.a

encoding/libbase64.a: $(go_encoding_base64_files) bytes.gox io.gox os.gox \
		strconv.gox
	$(BUILDARCHIVE)
encoding/libbase64.la: encoding/libbase64.a

encoding/libbinary.a: $(go_encoding_binary_files) io.gox math.gox os.gox \
		reflect.gox
	$(BUILDARCHIVE)
encoding/libbinary.la: encoding/libbinary.a

encoding/libgit85.a: $(go_encoding_git85_files) bytes.gox io.gox os.gox \
		strconv.gox
	$(BUILDARCHIVE)
encoding/libgit85.la: encoding/libgit85.a

encoding/libhex.a: $(go_encoding_hex_files) os.gox strconv.gox strings.gox
	$(BUILDARCHIVE)
encoding/libhex.la: encoding/libhex.a

encoding/libpem.a: $(go_encoding_pem_files) bytes.gox encoding/base64.gox \
		strings.gox
	$(BUILDARCHIVE)
encoding/libpem.la: encoding/libpem.a

exp/libdatafmt.a: $(go_exp_datafmt_files) bytes.gox container/vector.gox \
		fmt.gox go/scanner.gox go/token.gox io.gox os.gox reflect.gox \
		runtime.gox strconv.gox strings.gox
	$(BUILDARCHIVE)
exp/libdatafmt.la: exp/libdatafmt.a

exp/libeval.a: $(go_exp_eval_files) bignum.gox go/ast.gox go/parser.gox \
		go/scanner.gox go/token.gox fmt.gox log.gox strconv.gox \
		strings.gox os.gox reflect.gox runtime.gox sort.gox template.gox
	$(BUILDARCHIVE)
exp/libeval.la: exp/libeval.a

exp/libiterable.a: $(go_exp_iterable_files) container/vector.gox
	$(BUILDARCHIVE)
exp/libiterable.la: exp/libiterable.a

go/libast.a: $(go_go_ast_files) go/token.gox unicode.gox utf8.gox
	$(BUILDARCHIVE)
go/libast.la: go/libast.a

go/libdoc.a: $(go_go_doc_files) container/vector.gox fmt.gox go/ast.gox \
		go/token.gox io.gox once.gox regexp.gox sort.gox strings.gox \
		template.gox
	$(BUILDARCHIVE)
go/libdoc.la: go/libdoc.a

go/libparser.a: $(go_go_parser_files) bytes.gox container/vector.gox fmt.gox \
		go/ast.gox go/scanner.gox go/token.gox io.gox os.gox path.gox \
		strings.gox
	$(BUILDARCHIVE)
go/libparser.la: go/libparser.a

go/libprinter.a: $(go_go_printer_files) bytes.gox container/vector.gox fmt.gox \
		go/ast.gox go/token.gox io.gox os.gox reflect.gox runtime.gox \
		strings.gox tabwriter.gox
	$(BUILDARCHIVE)
go/libprinter.la: go/libprinter.a

go/libscanner.a: $(go_go_scanner_files) bytes.gox container/vector.gox fmt.gox \
		go/token.gox io.gox os.gox sort.gox strconv.gox unicode.gox \
		utf8.gox
	$(BUILDARCHIVE)
go/libscanner.la: go/libscanner.a

go/libtoken.a: $(go_go_token_files) fmt.gox strconv.gox
	$(BUILDARCHIVE)
go/libtoken.la: go/libtoken.a

hash/libadler32.a: $(go_hash_adler32_files) hash.gox os.gox
	$(BUILDARCHIVE)
hash/libadler32.la: hash/libadler32.a

hash/libcrc32.a: $(go_hash_crc32_files) hash.gox os.gox
	$(BUILDARCHIVE)
hash/libcrc32.la: hash/libcrc32.a

image/libpng.a: $(go_image_png_files) bufio.gox compress/zlib.gox hash.gox \
		hash/crc32.gox image.gox io.gox os.gox strconv.gox
	$(BUILDARCHIVE)
image/libpng.la: image/libpng.a

testing/libiotest.a: $(go_testing_iotest_files) io.gox log.gox os.gox
	$(BUILDARCHIVE)
testing/libiotest.la: testing/libiotest.a

testing/libquick.a: $(go_testing_quick_files) flag.gox fmt.gox math.gox \
		os.gox rand.gox reflect.gox strings.gox
	$(BUILDARCHIVE)
testing/libquick.la: testing/libquick.a

testing/libscript.a: $(go_testing_script_files) fmt.gox os.gox rand.gox \
		reflect.gox strings.gox
	$(BUILDARCHIVE)
testing/libscript.la: testing/libscript.a

sysinfo.go: $(srcdir)/mksysinfo.sh
	$(SHELL) $(srcdir)/mksysinfo.sh
libsyscall.a: $(go_syscall_files) sync.gox
	$(BUILDARCHIVE)
libsyscall.la: libsyscall.a

# How to build a .gox file from a .a file.
BUILDGOX = \
	set -e; \
	rm -f $@.o; \
	$(CC) -r -nostdlib -o $@.o -Wl,--whole-archive $<; \
	objcopy -j .go_export $@.o $@.tmp; \
	mv -f $@.tmp $@; \
	rm -f $@.o

asn1.gox: asn1/libasn1.a
	$(BUILDGOX)
big.gox: big/libbig.a
	$(BUILDGOX)
bignum.gox: bignum/libbignum.a
	$(BUILDGOX)
bufio.gox: bufio/libbufio.a
	$(BUILDGOX)
bytes.gox: bytes/libbytes.a
	$(BUILDGOX)
ebnf.gox: ebnf/libebnf.a
	$(BUILDGOX)
exec.gox: exec/libexec.a
	$(BUILDGOX)
expvar.gox: expvar/libexpvar.a
	$(BUILDGOX)
flag.gox: flag/libflag.a
	$(BUILDGOX)
fmt.gox: fmt/libfmt.a
	$(BUILDGOX)
gob.gox: gob/libgob.a
	$(BUILDGOX)
hash.gox: hash/libhash.a
	$(BUILDGOX)
http.gox: http/libhttp.a
	$(BUILDGOX)
image.gox: image/libimage.a
	$(BUILDGOX)
io.gox: io/libio.a
	$(BUILDGOX)
json.gox: json/libjson.a
	$(BUILDGOX)
log.gox: log/liblog.a
	$(BUILDGOX)
malloc.gox: malloc/libmalloc.a
	$(BUILDGOX)
math.gox: math/libmath.a
	$(BUILDGOX)
net.gox: net/libnet.a
	$(BUILDGOX)
once.gox: once/libonce.a
	$(BUILDGOX)
os.gox: os/libos.a
	$(BUILDGOX)
path.gox: path/libpath.a
	$(BUILDGOX)
rand.gox: rand/librand.a
	$(BUILDGOX)
reflect.gox: reflect/libreflect.a
	$(BUILDGOX)
regexp.gox: regexp/libregexp.a
	$(BUILDGOX)
rpc.gox: rpc/librpc.a
	$(BUILDGOX)
runtime.gox: runtime/libruntime.a
	$(BUILDGOX)
sort.gox: sort/libsort.a
	$(BUILDGOX)
strconv.gox: strconv/libstrconv.a
	$(BUILDGOX)
strings.gox: strings/libstrings.a
	$(BUILDGOX)
sync.gox: sync/libsync.a
	$(BUILDGOX)
syscall.gox: libsyscall.a
	$(BUILDGOX)
tabwriter.gox: tabwriter/libtabwriter.a
	$(BUILDGOX)
template.gox: template/libtemplate.a
	$(BUILDGOX)
testing.gox: testing/libtesting.a
	$(BUILDGOX)
time.gox: time/libtime.a
	$(BUILDGOX)
unicode.gox: unicode/libunicode.a
	$(BUILDGOX)
utf8.gox: utf8/libutf8.a
	$(BUILDGOX)
xml.gox: xml/libxml.a
	$(BUILDGOX)

archive/tar.gox: archive/libtar.a
	$(BUILDGOX)

compress/flate.gox: compress/libflate.a
	$(BUILDGOX)
compress/gzip.gox: compress/libgzip.a
	$(BUILDGOX)
compress/zlib.gox: compress/libzlib.a
	$(BUILDGOX)

container/heap.gox: container/libheap.a
	$(BUILDGOX)
container/list.gox: container/liblist.a
	$(BUILDGOX)
container/ring.gox: container/libring.a
	$(BUILDGOX)
container/vector.gox: container/libvector.a
	$(BUILDGOX)

crypto/aes.gox: crypto/libaes.a
	$(BUILDGOX)
crypto/block.gox: crypto/libblock.a
	$(BUILDGOX)
crypto/hmac.gox: crypto/libhmac.a
	$(BUILDGOX)
crypto/md5.gox: crypto/libmd5.a
	$(BUILDGOX)
crypto/rc4.gox: crypto/librc4.a
	$(BUILDGOX)
crypto/rsa.gox: crypto/librsa.a
	$(BUILDGOX)
crypto/sha1.gox: crypto/libsha1.a
	$(BUILDGOX)
crypto/subtle.gox: crypto/libsubtle.a
	$(BUILDGOX)
crypto/tls.gox: crypto/libtls.a
	$(BUILDGOX)
crypto/x509.gox: crypto/libx509.a
	$(BUILDGOX)

debug/dwarf.gox: debug/libdwarf.a
	$(BUILDGOX)
debug/elf.gox: debug/libelf.a
	$(BUILDGOX)
debug/gosym.gox: debug/libgosym.a
	$(BUILDGOX)
debug/macho.gox: debug/libmacho.a
	$(BUILDGOX)
debug/proc.gox: debug/libproc.a
	$(BUILDGOX)

encoding/ascii85.gox: encoding/libascii85.a
	$(BUILDGOX)
encoding/base64.gox: encoding/libbase64.a
	$(BUILDGOX)
encoding/binary.gox: encoding/libbinary.a
	$(BUILDGOX)
encoding/git85.gox: encoding/libgit85.a
	$(BUILDGOX)
encoding/hex.gox: encoding/libhex.a
	$(BUILDGOX)
encoding/pem.gox: encoding/libpem.a
	$(BUILDGOX)

exp/datafmt.gox: exp/libdatafmt.a
	$(BUILDGOX)
exp/eval.gox: exp/libeval.a
	$(BUILDGOX)
exp/iterable.gox: exp/libiterable.a
	$(BUILDGOX)

go/ast.gox: go/libast.a
	$(BUILDGOX)
go/doc.gox: go/libdoc.a
	$(BUILDGOX)
go/parser.gox: go/libparser.a
	$(BUILDGOX)
go/printer.gox: go/libprinter.a
	$(BUILDGOX)
go/scanner.gox: go/libscanner.a
	$(BUILDGOX)
go/token.gox: go/libtoken.a
	$(BUILDGOX)

hash/adler32.gox: hash/libadler32.a
	$(BUILDGOX)
hash/crc32.gox: hash/libcrc32.a
	$(BUILDGOX)

image/png.gox: image/libpng.a
	$(BUILDGOX)

testing/iotest.gox: testing/libiotest.a
	$(BUILDGOX)
testing/quick.gox: testing/libquick.a
	$(BUILDGOX)
testing/script.gox: testing/libscript.a
	$(BUILDGOX)

TEST_PACKAGES = \
	asn1 \
	big \
	bignum \
	bufio \
	bytes \
	ebnf \
	exec \
	expvar \
	flag \
	fmt \
	gob \
	http \
	io \
	json \
	log \
	math \
	net \
	once \
	os \
	path \
	rand \
	reflect \
	regexp \
	rpc \
	sort \
	strconv \
	strings \
	sync \
	tabwriter \
	template \
	testing \
	time \
	unicode \
	utf8 \
	xml \
	archive/tar \
	compress/flate \
	compress/gzip \
	compress/zlib \
	container/heap \
	container/list \
	container/ring \
	container/vector \
	crypto/aes \
	crypto/block \
	crypto/hmac \
	crypto/md5 \
	crypto/rc4 \
	crypto/rsa \
	crypto/sha1 \
	crypto/subtle \
	crypto/tls \
	crypto/x509 \
	debug/dwarf \
	debug/elf \
	debug/macho \
	encoding/ascii85 \
	encoding/base64 \
	encoding/binary \
	encoding/git85 \
	encoding/hex \
	encoding/pem \
	exp/datafmt \
	exp/eval \
	exp/iterable \
	go/parser \
	go/printer \
	go/scanner \
	hash/adler32 \
	hash/crc32 \
	image/png \
	testing/quick \
	testing/script

check-am: libgo.la libgobegin.a $(toolexeclib_DATA)
	x=0; \
	GC="$(GCCGO) -L `${PWD_COMMAND}` -L `${PWD_COMMAND}`/.libs \
		-Wl,-R,`${PWD_COMMAND}`/.libs"; \
	GOLIBS="-lgotesting"; \
	export GC GOLIBS; \
	for d in $(TEST_PACKAGES); do \
	  echo -n "$${d} "; \
	  $(srcdir)/testsuite/gotest --srcdir=$(srcdir)/go/$$d || x=1; \
	done; \
	exit $$x
