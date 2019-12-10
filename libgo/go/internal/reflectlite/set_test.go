// Copyright 2011 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package reflectlite_test

import (
	"bytes"
	"go/ast"
	"go/token"
	. "internal/reflectlite"
	"io"
	"testing"
)

func TestImplicitSetConversion(t *testing.T) {
	// Assume TestImplicitMapConversion covered the basics.
	// Just make sure conversions are being applied at all.
	var r io.Reader
	b := new(bytes.Buffer)
	rv := ValueOf(&r).Elem()
	rv.Set(ValueOf(b))
	if r != b {
		t.Errorf("after Set: r=%T(%v)", r, r)
	}
}

var implementsTests = []struct {
	x interface{}
	t interface{}
	b bool
}{
	{new(*bytes.Buffer), new(io.Reader), true},
	{new(bytes.Buffer), new(io.Reader), false},
	{new(*bytes.Buffer), new(io.ReaderAt), false},
	{new(*ast.Ident), new(ast.Expr), true},
	{new(*notAnExpr), new(ast.Expr), false},
	{new(*ast.Ident), new(notASTExpr), false},
	{new(notASTExpr), new(ast.Expr), false},
	{new(ast.Expr), new(notASTExpr), false},
	{new(*notAnExpr), new(notASTExpr), true},
}

type notAnExpr struct{}

func (notAnExpr) Pos() token.Pos { return token.NoPos }
func (notAnExpr) End() token.Pos { return token.NoPos }
func (notAnExpr) exprNode()      {}

type notASTExpr interface {
	Pos() token.Pos
	End() token.Pos
	exprNode()
}

func TestImplements(t *testing.T) {
	for _, tt := range implementsTests {
		xv := TypeOf(tt.x).Elem()
		xt := TypeOf(tt.t).Elem()
		if b := xv.Implements(xt); b != tt.b {
			t.Errorf("(%s).Implements(%s) = %v, want %v", TypeString(xv), TypeString(xt), b, tt.b)
		}
	}
}

var assignableTests = []struct {
	x interface{}
	t interface{}
	b bool
}{
	{new(chan int), new(<-chan int), true},
	{new(<-chan int), new(chan int), false},
	{new(*int), new(IntPtr), true},
	{new(IntPtr), new(*int), true},
	{new(IntPtr), new(IntPtr1), false},
	{new(Ch), new(<-chan interface{}), true},
	// test runs implementsTests too
}

type IntPtr *int
type IntPtr1 *int
type Ch <-chan interface{}

func TestAssignableTo(t *testing.T) {
	for i, tt := range append(assignableTests, implementsTests...) {
		xv := TypeOf(tt.x).Elem()
		xt := TypeOf(tt.t).Elem()
		if b := xv.AssignableTo(xt); b != tt.b {
			t.Errorf("%d:AssignableTo: got %v, want %v", i, b, tt.b)
		}
	}
}
