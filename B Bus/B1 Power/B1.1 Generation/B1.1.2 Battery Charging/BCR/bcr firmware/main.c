#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "bcr.h"
#include "bcr_test.h"

int main (void)
{
	init ();
	
	bcrTest ();	

	return 0;
}
