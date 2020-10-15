/* sort 
   Implemented using a lheap as a PQ, and written as a filter.
*/

#include <stream.h>

typedef int boolean;

const boolean TRUE=1;
const boolean FALSE=0;

#include "lheap.h"
#include "lheap.cc"

int main() {

  /* Collect input from stdin, building up a lheap_list. */

  lheap_list lh_lst;

  int kv;

  while (1) {

    cin >> kv;

    if ( kv == -999 ) break;

    lheap* lh_ptr = new lheap(kv);

    lh_lst.insert( lh_ptr );
  }

  cout << "starting deletemin's " << endl;

  /* When input is finished, then make a new lheap from the lheap_list,
     and then write out the elements by repeated deletemin until the lheap
     is empty */

  lheap* lh_ptr = new lheap( &lh_lst ); /* will initialise by round robin */

  while ( lh_ptr->notempty() ) {
    cout << lh_ptr->deletemin() << endl;
  }
}

/* Transcript 

[wrigley:513] g++ L-main.cc

[wrigley:513] a.out
6
3
4
2
5
1
-999
starting deletemin's 
1
2
3
4
5
6

*/
