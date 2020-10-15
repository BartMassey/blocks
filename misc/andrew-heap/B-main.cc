#include <iostream.h>
#include <stdlib.h>
#include <time.h>

#include <sys/time.h>

typedef int boolean;

const boolean TRUE = 1;
const boolean FALSE = 0;

#include "B-heap.h"
#include "B-heap.cc"

const int BUFF=10;

int in_buf[BUFF];
int out_buf[BUFF];

struct timeval tp;
struct timezone tzp;
long init_sec, init_usec;
long end_sec, end_usec;

int random_int(int low, int high) {
  int out = rand();
  out = low + out%(high-low+1);
//  cout << "rand low=" << low << " high=" << high << " out= " << out << endl;
  return out;
}
  

main() {

  system("date");

  int i;


  /* read the numbers into a buffer */
/*
  for ( i = 0 ; i < BUFF ; i++ ) {
    cin >> in_buf[i];
    if ( in_buf[i] == -999 )
      break;
  }
*/


  /* put 0,...,BUFF-1 into the buffer and shuffle them */
   
  for ( i = 0 ; i < BUFF ; i++ ) {
    in_buf[i] = i;
  }
   
  
/* 
  for ( i = 0 ; i < BUFF ; i++ ) {
    cout << in_buf[i] << endl;
  }
*/

  time_t tnow;
  time(&tnow);
  srand(tnow);
  cout << "seed " << tnow << endl;
   
  int j;
  int tmp;
  for ( i = 0 ; i < BUFF ; i++ ) {
    j = random_int(i,BUFF-1);
    tmp = in_buf[i];
    in_buf[i]=in_buf[j];
    in_buf[j]=tmp;
  }

cout << "shuffled " << endl;
  
/* 
  for ( i = 0 ; i < BUFF ; i++ ) {
    cout << in_buf[i] << endl;
  }
*/

cout << "end shuffled " << endl;

  time(&init_sec);

  gettimeofday(  &tp, &tzp );
  cout << tp.tv_sec << " " << tp.tv_usec << endl;


  gettimeofday(  &tp, &tzp );
  cout << tp.tv_sec << " " << tp.tv_usec << endl;

  gettimeofday(  &tp, &tzp );
  cout << tp.tv_sec << " " << tp.tv_usec << endl;



   /* read from buffer into the heap */

  int input;
  B_heap* bh_ptr = new B_heap();
  
  for ( i = 0 ; i < BUFF ; i++ ) {
    input = in_buf[i];
    if ( input == -999 )
      break;
    else
      bh_ptr->insert(input);
  }

  cout << "insertions finished" << endl;

  time(&end_sec);
  cout << "seconds_taken_so_far " <<  end_sec - init_sec << endl;

#ifdef DEBUG_MAIN
  cout << " Insertions are now finished and the heap state is " << endl;
  bh_ptr->print_keys(0);
  cout << "Starting the deletemin's " << endl;
#endif
  
  for ( i = 0 ; i < BUFF ; i++ ) {

    if ( bh_ptr->empty() ) {
      if ( i < (BUFF-1) )
	out_buf[i] = -999;
      break;
    }

    out_buf[i] = bh_ptr->deletemin();

#ifdef DEBUG_MAIN
    cout << "deletemin returned " << out_buf[i] << endl;
    if ( out_buf[i] != i ) 
      cout << "deletemin error at " << i << endl;
//    cout << " the heap state is now  " << endl;
//    bh_ptr->print_keys(0);
#endif

  }

  time(&end_sec);

/*
  for ( i = 0 ; i < BUFF ; i++ ) {
    if ( out_buf[i] == -999 ) 
      break;
    cout <<  out_buf[i] << endl;
  }
*/


  for ( i = 0 ; i < BUFF ; i++ ) {
    if ( out_buf[i] != i ) 
      cout << "error out_buf[ " << i <<"] = " << out_buf[i] << endl;
  }

  cout << "entries_sorted " << i << endl;
  cout << "seconds_taken " <<  end_sec - init_sec << endl;

  delete bh_ptr;
}






