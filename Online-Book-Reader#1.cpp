#pragma warning(disable : 4996)
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
class BookSession; //forward declaration
class UserManager;
class Helper
{
public:
    static void ShowMenu(vector<string> choices) {
        cout << "\nMenu:\n";
        for (int i = 0;i < choices.size(); i++)
            cout << "\t" << i + 1 << ": " << choices[i] << endl;
    }
    static int ReadInt(pair<int, int> range) {
        cout << "\nEnter Choice " << range.first << " - " << range.second << ": ";
        int choice;
        cin >> choice;
        if(choice >= range.first && choice <= range.second)
        return choice;
        else
        {
            cout << "\nInvalid Input, Please Try Again\n";
        }
    }
    static bool ValidRange(pair<int , int> range , int pos) {
        if (pos >= range.first && pos <= range.second)
            return true;
        return false;
    }
    static vector<string> SplitString(string str , char delimiter = ',')
    {
        vector<string> tokens;
        string token;
        istringstream iss(str);
        while (getline(iss, token, delimiter))
            tokens.push_back(token);
        return tokens;

    }
   static const std::string currentDateTime() { // will be a helper method
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

        return buf;
    }
};

class Book
{
private:
    string isbn, title, authorName;
    vector<string> pages;
public:
    Book()
    {

    }
    Book(string line) {
        vector<string> data = Helper::SplitString(line);
        isbn = data[0];
        title = data[1];
        authorName = data[2];
        for (int i = 3; i < data.size(); i++) //push pages from the line , may help in file storage in the future
            pages.push_back(data[i]);
    }
    int GetTotalPages() {
        return GetPages().size();
    }
    string GetPagesContent(int pos) {
        return GetPages()[pos];
    }
    string GetIsbn()
    {
        return isbn;
    }
    string GetTitle()
    {
        return title;
    }
    string GetAuthorName()
    {
        return authorName;
    }
    const vector<string>& GetPages()
    {
        return pages;
    }
     void SetIsbn(string str)
    {
         isbn = str;
    }
    void SetTitle(string str)
    {
        title = str;
    }
    void SetAuthorName(string str)
    {
        authorName = str;;
    }
    void AddPage(string str)
    {
        pages.push_back(str);
    }
};
class BookSession
{
private:
    int current_page;
    Book* book;
    string current_date;
    void UpdateCurrentDate()
    {
        current_date = Helper::currentDateTime();
    }
    void NextPage() {
        if (Helper::ValidRange({ 0 , book->GetTotalPages() - 1 }, (current_page + 1)))
            ++current_page;
        else
            cout << "You are on the last page!\n";
    }
    void PreviousPage() {
        if (Helper::ValidRange({ 0 , book->GetTotalPages() }, (current_page - 1)))
            --current_page;
        else
            cout << "You are on the first page!\n";
    }
    void StopReading() {
        UpdateCurrentDate();
    }
public:
    BookSession() {}
    BookSession(Book* book) :book(book) {
        current_page = 0;
        current_date = Helper::currentDateTime();
    }
    void DisplaySession()  {
        cout << book->GetTitle() << " Page: " << GetCurrentPage() + 1 << "/" << book->GetTotalPages() << " - " << GetCurrentDate() << endl;
    }
    void ReadTheBook() {
        while (true)
        {
            cout << "Current Page: " << GetCurrentPage() + 1 << "/" << book->GetTotalPages() << "\n" << book->GetPagesContent(GetCurrentPage()) << endl;
            vector<string> Menu{ "Next Page" , "Previous Page" , "Stop Reading" };
            Helper::ShowMenu(Menu);
            int choice = Helper::ReadInt({ 1, Menu.size() });
            if (choice == 1)
                NextPage();
            else if (choice == 2)
                PreviousPage();
            else
            {
                StopReading();
                break;
            }
        }

    }
 
    int GetCurrentPage()
    {
        return current_page;
    }
    void SetCurrentPage(int current_page_)
    {
        current_page = current_page_;
    }
    const string& GetCurrentDate() //current_date is not temporary , so i can return it as a reference
    {
        return current_date;
    }

};

