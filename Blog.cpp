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
	// Command to create sample animals table
	bool done = false;
	string mainMenu = "[Main Menu]\n\n1. Make a new post\n2. Search for a post\n3.Exit\n\n";
	string commandPrompt = "* Enter command: ";
	char option = '0';
	while (!done) {
		cout << mainMenu << commandPrompt;
		cin >> option;
		switch (option) {
			case '1':
				cout << "\n New post selected" << endl;
				break;
			case '2':
				cout << "\n Search selected" << endl;
				break;
			case '3':
				cout << "\n Exit selected" << endl;
				done = true;
				break;
			default:
				cout << "\n Unknown command.." << endl;
				break;
		}
	}

	cout << endl << endl << "**--**   TESTING OTHER TEAM's DBMS   **--**" << endl;
	DBMSParse("CREATE TABLE animals (name VARCHAR(20), kind VARCHAR(8), years INTEGER) PRIMARY KEY (name, kind);");

	// Get table from DBMS 
	Table tab = GetRelation("animals");

	// Insert a row into table
	vector<string> first = {"Joe", "dog", "41"};
	tab.addTuple(first);


	cout << tab.getTuple(0)[0] << "  " << tab.getTuple(0)[1] << endl;
	cout << "**--**   TESTING ENDED   **--**" << endl << endl;

	return 0;
}
