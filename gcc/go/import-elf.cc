// import-elf.cc -- Go frontend read import data from an ELF file.

// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "go-system.h"

#include <cerrno>
#include <cstring>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

extern "C"
{
#include "intl.h"
#include "diagnostic.h"
}

#include "elfcpp.h"
#include "elfcpp_file.h"
#include "import.h"

// The functions in this file extract Go export data from an ELF
// object file or shared library.

const int Import::elf_magic_len;

// Return true if BYTES, which are from the start of the file, are an
// ELF magic number.

bool
Import::is_elf_magic(const char* bytes)
{
  const char elfmagic[Import::elf_magic_len] =
    {
      elfcpp::ELFMAG0, elfcpp::ELFMAG1, elfcpp::ELFMAG2, elfcpp::ELFMAG3
    };
  return memcmp(bytes, elfmagic, Import::elf_magic_len) == 0;
}

// The base of the template class used to read the ELF file.

class Elf_object
{
 public:
  Elf_object(const std::string& name, const unsigned char* contents,
	     off_t file_size, source_location location)
    : name_(name), contents_(contents), file_size_(file_size),
      location_(location), reported_error_(false)
  { }

  virtual ~Elf_object()
  { }

  // Look for a .go.export section.  If found, store the section
  // contents in *CONTENTS and return true.  Otherwise, return false.
  bool
  find_export_section(std::string* contents)
  { return this->do_find_export_section(contents); }

  // A view into the file.  This is used by elfcpp::Elf_file.
  class View
  {
   public:
    View(const unsigned char* data)
      : data_(data)
    { }

    const unsigned char*
    data() const
    { return this->data_; }

   private:
    const unsigned char* data_;
  };

  // Return a view into the file.  This is called by elfcpp::Elf_file.
  View
  view(off_t file_offset, off_t data_size);

  // A location in the file.
  class Location
  {
   public:
    Location(off_t file_offset, off_t data_size)
      : file_offset_(file_offset), data_size_(data_size)
    { }

    off_t
    file_offset() const
    { return this->file_offset_; }

    off_t
    data_size() const
    { return this->data_size_; }

   private:
    off_t file_offset_;
    off_t data_size_;
  };

  View
  view(const Location& loc);

  // Report an error and do not return.
  void
  error(const char* format, ...) ATTRIBUTE_GCC_DIAG(2, 3);

 protected:
  // Implemented by size/endian specific child class.
  virtual bool
  do_find_export_section(std::string* contents) = 0;

 private:
  // Report a truncation error.
  void
  report_error();

  // File name.
  std::string name_;
  // File contents.
  const unsigned char* contents_;
  // File size.
  off_t file_size_;
  // Source location of import statement.
  source_location location_;
  // Whether we have reported an error about this file.
  bool reported_error_;
};

// Report an error and do not return.

void
Elf_object::error(const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  diagnostic_info diagnostic;
  diagnostic_set_info(&diagnostic, format, &ap, this->location_, DK_ERROR);
  // FIXME: Can't return.
  sorry("can't continue");
}

// Report a truncation error.

void
Elf_object::report_error()
{
  if (!this->reported_error_)
    {
      error_at(this->location_, "%s: truncated ELF file", this->name_.c_str());
      this->reported_error_ = true;
    }
}

// Return a view into the file.  This is called by elfcpp::Elf_file.

Elf_object::View
Elf_object::view(off_t file_offset, off_t data_size)
{
  const unsigned char* data;
  if (file_offset + data_size <= this->file_size_)
    data = this->contents_ + file_offset;
  else
    {
      this->report_error();
      data = new unsigned char[data_size];
    }
  return View(data);
}

// Return a view given a location.

Elf_object::View
Elf_object::view(const Location& loc)
{
  const unsigned char* data;
  if (loc.file_offset() + loc.data_size() <= this->file_size_)
    data = this->contents_ + loc.file_offset();
  else
    {
      this->report_error();
      data = new unsigned char[loc.data_size()];
    }
  return View(data);
}

// This version of Elf_object is specific to the size and endianness
// of the ELF file.

