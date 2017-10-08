#include "postarray.h"

PostArray::PostArray()
{
	dim = 0;
}
int PostArray::size()
{
	return dim;
}
void PostArray::push(Post *pointer)
{
	//agregar al final si hay espacio
	if (dim == MAX_SIZE)
		return;
	queue[dim++] = pointer;
}
Post *PostArray::get(int index)
{
	if (index >= MAX_SIZE)
		return NULL;
	Post *aux = queue[index];
}
void PostArray::remove(int index)
{
	//TODO
}
