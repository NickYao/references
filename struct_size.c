// struct_size.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"


#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "list.h"
enum Type{
	CHAR = 1,
	SHORT = 2,
	INT = 3,
	DOUBLE = 4,
};

struct list_head head = {0};
int nested_flag = 0;
struct size{
	struct list_head list;
	int alignment;
	int is_struct;	
};

int start_struct(void)
{
	INIT_LIST_HEAD(&head); 
//	printf("%s %d\n", __func__, __LINE__);
	return 0;
}

int add_basic_type(enum Type type)
{
	struct size *sz = (struct size *)malloc(sizeof(struct size));
//	printf("%s %d\n", __func__, __LINE__);
	if (NULL == sz)
		return 1;
	
	if ((0 > type) || (DOUBLE < type)) 
		return 1;	
	
	memset(sz, 0, sizeof(*sz));
	if (nested_flag)
	{
		sz->is_struct = 1;
	}
	switch (type)
	{
		case INT:
			sz->alignment = 4;
			break;
		case CHAR:
			sz->alignment = 1;
			break;
		case SHORT:
			sz->alignment = 2;
			break;
		case DOUBLE:
			sz->alignment = 8;
			break;
		default:
			break;
	}
	list_add(&(sz->list), &head); 
	return 0;	
}

int add_array(enum Type type, unsigned int num)
{
	int i = 0;
	int ret = 0;
//	printf("%s %d\n", __func__, __LINE__);
	for (i = 0; i < num; i++)
	{
		ret = add_basic_type(type);
		if (0 != ret)
			return ret;
	}
	return 0;	
} 

int begin_nested_struct(void)
{
	nested_flag = 1;
	return 0;	
}

int end_nested_struct(void)
{
	nested_flag = 0;
	return 0;
}

int finish_struct(unsigned int *size)
{
	struct list_head *pos = NULL;
	int max_align = 0;
	struct size *sz = NULL;
	int offset = 0;
//	printf("%s %d\n", __func__, __LINE__);
	list_for_each(pos, &head)
	{
		sz = list_entry(pos, struct size, list); 
		if (sz->alignment > max_align)
			max_align = sz->alignment;
	}
//	printf("%s %d\n", __func__, __LINE__);
	list_for_each(pos, &head)
	{
		sz = list_entry(pos, struct size, list);
		offset += sz->alignment;
		while (offset % sz->alignment)
			offset++;
	}
//	printf("%s %d\n", __func__, __LINE__);
	while (offset % max_align)
	{
		offset++;
	}
	*size = offset;
//	printf("%s %d\n", __func__, __LINE__);
	list_for_each(pos, &head)
	{
		sz = list_entry(pos, struct size, list);
		list_del(pos);
		free(sz);
	}
//	printf("%s %d\n", __func__, __LINE__);
	return 0;
}

struct test {
	double d;
	int i;
	struct {
		char c;
		int j;
	}s;
	short k;	
};

int main()
{
	unsigned int size = 0;
	
	printf("sizeof double = %d\n", sizeof(double));
	printf("sizeof long = %d\n", sizeof(long));
	printf("sizeof int = %d\n", sizeof(int));
	printf("sizeof struct test = %d\n", sizeof(struct test));
//	printf("%s %d\n", __func__, __LINE__);
	start_struct();
	add_basic_type(DOUBLE);
	add_basic_type(INT);
	begin_nested_struct();
	
	add_basic_type(CHAR);
	add_basic_type(INT);
	end_nested_struct();
	finish_struct(&size);
	
	return 0;
}