class BookManager
{
private:
    map<string, Book*> mp_isbn_book;
    BookSession* session;
    Book* current_book;
public:
    BookManager()
    {
        LoadData();
    }
    void AddBook(Book & book) {
        mp_isbn_book.insert({ book.GetIsbn() , &book });
    }
    void ListAvailableBooks() {
        int i = 1;
        for (auto& pair : mp_isbn_book)
        {
            cout << i++ << ") " << pair.second->GetTitle() << endl;
        }
    }
    void LoadData()
    {
        mp_isbn_book.clear();
        Book* b1 = new Book("12545,C++ How To Program,Mohamed Emad,Hello this is c++ HTP,Programming C++ is harder than other high level langs,do your best in foundations using c++,Bye!");
        mp_isbn_book.insert({ b1->GetIsbn() , b1 });
        Book* b2 = new Book("12448,Data Structures using C++,Mohamed_Reda,Data Structures are important in general,specially in c++ , you should learn them for better problem solving exp,End");
        mp_isbn_book.insert({ b2->GetIsbn() , b2 });
        Book* b3 = new Book("5484,introduction to algorithms,Ahmed_Nader,Algorithms are indespensable in CS,it's very important concept to focus effort on , algos are like binary search,End");
        mp_isbn_book.insert({ b3->GetIsbn() , b3 });
    }
    ~BookManager()
    {
        for (auto pair : mp_isbn_book)
        {
            delete pair.second;
            pair.second = nullptr;
        }
    }
    map<string, Book*>& GetMpIsbnBook()
    {
        return mp_isbn_book;
    }
};
class User
{
    string username, password, name, email;
    vector<BookSession> reading_history;
    bool is_admin;
public:
    User()
    {

    }
    User(string line)
    {
        vector<string> data = Helper::SplitString(line);
        username = data[0];
        password = data[1];
        name = data[2];
        email = data[3];
        is_admin = stoi(data[4]);
    }

