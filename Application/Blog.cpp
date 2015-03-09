#define _CRT_SECURE_NO_WARNINGS

#include "DbmsApi.h"
#include "Blog.h"

class NoFile{};

using namespace std;

/*
*	Helper
*		Functions
*/

vector<string> tagParser(string tags) {
	vector<string> formattedTags;
	while (tags.length() > 0) {
		while (tags[0] == '/' || tags[0] == ' ' || tags[0] == ',') tags.erase(0, 1);
		size_t pos_comma = tags.find(',');
		size_t pos_slash = tags.find('/');
		size_t pos = min(pos_comma, pos_slash);
		formattedTags.push_back(tags.substr(0, pos));
		if (pos == string::npos) tags.erase(0, tags.length());
		tags.erase(0, pos + 1);
	}
	return formattedTags;
}

vector<string> getInputTags() {
	string line = "";
	string tag = "";
	string tags = "";
	vector<string> tagList;
	cout << "* Enter Tag(s):\n";
	cout << "Enter DONE when finished entering tags" << endl;
	// while loop to get all tags
	while (getline(cin, line)) {
		istringstream iss(line);
		iss >> tag;
		if (tag == "DONE"){
			break;
		}
		else {
			tags += tag + "/";
			tagList.push_back(tag);
		}
	}
	tagList.push_back(tags);
	return tagList;
}

// Checks if input is valid and asks for valid answer if it is not
bool enableCommenting(string commenting) {
	bool goodAnswer = false;
	while (true) {
		if (commenting == "yes" || commenting == "Yes" || commenting == "y" || commenting == "Y") {
			return true;
		}
		else if (commenting == "no" || commenting == "No" || commenting == "n" || commenting == "N") {
			return false;
		}
		else {
			cout << "Please enter a valid answer (yes/no): ";
			cin >> commenting;
			cin.ignore();
		}
	}
}

/*
*	Post Function 
*		Definitions
*/

// Constructors
Post::Post(const Post& p) {
	title = p.title;
	author = p.author;
	content = p.content;
	date = p.date;
	tags = p.tags;
	allowComments = p.allowComments;
	postId = p.postId;
	parentId = p.parentId;
}
Post::Post(vector<string> rawData) {
	title = rawData[0];
	author = rawData[1];
	date = rawData[2];
	tags = tagParser(rawData[3]);
	content = rawData[4];
	if (rawData[5] == "1") allowComments = true;
	else allowComments = false;
	postId = atoi(rawData[6].c_str());
	parentId = atoi(rawData[7].c_str());
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
vector<string> Post::getTags() { return tags; }
// Returns true if the argument tags matches the post's tags
bool Post::containsTags(vector<string> t) {
	if (t.size() > tags.size()) return false;
	vector<bool> allFound;
	for (int i = 0; i < t.size(); ++i) {
		bool found = false;
		for (int j = 0; j < tags.size(); ++j) {
			if (tags[j] == t[i]) found = true;
		}
		allFound.push_back(found);
	}
	for (int k = 0; k < allFound.size(); ++k) {
		if (!allFound[k]) return false;
	}
	return true;
}
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
	editMenu = "Edit Menu]\n\nEdit:\n1. Title\n2. Author\n3. Content\n4. Tag(s)\n5. Commenting\n6. Return to Main Menu\n\n";
    postMenu = "1. View\n2. Edit\n3. Delete\n4. Comment\n5. Return to Main Menu\n\n";
	commandPrompt = "* Enter command: ";
	loginPrompt = "[Login / Register]\n\n1. Login\n2. Register\n3. Exit\n\n";
	loginSequence = "[Login]\n\n";
	registerSequence = "[Register]\n\n";
	done = false;	
	done2 = false;
}

// This function is the public interface to begin the Blog App
void Blog::initiateBlog() {
	MenuSequence();
}

// This function attempts to load all Post data stored
//  in the database.
void Blog::loadSavedPosts() {
	// Get Relation from database file
	posts.clear();
	Table postTable = GetRelation("app");
	int i = 0;
	bool finished = false;
	// While loop to extract all post data
	while (!finished) {
		vector<string> temp = postTable.getTuple(i);
		// size == 0, no more posts in database
		if (temp.size() == 0) finished = true;
		else if (temp.size() == 8) { // size == 8 means all required data for a post is present
			posts.push_back(Post(temp));
		} // Otherwise post data is corrupt
		else cout << "\n\n\n ERROR LOADING POST DATA WITH TITLE: " + temp[0] + "\n\n\n";

		++i;
	}
}

