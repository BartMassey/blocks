/*
Driving script for the heap-based sorting.

Must define
-DBUFF=n 
it will default to using a permutation of 0,...,n-1 for input
However if also define
-DPIPE : then will take up to n numbers from stdin (terminate with -999)

Other Options:
-DDEBUG_MAIN to get some pictures of the heap
-DDEBUG   for very gory details
-DBHEAP will use Binomial (else defaults to Fibonacci).

NOTE: if use Fibonacci then must also

-DLOG_BUFF=m with m=O(n) but definitely large enough to contain the
             possible largest degree taht can occur.
*/

#include <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef int boolean;
const boolean TRUE = 1;
const boolean FALSE = 0;

void tab_by( int indent ) { /* used by the print routines */
  while( indent > 0 ) {
    cout << " " ;
    indent--;
  }
}
#ifdef BHEAP
#include "B-heap.h"
#include "B-heap.cc"
#else
#include "F-heap.h"
#include "F-heap.cc"
#endif

time_t tnow;
struct timeval tp;
struct timezone tzp;
long init_sec, init_usec;
long end_sec, end_usec;

int random_int(int low, int high) { /* random int in the range low,...,high */
  return low + rand()%(high-low+1);
}

/*--------------------------------*/

main() {
     
  int in_buf[BUFF];
  int out_buf[BUFF];
  int i;

#ifdef PIPE
  /* read number from stdin into in_buf */
  for ( i = 0 ; i < BUFF ; i++ ) {
    cin >> in_buf[i];
    if ( in_buf[i] == -999 )
      break;
  }
#else
  /* put 0,...,BUFF-1 into in_buf and shuffle them */
  for ( i = 0 ; i < BUFF ; i++ ) {
    in_buf[i] = i;
  }
  time(&tnow);
  srand(tnow);
  cout << "shuffling_seed " << tnow << endl;
  int j,tmp;
  for ( i = 0 ; i < BUFF ; i++ ) {
    j = random_int(i,BUFF-1);
    tmp = in_buf[i];
    in_buf[i]=in_buf[j];
    in_buf[j]=tmp;
  }
#endif

  gettimeofday(  &tp, &tzp );
  init_sec =  tp.tv_sec;
  init_usec = tp.tv_usec;

#ifdef BHEAP
  cout << "Using binomial heap" << endl;
  B_heap* h_ptr = new B_heap();
#else
  cout << "Using Fibonacci heap" << endl;
  F_heap* h_ptr = new F_heap();
#endif

   /* read from buffer into the heap */
  int input;
  for ( i = 0 ; i < BUFF ; i++ ) {
    input = in_buf[i];
    if ( input == -999 )
      break;
    else
      (void) h_ptr->insert(input);
#ifdef DEBUG_MAIN
      cout << "After insertion of " << input << 
        " the heap state is now " << endl;
      h_ptr->print_keys(0);
#endif
  }

  cout << "insertions finished" << endl;

  gettimeofday(  &tp, &tzp );
  end_sec =  tp.tv_sec;
  end_usec = tp.tv_usec;
  cout << "seconds_taken_so_far " <<  
    (end_sec - init_sec) + (end_usec - init_usec)/1000000.0  << endl;

#ifdef DEBUG_MAIN
  cout << "The heap state is now " << endl;
  h_ptr->print_keys(0);
  cout << "Starting the deletemin's " << endl;
#else
#endif
  
  for ( i = 0 ; i < BUFF ; i++ ) {

    if ( h_ptr->empty() ) {
      if ( i < (BUFF-1) )
	out_buf[i] = -999;
      break;
    }
    else {
      out_buf[i] = h_ptr->deletemin();

#ifdef DEBUG_MAIN
      cout << "deletemin returned " << out_buf[i] << endl;
#endif

#ifdef DEBUG_MAIN
#ifndef PIPE
      if ( out_buf[i] != i ) 
	cout << "deletemin error at " << i << endl;
#endif
#endif

#ifdef DEBUG_MAIN
      cout << "The heap state is now  " << endl;
      h_ptr->print_keys(0);
#endif
    }
  }

  gettimeofday(  &tp, &tzp );
  end_sec =  tp.tv_sec;
  end_usec = tp.tv_usec;

  /* Finally print out the out_buf contents */

  cout << "Sorted numbers (or just the errors):" << endl;

  for ( i = 0 ; i < BUFF ; i++ ) {
    if ( out_buf[i] == -999 ) 
      break;
#ifndef PIPE
    if ( out_buf[i] != i ) 
      cout << "error out_buf[ " << i <<"] = " << out_buf[i] << endl;
#else
    cout <<  out_buf[i] << endl;
#endif
  }

  cout << "entries_sorted " << i << endl;
  cout << "seconds_taken " <<  
    (end_sec - init_sec) + (end_usec - init_usec)/1000000.0  << endl;

  delete h_ptr;
}






