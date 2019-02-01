/******************************************************************************
 * 
 * Name: 	Zaid Albirawi, Brandon Mathew
 * Email: 	zalbiraw@uwo.ca, bmathew8@uwo.ca
 *
 * simulate.c 
 *
 ******************************************************************************/

#include "simulate.h"

int memory, max_memory, mode, time_quantum;
FILE * fp;
d_linked_list_t * jobs;
pthread_mutex_t lock;

void * run(void *j) {
  //lock for mode and jobs...
  //pthread_mutex_lock(&lock);
  job_t *job = get_next_job(mode, jobs);
  //...unlock for mode and jobs
  //pthread_mutex_unlock(&lock);
  int number, required_memory;

  while (job != NULL) {

    number = job->number;
    required_memory = job->required_memory;

    /**********************************************************************
     * checks if the memory requested exceeds maximum memory
     **********************************************************************/
    //lock for max_memory and fp...
    pthread_mutex_lock(&lock);
    if (required_memory > max_memory) {
      /******************************************************************
       * inform user that the job won't run, clean and terminate
       ******************************************************************/
      print_exceed_memory(fp, number);
      //...unlock for max_memory and fp
      
      free(job);
      return NULL;
    }
    pthread_mutex_unlock(&lock);

    /**********************************************************************
     * runs job
     **********************************************************************/
    //lock for memory...
    //pthread_mutex_lock(&lock);
    if (required_memory <= memory){
    	execute_job(job);
    }
    //...unlock for memory
    //pthread_mutex_unlock(&lock);
    /**********************************************************************
     * checks if the memory requested exceeds current available memory
     **********************************************************************/
    else{
      /******************************************************************
       * inform user that the job doesn't have enough resources at the 
       * moment, add the job back to the list
       ******************************************************************/
      //lock for fp and jobs...
      pthread_mutex_lock(&lock);
      print_insufficient_memory(fp, number);

      enqueue(jobs, job);
      //...unlock for fp and jobs
      pthread_mutex_unlock(&lock);
    }
    //lock for mode and jobs...
    pthread_mutex_lock(&lock);
    job = get_next_job(mode, jobs);
    //...unlock for mode and jobs
    pthread_mutex_unlock(&lock);

  }

  return NULL;
}

/******************************************************************************
 * 
 ******************************************************************************/
void simulate(int memory_value, int mode_value, int time_quantum_value,
  d_linked_list_t * list) {
  pthread_mutex_init( & lock, NULL);
  /**************************************************************************
   * opens output file
   **************************************************************************/
  //pthread_mutex_lock(&lock);
  fp = fopen(SYSTEM_OUTPUT, WRITE);

  /**************************************************************************
   * check if the system file is in the directory
   **************************************************************************/
  if (fp == NULL) {
    printf("Unable to open %s file\n", SYSTEM_OUTPUT);
    exit(FAILURE);
  }
  //pthread_mutex_unlock(&lock);

  /**************************************************************************
   * set global variables
   **************************************************************************/
  //pthread_mutex_lock(&lock);
  memory = max_memory = memory_value;

  /**************************************************************************
   * set executing mode
   **************************************************************************/
  mode = mode_value;
  time_quantum = time_quantum_value;
  print_mode(fp, mode);
  jobs = list;
  //pthread_mutex_unlock(&lock);

  /**************************************************************************
   * create threads and run jobs
   **************************************************************************/
  pthread_t threads[NUMBER_OF_THREADS];
  for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
    if (pthread_create( & threads[i], NULL, run, NULL)) {
      printf("Error: failed to create thread.\n");
      exit(FAILURE);
    }
  }

  /**********************************************************************
   * wait for the jobs to finish executing
   **********************************************************************/
  for (int i = 0; i < NUMBER_OF_THREADS; ++i){
    pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&lock);
}

void execute_job(job_t *job) {
  int number = job->number,

    required_memory = job->required_memory;

  //lock for mode...
  //pthread_mutex_lock(&lock);
  if (mode == 3) {
    round_robin(job);
  }
  //...unlock for mode
  //pthread_mutex_unlock(&lock);

  else {

    /******************************************************************
     * inform user that the job started executing and allocate mrmory
     ******************************************************************/
    //lock for fp...
    pthread_mutex_lock(&lock);
    print_starting(fp, number);
    //...unlock for fp
    pthread_mutex_unlock(&lock);
    allocate_memory(required_memory);

    /******************************************************************
     * run the job
     ******************************************************************/
    
    sleep(job->required_time);

    /******************************************************************
     * inform user that the job finished executing
     ******************************************************************/
    //lock for fp...
    pthread_mutex_lock(&lock);
    print_completed(fp, number);
    //...unlock for fp
    pthread_mutex_unlock(&lock);
    free(job);

    /******************************************************************
     * deallocate memory
     ******************************************************************/
    deallocate_memory(required_memory);
  }
}

void allocate_memory(int r) {
  //lock for memory...
  pthread_mutex_lock(&lock);
  memory -= r;
  print_allocate_memory(fp, memory, r);
  //...unlock for memory
  pthread_mutex_unlock(&lock);
}

void deallocate_memory(int r) {
  //lock for memory and fp...
  pthread_mutex_lock(&lock);
  memory += r;
  print_deallocate_memory(fp, memory, r);
  //...unlock for memory and fp
  pthread_mutex_unlock(&lock);
}

/*round robin algorithm*/
void round_robin(job_t *j2) {
  
  int number = j2->number;
  print_starting(fp, number);
    allocate_memory(j2->required_memory);
 
  if (j2->required_time > time_quantum) {
    
   //pthread_mutex_lock(&lock);
  //print_starting(fp, number);
 //pthread_mutex_unlock(&lock);
    sleep(time_quantum);
    //calculate the new required time,
    j2->required_time = j2->required_time - time_quantum;
    //and enqueue it.
    pthread_mutex_lock(&lock);
    //print_allocate_memory(fp, memory, j2->required_memory);
    enqueue(jobs, (void*) j2);
    pthread_mutex_unlock(&lock);
    
  }
      


  else{
    pthread_mutex_lock(&lock);
  print_starting(fp, number);
  pthread_mutex_unlock(&lock);
//allocate_memory(j2->required_memory);
    sleep(j2->required_time);
    deallocate_memory(j2->required_memory);
    pthread_mutex_lock(&lock);
    //print_deallocate_memory(fp, memory, j2->required_memory);

    print_completed(fp, number);
    pthread_mutex_unlock(&lock);
  }
  //pthread_mutex_unlock(&lock);

}
