
/*=============== B_cell methods =====================================*/

/*--------------------------------*/

int B_cell::degree() {
  if ( child_ptr == 0 ) 
    return 0;
  else
    return child_ptr->degree;
}

/*--------------------------------*/

B_cell* link( B_cell* c1_ptr , B_cell* c2_ptr ) {

  if ( c1_ptr->key >= c2_ptr->key ) {
    c2_ptr->put_below(c1_ptr);

    return c2_ptr;
  } 
  else
    return link( c2_ptr , c1_ptr );
}

/*--------------------------------*/

void B_cell::put_below(B_cell* c_below_ptr) {

  /* add the c_below_ptr cell to the left end of the 
     child list of the heap */

  /* deal with case where there is no child heap */

  if ( child_ptr == 0 ) {
    child_ptr = new B_heap();
    child_ptr->head = child_ptr->tail = child_ptr->min_ptr = c_below_ptr;
    c_below_ptr->left_ptr = c_below_ptr->right_ptr = 0;
    child_ptr->degree = 1;
    return;
  }

  B_cell* this_tail = child_ptr->tail ;
  c_below_ptr->right_ptr = this_tail;
  c_below_ptr->left_ptr = 0;

  if ( this_tail != 0 ) {
    this_tail->left_ptr = c_below_ptr;
  }


  (this->child_ptr)->tail = c_below_ptr;

  /* unset the min of the child heap, for safety */

  (this->child_ptr)->min_ptr = 0;
  
  child_ptr->degree++;
}

/*=============== B_heap methods =====================================*/

B_heap::B_heap() {
  head=0;
  tail=0;
  min_ptr=0;
  degree=0;
}

/*--------------------------------*/

B_heap::B_heap(int key_val) {
  
  head = tail = min_ptr = new B_cell(key_val);
  degree = 1;
}

/*--------------------------------*/

boolean B_heap::empty() {
  if ( head == 0 )
    return TRUE;
  else
    return FALSE;
}

/*--------------------------------*/

int B_heap::findmin() {
  return min_ptr->key;
}

/*--------------------------------*/

void B_heap::merge( B_heap* bh2_ptr) {
 
  /* if the heap to be added is empty then do nothing */

  if  ( bh2_ptr == 0 ||  bh2_ptr->empty() == TRUE ) return;

  /* firstly merge the two child lists into one double linked list that
     is ordered but might contain cells of the same degree */
  
#ifdef DEBUG
  cout << " about to partial merge " << endl;
  print_keys(0);
  cout << " with " << endl;
  bh2_ptr->print_keys(0);
#endif

  partial_merge( bh2_ptr);

  /* scan the resulting child list to reomove duplicate degrees
     by B_cell::link */
 
#ifdef DEBUG
  cout << " about to remove duplicates " << endl;
  print_keys(0);
#endif

  remove_duplicates();

  return;
}

/*--------------------------------*/

void B_heap::partial_merge(B_heap* bh2_ptr) {
 
  if ( bh2_ptr->degree == 0 ) { /* no work to do */
     return;
  }

  B_heap* bh1_ptr = this;

  if ( degree == 0 ) { /* convert this heap into bh2 */
    head = bh2_ptr->head;
    tail = bh2_ptr->tail;
    min_ptr = bh2_ptr->min_ptr;
    degree = bh2_ptr->degree;
    return;
  }

  /* now can assume that both child lists are non-empty */

  degree += (bh2_ptr->degree);

  B_cell* c1_ptr = bh1_ptr->head;
  B_cell* c2_ptr = bh2_ptr->head;

  B_cell* prev_ptr = 0 ;
  B_cell* curr_ptr ;

  B_cell* tmp_ptr;

  B_cell* new_head;
  B_cell* new_tail;

  int deg1,deg2;

  while (1) {

    /* this carries on until one of the input lists is empty */

    if ( c1_ptr == 0 || c2_ptr == 0 ) break;
    
    deg1 = c1_ptr->degree();
    deg2 = c2_ptr->degree();
    if ( deg1 > deg2 ) { /* swap into a canonical order */
      tmp_ptr = c1_ptr;
      c1_ptr = c2_ptr;
      c2_ptr = tmp_ptr;
    }
    else {

      /* c1 does not have a bigger degree than c2 hence move c1 onto
	 the new list */

      if ( prev_ptr == 0 ) {  /* account for the first cell */
	new_head = c1_ptr;
      }
      else {
	prev_ptr->left_ptr = c1_ptr;
      }
      
      c1_ptr->right_ptr = prev_ptr; /* attach to prev on the new list */

      prev_ptr = c1_ptr; /* reset left end of the new list */
      
      c1_ptr = c1_ptr->left_ptr; /* move to next on the c1 list */
    }

  } /* end of while (1) */

  /* one, but not both, of the lists is now empty, make sure it is c2 */
  
  if ( c2_ptr != 0 ) {
    tmp_ptr = c1_ptr;
    c1_ptr = c2_ptr;
    c2_ptr = tmp_ptr;
  }
  
  if ( c2_ptr != 0 )
    cout << "Error in partial_merge" << endl;
  
  /* attach the rest of c1 */
  
  c1_ptr->right_ptr = prev_ptr; 
  
  prev_ptr->left_ptr = c1_ptr;


tail = 0;  


  /* scan to find the end cell */
  
  while ( c1_ptr != 0 ) {
    new_tail = c1_ptr;
    c1_ptr=c1_ptr->left_ptr;
  }
  
  
  /* finally partially tidy up */
  
  tail = new_tail;
  
  /* could have kept track of the min_ptr but will rest it when
     remove duplicates, so just invalidate it for now */


  
  head = new_head;
  min_ptr = 0;
}

