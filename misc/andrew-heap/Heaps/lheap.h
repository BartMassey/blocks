/*
First attempt at a leftist heap.
No general delete, hence no dummy nodes to deal with.
*/

class bcell; /* (leftist) binary cell used in the heap implementation */
class lheap; /* maintains a leftist heap of  cells  */
class lheap_list; /* circular, singly linked, list of ptrs to lheaps */

/*----------------------------------------------------------------*/

/* Make the binary cells into a class so that can make use of the
constructor. However keep everything else protected, rather than
private, so that the owner can directly set them */

class bcell {

 private:
  
  void flip_children(); /* for use by ensure_leftist */
  
 protected:

  int rank; /* rank of the bcell. Definition depends on usage. */
  int key;  /* key(=value) for the cell. 
	       The number used to define the heap property */

  bcell* left;   /* ptr to the left child */
  bcell* right;  /* ptr to the right child */
  bcell* parent; /* ptr to the parent */

 public:

  bcell(int key_val) {
    rank=1 ;      /* ready to be a leaf node */
    key=key_val; 
    left=0;       /* external nodes assumed null */
    right=0;
    parent=0;
  }

  void ensure_leftist(); /* reorders the children, and resets the
			    rank, (when necessary) so as to ensure the
			    leftist property holds at this cell */

  friend class lheap;

};

/*----------------------------------------------------------------*/

class lheap {

 private:
  bcell* root_ptr;

 protected:

  bcell* pmerge(bcell* root1_ptr, bcell* root2_ptr); 

 public:

  lheap(); /* create an empty heap */

  lheap(int key_val); /* create a heap from one element.
			 This is necessary for the insert operation */
  
  lheap(lheap_list* lh_lst_ptr); /* create a leftist heap from a
			       circular singly linked list of lheaps.
			       (The round-robin initialisation) */

  /* assume the default destructor works okay */

  boolean notempty(); /* returns TRUE if the heap is non-empty,
			 FALSE otherwise */

  int findmin(); /* Returns the minimum key */

  int deletemin(); /* Deletes and returns the minimum key.
		      Does this in place, 
		      the old heap is destroyed. */

  void insert(int keyvalue); /* Create a one-heap from key-value and 
				merge it into this heap */
  
  friend lheap* merge(lheap* lh1, lheap* lh2); /* Destructively merges
						lh1 and lh2 and returns a 
						pointer to the result */

};

/*----------------------------------------------------------------*/

/* circular singly linked list to store the (one)-heaps */

class lheap_list_cell {

 protected:
  lheap* lh_ptr;
  lheap_list_cell* next;
 public:
   lheap_list_cell(lheap* lh_p);

  friend class lheap_list;
  friend class lheap;
};

class lheap_list {

 private:
  lheap_list_cell* head;

 public:

  lheap_list(); 

  void insert(lheap* lh_p);

  boolean merge_first_two();

  friend class lheap;
};
  




