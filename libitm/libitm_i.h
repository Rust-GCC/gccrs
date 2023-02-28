/* Copyright (C) 2008-2023 Free Software Foundation, Inc.
   Contributed by Richard Henderson <rth@redhat.com>.

   This file is part of the GNU Transactional Memory Library (libitm).

   Libitm is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   Libitm is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* The following are internal implementation functions and definitions.
   To distinguish them from those defined by the Intel ABI, they all
   begin with GTM/gtm.  */

#ifndef LIBITM_I_H
#define LIBITM_I_H 1

#include "libitm.h"
#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unwind.h>
#include "local_atomic"

/* Don't require libgcc_s.so for exceptions.  */
extern void _Unwind_DeleteException (_Unwind_Exception*) __attribute__((weak));


#include "common.h"

namespace GTM HIDDEN {

using namespace std;

typedef unsigned int gtm_word __attribute__((mode (word)));

// These values are given to GTM_restart_transaction and indicate the
// reason for the restart.  The reason is used to decide what STM
// implementation should be used during the next iteration.
enum gtm_restart_reason
{
  RESTART_REALLOCATE,
  RESTART_LOCKED_READ,
  RESTART_LOCKED_WRITE,
  RESTART_VALIDATE_READ,
  RESTART_VALIDATE_WRITE,
  RESTART_VALIDATE_COMMIT,
  RESTART_SERIAL_IRR,
  RESTART_NOT_READONLY,
  RESTART_CLOSED_NESTING,
  RESTART_INIT_METHOD_GROUP,
  NUM_RESTARTS,
  NO_RESTART = NUM_RESTARTS
};

} // namespace GTM

#include "target.h"
#include "rwlock.h"
#include "aatree.h"
#include "dispatch.h"
#include "containers.h"

#ifdef __USER_LABEL_PREFIX__
# define UPFX UPFX1(__USER_LABEL_PREFIX__)
# define UPFX1(t) UPFX2(t)
# define UPFX2(t) #t
#else
# define UPFX
#endif

namespace GTM HIDDEN {

// A log of (de)allocation actions.  We defer handling of some actions until
// a commit of the outermost transaction.  We also rely on potentially having
// both an allocation and a deallocation for the same piece of memory in the
// log; the order in which such entries are processed does not matter because
// the actions are not in conflict (see below).
// This type is private to alloc.c, but needs to be defined so that
// the template used inside gtm_thread can instantiate.
struct gtm_alloc_action
{
  // Iff free_fn_sz is nonzero, it must be used instead of free_fn, and vice
  // versa.
  void (*free_fn)(void *);
  void (*free_fn_sz)(void *, size_t);
  size_t sz;
  // If true, this is an allocation; we discard the log entry on outermost
  // commit, and deallocate on abort.  If false, this is a deallocation and
  // we deallocate on outermost commit and discard the log entry on abort.
  bool allocated;
};

struct gtm_thread;

// A transaction checkpoint: data that has to saved and restored when doing
// closed nesting.
struct gtm_transaction_cp
{
  gtm_jmpbuf jb;
  size_t undolog_size;
  aa_tree<uintptr_t, gtm_alloc_action> alloc_actions;
  size_t user_actions_size;
  _ITM_transactionId_t id;
  uint32_t prop;
  uint32_t cxa_catch_count;
  unsigned int cxa_uncaught_count;
  // We might want to use a different but compatible dispatch method for
  // a nested transaction.
  abi_dispatch *disp;
  // Nesting level of this checkpoint (1 means that this is a checkpoint of
  // the outermost transaction).
  uint32_t nesting;

  void save(gtm_thread* tx);
  void commit(gtm_thread* tx);
};

// An undo log for writes.
struct gtm_undolog
{
  vector<gtm_word> undolog;

