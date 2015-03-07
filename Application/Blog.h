#include <string>
#include <vector>
#include <iostream>
#include <limits>


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
		Post();
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
		bool done;
		vector<Post> posts;
		vector<Post> currentPosts;
	public:
		Blog() {
			mainMenu = "[Main Menu]\n\n1. Make a new post\n2. Search for a post\n3. Exit\n\n";
		    searchMenu = "[Search Menu]\n\nSearch by:\n1. Author\n2. Title\n3. Tag(s)\n4. Date\n5. Return to Main Menu\n\n";
		    postMenu = "1. View\n2. Edit\n3. Delete\n4. Comment\n5. Return to Main Menu\n\n";
			commandPrompt = "* Enter command: ";
			done = false;	
		}

		void displayPost(int p) {
			char option = '0';
			if (p < 0 || p >= currentPosts.size()) {
				cout << "\n\n--INCORRECT SELECTION--\n\n";
				return;
			}
			else {
				cout << '[' << currentPosts[p].getTitle() << "]\n\n" << postMenu << commandPrompt;
				cin >> option;
				cout << endl;
				switch (option) {
					case '1':
						cout << "\n\nView option selected\n\n";
						break;
					case '2':
						cout << "\n\nEdit option selected\n\n";
						break;
					case '3':
						cout << "\n\nDelete option selected\n\n";
						break;
					case '4':
						cout << "\n\nComment option selected\n\n";
						break;
					case '5':
						break;
					default:
						cout << "Unknown command..\n" << endl;
						break;
				}
			}
		}

		void displayCurrentPosts() {	
			int index = -1;		
			for (int i = 1; i <= currentPosts.size(); ++i) {
				cout << i << ". " << currentPosts[i-1].getTitle() << " (" << currentPosts[i-1].getDatePosted() << ")\n";
			}
			cout << currentPosts.size() + 1 << ". Return to Main Menu\n\n";
			cout << "* Enter ID: ";
			if (cin >> index);  
			else {
				cin.clear(); 
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\n\nYou break it, you buy it...\n\n";
				return;	 
			}
			displayPost(index);
		}

		
		void searchAuthor(string name) {
			for (int i = 0; i < posts.size(); ++i) {
				if (posts[i].getAuthor() == name) {
					Post temp(posts[i]);
					currentPosts.push_back(temp);
				}
			}
			if (currentPosts.size() > 0) {
				cout << '[' << name << "'s Posts]\n\n";
				displayCurrentPosts();
			}
			else cout << "No Posts Found\n\n";
		}

		void searchTitle(string title) {
			char option = '0';
			for (int i = 0; i < posts.size(); ++i) {
				if (posts[i].getTitle() == title){
					Post temp(posts[i]);
					currentPosts.push_back(temp);
				}
			}
			if (currentPosts.size() > 0) {
				cout << '[' << title << "'s Posts]\n\n";
				displayCurrentPosts();
			}
			else cout << "No Posts Found\n\n";
		}
		void searchTags(string tag) {
			vector<string> tags;
			tags.push_back(tag);
			// Must parse argument for multiple tags
			for (int i = 0; i < posts.size(); ++i) {
				if (posts[i].getTags() == tags)	{
					Post temp(posts[i]);
					currentPosts.push_back(temp);
				}
			}
			if (currentPosts.size() > 0) {
				cout << '[' << tag << "'s Posts]\n\n";
				//displayCurrentPosts();
			}
			else cout << "No Posts Found\n\n";
		}
		void searchDate(string day) {
			for (int i = 0; i < posts.size(); ++i) {
				if (posts[i].getDatePosted() == day){
					Post temp(posts[i]);
					currentPosts.push_back(temp);
				}
			}
			if (currentPosts.size() > 0) {
				cout << '[' << day << "'s Posts]\n\n";
				displayCurrentPosts();
			}
			else cout << "No Posts Found\n\n";
		}
		

		void SearchSequence() {
			char option = '0';
			string searchFor = "";
			cout << searchMenu << commandPrompt;
			cin >> option;
			cout << endl;
			switch (option) {
				case '1':
					cout << "* Enter author: ";
					cin >> searchFor;
					cout << endl;
					searchAuthor(searchFor);
					break;
				case '2':
					cout << "* Enter title: ";
					cin >> searchFor;
					cout << endl;
					searchTitle(searchFor);
					break;
				case '3':
					cout << "* Enter tags (seperated by commas): ";
					cin >> searchFor;
					cout << endl;
					searchTags(searchFor);
					break;
				case '4':
					cout << "* Enter date: ";
					cin >> searchFor;
					cout << endl;
					searchDate(searchFor);
					break;
				case '5':
					break;
				default:
					cout << "Unknown command..\n" << endl;
					break;
			}
		}

		void MenuSequence() {
			while (!done) {
				char option = '0';
				cout << mainMenu << commandPrompt;
				cin >> option;
				cout << endl;
				switch (option) {
					case '1':
						cout << "New post selected\n" << endl;
						break;
					case '2':
						SearchSequence();
						break;
					case '3':
						cout << "Exit selected\n" << endl;
						done = true;
						break;
					default:
						cout << "Unknown command..\n" << endl;
						break;
				}
			}	
		}

};