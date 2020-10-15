/* The structure given in CLR requires a reverse of the list of
children of a node whenever deletemin wants to remove that node.  To
avoid this I make the list of children doubly linked.  This makes it a
lot more like the Fibonacci heap.  However I also make the child list
null terminated rather than circular. 

Degree ordering:   degrees should increase as we go left.

Heap ordering: keys should necer decrease as we go down to the children

I also observe that the list of children of a heaped binomial tree B_k
forms a binomial heap in which the root list happens to be full in the
sense that it contains subtrees of all degrees 1,...,k-1.

This means the child list of a cell can be stored as a heap which
happens to be full. This simplifies (at least conceptually) the code,
but does increase the constant factors because of the extra levfel of
indirection.

*/ 

class B_heap;
class B_cell;

class B_cell {

 private:

  int key;

  B_heap* child_ptr;

  /* want to join these together into a doubly linked (null terminated) list 
     with degrees increasing to the left */

  B_cell* left_ptr;
  B_cell* right_ptr;

  void put_below(B_cell* c_below_ptr); /* attaches the c_below cell to
					  the left end of the child heap */

 public:

  B_cell(int key_val) {
    key = key_val;  /* initialise with the key */
    left_ptr = 0;
    right_ptr =0;
    child_ptr = 0;
  }

  int degree();

  friend B_cell* link( B_cell* c1_ptr,  B_cell* c2_ptr );

  friend class B_heap;

};

class B_heap {

 private:

  B_cell* head; /* smallest degree end of the child list */
  B_cell* tail; /* largest degree end of the child list */

  B_cell* min_ptr;  /* cell in the child list with the smallest key */

  int degree; /* number of cells in the child list */

  boolean full; /* whether the list of cells have degree 1,...,degree-1 
		   in which case this B_heap is actually a binomial tree.
		   Basically only set when this heap is not the root heap.
		   */

  void partial_merge(B_heap* bh2_ptr); /* merges the child list of 
					  bh_2 into the child list of this 
					  heap, but leaves duplicates */


  void remove_duplicates(); /* tidies up a child list after 
			       partial_merge, */


 public:

  B_heap();            /* create an empty heap */
  B_heap(int key_val); /* create a heap from just the one element */

  boolean empty();

  void print_keys(int indent); /* scan in pre-order, by increasing degree
				  with indent */

  void insert(int key_val);

  int deletemin();
  int findmin();

  void merge( B_heap* bh2_ptr);
  
  friend class B_cell;
};
