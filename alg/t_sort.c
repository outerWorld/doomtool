
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort.h"

static int show_int_arr(int *arr, int num)
{
	int i = 0;

	for (i = 0; i < num; i++) {
		fprintf(stdout, "%d ", arr[i]);
	}
	fprintf(stdout, "\n");

	return 0;
}

int main(int argc, char *argv[])
{
	sort_data_t sort_data;
	int arr[5] = { 3, 2, 1, 5, 4};

	sort_data_init(&sort_data, INT_TYPE);
	sort_data.unsorted = (char*)arr;
	sort_data.e_num = sizeof(arr)/sizeof(arr[0]);
	sort_data.method = SORT_INSERTION;

	show_int_arr(arr, 5);
	sort(&sort_data);
	show_int_arr(arr, 5);

	return 0;
}
