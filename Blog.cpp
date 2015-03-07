#include "libtest/DbmsApi.h"
#include "Blog.h"

using namespace std;

/*
*	Post Function 
*		Definitions
*/

// Constructors
Post::Post() : title(""), author(""), content(""), allowComments(false), date(""), postId(-1), parentId(-1) {}
Post::Post(string ti = "", string au = "", string co = "", string day = "", bool allow = false, int post = -1, int parent = -1) {
	title = ti;
	author = au;
	content = co;
	date = day;
	allowComments = allow;
	postId = post;
	parentId = parent;
}
// Setters
void Post::setTitle(string ti) {title = ti;}
void Post::setAuthor(string au) {author = au;}
void Post::setContent(string co) {content = co;}
void Post::setCommenting(bool allow) {allowComments = allow;}
void Post::setDate(string day) {date = day;}
void Post::setPostID(int id) {postId = id;}
void Post::setParentID(int id) {parentId = id;}
// Getters
string Post::getTitle() {return title;}
string Post::getAuthor() {return author;}
string Post::getContent() {return content;}
string Post::getDatePosted() {return date;}
bool Post::getCommenting() {return allowComments;}
int Post::getPostID() {return postId;}
int Post::getParentID() {return parentId;}




int main() {
	DBMSParse("CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);");
	Table tab = GetRelation("animals");
	vector<string> first = {"Joe", "dog", "41"};
	tab.addTuple(first);
	cout << tab.getTuple(0)[0] << tab.getTuple(0)[1] << endl;

	return 0;
}
