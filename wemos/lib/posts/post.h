#ifndef POST_H
#define POST_H

#include <string>

using namespace std;

enum post_type
{
	t_matrix,
	t_phrase,
	t_predef
};

class Post
{
  public:
	enum post_type type;
};

class PostMatrix : public Post
{
  public:
	PostMatrix();
	bool matrix[8][24];
};

class PostPhrase : public Post
{
  public:
	PostPhrase(string s);
	string getStr(void);

  private:
	string str;
};

class PostPredef : public Post
{
  public:
	PostPredef(int);
	int getId(void);

  private:
	int id;
};

#endif