void Blog::editPost(int p) {
	char option = '0';
	int index = 0;
	int curIndex = 0;
	for (int i = 0; i < posts.size(); ++i) {
		if (posts[i].getPostID() == p) index = i;
	}
	for (int i = 0; i < currentPosts.size(); ++i) {
		if (currentPosts[i].getPostID() == p) curIndex = i;
	}
	string newData = "";
	bool commenting = false;
	string tags = "";
	vector<string> newTags;
	cout << "\n[" << currentPosts[curIndex].getTitle() << "'s " << editMenu << commandPrompt;
	cin >> option;
	cin.ignore();
	cout << endl;
	switch (option) {
	case '1':
		cout << "\n\n* Current title: " + currentPosts[curIndex].getTitle() + "\n"
			<< "* Enter new title: ";
		getline(cin, newData);
		currentPosts[curIndex].setTitle(newData);
		posts[index].setTitle(newData);
		if (DBMSParse("UPDATE app SET title=\"" + newData + "\" WHERE (id == " + to_string(p) + ");"))
			cout << "\n\nTitle changed successfully.\n\n";
		else cout << "\n\nAn error occured when changing title!\n\n";
		break;
	case '2':
		cout << "\n\n* Current author: " + currentPosts[curIndex].getAuthor() + "\n"
			<< "* Enter new author: ";
		getline(cin, newData);
		currentPosts[curIndex].setAuthor(newData);
		posts[index].setAuthor(newData);
		if (DBMSParse("UPDATE app SET author=\"" + newData + "\" WHERE (id == " + to_string(p) + ");"))
			cout << "\n\nAuthor changed successfully.\n\n";
		else cout << "\n\nAn error occured when changing author!\n\n";
		break;
	case '3':
		cout << "\n\n* Current content: " + currentPosts[p - 1].getContent() + "\n"
			<< "* Enter new content: ";
		getline(cin, newData);
		currentPosts[curIndex].setContent(newData);
		posts[index].setContent(newData);
		if (DBMSParse("UPDATE app SET content=\"" + newData + "\" WHERE (id == " + to_string(p) + ");"))
			cout << "\n\nContent changed successfully.\n\n";
		else cout << "\n\nAn error occured when changing content!\n\n";
		break;
	case '4':
		cout << "\n\n* Current tags: ";
		for (int i = 0; i < currentPosts[curIndex].getTags().size(); ++i) {
			cout << currentPosts[curIndex].getTags()[i];
			if (i != currentPosts[curIndex].getTags().size() - 1) cout << ", ";
		}
		cout << "\n";
		newTags = getInputTags();
		tags = newTags[newTags.size() - 1];
		newTags.erase(newTags.end()-1);
		currentPosts[curIndex].setTags(newTags);
		posts[index].setTags(newTags);
		if (DBMSParse("UPDATE app SET tags=\"" + tags + "\" WHERE (id == " + to_string(p) + ");"))
			cout << "\n\nTags changed successfully.\n\n";
		else cout << "\n\nAn error occured when changing tags!\n\n";
		break;
	case '5':
		cout << "\n\n* Commenting currently allowed: ";
		if (currentPosts[curIndex].getCommenting()) cout << "yes\n";
		else cout << "no\n";
		cout << "* Would you like to allow commenting? (yes/no): ";
		getline(cin, newData);
		commenting = enableCommenting(newData);
		if (commenting) newData = "1";
		else newData = "0";
		currentPosts[curIndex].setCommenting(commenting);
		posts[index].setCommenting(commenting);
		if (DBMSParse("UPDATE app SET commenting=" + newData + " WHERE (id == " + to_string(p) + ");"))
			cout << "\n\nCommenting changed successfully.\n\n";
		else cout << "\n\nAn error occured when changing commenting!\n\n";
		break;
	case '6':
		cout << endl;
		break;
	default:
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Unknown command..\n" << endl;
		break;
	}

}

// This function deletes a certain post
void Blog::deletePost(int p) {
	// Issue delete command to database
	if (DBMSParse("DELETE FROM app WHERE (id == "
		+ to_string(p) + " || parentId == " + to_string(p) + ");")) {
		cout << "\n\nPost and comments successfully deleted.\n\n";
		// Reload posts from database
		loadSavedPosts();
	}
	else {
		cout << "\n\nAn error occured when attempting to delete post..\n\n";
	}
}


