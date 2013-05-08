
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort.h"

#define _ALLOC(pbuf, type, size, ret) \
pbuf = (type)malloc(size); \
if (!pbuf) return (ret); \
memset(pbuf, 0x00, size);

#define _FREE(pbuf) free(pbuf);

static int int_cmp(char *d1, char *d2)
{
	return (*(int*)d1) - (*(int*)d2);
}

static int int_assign(char *d1, char *d2)
{
	return (*(int*)d2) = (*(int*)d1), 0;
}

int sort_data_init(sort_data_p p_data, int type)
{
	switch (type) {
		case INT_TYPE:
			p_data->e_size = sizeof(int);
			p_data->p_cmp = int_cmp;
			p_data->p_assign = int_assign;
			break;
		case CHAR_TYPE:
			break;
		default:
			break;
	}
}

// support 
//		basic types: int, char *, char**
//		self defined types
//
static int sort_ins(char *data, int size, int elem_size, cmp_func_p p_cmp, assign_func_p p_assign)
{
	int i, j;
	char *temp = NULL;

	_ALLOC(temp, char*, elem_size, -1);

	for (i = 1; i < size; i++) {
		p_assign(data+i*elem_size, temp);

		for (j = i-1; j >= 0; j--) {
			if (p_cmp(temp, data+j*elem_size) < 0) {
				p_assign(data+j*elem_size, data+(j+1)*elem_size);
			} else break;
		}

		j++;
		if (j != i) p_assign(temp, data+j*elem_size);
	}

	_FREE(temp);

	return 0;
}

int sort(sort_data_p p_data)
{

	switch (p_data->method) {
		case SORT_INSERTION:
			return sort_ins(p_data->unsorted, p_data->e_num, p_data->e_size, p_data->p_cmp, p_data->p_assign);
		case SORT_BUBBLE:
			break;
		case SORT_QUICK:
			break;
		default:
			return -1;
	}

	return 0;
}
