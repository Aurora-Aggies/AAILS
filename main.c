#include <stdio.h>
#include "Cycle.h"
#include "Cycle_Phase.h"

/* IMPORTANT NOTES: */
// - Time: 
//   2000 => 20:00 (24 hr) => 8:00 PM (12 hr)

int main()
{
	Cycle c;
	c.DayHours = 10;

	printf("I have %d hours.\n", c.DayHours);

	return 0;
}