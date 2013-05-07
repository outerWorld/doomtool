
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort.h"

// support 
//		basic types: int, char *, char**
//		self defined types
//
static int sort_ins(void *data, int size, int elem_size, cmp_func_p p_cmp, swap_func_p p_swap)
{
}

int sort(sort_data_p p_data)
{

	switch (p_data->method) {
		case SORT_INSERTION:
			break;
		case SORT_BUBBLE:
			break;
		case SORT_QUICK:
			break;
		default:
			return -1;
	}

	return 0;
}
