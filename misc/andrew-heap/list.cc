#include <stream.h>
#include <malloc.h>

/*-----------------------------------------------------------------------*/

/* doubly linked list cell, pointers to go both ways down the list */

class cell {

 private: 

  /* here we have are to think of a cell as having a _set_ of two
     pointers to other cells. The concept of which way around the list we
     are going will be kept by the methods ensuring that we carry on going
     in whatever way we started. */

  cell* ncells[2];   /* neighboring cells */

  int   elem; /* the actual stored number */

 protected:
  
  inline void setcell(int i,cell* cptr) {ncells[i]=cptr;}

 public:

  cell(int i, cell* c0, cell* c1) {
    elem = i;
    ncells[0]=c0;
    ncells[1]=c1;
  }

  inline int value() {return elem;};

  inline cell* ncell(int i) { return ncells[i]; };

  inline cell* diffcell(cell* cptr) {
    if (ncells[0] != cptr ) return ncells[0];
    return ncells[1];
    }
  
  void invert();

  friend class list;
};

void  cell::invert() {
  cell* tmp=ncells[0];
  ncells[0]=ncells[1];
  ncells[1]=tmp;
}

/*-----------------------------------------------------------------------*/

class list {

 private:

  int size; 

  cell* head;

 public:

  list() {size=0; head=NULL;}

  int insert(int i);

  void reverse();

  void print();

  inline int get_size() {return(size);}

  friend int append_list(list* ptr1, list* lptr2);

};

/*-----------------------------------------------------------------------*/

void list::reverse() {

  cout << "about to reverse" << endl; 

  cell* oldhead=head;
  cell* last = head->ncell(1);
  
  head=last;
  
  if ( (head->ncell(0)) == oldhead )  head->invert();

}

/*-----------------------------------------------------------------------*/

void list::print() {

  cout << "[ ";

  if (head==NULL) { cout << "]" << endl; return; }

  cell* tmp;
  cell* curr=head;
  cell* prev = (head->ncell(1)); /* so starts forwards along ncell[0] */

  do {

    cout << curr->value() << " ";

    tmp = curr;

    /* move to the next cell, which is defined as one of the ncells
       with a preference for one that is not the the previous cell (though in
       the case of a two element list it still is the prev cell) */

    curr = curr->diffcell(prev);

    prev = tmp;

  } while (curr != head );

  cout << "]" << endl;
}

/*------------------------*/

int list::insert(int elem) {

  cout << "about to insert (prepend) " << elem <<  endl; 

  if (head==NULL) {
    head = new cell(elem,NULL,NULL);
    head->setcell(0,head);
    head->setcell(1,head);

    return size++;
  }
    
  cell* oldhead = head;
  cell* last    = head->ncell(1) ;
  
  head = new cell(elem, oldhead, last );
  
  /* need to ensure oldhead points back to new head */
  
  oldhead->setcell(1,head);
  
  /* find which of the last cell ptrs still points to oldhead, 
     (if the original list was size=1 then both pointed to oldhead,
     but now only one of them does). This one must be reset to point at the 
     at the new head */

  if (last->ncell(0) == oldhead) 
    last->setcell(0,head);
  else
    last->setcell(1,head);

  return size++;
}

/*------------------------*/

int append_list(list* lptr1, list* lptr2) {

  /* list2 is destructively appended to list1,
     return the new total size. */  

  cout << "about to append ";
  lptr2->print();
  cout << " to ";
  lptr1->print();
  cout << endl;

  if ( lptr2->size == 0 ) {
    cout << "second list empty: nothing changed " << endl << endl;
    return lptr1->size;
  }

  int newsize = lptr1->size + lptr2->size;

  cell* head1 = lptr1->head;
  cell* head2 = lptr2->head;

  cell* last1 = head1->ncell(1);
  cell* last2 = head2->ncell(1);

  head2->setcell(1,last1);
  head1->setcell(1,last2);
  
  if ( (last1->ncell(0)) == head1 ) 
    last1->setcell(0,head2);
  else
    last1->setcell(1,head2);

  if ( (last2->ncell(0)) == head2 ) 
    last2->setcell(0,head1);
  else
    last2->setcell(1,head1);

  lptr1->size=newsize;
  lptr2->size=newsize;

  return newsize;
}

/*------------------------*/

void main() {

  cout << "TESTS ON LIST ONE " << endl;

  list* l1 = new list();    l1->print(); 

  l1->insert(3);            l1->print(); 
  l1->reverse();            l1->print();
  l1->reverse();            l1->print();

  l1->insert(5);            l1->print(); 
  l1->reverse();            l1->print();
  l1->reverse();            l1->print();

  l1->insert(7);            l1->print(); 
  l1->reverse();            l1->print();
  l1->reverse();            l1->print();
  l1->reverse();            l1->print();

  l1->insert(1);            l1->print(); 
  l1->reverse();            l1->print();

  l1->insert(9);            l1->print(); 
  l1->reverse();            l1->print();

  cout << "TESTS ON LIST TWO " << endl;

  list* l2 = new list();    l2->print(); 


  cout << "APPEND TWO TO ONE" << endl;

  append_list(l1,l2); 
  l1->print();

  l2->insert(-3);            l2->print(); 
  l2->insert(-2);            l2->print(); 
  l2->insert(-1);            l2->print(); 
  l2->reverse();            l2->print();
  l2->reverse();            l2->print();

  cout << "APPEND TWO TO ONE" << endl;

  append_list(l1,l2); 

  cout << "ONE is now "; l1->print();
  cout << "TWO is now "; l2->print();

cout << "RECHECK PROPERTIES OF ONE" << endl;

  l1->reverse();            l1->print();

  l1->insert(30);            l1->print(); 
  l1->reverse();            l1->print();
  l1->reverse();            l1->print();

cout << "RECHECK TWO" << endl;

  l2->print();
}

/*-----------------------------------------------------------------------*/

