#include <string>
#include <vector>
#include <algorithm>
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
		Post(vector<string> rawData);
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
		bool containsTags(vector<string> tags);
		bool getCommenting();
		int getPostID();
		int getParentID();
};

class Blog {
	private:
		string mainMenu;
		string searchMenu;
		string editMenu;
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

		// Initialization functions
		void loadSavedPosts();
		// Post modification
		void editPost(int p);
		void deletePost(int p);
		// Search functions	
		void searchAuthor(string name);
		void searchTitle(string title);
		void searchTags(string tag);
		void searchDate(string day);
		// Result functions
		void displayPostMenu(int p);
		void displayCurrentPosts();
		void printPost(Post post);
		void printComments(int parentId);
		// Sequence functions
		void newPostSequence(int parent);
		bool LoginSequence();
		bool RegisterSequence();
		void SearchSequence();
		void MenuSequence();
	public:
		Blog();
		// Login function
		bool login();
		// Function to start Blog app
		void initiateBlog();

};