// This function displays the post menu, allowing for a selection of 
// view, edit, delete or comment. This function needs it's implementation finished
void Blog::displayPostMenu(int p) {
	char option = '0';
	// Check for valid post selection
	if (p <= 0 || p > currentPosts.size()) {
		cout << "\n\n--INCORRECT SELECTION--\n\n";
		return;
	}
	else {
		cout << "\n[" << currentPosts[p-1].getTitle() << "]\n\n" << postMenu << commandPrompt;
		cin >> option;
		cin.ignore();
		cout << endl;
		switch (option) {
			case '1':
				cout << "\n\nView option selected\n\n";
				printPost(currentPosts[p-1]);
				break;
			case '2':
				cout << "\n\nEdit option selected\n\n";
				editPost(currentPosts[p - 1].getPostID());
				break;
			case '3':
				cout << "\n\nDelete option selected\n\n";
				deletePost(currentPosts[p-1].getPostID());
				break;
			case '4':
				if (currentPosts[p-1].getCommenting()) {
					cout << "\n\nComment option selected\n";
					newPostSequence(currentPosts[p-1].getPostID());
				}
				else {
					cout << "\n\nComments disabled for this post.\n\n";
				}
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
		cout << i << ". " << currentPosts[i-1].getTitle() << " (" << currentPosts[i-1].getDatePosted() << ")" << endl;
	}
	cout << currentPosts.size() + 1 << ". Return to Main Menu\n\n";
	cout << "* Enter ID: ";
	if (cin >> index) {cin.ignore();}  
	else {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\n\nYou break it, you buy it...\n\n";
		return;	 
	}
	// Calls display on selected post if they did not choose to return to menu
	if (index != currentPosts.size() + 1)
		displayPostMenu(index);
}

// Prints out Post and Comments attached to that Post
void Blog::printPost(Post post) {
	cout << "\n--------------------------------------------------------------------------------" << endl;
	cout << "By: " << post.getAuthor() << endl;
	cout << "Date: " << post.getDatePosted() << endl;
	cout << "\n" << post.getTitle() << endl;
	cout << post.getContent() << endl;
	vector<string> postTags = post.getTags();
	cout << "\nTags: ";
	for (int i = 0; i < postTags.size(); ++i) {
		cout << postTags[i];
		if (i != postTags.size() - 1) {
			cout << ", ";
		}
	}
	cout << "\n\nComments:\n";
	printComments(post.getPostID());

	cout << "\n--------------------------------------------------------------------------------" << endl;
}

// Helper function used to print comments
void Blog::printComments(int parentId) {
	vector<Post> commentList;
	Post currentComment;
	for (int i = 0; i < posts.size(); ++i) {
		if (posts[i].getParentID() == parentId)
			commentList.push_back(posts[i]);
	}
	if (commentList.empty())
		cout << "No Comments Available." << endl;
	for (int j = 0; j < commentList.size(); ++j) {
		currentComment = commentList[j];
		cout << j + 1 << ".\tOn " << currentComment.getDatePosted() << ", "
				<< currentComment.getAuthor() << " said:" << endl;
		cout << "\t\t\t" << currentComment.getContent() << "\n\n";
	}
}

// Searches for posts that has a matching author
void Blog::searchAuthor(string name) {
	currentPosts.clear();
	for (int i = 0; i < posts.size(); ++i) {
		if (posts[i].getAuthor() == name && posts[i].getParentID() == -1) {
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
	currentPosts.clear();
	char option = '0';
	for (int i = 0; i < posts.size(); ++i) {
		if (posts[i].getTitle() == title && posts[i].getParentID() == -1){
			Post temp(posts[i]);
			currentPosts.push_back(temp);
		}
	}
	if (currentPosts.size() > 0) {
		cout << "[Posts with '" << title << "' as a Title]\n\n";
		displayCurrentPosts();
	}
	else {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "No Posts Found\n\n";
	}
}

// Searches for posts that contain all of the searched for tags
void Blog::searchTags(string tag) {
	currentPosts.clear();
	vector<string> tags = tagParser(tag);
	// Must parse argument for multiple tags
	for (int i = 0; i < posts.size(); ++i) {
		if (posts[i].containsTags(tags) && posts[i].getParentID() == -1)	{
			Post temp(posts[i]);
			currentPosts.push_back(temp);
		}
	}
	if (currentPosts.size() > 0) {
		cout << "[Posts with " << tag << " as a Tag]\n\n";
		displayCurrentPosts();
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
	currentPosts.clear();
	for (int i = 0; i < posts.size(); ++i) {
		if (posts[i].getDatePosted() == day && posts[i].getParentID() == -1){
			Post temp(posts[i]);
			currentPosts.push_back(temp);
		}
	}
	if (currentPosts.size() > 0) {
		cout << "[Posts made on " << day << "]\n\n";
		displayCurrentPosts();
	}
	else  {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "No Posts Found\n\n";
	}
}

// Sequence containing insert menu
void Blog::newPostSequence(int parent = -1) {
	Post newPost;
	string dataString = "";
	string tags = "";
	string commentable = "";
	string postTitle = "";
	string postContent, line, tag, commenting;
	vector<string> tagList;
	newPost.setParentID(parent);
	newPost.setAuthor(author);
	if (parent == -1) {
		cout << "* Enter Title: ";
		getline(cin, postTitle);
		cout << endl;
		cout << "* Enter Post Content: ";
	}
	else {
		cout << endl;
		cout << "* Enter Comment Content: ";
	}
	newPost.setTitle(postTitle);
	getline(cin, postContent);
	cout << endl;
	newPost.setContent(postContent);
	tagList = getInputTags();
	tags = tagList[tagList.size() - 1];
	tagList.erase(tagList.end()-1);
	newPost.setTags(tagList);
	cout << endl;
	
	if (parent == -1) {
		cout << "* Would you like to allow commenting? (yes/no): ";
		cin >> commenting;
		cin.ignore();
	}
	else commenting = "no";
	if (enableCommenting(commenting)) {
		newPost.setCommenting(true);
		commentable = "1";
	}
	else {
		newPost.setCommenting(false);
		commentable = "0";
	}

	if (posts.empty()) {
		newPost.setPostID(1);
	}
	else newPost.setPostID(posts.back().getPostID() + 1);

	// Gets current system date, parses it, and sets as post date
	time_t rawtime;
	time(&rawtime);
	string currentDate = ctime(&rawtime);
	currentDate.erase(currentDate.find("\n"), currentDate.find("\n") + 1);
	currentDate.erase(0, 4);
	currentDate.insert(3, "/");
	if (currentDate[4] == ' ') {
		currentDate.erase(4, 2);
		currentDate.insert(4, "0");
	}
	else currentDate.erase(4, 1);
	currentDate.erase(6, 10);
	currentDate.insert(6, "/");
	newPost.setDate(currentDate);

	dataString = " (\"" + postTitle + "\", \"" + author + "\", \"" + currentDate + "\", \"" + tags + "\", \"" + postContent + "\", " + commentable;
	if (posts.size() == 0) dataString += ", 1, ";
	else {
		int temp = posts[posts.size() - 1].getPostID() + 1;
		dataString += ", " + to_string(temp) + ", ";
	}
	dataString += to_string(parent) + ");";
	if (DBMSParse("INSERT INTO app VALUES FROM" + dataString)) {
		posts.push_back(newPost);
		cout << "\nPost succesfully made!\n\n";
	}
	else {
		cout << "\nAn error occured when attempting to make post!\n\n";
	}
}

// Sequence containing search menu
void Blog::SearchSequence() {
	char option = '0';
	string searchFor = "";
	cout << searchMenu << commandPrompt;
	cin >> option;
	cin.ignore();
	cout << endl;
	switch (option) {
		case '1':
			cout << "* Enter author: ";
			getline(cin, searchFor);
			cout << endl;
			searchAuthor(searchFor);
			break;
		case '2':
			cout << "* Enter title: ";
			getline(cin, searchFor);
			cout << endl;
			searchTitle(searchFor);
			break;
		case '3':
			cout << "* Enter tags (seperated by commas): ";
			getline(cin, searchFor);
			cout << endl;
			searchTags(searchFor);
			break;
		case '4':
			cout << "* Enter date: ";
			cin >> searchFor;
			cin.ignore();
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
	try { // Try to open the database file containing Post data
		if(!DBMSParse("OPEN app")) throw NoFile();
		loadSavedPosts();
	}
	catch (NoFile& e) { // If it does not exist (first time running app), then issue a create command
		// Command to create a table
		DBMSParse("CREATE TABLE app (title VARCHAR(20), author VARCHAR(20), date VARCHAR(10), tags VARCHAR(100), content VARCHAR(250), commenting INTEGER, id INTEGER, parentId INTEGER) PRIMARY KEY (title, author);");
	}
	while (!done) {
		char option = '0';
		cout << mainMenu << commandPrompt;
		cin >> option;
		cin.ignore();
		cout << endl;
		switch (option) {
			case '1': // Create a New Post
				cout << "New post selected\n" << endl;
				newPostSequence();
				break;
			case '2': // Search for a Post
				SearchSequence();
				break;
			case '3': // Exit Blog app
				cout << "Exit selected\n" << endl;
				// Save the blog app data into the database
				done = DBMSParse("WRITE app");
				break;
			default: // Unrecognized command? Clear buffer completely and retry
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
	cin.ignore();
	cout << "* Last Name: ";
	cin >> last_name;
	cin.ignore();
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
	cin.ignore();
	cout << "* Last Name: ";
	cin >> last_name;
	cin.ignore();
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
		cin.ignore();
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
	return true;
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
	app.initiateBlog();
	return 0;
}
