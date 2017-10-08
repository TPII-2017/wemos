#include "postarray.h"
#include <iostream>
using namespace std;

int main()
{
	PostArray cola;
	
	cola.push(new PostPredef(7));	
	cola.push(new PostPhrase("Hola wachin"));
	cola.push(new PostMatrix());
	cola.push(new PostPhrase("Segundo coso"));
	
	for (int i = 0; i < cola.size(); i++)
	{
		Post * aux = cola.get(i);
		switch ((*aux).type)
		{
		case t_matrix:
			cout << "Post de tipo matrix\n";
			//cout << "Value: " << static_cast<PostMatrix*>(array[size])->getId() << "\n";
			break;
		case t_phrase:
			cout << "Post de tipo phrase\n";
			cout << "Value: " << static_cast<PostPhrase*>(aux)->getStr() << "\n";
			break;
		case t_predef:
			cout << "Post de tipo predef\n";
			cout << "Value: " << static_cast<PostPredef*>(aux)->getId() << "\n";
			break;
		}
	}
	
	return 0;
}