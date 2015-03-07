#include "DbmsApi.h"
#include "Blog.h"

using namespace std;

/*
*	Post Function 
*		Definitions
*/

// Constructors
Post::Post() : title(""), author(""), content(""), allowComments(false), date(""), postId(-1), parentId(-1) {}
Post::Post(Post& p) {
	title = p.title;
	author = p.author;
	content = p.content;
	date = p.date;
	allowComments = p.allowComments;
	postId = p.postId;
	parentId = p.parentId;
}
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
void Post::setTags(vector<string> t) {tags = t;}
// Getters
string Post::getTitle() {return title;}
string Post::getAuthor() {return author;}
string Post::getContent() {return content;}
string Post::getDatePosted() {return date;}
vector<string> Post::getTags() {return tags;}
bool Post::getCommenting() {return allowComments;}
int Post::getPostID() {return postId;}
int Post::getParentID() {return parentId;}





int main() {
	Blog app;
	bool done = false;
	char option = '0';

	// Initiate Blog App
	app.MenuSequence();

	cout << endl << endl << "**--**   TESTING OTHER TEAM's DBMS   **--**" << endl;
	// Command to create sample animals table
	DBMSParse("CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);");

	// Get table from DBMS 
	Table tab = GetRelation("animals");

	// Insert a row into table
	vector<string> first = {"Joe", "dog", "41"};
	tab.addTuple(first);


	cout << tab.getTuple(0)[0] << "  " << tab.getTuple(0)[1] << endl;
	cout << "**--**         TESTING ENDED         **--**" << endl << endl;

	return 0;
}
