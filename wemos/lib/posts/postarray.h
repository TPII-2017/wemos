#ifndef POSTARRAY_H
#define POSTARRAY_H

#include "post.h"

#define MAX_SIZE 10

class PostArray
{
public:
	PostArray();
	int size();
	void push(Post *pointer);
	Post *get(int index);
	void remove(int index);

private:
	Post *queue[MAX_SIZE];
	int dim;
};

#endif