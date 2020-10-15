
/*------------------- lheap_list_cell methods -----------------------*/

lheap_list_cell::lheap_list_cell(lheap* lh_p) {
  lh_ptr=lh_p;
  next=0;
}


/*------------------- lheap_list methods -----------------------------*/

lheap_list::lheap_list() {
  head = 0;
}

void lheap_list::insert(lheap* lh_p) {

  lheap_list_cell*  cell_ptr = new  lheap_list_cell(lh_p);

  /* does this disappear after the function finishes 
     i.e. scope of this object is what ?? */

  lheap_list_cell* head_cell_p;

  head_cell_p = this->head;

  if (head_cell_p == 0) {

    this->head = cell_ptr;
    cell_ptr->next=cell_ptr;

  } else {
    
    this->head=cell_ptr;
    cell_ptr->next = head_cell_p->next;
    head_cell_p->next=cell_ptr;
  }
}

/*-------------------------- lheap methods -----------------------------*/

lheap::lheap() {
  root_ptr = 0;
}

lheap::lheap(int key_val) {
  root_ptr = new bcell(key_val);
}


lheap::lheap(lheap_list* lh_lst_ptr) {

  if ( lh_lst_ptr->head == 0 ) {
    root_ptr = 0 ;
    return;
  }

  while ( (lh_lst_ptr->merge_first_two()) == TRUE ) 
    /*do nothing */;

  /* now *lh_lst_ptr must contain precisely one ptr to a heap */

  root_ptr = ((lh_lst_ptr->head)->lh_ptr)->root_ptr;  /* BAD ????? */
}


boolean lheap_list::merge_first_two() {

  lheap_list_cell* first_list_cell_ptr = head ;

  if ( first_list_cell_ptr == 0 ) {
    return FALSE;
  }

  lheap_list_cell* second_list_cell_ptr = first_list_cell_ptr->next ;

  if ( second_list_cell_ptr == first_list_cell_ptr ) {
    return FALSE;
  }

  /* merge the two associated heaps into the heap for the first cell,
     and take out the second cell */

  first_list_cell_ptr->lh_ptr = 
    merge( first_list_cell_ptr->lh_ptr, second_list_cell_ptr->lh_ptr );

  first_list_cell_ptr->next = second_list_cell_ptr->next;

  delete  second_list_cell_ptr;

  return TRUE;
}

int lheap::findmin() {
  if ( root_ptr == 0 ) {
    cout << endl << "ERROR: findmin called on empty heap" << endl;
    return -999;
  } 
  else
    return root_ptr->key;
}

boolean  lheap::notempty() {
  if ( root_ptr != 0 ) 
    return TRUE;
  else 
    return FALSE;
}

int lheap::deletemin() {
  if ( root_ptr == 0 ) {
    cout << endl << "ERROR: delete called on empty heap" << endl;
    return -999;
  } 

  int output = root_ptr->key;

  /* get the children of the root cell (if any) and merge them */

  bcell* left_child_ptr = root_ptr->left;
  bcell* right_child_ptr = root_ptr->right;

  delete root_ptr; /* destroys the root bcell */

  root_ptr = pmerge( left_child_ptr, right_child_ptr );

  return output;
}


bcell* lheap::pmerge(bcell* root1_ptr, bcell* root2_ptr) {

  if ( root1_ptr == 0 )
    return root2_ptr;

  if ( root2_ptr == 0 )
    return root1_ptr;

  /* make sure the keys are not out of order */

  if ( root1_ptr->key > root2_ptr->key ) {
    bcell* tmp_ptr=root1_ptr;
    root1_ptr = root2_ptr;
    root2_ptr = tmp_ptr;
  }

  /* merge on the right child of  *root1_ptr */

  root1_ptr->right = pmerge( root1_ptr->right, root2_ptr );

  root2_ptr = 0 ; /* safety */

  /* make sure the root cell child orderings and rank is correct */

  root1_ptr->ensure_leftist();

  return root1_ptr;
}

void bcell::ensure_leftist() {
  
  if ( left == 0 ) {
    rank = 1;
    if ( right != 0 )
      flip_children();
    return;    
  }

  if ( right == 0 ) {
    rank = 1 ;
    return;
  }

  /* now can assume neither child is null */

  if ( left->rank < right->rank )
    flip_children();

  rank = 1 + right->rank;
  return;
}

void bcell::flip_children() {
  bcell* tmp_ptr = right ;
  right = left;
  left = tmp_ptr;
}
  
lheap* merge(lheap* lh1, lheap* lh2) {

  bcell* root1_ptr = lh1->root_ptr;
  bcell* root2_ptr = lh2->root_ptr;
  

  bcell* new_root_ptr = lh1->pmerge( root1_ptr, root2_ptr );

  delete lh2;

  lh1->root_ptr = new_root_ptr;

  return lh1;
}