  // Log the previous value at a certain address.
  // The easiest way to inline this is to just define this here.
  void log(const void *ptr, size_t len)
  {
    size_t words = (len + sizeof(gtm_word) - 1) / sizeof(gtm_word);
    gtm_word *undo = undolog.push(words + 2);
    memcpy(undo, ptr, len);
    undo[words] = len;
    undo[words + 1] = (gtm_word) ptr;
  }

  void commit () { undolog.clear(); }
  size_t size() const { return undolog.size(); }

  // In local.cc
  void rollback (gtm_thread* tx, size_t until_size = 0);
};

// An entry of a read or write log.  Used by multi-lock TM methods.
struct gtm_rwlog_entry
{
  atomic<gtm_word> *orec;
  gtm_word value;
};

// Contains all thread-specific data required by the entire library.
// This includes all data relevant to a single transaction. Because most
// thread-specific data is about the current transaction, we also refer to
// the transaction-specific parts of gtm_thread as "the transaction" (the
// same applies to names of variables and arguments).
// All but the shared part of this data structure are thread-local data.
// gtm_thread could be split into transaction-specific structures and other
// per-thread data (with those parts then nested in gtm_thread), but this
// would make it harder to later rearrange individual members to optimize data
// accesses. Thus, for now we keep one flat object, and will only split it if
// the code gets too messy.
struct gtm_thread
{

  struct user_action
  {
    _ITM_userCommitFunction fn;
    void *arg;
    bool on_commit;
    _ITM_transactionId_t resuming_id;
  };

  // The jump buffer by which GTM_longjmp restarts the transaction.
  // This field *must* be at the beginning of the transaction.
  gtm_jmpbuf jb;

  // Data used by local.c for the undo log for both local and shared memory.
  gtm_undolog undolog;

  // Read and write logs.  Used by multi-lock TM methods.
  vector<gtm_rwlog_entry> readlog;
  vector<gtm_rwlog_entry> writelog;

  // Data used by alloc.c for the malloc/free undo log.
  aa_tree<uintptr_t, gtm_alloc_action> alloc_actions;

  // Data used by useraction.c for the user-defined commit/abort handlers.
  vector<user_action> user_actions;

  // A numerical identifier for this transaction.
  _ITM_transactionId_t id;

  // The _ITM_codeProperties of this transaction as given by the compiler.
  uint32_t prop;

  // The nesting depth for subsequently started transactions. This variable
  // will be set to 1 when starting an outermost transaction.
  uint32_t nesting;

  // Set if this transaction owns the serial write lock.
  // Can be reset only when restarting the outermost transaction.
  static const uint32_t STATE_SERIAL		= 0x0001;
  // Set if the serial-irrevocable dispatch table is installed.
  // Implies that no logging is being done, and abort is not possible.
  // Can be reset only when restarting the outermost transaction.
  static const uint32_t STATE_IRREVOCABLE	= 0x0002;

  // A bitmask of the above.
  uint32_t state;

  // In order to reduce cacheline contention on global_tid during
  // beginTransaction, we allocate a block of 2**N ids to the thread
  // all at once.  This number is the next value to be allocated from
  // the block, or 0 % 2**N if no such block is allocated.
  _ITM_transactionId_t local_tid;

  // Data used by eh_cpp.c for managing exceptions within the transaction.
  uint32_t cxa_catch_count;
  // If cxa_uncaught_count_ptr is 0, we don't need to roll back exceptions.
  unsigned int *cxa_uncaught_count_ptr;
  unsigned int cxa_uncaught_count;
  void *eh_in_flight;

  // Checkpoints for closed nesting.
  vector<gtm_transaction_cp> parent_txns;

  // Data used by retry.c for deciding what STM implementation should
  // be used for the next iteration of the transaction.
  // Only restart_total is reset to zero when the transaction commits, the
  // other counters are total values for all previously executed transactions.
  // restart_total is also used by the HTM fastpath in a different way.
  uint32_t restart_reason[NUM_RESTARTS];
  uint32_t restart_total;

