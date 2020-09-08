/*
	Author: Nawaf Alharbi
	Date: Oct-2017
	Vancouver Island University 
*/

#include <iostream>
#include <occi.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
using namespace oracle::occi;



// a function that displays welcome menu and the various options available
void WELCOME () {
	cout << " =============== Welcome to the FORUMS application =============== "<< endl;
	cout << " please select one of the following options: " << 
	endl << " A .... to become a member " <<
	endl << " P .... to post a message, if you are already have a user ID " <<
	endl << " S .... to look at the thread IDs and topics inside a given board " << 
	endl << " R .... to read more about a specific thread " <<
    endl << " Q .... to quit the program " <<        
	endl;
}

// a function that get user information and enter them to the users table in the database 
void INSERT_INTO_USERS (Connection *conn) { 
	string userid;
	string name; 
	string email;
	string country;
	int age;
	
	cout << "please enter your desired user id " << endl;
	getline(cin, userid);

	// a string containing the query to be executed 
	string queryStr = "select userid from users where userid = '" + userid + "'";

	// create a statment 
    Statement *ustmt = conn->createStatement();

    // create result set that contains information about the username entered by the user 
    ResultSet *rs = ustmt->executeQuery(queryStr);

    // a boolean var that contains information about the existence of the name in the database 
     bool exists = rs->next();

    // if the name exists in the database already, exists will return 1  
     if (exists == 1) {
          cout << "The user name is already taken, please try again with a diffrent user name "<<endl;
          INSERT_INTO_USERS (conn);
          return;
     }

    // close the result set and termenate the statment 
	ustmt->closeResultSet(rs);
    conn->terminateStatement(ustmt);


	// get information from the user 
	cout << "please enter your full name " << endl;
	getline(cin, name);
	cout << "please enter your email address " << endl;
	getline(cin, email);
	cout << "please enter your country of residence " << endl;
	getline(cin, country);
	cout << "please enter your age " << endl;
	cin >> age;
	
	if (cin.fail()){
		cout << "the age entered was not a number, please try again!" <<endl ;
		cin.clear();
		cin.ignore (256, '\n');
		INSERT_INTO_USERS (conn);
        return;
	}
	
	// setting the parameter 1 2 3 4 5 to the data entered by the user 
	Statement *stmt = conn->createStatement("insert into users values (:1, :2, :3, :4, :5)");
	stmt->setString(1, userid);
	stmt->setString(2, name);
	stmt->setInt(3, age);
	stmt->setString(4, email);
	stmt->setString(5, country);
	
	// success indication 
	int rows = stmt->executeUpdate();
	cout << "WELCOME TO OUR FAMILY! " << 
	endl << "you are now a member in our website! " <<
	endl << "a number of "<< rows << " membership has been created. " << 
	endl;

	//termnating the statment 
	conn->terminateStatement(stmt);
    
	
}


void INSERT_INTO_MESSAGES(Connection *conn){
	
    string userid;
    string choice; 
	int tid;
	string topic;
	string bid;
	string queryStr = "select max (tid) from threads";
	
	Statement *stmt = conn->createStatement();            

	// create result set that containg the maximum tid in the table 
	ResultSet *rs = stmt->executeQuery(queryStr);

	// get the maximum tid and store it in the variable tid        
	rs->next();   
	tid= rs->getInt(1);
	tid++;

	stmt->closeResultSet(rs);
	conn->terminateStatement(stmt);
	
		
	cout << "please enter your username " << endl;
	getline(cin, userid);

	// a string containing the query to be executed 
	string QueryStr = "select userid from users where userid = '" + userid + "'";

	//
	Statement *estmt = conn->createStatement();

	// create result set that contain the username that was 
	ResultSet *res = estmt->executeQuery(QueryStr);

	// contain information about the existence of the name in the database 
	bool exists = res->next();

	if (exists == 0) {
	    cout << "The user name does not exists in the database. please join our website to be able to post messages "<<
	    endl << "select option 'A' to become a member in seconds "<<
	    endl;
	    return;
	}


	estmt->closeResultSet(res);
    conn->terminateStatement(estmt);


	
	cout <<"please enter the board id: "<< 
	endl <<"java .... for questions related to java programming "<<
	endl <<"cplusplus .... for questions relaated to c++ programming "<<
	endl <<"c .... for sharp C programming "<<
	endl;
	getline (cin,bid);
	cout << "please enter the topic "<<endl;
	getline (cin,topic); 
	
	Statement *ustmt = conn->createStatement("insert into threads values (:1, :2, :3)");
	ustmt->setInt(1, tid);
	ustmt->setString(2, topic);
	ustmt->setString(3, bid);
	
	// success indication 
	int rows = ustmt->executeUpdate();
	cout << "we have inserted your entry to the threads table " <<
	endl << " a number of " << rows << " entry has been added." << endl;
	
	conn->terminateStatement(ustmt);
    

	int mid;
	string body;
	
	string querryStr = "select max (mid) from messages";
	
	Statement *mstmt = conn->createStatement();            

	// create result set that containg the maximum tid in the table 
	ResultSet *rss = mstmt->executeQuery(querryStr);

	// get the maximum DID and store it in the variable DID        
	rss->next();   
	mid= rss->getInt(1);
	mid++;
	mstmt->closeResultSet(rss);
	conn->terminateStatement(mstmt);


	cout << "please enter the body of the message " << endl;
	getline(cin, body);
	
	Statement *sstmt = conn->createStatement("insert into messages values (:1, :2, :3, :4)");
	sstmt->setInt(1, mid);
	sstmt->setString(2, userid);
	sstmt->setString(3, body);
	sstmt->setInt(4, tid);
	
	int Rows = sstmt->executeUpdate();
	cout << "your message has been posted succsefully " <<
	endl << "the number of " << Rows << " messages has been posted" << endl;
	
	conn->terminateStatement(sstmt);
	
}