    string GetUsername() { return username; }
    string GetPassword() { return password; }
    string GetName() { return name; }
    string GetEmail() { return email; }
    bool IsAdmin()
    {
        return is_admin;
    }
    void AddBookReadingHistory(BookSession  book_session)
    {
        reading_history.push_back(book_session);
    }
      vector<BookSession>& GetReadingHistpry()
    {
        return reading_history;
    }
      void WelcomeSentence()
      {
          cout << "\nWelcome " << name;
          if (is_admin)
              cout << " | Admin View";
          else
              cout << " | User View";
          cout << endl;
      }
    void DisplayProfile()
    {
        ostringstream oss;
        oss << "Name: " << username;
        if (is_admin)
            oss << " | Admin";
        oss << "\n";
        oss << "Email: " << email << "\n";
        oss << "User name: " << username << "\n";
        cout << oss.str();
    }

};
class UserManager
{
private:
    map<string, User> mp_username_user;
    User* current_user;
    void ReadUserLoginInfo(pair<string, string>& login_info)
    {
        string str;
        cout << "Enter Username: ";
        cin >> str;
        login_info.first = str;
        cout << "Enter Password: ";
        cin >> str;
        login_info.second = str;
    }
    void ReadUserSignUpInfo(pair<string, User>& p)
    {
        ostringstream oss;
        string username; //need it to add to the pair
        string str;
        cout << "Enter Username: ";
        cin >> str;
        oss << str << ",";
        username = str;
        cout << "Enter Password: ";
        cin >> str;
        oss << str << ",";
        cout << "Enter Name: ";
        cin >> str;
        oss << str << ",";
        cout << "Enter Email: ";
        cin >> str;
        oss << str << ",0";
        p = { username , User(oss.str()) };
    }
    void Login() {
        LoadData();
        pair<string, string> login_info;
        ReadUserLoginInfo(login_info);

        if (!mp_username_user.count(login_info.first))
        {
            cout << "\nUsername not found\n";
            Login();
        }
        User& user_now = mp_username_user[login_info.first];
        if (user_now.GetPassword() == login_info.second)
        {
            cout << "\nLogin Succeeded \n";
            current_user = &user_now;
            current_user->WelcomeSentence();
        }else{
                cout << "\nInvalid password\n";
                Login();
        }

    }
    void SignUp() {
        pair<string, User> p;
        ReadUserSignUpInfo(p);
        mp_username_user.insert(p);
        current_user = &mp_username_user[p.first];
    }
    void LoadData() {
        mp_username_user.clear();
        User user("mohamed,1234,Mohamed_Emad_Mazy,mo@gmail.com,1");
        mp_username_user[user.GetUsername()] = user;
        user = User("ahmed,4567,Ahmed_Nader_Elsayed,ah@gmail.com,0");
        mp_username_user[user.GetUsername()] = user;
        user = User("mostafa,4567,Mostafa_Mansour_Ziad,mostafa@gmail.com,0");
        mp_username_user[user.GetUsername()] = user;

    }
public:
    bool AccessSystem()
    {
        vector<string> choices{ "Login" , "Signup", "Exit"};
        Helper::ShowMenu(choices);
        int choice = Helper::ReadInt({ 1 , choices.size() });
        if (choice == 1)
            Login();
        else if (choice == 2)
            SignUp();
        else
            return false;
        return true;
    }
    User& GetCurrentUser()
    {
        return *current_user;
    }
};
class UserStandard
{
private:
    UserManager& user_manager;
    BookManager& book_manager;
    void DisplayProfile() {
        user_manager.GetCurrentUser().DisplayProfile();
    }
    void ListnSelectFromAvailableBooks()
    {
        book_manager.ListAvailableBooks();
        int choice = Helper::ReadInt({ 1 , (int)book_manager.GetMpIsbnBook().size() });
        int i = 0;
        for (auto& pair : book_manager.GetMpIsbnBook())
        {
            if (choice == (++i))
            {
                BookSession session = BookSession(pair.second);
                session.ReadTheBook();
                user_manager.GetCurrentUser().AddBookReadingHistory(session); //why not make a method called addBookHistory in user manager?
            }
        }

    }
    void DisplayReadinghistory() {
        User* current_user = &user_manager.GetCurrentUser();
        if (current_user->GetReadingHistpry().size() == 0)
        {
            cout << "\nYou don't have any Book Sessions yet, Try to read a book!\n";
            return;
        }
        for (auto& session : current_user->GetReadingHistpry())
        {
            session.DisplaySession();
        }
        int choice = Helper::ReadInt({ 1 , current_user->GetReadingHistpry().size() });
        BookSession& current_session = current_user->GetReadingHistpry()[choice - 1];
        current_session.ReadTheBook();
    }
public:
    UserStandard(UserManager& user_manager,BookManager& book_manager) :user_manager(user_manager), book_manager(book_manager)
     {
 
     } 
    void DisplayMenu()
    {
        vector<string> choices{"View Profile" , "List & Select From My Reading History" , "List & Select From Available Books" , "Logout"};
        while (true)
        {
        Helper::ShowMenu(choices);
            int choice = Helper::ReadInt({ 1 , (int)choices.size() });
            if (choice == 1)
                DisplayProfile();
            else if (choice == 2)
                DisplayReadinghistory();
            else if (choice == 3)
                ListnSelectFromAvailableBooks();
            else
                break;
        }
    }
   
};
class UserAdmin
{
private:
    UserManager& user_manager;
    BookManager& book_manager;
    void GetBookInfo(Book& book)
    {
        
        string str;

        cout << "Enter ISBN: ";
        cin >> str;
       book.SetIsbn(str);

        cout << "Enter Title: ";
        cin >> str;
        book.SetTitle(str);

        cout << "Enter Author Name: ";
        cin >> str;
        book.SetAuthorName(str);

        cout << "Enter How many pages: ";
        int pages_count;
        cin >> pages_count;

        for (int page = 0; page < pages_count; ++page) {
            cout << "Enter page # " << page + 1 << ": ";
            cin >> str;
            book.AddPage(str);
        }
    }
    void AddBook()
    {
        Book* book = new Book();
        GetBookInfo(*book);
        book_manager.AddBook(*book);
    }
public:
    UserAdmin(UserManager& user_manager, BookManager& book_manager) : user_manager(user_manager), book_manager(book_manager) {
    } 
    void DisplayProfile() {
        user_manager.GetCurrentUser().DisplayProfile();
    }
    void DisplayMenu()
    {
        vector<string> choices{ "View Profile" , "Add Book" , "Logout" };
        while (true)
        {
        Helper::ShowMenu(choices);
            int choice = Helper::ReadInt({ 1 , (int)choices.size() });
            if (choice == 1)
                DisplayProfile();
            else if (choice == 2)
                AddBook();
            else 
                break;
        }
    }

};
class System
{
    private:
    BookManager b;
    UserManager m;
    public:
        void run()
        {
            if (m.AccessSystem()) {
                if (m.GetCurrentUser().IsAdmin())
                {
                    UserAdmin a = UserAdmin(m, b);
                    a.DisplayMenu();
                    run();

                }
                else
                {
                    UserStandard u = UserStandard(m, b);
                    u.DisplayMenu();
                    run();
                }
            }
            cout << "\nExiting the WebSite ..... See you!\n";
        }
};

int main()
{
    System OnlineReader;
    OnlineReader.run(); 
}
