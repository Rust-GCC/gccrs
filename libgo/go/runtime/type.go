// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

/*
 * Runtime type representation.
 *
 * The following files know the exact layout of these
 * data structures and must be kept in sync with this file:
 *
 *	../../cmd/gc/reflect.c
 *	../reflect/type.go
 *	type.h
 */

package runtime

import "unsafe"

// Values which can appear in the commonType.code field.

const (
	BoolTypeCode		= 0;
	Float32TypeCode		= 1;
	Float64TypeCode		= 2;
	FloatTypeCode		= 3;
	Int16TypeCode		= 4;
	Int32TypeCode		= 5;
	Int64TypeCode		= 6;
	Int8TypeCode		= 7;
	IntTypeCode		= 8;
	Uint16TypeCode		= 9;
	Uint32TypeCode		= 10;
	Uint64TypeCode		= 11;
	Uint8TypeCode		= 12;
	UintTypeCode		= 13;
	StringTypeCode		= 14;
	UintptrTypeCode		= 15;
	DotDotDotTypeCode	= 16;
	UnsafePointerTypeCode	= 17;
	ArrayTypeCode		= 18;
	SliceTypeCode		= 19;
	ChanTypeCode		= 20;
	FuncTypeCode		= 21;
	InterfaceTypeCode	= 22;
	MapTypeCode		= 23;
	PtrTypeCode		= 24;
	StructTypeCode		= 25;
)

// All types begin with a few common fields needed for
// the interface runtime.
type commonType struct {
	Code		uint8;						// type code
	align		uint8;						// alignment of variable with this type
	fieldAlign	uint8;						// alignment of struct field with this type
	size		uintptr;					// size in bytes
	hash		func(unsafe.Pointer, uintptr);			// hash function
	equal		func(unsafe.Pointer, unsafe.Pointer, uintptr);	// equality function
	string		*string;					// string form; unnecessary  but undeniably useful
	*uncommonType;							// (relatively) uncommon fields
}

// Externally visible name.
type Type commonType

// Method on non-interface type
type method struct {
	hash	uint32;		// hash of name + pkg + typ
	name	*string;	// name of method
	pkgPath	*string;	// nil for exported Names; otherwise import path
	typ	*Type;		// .(*FuncType) underneath
	tfn	unsafe.Pointer;	// fn used for normal method call
}

// uncommonType is present only for types with names or methods
// (if T is a named type, the uncommonTypes for T and *T have methods).
// Using a pointer to this struct reduces the overall size required
// to describe an unnamed type with no methods.
type uncommonType struct {
	name	*string;	// name of type
	pkgPath	*string;	// import path; nil for built-in types like int, string
	methods	[]method;	// methods associated with type
}

// BoolType represents a boolean type.
type BoolType commonType

// Float32Type represents a float32 type.
type Float32Type commonType

// Float64Type represents a float64 type.
type Float64Type commonType

// FloatType represents a float type.
type FloatType commonType

// Int16Type represents an int16 type.
type Int16Type commonType

// Int32Type represents an int32 type.
type Int32Type commonType

// Int64Type represents an int64 type.
type Int64Type commonType

// Int8Type represents an int8 type.
type Int8Type commonType

// IntType represents an int type.
type IntType commonType

// Uint16Type represents a uint16 type.
type Uint16Type commonType

// Uint32Type represents a uint32 type.
type Uint32Type commonType

// Uint64Type represents a uint64 type.
type Uint64Type commonType

// Uint8Type represents a uint8 type.
type Uint8Type commonType

// UintType represents a uint type.
type UintType commonType

// StringType represents a string type.
type StringType commonType

// UintptrType represents a uintptr type.
type UintptrType commonType

// DotDotDotType represents the ... that can
// be used as the type of the final function parameter.
type DotDotDotType commonType

// UnsafePointerType represents an unsafe.Pointer type.
type UnsafePointerType commonType

// ArrayType represents a fixed array type.
type ArrayType struct {
	commonType;
	elem	*Type;	// array element type
	len	uintptr;
}

// SliceType represents a slice type.
type SliceType struct {
	commonType;
	elem	*Type;	// slice element type
}

// ChanDir represents a channel type's direction.
type ChanDir int

const (
	RecvDir		ChanDir	= 1 << iota;	// <-chan
	SendDir;		// chan<-
	BothDir			= RecvDir | SendDir;	// chan
)

// ChanType represents a channel type.
type ChanType struct {
	commonType;
	elem	*Type;		// channel element type
	dir	uintptr;	// channel direction (ChanDir)
}

// FuncType represents a function type.
type FuncType struct {
	commonType;
	in	[]*Type;	// input parameter types
	out	[]*Type;	// output parameter types
}

// Method on interface type
type imethod struct {
	hash	uint32;		// hash of name + pkg + typ; same hash as method
	name	*string;	// name of method
	pkgPath	*string;	// nil for exported Names; otherwise import path
	typ	*Type;		// .(*FuncType) underneath
}

// InterfaceType represents an interface type.
type InterfaceType struct {
	commonType;
	methods	[]imethod;	// sorted by hash
}

// MapType represents a map type.
type MapType struct {
	commonType;
	key	*Type;	// map key type
	elem	*Type;	// map element (value) type
}

// PtrType represents a pointer type.
type PtrType struct {
	commonType;
	elem	*Type;	// pointer element (pointed at) type
}

// Struct field
type structField struct {
	name	*string;	// nil for embedded fields
	pkgPath	*string;	// nil for exported Names; otherwise import path
	typ	*Type;		// type of field
	tag	*string;	// nil if no tag
	offset	uintptr;	// byte offset of field within struct
}

// StructType represents a struct type.
type StructType struct {
	commonType;
	fields	[]structField;	// sorted by offset
}

/*
 * Must match iface.c:/Itab and compilers.
 */
type Itable struct {
	Itype	*Type;	// (*tab.inter).(*InterfaceType) is the interface type
	Type	*Type;
	link	*Itable;
	bad	int32;
	unused	int32;
	Fn	[100000]uintptr;	// bigger than we'll ever see
}
