#ifndef CYCLE_PHASE_H
#define CYCLE_PHASE_H

/* Example 
	- "Sunset"
	- 2700 K
	- 1500 Lumens
*/
	
typedef struct Cycle_Phase{

	char Title[10];
	int ColorTemperature; // i.e. 5000 => 5000K
	int Lumens; // ? 

	//float Intensity; // percentage i.e. 0.80 => 80%
} Cycle_Phase;

#endif