/*--------------------------------*/

void B_heap::remove_duplicates() {
 /* ensure the heap is in a good state */
  
  if ( head == 0 ) {
    min_ptr =0 ;
    tail = 0;
    return;
  }

  /* first scan to remove dupliactes by appropriate linking */
  
  B_cell* prev_ptr = 0;
  B_cell* low_ptr = head;
  B_cell* high_ptr = (low_ptr->left_ptr);
  B_cell* next_high_ptr;
  
  B_cell* tmp_ptr;

  int high_deg;
  int low_deg;

  int min_key = (head->key); /* so can reset the min_ptr as we traverse */
  min_ptr = low_ptr;

  while ( high_ptr != 0 ) {

    if ( (low_ptr->key) < min_key ) {
      min_ptr = low_ptr;
      min_key = min_ptr->key;
    }


    if ( (high_ptr->key) < min_key ) {
      high_ptr = high_ptr;
      min_key = min_ptr->key;
    }

#ifdef DEBUG
    cout << "remove_duplicates: while loop " << endl;
    print_keys(0);
#endif

    next_high_ptr = high_ptr->left_ptr ;

    high_deg=high_ptr->degree();
    low_deg=low_ptr->degree();

    if ( high_deg < low_deg ) { /* just put into correct order */

      /* swap the hifgh and low cells */

      if (  next_high_ptr != 0 )
	next_high_ptr->right_ptr = low_ptr;
      else 
	tail = low_ptr;


      high_ptr->left_ptr = low_ptr;
      high_ptr->right_ptr = prev_ptr;

      low_ptr->left_ptr = next_high_ptr;
      low_ptr->right_ptr = high_ptr;
      
      if ( prev_ptr != 0 )
	prev_ptr->left_ptr = high_ptr;
      else 
	head = high_ptr;

      /* now swap the high and low pointers */

      tmp_ptr = low_ptr;
      low_ptr = high_ptr;
      high_ptr = tmp_ptr;


    }
    else if ( high_deg > low_deg ) { /* good state so just move to the left */
      prev_ptr = low_ptr;
      low_ptr=high_ptr;
      high_ptr=(high_ptr->left_ptr);
    }
    else { /* need to link the cells into one cell */

      next_high_ptr = (high_ptr->left_ptr) ;
      
      low_ptr = link( high_ptr, low_ptr ); /* is the new single cell */
      degree--;

      low_ptr->left_ptr = next_high_ptr;

      low_ptr->right_ptr = prev_ptr;

      if (prev_ptr !=0 ) 
	prev_ptr->left_ptr = low_ptr ;
      else 
	head = low_ptr;

      if ( next_high_ptr !=0 ) {
	next_high_ptr->right_ptr = low_ptr;
	high_ptr = next_high_ptr;
      } else {
	/* there was no next high, and high has gone, hence 
	   low_ptr is now the tail */
	tail = low_ptr;
	break;
      }
    }

  }

  if ( (tail->key) < min_key )
    min_ptr = tail;
  
}

/*--------------------------------*/

B_cell* B_heap::insert(int key_val) {

  /* create a one-heap,  and merge it into this heap */

  B_heap* bh_ptr = new B_heap(key_val);
  B_cell* c_ptr = bh_ptr->head;

  merge( new B_heap(key_val) );
  return c_ptr;
}


/*--------------------------------*/

int B_heap::deletemin() {

  int output = min_ptr->key;

  B_cell* left_of_min_ptr = min_ptr->left_ptr;
  B_cell* right_of_min_ptr = min_ptr->right_ptr;
  B_heap* heap_of_min_ptr = min_ptr->child_ptr;

  /* unstitch the min_cell from the child list */

  if ( left_of_min_ptr != 0 ) 
    left_of_min_ptr->right_ptr =  right_of_min_ptr;
  else
    tail = right_of_min_ptr;
 
  if ( right_of_min_ptr != 0 ) 
    right_of_min_ptr->left_ptr =  left_of_min_ptr;
  else
    head = left_of_min_ptr;

  /* reset the min_ptr */

  min_ptr = head;
  if ( head != 0 ) {
    B_cell* c_ptr = head;
    int min_key=head->key;
    while (c_ptr != 0 ) {
      tail = c_ptr;
      if ( c_ptr->key < min_key) {
	min_ptr = c_ptr;
	min_key = min_ptr->key;
      }
      c_ptr = c_ptr->left_ptr;
    }
  }

  
  degree--;

  merge( heap_of_min_ptr );

  return output;
}

/*--------------------------------*/

void B_heap::print_keys(int indent) {

  B_cell* c_ptr = head;

  while ( c_ptr != 0 ) {

    tab_by( indent ) ; 

    if (c_ptr == min_ptr )
      cout << "m";

    cout << c_ptr->key << "(" << c_ptr->degree() << ")"  << endl;

    if ( c_ptr == head && c_ptr->right_ptr != 0 ) {
      tab_by(indent);
      cout << "bad right on head" << endl;
    }

    if ( c_ptr == tail && c_ptr->left_ptr != 0 ) {
      tab_by(indent);
      cout << "bad left on tail" << endl;
    }
    
    if ( c_ptr->child_ptr != 0 ) 
      (c_ptr->child_ptr)->print_keys(indent+6);
    
    c_ptr = (c_ptr->left_ptr);
  }
}
/*======================================================*/
	

