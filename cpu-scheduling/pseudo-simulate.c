/*
* get user input:
* 	process # -> save to n
* 	then loop on n, and populate process struct 
* 	each proc need arrival time, burst time
*
* struct process
* 	int process_id
* 	int arrivalTime
* 	int burstTime
*
* simulators:
* FCFS, RR
* 	return -> print order of proc, avg waiting time, avg turnaround time 
*
* so we have list of proc, main sort this list by arrival time, simulator(s) will take list
* FCFS:
* 	list input is exact order to print/execute on
* 	create 2d array of [n][2], where each arr will store wait, turnaround time
*	
*	int time = 0
*	double waitSum = 0;
*	double turnaroundSum = 0;
	init time to first arrival time
* 	iterate to n (n is len proc list)
* 		process_date[i][0] = time - arrival (set wait time)
*
* 		time += burst_time
* 		process_date[i][1] = process_date[i][0] + burst_time 
* 		waitSum += process_date[i][0]
* 		turnAroundSum += process_date[i][1]
*
* 	print process list
* 	print waitSum / n
* 	print turnaroundSum / n
*
* 		
* RR:
*
*
*
* 		
* 	
*
* /
