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
		Post();
		Post(string ti, string au, string co, bool allow, string day, int post, int parent);
		void setTitle(string ti);
		void setAuthor(string au);
		void setContent(string co);
		void setCommenting(bool allow);
		void setDate(string day);
		void setPostID(int id);
		void setParentID(int id);
		string getTitle();
		string getAuthor();
		string getContent();
		string getDatePosted();
		bool getCommenting();
		int getPostID();
		int getParentID();
};