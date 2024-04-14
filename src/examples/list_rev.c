#include "list.h"
#include "append.h"

/*@
function [rec] (datatype seq) snoc(datatype seq xs, i32 y) {
  match xs {
    Seq_Nil {} => {
      Seq_Nil {}
    }
    Seq_Cons {head : h, tail : zs}  => {
      snoc (rev(zs), h)
    }
  }
}

function [rec] (datatype seq) rev(datatype seq xs) {
  match xs {
    Seq_Nil {} => {
      Seq_Nil {}
    }
    Seq_Cons {head : h, tail : zs}  => {
      snoc (rev(zs), h)
    }
  }
}



lemma append_nil_r (datatype seq l1)
requires true
ensures append(l1, Seq_Nil {}) == l1

lemma append_cons_r (datatype seq l1, i32 x, datatype seq l2)
requires true
ensures append(l1, Seq_Cons {head: x, tail: l2})
        == append(snoc(l1, x), l2)
@*/

/*
when L2 == cons (L2_hd, L2_tl)
HAVE append(rev(L2_tl), cons(L2_hd, L1))
       ==                                (by append_cons_r)
     append(snoc(rev(L2_tl),L2_hd), L1) 
       ==                                (by definition of rev)
NEED append(rev(cons(L2_hd, L2_tl)), L1)
*/


struct int_list* IntList_rev_aux(struct int_list* xs, struct int_list* ys)
/*@ requires take L1 = IntList(xs) @*/
/*@ requires take L2 = IntList(ys) @*/
/*@ ensures take R = IntList(return) @*/
/*@ ensures R == append(rev(L2), L1) @*/
{
  if (ys == 0) {
    /*@ unfold rev(L2); @*/
    /*@ unfold append(Seq_Nil {},L1); @*/
    return xs;
  } else {
    /*@ unfold rev(L2); @*/
    /*@ apply append_cons_r(rev(tl(L2)), hd(L2), L1); @*/
    struct int_list *tmp = ys->tail;
    ys->tail = xs;
    return IntList_rev_aux(ys, tmp);
  }
}

struct int_list* IntList_rev(struct int_list* xs)
/*@ requires take L1 = IntList(xs) @*/
/*@ ensures take L1_rev = IntList(return) @*/
/*@ ensures L1_rev == rev(L1) @*/
{
  /*@ apply append_nil_r(rev(L1)); @*/
  return IntList_rev_aux (0, xs);
}





struct int_list* IntList_rev_loop(struct int_list *xs)
/*@ requires take L = IntList(xs)
    ensures  take L_ = IntList(return);
             L_ == rev(L)
@*/
{
  struct int_list *last = 0;
  struct int_list *cur = xs;
  /*@ apply append_nil_r(rev(L)); @*/
  while(1) 
  /*@ inv take L1 = IntList(last);
          take L2 = IntList(cur);
          append(rev(L2), L1) == rev(L)
  @*/
  {
    if (cur == 0) {
      /*@ unfold rev(Seq_Nil {}); @*/
      /*@ unfold append(Seq_Nil {}, L1); @*/
      return last;
    }
    struct int_list *tmp = cur->tail;
    cur->tail = last;
    last = cur;
    cur = tmp;
    /*@ unfold rev(L2); @*/
    /*@ apply append_cons_r (rev (tl(L2)), hd(L2), L1); @*/
  }
}





/*
append(rev(L2), L1) 

by def rev(L2)

append(snoc (rev (tl(L2)), hd(L2)), L1) 

by append_cons_r (rev (tl(L2)), hd(L2), L1)

append(rev (tl(L2)), Seq_Cons {head: hd(L2), tail: L1})
*/