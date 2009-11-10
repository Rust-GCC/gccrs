// socket.go -- Socket handling.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Low-level socket interface.
// Only for implementing net package.
// DO NOT USE DIRECTLY.

package syscall

import "unsafe"

type RawSockaddrInet4 struct {
	Family uint16;
	Port uint16;
	Addr [4]byte /* in_addr */;
	Zero [8]uint8;
}
const SizeofSockaddrInet4 = 16

type RawSockaddrInet6 struct {
	Family uint16;
	Port uint16;
	Flowinfo uint32;
	Addr [16]byte /* in6_addr */;
	Scope_id uint32;
}
const SizeofSockaddrInet6 = 28

type RawSockaddrUnix struct {
	Family uint16;
	Path [108]int8;
}
const SizeofSockaddrUnix = 110

type RawSockaddr struct {
	Family uint16;
	Data [14]int8;
}

type RawSockaddrAny struct {
	Addr RawSockaddr;
	Pad [12]int8;
}
const SizeofSockaddrAny = 0x1c;

// For testing: clients can set this flag to force
// creation of IPv6 sockets to return EAFNOSUPPORT.
var SocketDisableIPv6 bool

type Sockaddr interface {
	sockaddr() (ptr *RawSockaddrAny, len Socklen_t, errno int);	// lowercase; only we can define Sockaddrs
}

type SockaddrInet4 struct {
	Port int;
	Addr [4]byte;
	raw RawSockaddrInet4;
}

func (sa *SockaddrInet4) sockaddr() (*RawSockaddrAny, Socklen_t, int) {
	if sa.Port < 0 || sa.Port > 0xFFFF {
		return nil, 0, EINVAL;
	}
	sa.raw.Family = AF_INET;
	p := (*[2]byte)(unsafe.Pointer(&sa.raw.Port));
	p[0] = byte(sa.Port>>8);
	p[1] = byte(sa.Port);
	for i := 0; i < len(sa.Addr); i++ {
		sa.raw.Addr[i] = sa.Addr[i];
	}
	return (*RawSockaddrAny)(unsafe.Pointer(&sa.raw)), SizeofSockaddrInet4, 0;
}

type SockaddrInet6 struct {
	Port int;
	Addr [16]byte;
	raw RawSockaddrInet6;
}

func (sa *SockaddrInet6) sockaddr() (*RawSockaddrAny, Socklen_t, int) {
	if sa.Port < 0 || sa.Port > 0xFFFF {
		return nil, 0, EINVAL;
	}
	sa.raw.Family = AF_INET6;
	p := (*[2]byte)(unsafe.Pointer(&sa.raw.Port));
	p[0] = byte(sa.Port>>8);
	p[1] = byte(sa.Port);
	for i := 0; i < len(sa.Addr); i++ {
		sa.raw.Addr[i] = sa.Addr[i];
	}
	return (*RawSockaddrAny)(unsafe.Pointer(&sa.raw)), SizeofSockaddrInet6, 0;
}

type SockaddrUnix struct {
	Name string;
	raw RawSockaddrUnix;
}

func (sa *SockaddrUnix) sockaddr() (*RawSockaddrAny, Socklen_t, int) {
	name := sa.Name;
	n := len(name);
	if n >= len(sa.raw.Path) || n == 0 {
		return nil, 0, EINVAL;
	}
	sa.raw.Family = AF_UNIX;
	for i := 0; i < n; i++ {
		sa.raw.Path[i] = int8(name[i]);
	}
	if sa.raw.Path[0] == '@' {
		sa.raw.Path[0] = 0;
	}

	// length is family, name, NUL.
	return (*RawSockaddrAny)(unsafe.Pointer(&sa.raw)), 1 + Socklen_t(n) + 1, 0;
}

func anyToSockaddr(rsa *RawSockaddrAny) (Sockaddr, int) {
	switch rsa.Addr.Family {
	case AF_UNIX:
		pp := (*RawSockaddrUnix)(unsafe.Pointer(rsa));
		sa := new(SockaddrUnix);
		if pp.Path[0] == 0 {
			// "Abstract" Unix domain socket.
			// Rewrite leading NUL as @ for textual display.
			// (This is the standard convention.)
			// Not friendly to overwrite in place,
			// but the callers below don't care.
			pp.Path[0] = '@';
		}

		// Assume path ends at NUL.
		// This is not technically the Linux semantics for
		// abstract Unix domain sockets--they are supposed
		// to be uninterpreted fixed-size binary blobs--but
		// everyone uses this convention.
		n := 0;
		for n < len(pp.Path) && pp.Path[n] != 0 {
			n++;
		}
		bytes := (*[len(pp.Path)]byte)(unsafe.Pointer(&pp.Path[0]));
		sa.Name = string(bytes[0:n]);
		return sa, 0;

	case AF_INET:
		pp := (*RawSockaddrInet4)(unsafe.Pointer(rsa));
		sa := new(SockaddrInet4);
		p := (*[2]byte)(unsafe.Pointer(&pp.Port));
		sa.Port = int(p[0])<<8 + int(p[1]);
		for i := 0; i < len(sa.Addr); i++ {
			sa.Addr[i] = pp.Addr[i];
		}
		return sa, 0;

	case AF_INET6:
		pp := (*RawSockaddrInet6)(unsafe.Pointer(rsa));
		sa := new(SockaddrInet6);
		p := (*[2]byte)(unsafe.Pointer(&pp.Port));
		sa.Port = int(p[0])<<8 + int(p[1]);
		for i := 0; i < len(sa.Addr); i++ {
			sa.Addr[i] = pp.Addr[i];
		}
		return sa, 0;
	}
	return nil, EAFNOSUPPORT;
}

