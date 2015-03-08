#include "DbmsApi.h"
#include "Blog.h"

using namespace std;

/*
*	Post Function 
*		Definitions
*/

// Constructors
Post::Post() : title(""), author(""), content(""), allowComments(false), date(""), postId(-1), parentId(-1) {}
Post::Post(const Post& p) {
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


/*
*	Blog Application
*		Function
*			Definitions
*/
// Constructor
Blog::Blog() {
	mainMenu = "[Main Menu]\n\n1. Make a new post\n2. Search for a post\n3. Exit\n\n";
    searchMenu = "[Search Menu]\n\nSearch by:\n1. Author\n2. Title\n3. Tag(s)\n4. Date\n5. Return to Main Menu\n\n";
    postMenu = "1. View\n2. Edit\n3. Delete\n4. Comment\n5. Return to Main Menu\n\n";
	commandPrompt = "* Enter command: ";
	loginPrompt = "[Login / Register]\n\n1. Login\n2. Register\n3. Exit\n\n";
	loginSequence = "[Login]\n\n";
	registerSequence = "[Register]\n\n";
	done = false;	
	done2 = false;
}

// This function displays the post menu, allowing for a selection of 
// view, edit, delete or comment. This function needs it's implementation finished
void Blog::displayPost(int p) {
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
				cin.clear(); 
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Unknown command..\n" << endl;
				break;
		}
	}
}

// Display intermediate results from search
// i.e. multiple posts with same author, date, etc..
void Blog::displayCurrentPosts() {	
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
	// Calls display on selected post
	displayPost(index);
}

// Searches for posts that has a matching author
void Blog::searchAuthor(string name) {
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
	else  {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "No Posts Found\n\n";
	}
}

// Searches for posts that match the specified title
void Blog::searchTitle(string title) {
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
	else {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "No Posts Found\n\n";
	}
}

// Incomplete search feature, must parse tag argument and
// insert each tag into vector of tags to compare with Post tags
void Blog::searchTags(string tag) {
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
	else  {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "No Posts Found\n\n";
	}
}

// Searches for post that match the specified date
// parsing date could be added
void Blog::searchDate(string day) {
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
	else  {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "No Posts Found\n\n";
	}
}

// Sequence containing search menu
void Blog::SearchSequence() {
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
			cin.clear(); 
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Unknown command..\n" << endl;
			break;
	}
}

// Begins main menu of blog application, does not end until Exit option is selected
void Blog::MenuSequence() {
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
				cin.clear(); 
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Unknown command..\n" << endl;
				break;
		}
	}	
}


bool Blog::LoginSequence() {
	string name = "";
	string first_name;
	string last_name;
	cout << loginSequence << "* First Name: ";
	cin >> first_name;
	cout << "* Last Name: ";
	cin >> last_name;
	cout << endl;
	name = first_name + " " + last_name;
	string u_name;
	string line = "";
	ifstream infile;
	infile.open("credentials.txt");
	if (!infile.is_open())
	{
		cout << "+ Error: credentials file was not opened properly\n" << endl;
		return false;
	}
	while (getline(infile, line)) {
		if (line.empty()) { continue; }
		if (name == line) {
			infile.close();
			author = name;
			return true;
		}
	}
	infile.close();
	cout << "+ Name not found\n" << endl;
	return false;
}

bool Blog::RegisterSequence() {
	cout << registerSequence;
	bool isDone = false;
	string name = "";
	string first_name;
	string last_name;
	cout << "* First Name: ";
	cin >> first_name;
	cout << "* Last Name: ";
	cin >> last_name;
	cout << endl;
	name = first_name + " " + last_name;
	string line = "";
	ifstream infile;
	infile.open("credentials.txt");
	if (!infile.is_open())
	{
		cout << "+ Error: credentials file was not opened properly\n" << endl;
		return false;
	}
	while (!infile.eof()) {
		getline(infile, line);
		if (line.empty()) { continue; }
		if (name == line) {
			infile.close();
			cout << "+ Name already registered\n" << endl;
			return false;
		}
	}
	infile.close();
	ofstream outfile;
	outfile.open("credentials.txt", std::ofstream::out | std::ofstream::app);
	if (!outfile.is_open())
	{
		cout << "Error: credentials file was not opened properly\n" << endl;
		return false;
	}
	outfile << endl << name;
	outfile.close();
	return true;
}

bool Blog::login() {
	while (!done2) {
		char option = '0';
		cout << loginPrompt << commandPrompt;
		cin >> option;
		cout << endl;
		switch (option) {
		case '1':
			if (LoginSequence()) {
				done2 = true;
			}
			break;
		case '2':
			RegisterSequence();
			break;
		case '3':
			cout << "Exit selected\n" << endl;
			return false;
			break;
		default:
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Unknown command..\n" << endl;
			break;
		}
	}
}


int main() {
	Blog app;
	bool done = false;
	char option = '0';

	// User Login / Registering
	if (!app.login()) {
		return 0;
	}

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