template<int size, bool big_endian>
class Sized_elf_object : public Elf_object
{
 public:
  Sized_elf_object(const std::string& name, const unsigned char* contents,
		   off_t file_size, source_location location,
		   const elfcpp::Ehdr<size, big_endian>& ehdr)
    : Elf_object(name, contents, file_size, location),
      elf_file_(this, ehdr)
  { }

 protected:
  // Return the contents of the .go.export section.
  bool
  do_find_export_section(std::string* contents);

 private:
  // The ELF file accessor.
  elfcpp::Elf_file<size, big_endian, Elf_object> elf_file_;
};

// Look for a section named .go.export.  If we find it, return true
// and set *CONTENTS to its contents.

template<int size, bool big_endian>
bool
Sized_elf_object<size, big_endian>::do_find_export_section(
    std::string* contents)
{
  unsigned int shnum = this->elf_file_.shnum();
  for (unsigned int i = 0; i < shnum; ++i)
    {
      if (this->elf_file_.section_name(i) == ".go_export")
	{
	  Elf_object::Location l = this->elf_file_.section_contents(i);
	  Elf_object::View v = this->view(l);
	  contents->assign(reinterpret_cast<const char*>(v.data()),
			   l.data_size());
	  return true;
	}
    }
  return false;
}

// This function extracts Go export data from an ELF object file.  If
// it finds any, it returns an Import::Stream object for it.  NAME is
// the name of the file.  The object file has already been opened: FD
// is the file descriptor and FILE_SIZE is the size of the file.
// OFFSET is the offset at which to start reading from the file; this
// is to support archives.  LOCATION is the location of the import
// statement.

Import::Stream*
Import::find_elf_export_data(const std::string& name, int fd, off_t offset,
			     source_location location)
{
  struct stat st;
  if (fstat(fd, &st) < 0)
    {
      error_at(location, "%s: %s", name.c_str(), strerror(errno));
      return NULL;
    }
  const off_t file_size = st.st_size;

  // For now we assume that the host system support mmap.  This could
  // be changed to support reading the bytes directly.  FIXME.
  void* map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (map == MAP_FAILED)
    {
      error_at(location, "%s: mmap failed: %s", name.c_str(),
	       strerror(errno));
      return NULL;
    }

  const unsigned char* contents = static_cast<const unsigned char*>(map);
  contents += offset;

  int c = contents[elfcpp::EI_CLASS];
  if (c != elfcpp::ELFCLASS32 && c != elfcpp::ELFCLASS64)
    {
      error_at(location, "%s: unsupported ELF class %d", name.c_str(), c);
      munmap(map, file_size);
      return NULL;
    }

  int d = contents[elfcpp::EI_DATA];
  if (d != elfcpp::ELFDATA2LSB && d != elfcpp::ELFDATA2MSB)
    {
      error_at(location, "%s: unsupported ELF data encoding %d",
	       name.c_str(), d);
      munmap(map, file_size);
      return NULL;
    }

  bool is_big_endian = d == elfcpp::ELFDATA2MSB;

  Elf_object* object;
  if (c == elfcpp::ELFCLASS32)
    {
      if (is_big_endian)
	{
	  elfcpp::Ehdr<32, true> ehdr(contents);
	  object = new Sized_elf_object<32, true>(name, contents, file_size,
						  location, ehdr);
	}
      else
	{
	  elfcpp::Ehdr<32, false> ehdr(contents);
	  object = new Sized_elf_object<32, false>(name, contents, file_size,
						   location, ehdr);
	}
    }
  else
    {
      if (is_big_endian)
	{
	  elfcpp::Ehdr<64, true> ehdr(contents);
	  object = new Sized_elf_object<64, true>(name, contents, file_size,
						  location, ehdr);
	}
      else
	{
	  elfcpp::Ehdr<64, false> ehdr(contents);
	  object = new Sized_elf_object<64, false>(name, contents, file_size,
						   location, ehdr);
	}
    }

  std::string export_data;
  bool ret = object->find_export_section(&export_data);

  delete object;
  munmap(map, file_size);

  if (ret)
    return new Stream_from_string(export_data);
  else
    return NULL;
}