  // *** The shared part of gtm_thread starts here. ***
  // Shared state is on separate cachelines to avoid false sharing with
  // thread-local parts of gtm_thread.

  // Points to the next thread in the list of all threads.
  gtm_thread *next_thread __attribute__((__aligned__(HW_CACHELINE_SIZE)));

  // If this transaction is inactive, shared_state is ~0. Otherwise, this is
  // an active or serial transaction.
  atomic<gtm_word> shared_state;

  // The lock that provides access to serial mode.  Non-serialized
  // transactions acquire read locks; a serialized transaction acquires
  // a write lock.
  // Accessed from assembly language, thus the "asm" specifier on
  // the name, avoiding complex name mangling.
  static gtm_rwlock serial_lock __asm__(UPFX "gtm_serial_lock");

  // The head of the list of all threads' transactions.
  static gtm_thread *list_of_threads;
  // The number of all registered threads.
  static unsigned number_of_threads;

  // In alloc.cc
  void commit_allocations (bool, aa_tree<uintptr_t, gtm_alloc_action>*);
  void record_allocation (void *, void (*)(void *));
  void forget_allocation (void *, void (*)(void *));
  void forget_allocation (void *, size_t, void (*)(void *, size_t));
  void discard_allocation (const void *ptr)
  {
    alloc_actions.erase((uintptr_t) ptr);
  }

  // In beginend.cc
  void rollback (gtm_transaction_cp *cp = 0, bool aborting = false);
  bool trycommit ();
  void restart (gtm_restart_reason, bool finish_serial_upgrade = false)
        ITM_NORETURN;

  gtm_thread();
  ~gtm_thread();

  static void *operator new(size_t);
  static void operator delete(void *);

  // Invoked from assembly language, thus the "asm" specifier on
  // the name, avoiding complex name mangling.
  static uint32_t begin_transaction(uint32_t, const gtm_jmpbuf *)
	__asm__(UPFX "GTM_begin_transaction") ITM_REGPARM;
  // In eh_cpp.cc
  void init_cpp_exceptions ();
  void revert_cpp_exceptions (gtm_transaction_cp *cp = 0);

  // In retry.cc
  // Must be called outside of transactions (i.e., after rollback).
  void decide_retry_strategy (gtm_restart_reason);
  abi_dispatch* decide_begin_dispatch (uint32_t prop);
  void number_of_threads_changed(unsigned previous, unsigned now);
  // Must be called from serial mode. Does not call set_abi_disp().
  void set_default_dispatch(abi_dispatch* disp);

  // In method-serial.cc
  void serialirr_mode ();

  // In useraction.cc
  void rollback_user_actions (size_t until_size = 0);
  void commit_user_actions ();
};

} // namespace GTM

#include "tls.h"

namespace GTM HIDDEN {

// An unscaled count of the number of times we should spin attempting to
// acquire locks before we block the current thread and defer to the OS.
// This variable isn't used when the standard POSIX lock implementations
// are used.
extern uint64_t gtm_spin_count_var;

extern "C" uint32_t GTM_longjmp (uint32_t, const gtm_jmpbuf *, uint32_t)
	ITM_NORETURN ITM_REGPARM;

extern "C" void GTM_LB (const void *, size_t) ITM_REGPARM;

extern void GTM_error (const char *fmt, ...)
	__attribute__((format (printf, 1, 2)));
extern void GTM_fatal (const char *fmt, ...)
	__attribute__((noreturn, format (printf, 1, 2)));

extern abi_dispatch *dispatch_serial();
extern abi_dispatch *dispatch_serialirr();
extern abi_dispatch *dispatch_serialirr_onwrite();
extern abi_dispatch *dispatch_gl_wt();
extern abi_dispatch *dispatch_ml_wt();
extern abi_dispatch *dispatch_htm();


} // namespace GTM

#endif // LIBITM_I_H