void SEARCH_BOARDS (Connection *conn) {
	string bid;
	string tid;
	string topic;
	
	cout <<"please enter the board id: "<< 
	endl <<"java .... for questions related to java programming "<<
	endl <<"cplusplus .... for questions relaated to c++ programming "<<
	endl <<"c .... for sharp C programming "<<
	endl;
	
	getline (cin,bid);
	
	string queryStr = "select tid, topic from threads where bid = '" + bid + "'";
  
	Statement *stmt = conn->createStatement();            

	// create result set that containg the maximum tid in the table 
	ResultSet *rs = stmt->executeQuery(queryStr);

	// get the maximum tid and store it in the variable tid        
	while (rs->next()) {
	tid = rs->getString(1);
	topic = rs->getString(2);
	cout << "Thread ID: " << tid <<
	endl << "topic: " << topic <<
	endl;
	}
}

void READ_MORE (Connection *conn){
    string body;
	string threadID;
	cout << "please enter the thread ID "<<endl;
	cin >> threadID;

	string queryStr = "select tid from messages where tid = " + threadID ;
	Statement *stmt = conn->createStatement();            

	string querryStr = "select body from messages where tid = " + threadID ;
    Statement *ustmt = conn->createStatement();

	
	// create result set that containg the maximum tid in the table 
	ResultSet *rs = stmt->executeQuery(queryStr);
     
	ResultSet *rss = ustmt->executeQuery(querryStr);
	
	
	bool exists = rs->next();

	if (exists == 0){
	  cout << "the thread ID you entered is invalid! " <<
	  endl << "to look for a specific thread ID, please choose the option 'S' from the main menu "<<
	  endl;
	  return;
	} else {
	  if (rss->next()) {
	  body = rss->getString(1);
	  cout << "the body of the messages: "<< body <<endl;
	  } else {
		  cout << "No body" << endl;
          }
	}
      


 }   
string readPassword()
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag =  (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );

    string password = "";
    getline(cin, password);

    settings.c_lflag = (settings.c_lflag |   ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
    return password;
}




int main () {
    bool termenate = false;
    char option;
    string cleaner;
   
    //declaring variables that will be used in main
    string userName = "";
    string password;

    password = readPassword();
    const string connectString = "sunfire.csci.viu.ca";
    // creates an enviroment 
    Environment *env = Environment::createEnvironment();
    Connection *conn = env->createConnection
							  (userName, password, connectString);   
    while(!termenate){
        
	WELCOME();
	cin >> option; 
	getline (cin,cleaner);
	
	if (option == 'a' || option == 'A') {
	  INSERT_INTO_USERS(conn);
	} else if (option == 'p' || option == 'P') {
	  INSERT_INTO_MESSAGES(conn);  
	} else if (option == 's' || option == 'S') {
	  SEARCH_BOARDS (conn);	
	} else if (option == 'r' || option == 'R') {
	  READ_MORE(conn);
	} else if (option == 'q' || option == 'Q') { 
	  cout << "THANK YOU FOR USING THE APPLICATION! "<<
	  endl << "GOODBYE! "<<
	  endl;
	  env->terminateConnection(conn);
	  Environment::terminateEnvironment(env);
	  termenate = true;
	} else {
	  cout << "please enter a valid choice"<<endl;
	}
		

     }
}
