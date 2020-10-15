/* 
The structure of the cells is the same as for Binomial cells.  They
are designed to form part of a doubly linked list, and carry heap
pointers as children.

The heap class are also very similar to the Binomial heap class.  That
is it functions mostly as a wrapper for a doubly linked
null-terminated list of cells.  Carrying the information of the head,
tail, and a pointer to the cell with the minimum key.  

In an effort to see if any code reuse was possible I decided to try
and make the binomial and Fibonacci as similar as possible. Hence I
kept the the Binomial child list as a doubly linked list instead of
singly linked list (also motivated by a desire to avoid a list
reversal that is needed in the CLR structure). I also decide to keep
the Fibonacci as a non-circular list and use the head as typically
different from the tail. 

The main difference at the level of the header files is the use of a
auxiliary class cell_ptr_array by the heap, when doing the
consolidation.
*/ 

class F_heap;
class F_cell;

/*=================================================================*/

class F_cell {

 private:

  int key;

  F_heap* child_ptr;

  /* want to join these together into a doubly linked (null terminated) list */
  F_cell* left_ptr;
  F_cell* right_ptr;

  void put_below(F_cell* c_below_ptr); /* attaches the c_below cell to
					  the left end of the child heap */
 public:

  F_cell(int key_val) {
    key = key_val;  /* initialise with the key */
    left_ptr = 0;
    right_ptr =0;
    child_ptr = 0;
  }

  int degree();

  friend F_cell* link( F_cell* c1_ptr,  F_cell* c2_ptr );

  friend class F_heap;
};

/*=================================================================*/

class F_heap {

 private:

  F_cell* head; /* head of the child list */

  F_cell* tail; /* tail of the child list */

  F_cell* min_ptr;  /* cell in the child list with the smallest key */

  int degree; /* number of cells in the child list */

  void consolidate(); /* ships all the child cells out to a cell_array
			 and gets back a set with all different degrees,
			 also resets the min_ptr and degree correctly */

  void insert_cell(F_cell* c_ptr ); /* appends the F_cell to the child list,
				     and maintains the min_ptr and degree */

  F_heap* delete_cell(F_cell* c_ptr); /* deletes c_ptr from the child list,
					 possibly sets min_ptr to null */

 public:

  F_heap() {            /* create an empty heap */
    head = min_ptr = tail = 0;
    degree = 0;
  }

  boolean empty();

  F_cell* insert(int key_val); /* creates an F_cell to hold the key, 
				  puts it into the heap (lazily), and 
				  returns a pointer to the F_cell */

  int findmin();

  int deletemin();

  void merge( F_heap* h2_ptr);

  void print_keys(int indent); /* scan in pre-order, by increasing degree
				  with indent */

  friend class F_cell;
};

/*==============================================================*/

class cell_ptr_array {

 private:

  F_cell* A[LOG_BUFF]; /* A is an array of pointers to F_cells,
			 the cell *A[d] (if present) should have degree d */
  
  int max_deg; /* current upper bound on the degrees of the cells contained.
		  Useful to extracting the cells in descending order of degree 
		  */
  
 public:

  cell_ptr_array(); /* Takes O(LOG_BUFF) time to initialise everything to 0 */

  void insert(F_cell* c_ptr); /* puts c_ptr into the array. Does
				 linking of cells, when necessary, to keep
				 the degrees correct. */

  F_cell* extract_largest(); /* extracts the cell with the largest degree */ 
};
  


