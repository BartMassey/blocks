/*=============== F_cell methods =====================================*/
/*--------------------------------*/

int F_cell::degree() {
  if ( child_ptr == 0 ) 
    return 0;
  else
    return child_ptr->degree;
}
/*--------------------------------*/

F_cell* link( F_cell* c1_ptr , F_cell* c2_ptr ) {

  if ( c1_ptr->key >= c2_ptr->key ) {
    c2_ptr->put_below(c1_ptr);
    return c2_ptr;
  } 
  else
    return link( c2_ptr , c1_ptr );
}
/*--------------------------------*/

void F_cell::put_below(F_cell* c_below_ptr) {

  /* add the c_below_ptr cell to the left end of the 
     child list of the heap */

  /* deal with case where there is no child heap */

  if ( child_ptr == 0 ) {
    child_ptr = new F_heap();
    child_ptr->head = child_ptr->tail = child_ptr->min_ptr = c_below_ptr;
    c_below_ptr->left_ptr = c_below_ptr->right_ptr = 0;
    child_ptr->degree = 1;
    return;
  }

  F_cell* this_tail = child_ptr->tail ;
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
/*=============== F_heap methods =====================================*/

/*-------------------------------------------*/

void F_heap::consolidate() {

  /* empty the heap by moving all the cells out into the array */

  cell_ptr_array* A_ptr = new cell_ptr_array();
  F_cell* c_ptr = head;
  F_cell* next_c_ptr;
  
  while ( c_ptr != 0 ) {
    next_c_ptr = (c_ptr->left_ptr);
    A_ptr->insert( c_ptr );
    c_ptr = next_c_ptr;
  }

  /* make sure this F_heap is properly emptied */

  head = min_ptr = tail = 0;
  degree = 0;

  /* now move the consolidated cells back in from the array */

  while (1) {
    c_ptr = A_ptr->extract_largest();
    if ( c_ptr == 0 )
      break;
    else
      insert_cell( c_ptr );
  }

  delete A_ptr;
}

/*-------------------------------------------------*/

void F_heap::insert_cell(F_cell* c_ptr) {

  /* Adds *c_ptr to the child list at the head.
     Also maintains the min_ptr and degree. */

  if ( head == 0 ) {
    tail = min_ptr = head = c_ptr;
    c_ptr->left_ptr = c_ptr->right_ptr = 0;
    degree = 1;
  }
  else {
    head->right_ptr = c_ptr;
    c_ptr->left_ptr = head;
    head = c_ptr;
    c_ptr->right_ptr = 0;
    degree++;
    if ( min_ptr->key > c_ptr->key )
      min_ptr = c_ptr;
  }
}
  
/*--------------------------------------------------*/

F_cell* F_heap::insert(int key_val ) { 
  
  /* Creates a cell to hold key_val and inserts it into this heap */

  F_cell* c_ptr = new F_cell(key_val);
  insert_cell( c_ptr );
  return c_ptr;
}

/*--------------------------------------------------*/

boolean F_heap::empty() {
  if ( head == 0 )
    return TRUE;
  else
    return FALSE;
}

/*-------------------------------------------*/

F_heap* F_heap::delete_cell(F_cell* c_ptr) {

  /* removes c_ptr from the child list, and returns
     the heap under c_ptr */

  F_heap* output = c_ptr->child_ptr;

  F_cell* left_cell = c_ptr->left_ptr;
  F_cell* right_cell = c_ptr->right_ptr;

  if ( c_ptr == min_ptr ) 
    min_ptr = 0;

  if ( head == tail ) {
    head = tail = min_ptr = 0 ;
    degree = 0;
    return output;
  }

  if ( c_ptr == head ) {
    head = left_cell;
    head->right_ptr = 0;
  }
  else {
    (right_cell->left_ptr) = left_cell;
  }
  
  if ( c_ptr == tail ) {
    tail = right_cell;
    tail->left_ptr = 0;
  } 
  else {
    (left_cell->right_ptr) = right_cell;
  }

  degree--;
  return output;
}

/*-------------------------------------------*/

int F_heap::deletemin() {

  if ( head != 0 && min_ptr == 0 )
    consolidate();

  int output=min_ptr->key;

  /* get the F_heap* (possibly null) from the min cell */

  F_heap* fh_ptr = delete_cell(min_ptr);

  merge(fh_ptr);

  consolidate();

  return output;
}

/*-------------------------------------------*/

void F_heap::merge(F_heap* fh_ptr) {

  if ( fh_ptr == 0 || fh_ptr->head == 0 ) 
    return;
  
  if ( head == 0 ) { /* these one is empty so copy it */
    head = fh_ptr->head;
    tail = fh_ptr->tail;
    min_ptr = fh_ptr->min_ptr;
    degree = fh_ptr->degree;
  }
  else {
    /* neither is empty */ 
    
    /* reset the min_ptr when it is easy */
    
    if ( min_ptr != 0 && fh_ptr->min_ptr != 0 )
      if ( min_ptr->key > (fh_ptr->min_ptr)->key )
	min_ptr = fh_ptr->min_ptr;
    
    /* ???? couldn't we here do a consildate on whichever heap needs it ?? */
    /* attach fh_ptr->head to this->tail */
    
    degree += fh_ptr->degree;
    
    (fh_ptr->head)->right_ptr = tail;
    (tail->left_ptr) = fh_ptr->head;
    tail = fh_ptr->tail;
  }

  /* finally then delete the other */
  fh_ptr->head = fh_ptr->tail = fh_ptr->min_ptr = 0;
  delete fh_ptr;
}

/*--------------------------------*/

void F_heap::print_keys(int indent) {

  F_cell* c_ptr = head;

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

/*=============== cell_ptr_array methods =================================*/
/*-------------------------------------------*/

cell_ptr_array::cell_ptr_array() {

  int i;
  for( i=0 ; i<LOG_BUFF ; i++ )
    A[i]=0;
  max_deg = (LOG_BUFF-1);
}

/*-------------------------------------------*/

void cell_ptr_array::insert(F_cell* c_ptr) {
  
  /* adds a cell to the array, and uses linking to ensure have only
     one cell per degree */

  int deg_c = c_ptr->degree();
  
  if ( A[ deg_c ] == 0 ) { /* no previous cell of this degree */
    A[ deg_c ] = c_ptr;
    if ( deg_c > max_deg )
      max_deg = deg_c;
  }
  else { /* cell already there, hence link them, and reattempt to insert */
 
    c_ptr = link( c_ptr, A[ deg_c] );
    A[ deg_c ] = 0;
    if( max_deg == deg_c )
      max_deg = (LOG_BUFF - 1); /* a safe upper bound on the degrees present */
    insert (c_ptr);
  }
}

/*-------------------------------------------*/

F_cell* cell_ptr_array::extract_largest() {

  /* scan down, from max_deg, to find a non-null entry */

  int deg = max_deg;

  while (1) {
    
    if ( deg < 0 ) {
      max_deg=-1;
      return 0;
    }

    if ( A[deg] != 0 ) {
      F_cell* output = A[deg];
      A[deg] = 0;
      max_deg = deg - 1;
      return output;
    }
      
    deg--;
  }
}
/*===============================================================*/
