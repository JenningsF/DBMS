#include <string>
#include <vector>
#include <iostream>


using namespace std;

class Post {
	private:
		string title;
		string author;
		string content;
		string date;
		bool allowComments;
		int postId;
		int parentId;
	public:
		Post() : title(""), author(""), content(""), allowComments(false), date(""), postId(-1), parentId(-1) {}
		Post(string ti, string au, string co, bool allow, string day, int post, int parent);
		void setTitle(string ti) {title = ti;}
		void setAuthor(string au) {author = au;}
		void setContent(string co) {content = co;}
		void setCommenting(bool allow) {allowComments = allow;}
		void setDate(string day) {date = day;}
		void setPostID(int id) {postId = id;}
		void setParentID(int id) {parentId = id;}
		string getTitle() {return title;}
		string getAuthor() {return author;}
		string getContent() {return content;}
		string getDatePosted() {return date;}
		bool getCommenting() {return allowComments;}
		int getPostID() {return postId;}
		int getParentID() {return parentId;}
};