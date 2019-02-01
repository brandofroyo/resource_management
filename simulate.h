/******************************************************************************
* 
* Name: 	Zaid Albirawi, Brandon Mathew
* Email: 	zalbiraw@uwo.ca, bmathew8@uwo.ca
*
* simulate.h
*
******************************************************************************/

#ifndef SIMULATE_H
#define SIMULATE_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "scheduler.h"
#include "job.h"
#include "d_linked_list.h"
#include "helper.h"

#define SYSTEM_OUTPUT "system.out"
#define NUMBER_OF_THREADS 4

void* run();
void simulate(int, int, int, d_linked_list_t*);
void execute_job(job_t*);
void allocate_memory(int);
void deallocate_memory(int);
void round_robin(job_t*);

#endif