type Linger struct {
	Onoff int32;
	Linger int32;
}

func libc_accept(fd int, sa *RawSockaddrAny, len *Socklen_t) int __asm__ ("accept");
func libc_bind(fd int, sa *RawSockaddrAny, len Socklen_t) int __asm__ ("bind");
func libc_connect(fd int, sa *RawSockaddrAny, len Socklen_t) int __asm__ ("connect");
func libc_socket(domain, typ, protocol int) int __asm__ ("socket");
func libc_setsockopt(fd, level, optname int, optval *byte, optlen Socklen_t)
     int __asm__ ("setsockopt");
func libc_listen(fd, backlog int) int __asm__ ("listen");
func libc_getsockopt(fd, level, optname int, optval *byte, optlen *Socklen_t)
     int __asm__ ("getsockopt");
func libc_getsockname(fd int, sa *RawSockaddrAny, len *Socklen_t) int __asm__ ("getsockname");
func libc_getpeername(fd int, sa *RawSockaddrAny, len *Socklen_t) int __asm__ ("getpeername");
func libc_recv(fd int, buf *byte, len Size_t, flags int) Ssize_t __asm__ ("recv");
func libc_recvfrom(fd int, buf *byte, len Size_t, flags int,
	from *RawSockaddrAny, fromlen *Socklen_t) Ssize_t __asm__("recvfrom");
func libc_send(fd int, buf *byte, len Size_t, flags int) Ssize_t __asm__("send");
func libc_sendto(fd int, buf *byte, len Size_t, flags int,
	to *RawSockaddrAny, tolen Socklen_t) Ssize_t __asm__("sendto");
func errno_location() *int __asm__ ("__errno_location");

func Accept(fd int) (nfd int, sa Sockaddr, errno int) {
	var rsa RawSockaddrAny;
	var len Socklen_t = SizeofSockaddrAny;
	nfd = libc_accept(fd, &rsa, &len);
	if nfd < 0 {
		errno = *errno_location();
		return;
	}
	sa, errno = anyToSockaddr(&rsa);
	if errno != 0 {
		Close(nfd);
		nfd = 0;
	}
	return;
}

func Bind(fd int, sa Sockaddr) (errno int) {
	ptr, n, err := sa.sockaddr();
	if err != 0 {
		return err;
	}
	if libc_bind(fd, ptr, n) < 0 {
		errno = *errno_location();
	}
	return;
}

func Connect(fd int, sa Sockaddr) (errno int) {
	ptr, n, err := sa.sockaddr();
	if err != 0 {
		return err;
	}
	if libc_connect(fd, ptr, n) < 0 {
		errno = *errno_location();
	}
	return;
}

func Socket(domain, typ, proto int) (fd, errno int) {
  if domain == AF_INET6 && SocketDisableIPv6 {
    return -1, EAFNOSUPPORT
  }
  fd = libc_socket(int(domain), int(typ), int(proto));
  if fd < 0 {
    errno = *errno_location();
  }
  return;
}

func Listen(fd int, n int) (errno int) {
  r := libc_listen(int(fd), int(n));
  if r < 0 { errno = *errno_location() }
  return;
}

func setsockopt(fd, level, opt int, valueptr uintptr, length Socklen_t) (errno int) {
  r := libc_setsockopt(fd, level, opt, (*byte)(unsafe.Pointer(valueptr)),
		       length);
  if r < 0 { errno = *errno_location() }
  return;
}

func SetsockoptInt(fd, level, opt int, value int) (errno int) {
	var n = int32(value);
	return setsockopt(fd, level, opt, uintptr(unsafe.Pointer(&n)), 4);
}

func SetsockoptTimeval(fd, level, opt int, tv *Timeval) (errno int) {
	return setsockopt(fd, level, opt, uintptr(unsafe.Pointer(tv)), Socklen_t(unsafe.Sizeof(*tv)));
}

func SetsockoptLinger(fd, level, opt int, l *Linger) (errno int) {
	return setsockopt(fd, level, opt, uintptr(unsafe.Pointer(l)), Socklen_t(unsafe.Sizeof(*l)));
}

func Getsockname(fd int) (sa Sockaddr, errno int) {
	var rsa RawSockaddrAny;
	var len Socklen_t = SizeofSockaddrAny;
	if libc_getsockname(fd, &rsa, &len) != 0 {
		errno = *errno_location();
		return;
	}
	return anyToSockaddr(&rsa);
}

func Getpeername(fd int) (sa Sockaddr, errno int) {
	var rsa RawSockaddrAny;
	var len Socklen_t = SizeofSockaddrAny;
	if libc_getpeername(fd, &rsa, &len) != 0 {
		errno = *errno_location();
		return;
	}
	return anyToSockaddr(&rsa);
}

func Recvfrom(fd int, p []byte, flags int) (n int, from Sockaddr, errno int) {
	var rsa RawSockaddrAny;
	var slen Socklen_t = SizeofSockaddrAny;
	var _p0 *byte;
	if len(p) > 0 { _p0 = &p[0]; }
	r := libc_recvfrom(fd, _p0, Size_t(len(p)), flags, &rsa, &slen);
	n = int(r);
	if r == -1 {
		errno = *errno_location();
	} else {
		from, errno = anyToSockaddr(&rsa);
	}
	return;
}

func Sendto(fd int, p []byte, flags int, to Sockaddr) (errno int) {
	ptr, n, err := to.sockaddr();
	if err != 0 {
		return err;
	}
	var _p0 *byte;
	if len(p) > 0 { _p0 = &p[0]; }
	r := libc_sendto(fd, _p0, Size_t(len(p)), flags, ptr, n);
	if r == -1 { errno = *errno_location(); }
	return;
}


// FIXME: No getsockopt.
