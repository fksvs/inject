#include <stdlib.h>
#include "random.h"

unsigned int rand_addr()
{
	return rand();
}

unsigned short rand_port()
{
	return rand() % 65535;
}
