/************************************************************
Andrew Parkes      April, 1995

CIS 513 HW ONE Question 4b. For description see written notes 
*************************************************************/

#include <iostream.h>
#include <malloc.h>

struct pair {  int index;  int value; };

class array {
 private:

  int def_val; // what we would initialise array values to if we had time

  int n; // indices for array run from 0,...,n-1

  pair* pairs; // base of pairs[0,...,n-1]

  int j;   // means pairs[0,...,j-1] are valid

  pair** ptrs;  //  base of ptrs[0...n-1] that each point 
                //  to a pair (i,val(i)) if valid

 public:

  array(int array_size,int pseudo_init_value);

  int access(int i);  // if val(i) exists then return it, else return the 
                      // def_val (meaning the value we would have 
                      // initialised to if we had the time

  int update(int i, int v); // if val(i) is already set then reset it to v,
                            // else extend pairs[1...j] by a new pair, (i,v)
                            // make ptrs(i) point to this new pair
                            // and do j++
};

/*==================================================*/

 array::array(int size,int k) {
   n=size;
   pairs = (pair *) malloc( n*sizeof(pair));
   ptrs = (pair**) malloc(n*sizeof(pair*));
   j=-1; // so no pairs are valid
   def_val=k;
 }

/*-----------------------------------*/

int array::access(int i) {

  if ( i < 0 || i >= n ) {
    cout << "WARNING array::access. index out of range, returning default" 
                                                                       << endl;
    return(def_val);
  }

  pair* pair_ptr = ptrs[i];
  
  /*  if  *pair_ptr  is outside the controlled region or 
      if index in pair does not match then return the default */
  
  if (  pair_ptr < pairs      || 
      pair_ptr > pairs + j  || 
      pair_ptr->index  != i    ) return(def_val);
  
  /* if get to here then the value in the pair is valid, hence */
  
  return(  pair_ptr->value );
}

/*-----------------------------------*/

int array::update(int i, int v) {

    if ( i < 0 || i >= n ) {
      cout << "WARNING array::update. index out of range." << endl;
      return(1);
    }

    pair* pair_ptr = ptrs[i];

    if (    (pair_ptr < pairs)      ||  
            (pair_ptr > pairs + j)  || 
            (pair_ptr->index != i)     ) {

      /* pair_ptr was invalid, so set and validate the new pair */

      j++;
      
      ptrs[i] = pairs + j ;

      ptrs[i]->index=i;
      ptrs[i]->value=v;
      return(0);
    }
    else
    {
      /* valid value for i already exists, so just change it */

      pair_ptr->value = v;
      return(0);
    }
}

/*==================================================*/

main () {

  array* a1 = new array(10,999);

  cout << a1->access(2)    << endl;

  cout << a1->update(2,10) << endl;

  cout << a1->access(2)    << endl;

  cout << a1->update(2,12) << endl;

  cout << a1->access(2)    << endl;

  cout << a1->update(12,9) << endl;

  cout << a1->access(12)   << endl;
}

/*  RESULTS

[spark:513] a.out
999
0
10
0
12
WARNING array::update. index out of range.
1
WARNING array::access. index out of range, returning default
999

*/
