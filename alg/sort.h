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
	SORT_SELECT,
	SORT_QUICK,
};

enum {
	INT_TYPE = 0x00,
	CHAR_TYPE,
	SELF_DEF_TYPE,
};

typedef int (*cmp_func_p)(char *d1, char *d2);
typedef int	(*swap_func_p)(char *d1, char *d2);
typedef int (*assign_func_p)(char *d1, char *d2);

typedef struct _sort_data_s {
	char 			*unsorted;
	int				e_size;
	int				e_num;
	int				method;
	cmp_func_p 		p_cmp;
	swap_func_p 	p_swap;
	assign_func_p 	p_assign;
}sort_data_t, *sort_data_p;

int sort_data_init(sort_data_p p_data, int type);

int sort(sort_data_p p_data);

#endif // __SORT_H
