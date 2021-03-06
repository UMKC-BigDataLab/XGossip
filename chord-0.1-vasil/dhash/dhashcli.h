#ifndef __DHASH_CLI_H_
#define __DHASH_CLI_H_

#include <sys/types.h>
#include "adb_prot.h"
#include "dhash.h"
#include "dhblock_srv.h"

// SFS includes
#include <vec.h>

typedef callback<void, dhash_stat, chordID>::ref cbinsert_t;
typedef callback<void, dhash_stat, vec<chordID> >::ref cbinsert_path_t;
typedef callback<void, dhash_stat, vec<chord_node>, route>::ref
  dhashcli_lookupcb_t;
typedef	callback<void, dhash_stat, bool>::ref sendblockcb_t;

// Forward declarations
class adb;
class vnode;
class dhash_block;
class route_iterator;
struct dhblock;

class dhashcli {
public:
  struct rcv_state {
    blockID key;
    route r;
    vec<timespec> times;
    int errors;
    
    bool succopt;
    int incoming_rpcs;
    
    vec<chord_node> succs;
    size_t nextsucc;
    
    cb_ret callback;

    bool completed;


    void timemark () {
      timespec x;
      clock_gettime (CLOCK_REALTIME, &x);
      times.push_back (x);
    }
    
    void complete (dhash_stat s, ptr<dhash_block> b) {
      completed = true;
      (callback) (s, b, r);
    }

    rcv_state (blockID key, cb_ret cb) :
      key (key),
      errors (0),
      succopt (false),
      incoming_rpcs (0),
      nextsucc (0),
      callback (cb),
      completed (false)
    {
      timemark ();
    }
  };

  // State for a fragment store
  struct sto_state {
    ref<dhash_block> block;
    vec<chord_node> succs;
    cbinsert_path_t cb;
    
    u_int out;
    u_int good;
    
    sto_state (ref<dhash_block> b, cbinsert_path_t x) :
      block (b), cb (x), out (0), good (0) {}
  };

private:
  ptr<vnode> clntnode;
  bool ordersucc_;
  ptr<dhash> dh;


  void doassemble (ptr<rcv_state> rs, ptr<dhblock> block,
		   vec<chord_node> succs, ptr<str> = NULL);
  void dofetchrec_execute (blockID b, cb_ret cb);
  void dofetchrec_cb (timespec s, blockID b, cb_ret cb,
		      ptr<dhash_fetchrec_res> res, clnt_stat s);
  
  void lookup_findsucc_cb (chordID blockID, dhashcli_lookupcb_t cb,
			   vec<chord_node> s, route path, chordstat err);
    
  void insert_lookup_cb (ref<dhash_block> block, cbinsert_path_t cb, int options, 
			 dhash_stat status, vec<chord_node> succs, route r);
  void insert_store_cb (ref<sto_state> ss, route r, u_int i, 
			u_int nstores, u_int min_needed,
			dhash_stat err, chordID id, bool present);
  
  void insert_store_cb_strict (ref<sto_state> ss, route r, u_int i, 
			u_int nstores, u_int min_needed,
			dhash_stat err, chordID id, bool present);

  void fetch_frag (ptr<rcv_state> rs, ptr<dhblock> block, ptr<str> = NULL);

  void retrieve_lookup_cb (ptr<rcv_state> rs, ptr<dhblock> block,
					ptr<str>,
			   vec<chord_node> succs, route r,
			   chordstat status);
  void retrieve_fetch_cb (ptr<rcv_state> rs, u_int i, ptr<dhblock> b, ptr<str>,
			  ptr<dhash_block> block);

  void insert_succlist_cb (ref<dhash_block> block, cbinsert_path_t cb,
			   chordID guess, int options,
			   vec<chord_node> succs, chordstat status);
  void retrieve_block_hop_cb (ptr<rcv_state> rs, route_iterator *ci,
			     int options, int retries, ptr<chordID> guess,
			     bool done);

  void retrieve_dl_or_walk_cb (ptr<rcv_state> rs, dhash_stat status,
                               int options, int retries, ptr<chordID> guess,
			       ptr<dhash_block> blk);

  void sendblock_fetch_cb (ptr<location> dst, blockID bid_to_send,
			   sendblockcb_t cb, int nonce, ptr<str> filter, adb_status stat,
			   chordID key, str data);
  void sendblock_cb (callback<void, dhash_stat, bool>::ref cb, 
		     dhash_stat err, chordID dest, bool present);

  bool on_timeout (ptr<rcv_state> rs, 
		   ptr<dhblock> b, ptr<str>,
		   chord_node dest,
		   int retry_num);

public:
  dhashcli (ptr<vnode> node, ptr<dhash> dh);

  void assemble (blockID b, cb_ret cb, vec<chord_node> succs, route r);
  void retrieve (blockID blockID, cb_ret cb, 
		 int options = 0, 
		 //ptr<chordID> guess = NULL);
	// Praveen
		 ptr<str> data = NULL, ptr<chordID> guess = NULL);

  void insert (ref<dhash_block> block, cbinsert_path_t cb, 
	       int options = 0, 
	       ptr<chordID> guess = NULL);

  void sendblock (ptr<location> dst, blockID bid,
		  ptr<dhblock_srv> db, sendblockcb_t cb,
		  //int nonce = 0);
			// Praveen
		  int nonce = 0, ptr<str> data = NULL);

  //send a specific fragment (not the one in the DB)
  void sendblock (ptr<location> dst, blockID bid, str data,
		  sendblockcb_t cb, int nonce = 0);

  void lookup (chordID blockID, dhashcli_lookupcb_t cb);
};

#endif
