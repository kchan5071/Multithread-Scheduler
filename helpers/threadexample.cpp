#include <stdio.h>
#include <pthread.h>

#include <string>
#include <fstream>
#include <iostream>

#include "shared.h"

// NOTE this source code file is for simple demonstration
// of how command line parsing and pthreads work, 
// this file does NOT compile

// Thread for populating a dictionary tree
#include "populatetree.h"
// Thread for reading in prefixes and queuing them
#include "readprefix.h"
// Thread for searching and counting prefixes in a dictionary tree
#include "countprefix.h"

using namespace std; // Make sure std components accessible

void* print_message_function( void *ptr )
{
  char *message;
  message = (char *) ptr;
  printf("%s", message);
  pthread_exit(0);
}

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
    
  bool verbose = false;  // chatty or quiet program
  int option;   /* command line switch */
  int idx;  // general purpose index variable
  int count = 100;  // some counter set by -n, set default
  // other stuff (e.g. declarations)

  /*
   * This example uses the standard C library getopt (man -s3 getopt)
   * It relies on several external variables that are defined when
   * the getopt.h routine is included. On POSIX2 compliant machines
   * <getopt.h> is included in <unistd.h> and only the unix standard
   * header need be included.
   *
   * getopt takes:
   *    argc - Number of strings in argv
   *    argv - Strings for each token (contiguous characters in the
   *           command line. Example:
   *           ./program -n 56 -v boo bear
   *           argv[0] = "a.out", argv[1] = "-n", argv[2] = "56"
   *           argv[3] = "-v", argv[4] = "boo", argv[5] = "bear"
   *    optionstr - String indicating optional arguments to process
   *           Options with a : after them expect some type of value
   *           after them.  Example:  "n:o:v" n and o expect arguments,
   *           v does not
   */
  while ( (option = getopt(argc, argv, "n:o:v:")) != -1) {
    /* If the option has an argument, optarg is set to point to the 
     * argument associated with the option.  For example, if
     * -n is processed, optarg points to "56" in the example above.
     */
    switch (option) {
    case 'n': /* Assume this takes a number */
      /* optarg will contain the string following -n
       * -n is expected to be an integer in this case, so convert the
       * string to an integer.
       */
      count = atoi(optarg);  
      break;

    case 'o': /* optarg points to whatever follows -o */
      // optarg contains the output, do something appropriate
      break;

    case 'v':  /* optarg is undefined */
      verbose = true;
      break;

    default:
      // print something about the usage and exit
      exit(BADFLAG); // BADFLAG is an error # defined in a header
    }

  }

  /*
   * Once the getop loop is done external variable optind contains
   * a number.  This is the first argument of argv to process
   * after all options have been processed.
   * argv[optind] is the next mandatory argument.
   */

  int idx = optind;

  /* If idx < argc, there are mandatory arguments to process */
  if (idx < argc) {
    /* Process positional arguments:
     *argv[idx] argv[idx+1], ..., argv[argc-1]
     */

    // ...
    
    idx ++;
    
    // continue to process more mandatory arguments 
    // if there are any left
    
    // you SHOULD know how many of mandatory arguments
    // are there
  }

  // sample pthread code
  pthread_attr_t	pthread_attr_default;
  
  pthread_t thread1;
  char *message1 = "Hello World";
  
  pthread_attr_init(&pthread_attr_default);
  pthread_create( &thread1, &pthread_attr_default,
		  (void *) &print_message_function, (void *) message1);
          
  // shared data to be used for communication between threads
  // main thread, populatetree, readprefix, countprefix
  SHARED_DATA sharedData;

  // The C way
  //SHARED_DATA *sharedData = (SHARED_DATA*) malloc(sizeof(SHARED_DATA));

  //initialize the sharedData, you can have a helper to do it
  //...
                 
  // populatetree thread, more comments here of what this thread does
  pthread_t populateTreeThread;
  // readprefix thread, more comments here of what this thread does
  pthread_t readPrefixThread;
  // countprefix thread, more comments here of what this thread does
  pthread_t countPrefixThread;

  if (pthread_create(&populateTreeThread, NULL,
                     &populateTree, &sharedData)) // or use the pointer one directly
  {
    //error handling
  }

  if (pthread_create(&readPrefixThread, NULL,
                     &readPrefixToQueue, &sharedData))
  {
    //error handling
  }

  // create and start countPrefix thread
  //....

  // print progress bar for populatetree thread
  // use the SHARED_DATA
    
  // print progress bar for countprefix thread
  // note this needs to wait for the completion of readprefix thread
  // use the SHARED_DATA

  exit(NORMALEXIT); // Normal Exit!
}
