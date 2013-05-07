/*
 * -*- encoding=utf-8 -*-
 * Author: huang chunping
 * Date	 : 2013-05-07
 * Desc	 : sort algorithm
 */

#ifndef __SORT_H
#define __SORT_H

enum {
	SORT_INSERTION = 0x00, //
	SORT_BUBBLE,
	SORT_QUICK,
};

typedef int (*cmp_func_p)(void *d1, void *d2);
typedef int	(*swap_func_p)(void *d1, void *d2);

typedef struct _sort_data_s {
	void 		*unsorted;
	int			e_size;
	int			e_num;
	int			method;
	cmp_func_p 	p_cmp;
	swap_func_p p_swap;
}sort_data_t, *sort_data_p;

int sort(sort_data_p p_data);

#endif // __SORT_H
