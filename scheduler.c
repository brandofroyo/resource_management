/******************************************************************************
 * 
 * Name: 	Zaid Albirawi, Brandon Mathew
 * Email: 	zalbiraw@uwo.ca, bmathew8@uwo.ca
 *
 * scheduler.c 
 *
 ******************************************************************************/

#include "scheduler.h"

job_t * get_next_job(int mode, d_linked_list_t* jobs) {
  job_t *j;
  job_t *k;

  //for fcfs...
  if (mode == 0) {
    //dequeue the element
    j = dequeue(jobs);
  }

  //for lifo...
  if (mode == 1) {
    //pop the element
    j = pop(jobs);
  }







  //for sjf...
  if (mode == 2) {

j = dequeue(jobs);
int n = jobs->size;

for(int i = 0; i < n; i++){
	
k = dequeue(jobs);
if(j->required_time > k->required_time){
	enqueue(jobs, j);
	j = k;
}
else{
	enqueue(jobs, k);
}
}
}


  


  //for RR
  if (mode == 3) {

    j = dequeue(jobs);

  }

  return j;
}