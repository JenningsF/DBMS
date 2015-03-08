#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <limits>
#include <fstream>
#include <climits>
#include <time.h>

using namespace std;

class Post {
	private:
		string title;
		string author;
		string content;
		string date;
		vector<string> tags;
		bool allowComments;
		int postId;
		int parentId;
	public:
		Post(const Post& p);
		Post(string ti, string au, string co,  string day, bool allow, int post, int parent);
		void setTitle(string ti);
		void setAuthor(string au);
		void setContent(string co);
		void setCommenting(bool allow);
		void setDate(string day);
		void setPostID(int id);
		void setParentID(int id);
		void setTags(vector<string> t);
		vector<string> getTags();
		string getTitle();
		string getAuthor();
		string getContent();
		string getDatePosted();
		bool getCommenting();
		int getPostID();
		int getParentID();
};

class Blog {
	private:
		string mainMenu;
		string searchMenu;
		string commandPrompt;
		string postMenu;
		string loginPrompt;
		string loginSequence;
		string registerSequence;
		string author;
		bool done;
		bool done2;
		vector<Post> currentPosts;
		vector<Post> posts;
		DBengine DB;
		Relation *rel;
		string tableName;
	public:
		Blog();

		// Login functions
		bool login();
		bool LoginSequence();
		bool RegisterSequence();

		// Result functions
		void displayPost(int p);
		void displayCurrentPosts();	

		// Search functions	
		void searchAuthor(string name);
		void searchTitle(string title);
		void searchTags(string tag);
		void searchDate(string day);

		// Sequence functions
		void newPostSequence(int parent);
		void SearchSequence();
		void MenuSequence();

};