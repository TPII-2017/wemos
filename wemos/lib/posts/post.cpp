#include "post.h"

PostMatrix::PostMatrix(void)
{
	type = t_matrix;
}

PostPhrase::PostPhrase(string s)
{
	type = t_phrase;
	str = s;
}
string PostPhrase::getStr(void)
{
	return str;
}

PostPredef::PostPredef(int var)
{
	type = t_predef;
	id = var;
}
int PostPredef::getId(void)
{
	return id;
}
