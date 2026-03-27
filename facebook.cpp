#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


class Post;
class Date;
class User;
class Page;
class Activity;
class Comment;
class Memory;

template<class T>
void Deallocate2DArray(T** array, int number)
{
	for (int i = 0; i < number; i++)
	{
		delete array[i];
	}
	delete[] array;
}

template<class T>
void Deallocate1DArray(T* array)
{
    delete[] array;
}

class Helper
{
public:
    static int StringLength(const char* str)
    {
        int length = 0;
        for (int i = 0; str[i] != '\0'; i++)
        {
            length++;
        }
        return length;
    }
    static void StringCopy(char*& dest, char*& src)
    {
        int i = 0;
        while (src[i] != '\0')
        {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

    static char* GetStringFromBuffer(const char* str) // overloaded for constant strings
    {
        int size = StringLength(str) + 1;
        char* temp = new char[size];
        for (int i = 0; i < size; i++)
        {
            temp[i] = str[i];
        }
        temp[size] = '\0';
        return temp;
    }

    static char* GetStringFromBuffer(char* str)
    {
        int length = StringLength(str);
        char* temp = new char[length + 1];
        StringCopy(temp, str);
        return temp;
    }
};

class PostContent 
{
public:
    virtual void printActivity() = 0;
    virtual void loadActivity(ifstream& fin) = 0;
};

class Activity : public PostContent
{
private:
    int type;
    char* value;

public:
    Activity()
    {
        type = 0;
        value = nullptr;
    }

    void loadActivity(ifstream& fin) override
    {
        fin >> type;
        char temp[50];
        fin.getline(temp, 50);
        value = Helper::GetStringFromBuffer(temp);
    }


    void printActivity() override
    {
        if (type == 1)
        {
            cout << " is feeling " << value;
        }
        if (type == 2)
        {
            cout << " is thinking about " << value;
        }
        if (type == 3)
        {
            cout << " is making " << value;
        }
        if (type == 4)
        {
            cout << " is celebrating " << value;
        }
    }
    ~Activity()
    {
        if (value != 0)
        {
            Deallocate1DArray(value);
        }
    }
};

class Object
{
private:
    char* id;
protected:
    Post** timeline;
    int total_posts;
public:
    Object()
    {
        id = nullptr;
        timeline = nullptr;
        total_posts = 0;
    }
    char* get_id()
    {
        return id;
    }

    void setID(char* rhs_id)
    {
        id = rhs_id;
    }

    virtual void printData()
    {

    };
    virtual void ViewTimeline()
    {

    };

    virtual void ViewMemory()
    {

    };

    virtual void printPost()
    {

    };

    virtual void AddPosttoTimeline(Post* ptr)
    {

    };

    ~Object()
    {
        if (id)
        {
            Deallocate1DArray(id);
            Deallocate1DArray(timeline);
        }
    }
};

class Comment
{
private:
    char* comment_id;
    char* text;
    Object* commentBy;
public:
    Comment()
    {
        comment_id = text = nullptr;
        commentBy = nullptr;
    }

    void setID(char* id)
    {
        comment_id = id;
    }

    void setCommentBy(Object* comment_ptr)
    {
        commentBy = comment_ptr;
    }
    void setText(const char* textptr)
    {
        int length = Helper::StringLength(textptr) + 1;
        char* temp = new char[length];
        strcpy_s(temp, length, textptr);
        text = temp;
    }
    void printComments()
    {
        commentBy->printData();
        cout << ":\"" << text << "\"" << endl;
    }

    ~Comment()
    {
        if (comment_id != 0)
        {
            Deallocate1DArray(comment_id);
        }
        if (text != 0)
        {
            Deallocate1DArray(text);
        }
    }
};

class Date
{
private:
    int day;
    int month;
    int year;

public:
    static Date currentDate;
    Date()
    {
        day = 0;
        month = 0;
        year = 0;
    }

    Date(int d, int m, int y)
    {
        day = d;
        month = m;
        year = y;
    }

    void LoadDate(ifstream& fin)
    {
        fin >> day;

        fin >> month;

        fin >> year;
    }

    static void setCurrentDate(int d, int m, int y)
    {
        currentDate.day = d;
        currentDate.month = m;
        currentDate.year = y;
    }

    int getDay()
    {
        return day;
    }
    int getMonth()
    {
        return month;
    }
    int getYear()
    {
        return year;
    }

    void PrintDate()
    {
        if (currentDate.day == day && currentDate.month == month && currentDate.year == year)
        {
            cout << "(1h)" << endl;
        }
        else if (currentDate.day - day < 4 && currentDate.month == month && currentDate.year == year)
        {
            cout << "(" << currentDate.day - day << "d)" << endl;
        }
        else
        {
            cout << "(" << day << " / " << month << " / " << year << ")" << endl;
        }
    }

    int TimelineDate()
    {
        if (currentDate.day == day && currentDate.month == month && currentDate.year == year)
        {
            return 1;
        }
        else if (currentDate.day - day < 3 && currentDate.month == month && currentDate.year == year)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

    static Date getCurrentDate()
    {
        return currentDate;
    }

    int operator -(Date rhs)
    {
        int _year = 0;
        if (day == rhs.day && month == rhs.month)
        {
            _year = year - rhs.year;
        }
        return _year;
    }

    bool operator < (const Date& second_date)
    {
        if (year != second_date.year)
        {
            return year < second_date.year;
        }
        if (month != second_date.month)
        {
            return month < second_date.month;
        }
        return day < second_date.day;
    }

    ~Date()
    {

    }
};

class Post
{
private:
    char* post_id;
    Object** likedBy;
protected: Object* sharedBy;
    char* text;
    int like_count;
    int comment_count;
    int checkposts;
    Comment** comments;
    int totalComments;
    int total_posts;
    int checkcomments;
    PostContent* activity;
protected: Date  sharedDate;
public:
    Post()
    {
        post_id = nullptr;
        likedBy = nullptr;
        sharedBy = nullptr;
        text = nullptr;
        activity = nullptr;
        comments = nullptr;
        like_count = 0;
        comment_count = 0;
        totalComments = 0;
        checkcomments = 0;
        checkposts = 0;
        total_posts = 11;
    }

    void setsharedBy(Object* id)
    {
        sharedBy = id;
    }

    Date getshareddate()
    {
        return sharedDate;
    }

    Object* getSharedBy()
    {
        return sharedBy;
    }

    void setSharedDate(const Date& _sharedDate)
    {
        sharedDate = _sharedDate;
    }

    void addLike(Object* ptr)
    {
        if (checkposts == 0)
        {
            likedBy = new Object * [10];
            for (int i = 0; i < 10; i++)
            {
                likedBy[i] = 0;
            }
        }
        likedBy[checkposts] = ptr;

        checkposts++;
    }

    int getTotalLikes()
    {
        return like_count;
    }

    void setTotalLikes(int t_likes)
    {
        like_count = t_likes;
    }

    void LoadPosts(ifstream& fin)
    {
        char buffer[40];
        fin >> buffer;
        post_id = Helper::GetStringFromBuffer(buffer);
        sharedDate.LoadDate(fin);

        char temp[80];
        fin.getline(temp, 80);
        fin.getline(temp, 80);
        text = Helper::GetStringFromBuffer(temp);

    }

    void PrintLikedBy()
    {
        if (likedBy != 0)
        {
            for (int i = 0; i < checkposts; i++)
            {
                likedBy[i]->printData();
                cout << endl;
            }
        }
    }

    void addActivity(ifstream& fin)
    {
        activity = new Activity;
        activity->loadActivity(fin);
    }

    virtual void printPost()
    {
        sharedBy->printData();

        if (activity != nullptr)
        {
            activity->printActivity();
        }
        sharedDate.PrintDate();
        cout << endl << "\"" << text << "\"" << endl;

        if (totalComments != 0)
        {
            for (int i = 0; i < totalComments; i++)
            {
                comments[i]->printComments();
            }
        }
        cout << endl;
    }

    void PrintMemory()
    {
        sharedBy->printData();

        if (activity != nullptr)
        {
            activity->printActivity();
        }
        sharedDate.PrintDate();
        cout << endl << "\"" << text << "\"" << endl;
    }

    void printpage()
    {
        sharedBy->printData();
        if (activity != nullptr)
        {
            activity->printActivity();
        }
        sharedDate.PrintDate();
        cout << endl << "\"" << text << "\"" << endl;
        if (totalComments != 0)
        {
            for (int i = 0; i < totalComments; i++)
            {
                comments[i]->printComments();
                cout << endl;
            }
        }
    }

    void viewLikedList()
    {
        cout << "Post Liked By:" << endl;
        if (like_count == 0)
        {
            cout << " This post has 0 likes";
        }
        cout << endl;
        for (int i = 0; i < like_count; i++)
        {
            cout << likedBy[i]->get_id();
            cout << "\t-";
            likedBy[i]->printData();
            cout << endl;
        }
    }
    void setValues(Post* temp, const char* text2)
    {
        sharedBy = temp->sharedBy;
        sharedDate = Date::currentDate;
        text = Helper::GetStringFromBuffer(text2);
    }

    char* getpost_id()
    {
        return post_id;
    }

    char* getText()
    {
        return text;
    }

    Date getDate()
    {
        return sharedDate;
    }

    int getday()
    {
        return sharedDate.getDay();
    }

    void addComment(Comment* ptr)
    {
        if (checkcomments == 0)
        {
            comments = new Comment * [10];
            for (int i = 0; i < 10; i++)
            {
                comments[i] = 0;
            }
        }
        comments[checkcomments] = ptr;
        checkcomments++;
    }

    void commentIncrement()
    {
        totalComments++;
    }

    void setPostComments(Comment* temp)
    {
        if (comments)
        {
            comments[comment_count] = temp;
            comment_count++;
        }
        else
        {
            comments = new Comment * [10];

            for (int i = 0; i < 10; i++)
            {
                comments[i] = nullptr;
            }

            comments[comment_count] = temp;
            comment_count++;
        }
    }
     virtual ~Post()
      {
          Deallocate1DArray(post_id);
          Deallocate1DArray(text);
          Deallocate1DArray(activity);
          Deallocate1DArray(comments);
          Deallocate1DArray(likedBy);
      }
};

class Page : public Object
{
private:
    char* title;
public:

    Page()
    {
        title = nullptr;
        total_posts = 0;
        timeline = 0;
    }

    void Load_Pages(ifstream& fin)
    {
        char buffer[50];

        fin >> buffer;
        setID(Helper::GetStringFromBuffer(buffer));

        fin.getline(buffer, 50);
        title = Helper::GetStringFromBuffer(buffer);
    }

    void printData()
    {
        cout << title;
    }

    void ViewTimeline()
    {
        if (total_posts == 0)
        {
            cout << "No posts ";
        }
        for (int i = 0; i < total_posts; i++)
        {
            timeline[i]->printpage();
            cout << endl;
        }
    }

    void PrintPageLatest()
    {
        for (int i = 0; i < total_posts; i++)
        {
            Date shareddate = timeline[i]->getshareddate();
            if (shareddate.TimelineDate() == 2 || shareddate.TimelineDate() == 1)
                timeline[i]->printPost();
        }
    }

    void AddPosttoTimeline(Post* ptr)
    {
        if (total_posts == 0)
        {
            timeline = new Post * [10];
            for (int i = 0; i < 10; i++)
            {
                timeline[i] = 0;
            }
        }
        timeline[total_posts] = ptr;
        total_posts++;
    }

    int getTotalPost()
    {
        return total_posts;
    }

    Post* getTimeline(int index)
    {
        return timeline[index];
    }

    char* gettitle()
    {
        return title;
    }

    ~Page()
    {
        Deallocate1DArray(title);
    }
};

class Memory : public Post
{
private:
    Post* orignialPost;
public:
    Memory()
    {
        orignialPost = 0;
    }
    Memory(Post* ptr, const char* text2)
    {
        orignialPost = ptr;
        setValues(ptr, text2);
    }

    void printPost()
    {
        cout << "~~~ ";
        sharedBy->printData();
        cout << " shared a memory ~~~ ";
        sharedDate.PrintDate();
        cout << getText() << endl;

        cout << "~~~" << sharedDate - orignialPost->getDate() << " years ago" << "~~~" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        orignialPost->PrintMemory();
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

    ~Memory()
    {
        if (orignialPost != 0)
        {
            Deallocate1DArray(orignialPost);
        }
    }
};


class User : public Object
{
private:
    char* fname;
    char* sname;
    User** FriendList;
    Page** LikedPages;
    Memory* newPost;
    int number_of_friends;
    int number_of_likedpages;
public:
    User()
    {
        fname = sname = nullptr;
        FriendList = new User * [10];
        LikedPages = new Page * [10];
        for (int i = 0; i < 10; i++)
        {
            FriendList[i] = nullptr;
            LikedPages[i] = nullptr;
        }
        total_posts = number_of_friends = number_of_likedpages = 0;
        newPost = nullptr;
    }
    void Load_Users(ifstream& fin)
    {
        char buffer[80];

        fin >> buffer;
        setID(Helper::GetStringFromBuffer(buffer));

        fin >> buffer;
        fname = Helper::GetStringFromBuffer(buffer);

        fin >> buffer;
        sname = Helper::GetStringFromBuffer(buffer);
    }

    void printData()
    {
        cout << fname << " " << sname;
    }
    void ViewMemory()
    {
        Date today = Date::getCurrentDate();
        for (int i = 0; timeline[i] != 0; i++)
        {
            Date old_date = timeline[i]->getDate();
            if (old_date.getDay() == today.getDay() && old_date.getMonth() == today.getMonth() && old_date.getYear() < today.getYear())
            {
                cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago." << endl << endl;
                cout << "On this day" << endl;
                cout << today.getYear() - old_date.getYear() << " years ago" << endl;
                timeline[i]->printPost();
            }
        }
    }

    void PrintUserLatest()
    {
        for (int i = 0; i < total_posts; i++)
        {
            Date shareddate = timeline[i]->getshareddate();
            if (shareddate.TimelineDate() == 2 || shareddate.TimelineDate() == 1)
                timeline[i]->printPost();
        }
    }

    void ViewHome()
    {
        printData();
        cout << " - Home Page" << endl << endl;
        for (int i = 0; i < total_posts; i++)
        {
            Date shareddate = timeline[i]->getshareddate();
            if (shareddate.TimelineDate() == 2 || shareddate.TimelineDate() == 1)
                timeline[i]->printPost();
        }
        cout << "------------------------------------------------------------" << endl;

        for (int i = 0; i < number_of_friends; i++)
        {
            FriendList[i]->PrintUserLatest();
        }
        cout << "------------------------------------------------------------" << endl;
        int i;
        for (i = 0; i < number_of_likedpages; i++)
        {
            LikedPages[i]->PrintPageLatest();
        }

    }

    int gettotalposts()
    {
        return total_posts;
    }

    char* getf()
    {
        return fname;
    }
    char* gets()
    {
        return sname;
    }

    User** getFriendList()
    {
        return FriendList;
    }

    void AddFriend(User* newfriend)
    {
        for (int i = 0; i < number_of_friends; i++)
        {
            if (FriendList[i] == newfriend)
            {
                return;
            }
        }

        User** newFriendList = new User * [number_of_friends + 1];
        for (int i = 0; i < number_of_friends; i++)
        {
            newFriendList[i] = FriendList[i];
        }
        newFriendList[number_of_friends] = newfriend;        
        delete[] FriendList;
        FriendList = newFriendList;
        number_of_friends++;
    }

    void LikePages(Page* page)
    {
        for (int i = 0; i < number_of_likedpages; i++)
        {
            if (LikedPages[i] == page)
            {
                return;
            }
        }

        Page** newLikedPages = new Page * [number_of_likedpages + 1];
        for (int i = 0; i < number_of_likedpages; i++)
        {
            newLikedPages[i] = LikedPages[i];
        }
        newLikedPages[number_of_likedpages] = page;
        delete[] LikedPages;
        LikedPages = newLikedPages;
        number_of_likedpages++;
    }

    void ViewFriendList()
    {
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << fname << " " << sname << " - Friend List" << endl;
        if (number_of_friends == 0)
        {
            cout << "User has no friends" << endl;
            return;
        }
        for (int i = 0; i < number_of_friends; i++)
        {
            cout << FriendList[i]->get_id() << " - " << FriendList[i]->getf() << " " << FriendList[i]->gets() << endl;
        }
        cout << "-------------------------------------------------------------------------------" << endl;
    }

    void ViewLikedPages()
    {
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << fname << " " << sname << " - Liked Pages" << endl;
        if (number_of_likedpages == 0)
        {
            cout << "No Page currently liked" << endl;
            return;
        }
        for (int i = 0; i < number_of_likedpages; i++)
        {
            cout << LikedPages[i]->get_id() << " -" << LikedPages[i]->gettitle() << endl;
        }
        cout << "-------------------------------------------------------------------------------" << endl;
    }

    void AddPosttoTimeline(Post* ptr)
    {
        if (total_posts == 0)
        {
            timeline = new Post * [10];
            for (int i = 0; i < 10; i++)
            {
                timeline[i] = 0;
            }
        }
        timeline[total_posts] = ptr;
        total_posts++;
    }

    void UpdateTimeline()
    {
        for (int i = 0; i < total_posts - 1; i++)
        {
            for (int j = 0; j < total_posts; j++)
            {
                if (timeline[i]->getDate() < timeline[j]->getDate())
                {
                    Post* temp = timeline[i];
                    timeline[i] = timeline[j];
                    timeline[j] = temp;
                }
                else
                {
                    Post* temp = timeline[j];
                    timeline[j] = timeline[i];
                    timeline[i] = temp;
                }
            }
        }
    }

    void ViewTimeline()
    {
        printData();
        cout << " -Timeline" << endl << endl;
        if (total_posts == 0)
        {
            cout << "No posts ";
        }
        for (int i = 0; i < total_posts; i++)
        {
            timeline[i]->printPost();
            cout << endl;
        }
    }

    void shareMemory(Post* ptr)
    {
        newPost = new Memory(ptr, "Never thought I will be a specialist in this field");
        AddPosttoTimeline(newPost);
    }

    ~User()
    {
        Deallocate1DArray(fname);
        Deallocate1DArray(sname);
        Deallocate1DArray(FriendList);
        Deallocate1DArray(LikedPages);
    }
};

Date Date::currentDate;
class Controller
{
private:
    User** AllUsers;
    Page** AllPages;
    Post** AllPosts;
    int totalUsers;
    int totalPages;
    int totalPosts;
    Object* current_user;
    int totalComments;
    int totalactivities;
    Activity* activity;
public:
    Controller()
    {
        AllUsers = new User * [totalUsers];
        AllPages = new Page * [totalPages];
        AllPosts = new Post * [totalPosts];
        for (int i = 0; i < totalUsers; i++)
        {
            AllUsers[i] = nullptr;
        }
        for (int i = 0; i < totalPages; i++)
        {
            AllPages[i] = nullptr;
        }
        for (int i = 0; i < totalPosts; i++)
        {
            AllPosts[i] = nullptr;
        }
        totalactivities = totalComments = totalPages = totalPosts = totalUsers = 0;
        current_user = nullptr;
        activity = nullptr;
    }



    void  LoadData()
    {
        ifstream userFile("User.txt");
        ifstream pageFile("Pages.txt");
        ifstream friendFile("FriendList.txt");
        ifstream Posts("post.txt");
        ifstream Comments("comments.txt");
        ifstream Activity("activity.txt");

        LoadAllUsers(userFile);
        LoadAllPages(pageFile);
        LinkUserandPages(friendFile);
        LoadPostsfromFile(Posts);
        LoadCommentsfromFile(Comments);
        LoadActivityfromFile(Activity);

        userFile.close();
        pageFile.close();
        friendFile.close();
        Posts.close();
        Comments.close();
        Activity.close();
    }

    void LoadAllUsers(ifstream& fin)
    {
        fin >> totalUsers;
        AllUsers = new User * [totalUsers];

        for (int i = 0; i < totalUsers; i++)
        {
            AllUsers[i] = new User;
            AllUsers[i]->Load_Users(fin);
        }
    }

    void LoadAllPages(ifstream& fin)
    {
        fin >> totalPages;
        AllPages = new Page * [totalPages];
        for (int i = 0; i < totalPages; i++)
        {
            AllPages[i] = new Page;
            AllPages[i]->Load_Pages(fin);
        }
        cout << endl;
    }

    User* SearchUserByID(const char* user_id)
    {
        for (int i = 0; i < totalUsers; i++)
        {
            if (strcmp(AllUsers[i]->get_id(), user_id) == 0)
            {
                return AllUsers[i];
            }
        }
        return nullptr;
    }

    Page* SearchPageByID(const char* page_id)
    {
        for (int i = 0; i < totalPages; i++)
        {
            if (strcmp(AllPages[i]->get_id(), page_id) == 0)
            {
                return AllPages[i];
            }
        }
        return nullptr;
    }

    Post* SearchPostByID(const char* post_id)
    {
        for (int i = 0; i < totalPosts; i++)
        {
            if (strcmp(AllPosts[i]->getpost_id(), post_id) == 0)
            {
                return AllPosts[i];
            }
        }
        return nullptr;
    }

    Object* SearchbyID(const char* id)
    {
        if (id[0] == 'u')
        {
            Object* userptr = SearchUserByID(id);
            return userptr;
        }
        else if (id[0] == 'p')
        {
            Object* pageptr = SearchPageByID(id);
            return pageptr;
        }
        else
        {
            return nullptr;
        }
    }

    void LinkUserandPages(ifstream& fin)
    {
        char buffer[40];
        while (fin >> buffer)
        {
            if (strcmp(buffer, "-1") != 0)
            {
                User* user = SearchUserByID(buffer);

                if (user != nullptr)
                {
                    while (fin >> buffer && strcmp(buffer, "-1") != 0)
                    {
                        User* friend_of_user = SearchUserByID(buffer);
                        if (friend_of_user != nullptr)
                        {
                            user->AddFriend(friend_of_user);
                        }
                    }
                    while (fin >> buffer && strcmp(buffer, "-1") != 0)
                    {

                        Page* page = SearchPageByID(buffer);
                        if (page != nullptr)
                        {
                            user->LikePages(page);
                        }
                    }
                }
            }
        }
    }

    void LoadPostsfromFile(ifstream& fin)
    {
        char buffer[80];
        fin >> totalPosts;
        AllPosts = new Post * [totalPosts];
        for (int i = 0; i < totalPosts; i++)
        {
            AllPosts[i] = new Post;
            AllPosts[i]->LoadPosts(fin);
            fin >> buffer;

            Object* ptr = SearchbyID(buffer);
            if (ptr)
            {
                AllPosts[i]->setsharedBy(ptr);
                ptr->AddPosttoTimeline(AllPosts[i]);
            }
            int likes = 0;
            fin >> buffer;
            while (buffer[0] != '-')
            {
                Object* likerptr = SearchbyID(buffer);
                AllPosts[i]->addLike(likerptr);
                likes++;
                fin >> buffer;
            }
            AllPosts[i]->setTotalLikes(likes);
        }
        cout << endl;
    }

    void LoadCommentsfromFile(ifstream& fin)
    {

        fin >> totalComments;
        char buffer[80];
        char id[10];

        for (int i = 0; i < totalComments; i++)
        {
            Comment* commentptr = new Comment;

            fin >> buffer;
            commentptr->setID(buffer);

            fin >> buffer;

            Post* ptr = SearchPostByID(buffer);


            ptr->addComment(commentptr);
            ptr->commentIncrement();

            fin >> id;

            Object* commentbyPTR = SearchbyID(id);
            commentptr->setCommentBy(commentbyPTR);
            fin.getline(buffer, 70);
            commentptr->setText(buffer);
        }
    }

    void newComment(const char* post, const char* text)
    {
        Post* pst = SearchPostByID(post);

        Comment* newComment = new Comment;
        newComment->setCommentBy(current_user);
        newComment->setText(text);

        pst->addComment(newComment);
        pst->commentIncrement();
    }

    void setcurrentuser(const char* id)
    {
        current_user = SearchUserByID(id);
    }

    void LikePost(const char* post)
    {
        Post* liked_post = SearchPostByID(post);

        int i = 0;
        i = liked_post->getTotalLikes();
        i++;
        liked_post->setTotalLikes(i);
        liked_post->addLike(current_user);
    }

    void LoadActivityfromFile(ifstream& fin)
    {
        char buffer[60];
        fin >> totalactivities;
        for (int i = 0; i < totalactivities; i++)
        {
            fin >> buffer;
            Post* post_id = SearchPostByID(buffer);
            post_id->addActivity(fin);
        }
    }

    ~Controller()
    {
        Deallocate2DArray(AllUsers, totalUsers);
        Deallocate2DArray(AllPages, totalPages);
        Deallocate2DArray(AllPosts, totalPosts);
    }

    void run()
    {
        cout << "Command: Set current System Date 17 4 2024" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        int day = 17, month = 4, year = 2024;
        Date::setCurrentDate(day, month, year);
        cout << "System Date: " << day << "/" << month << "/" << year << endl << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        setcurrentuser("u7");
        User* currentUser = SearchUserByID("u7");
        cout << "Command: Set current user u7" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;

        cout << currentUser->getf() << " " << currentUser->gets() << " successfully set as Current User" << endl << endl;

        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Command: View Friend List" << endl << endl;
        currentUser->ViewFriendList();
        cout << endl;

        cout << "Command: View Liked Pages" << endl << endl;
        currentUser->ViewLikedPages();

        Post* post = SearchPostByID("post10");

        cout << "Command: View Timeline" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        currentUser->ViewTimeline();
        cout << "-------------------------------------------------------------------------------" << endl;

        Post* current_post = SearchPostByID("post5");

        cout << "Command: ViewLikedList(post5)" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        current_post->viewLikedList();
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Command: LikePost(post5)" << endl;
        LikePost("post5");
        cout << "Command: ViewLikedList(post5)" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        current_post->viewLikedList();
        cout << "-------------------------------------------------------------------------------" << endl;


        Page* current_page = SearchPageByID("p1");


        cout << "Command: ViewPage(p1)" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        current_page->ViewTimeline();
        cout << "-------------------------------------------------------------------------------" << endl;


        cout << "Command: View Home  " << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        currentUser->ViewHome();
        cout << "-------------------------------------------------------------------------------" << endl;

        newComment("post4", "Good Luck for your Result");
        Post* current_post2 = SearchPostByID("post4");

        cout << "Command: Post comment(post4,Good Luck for your Result)" << endl;
        cout << "Command: ViewPost(Post4)" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        current_post2->printPost();
        cout << "-------------------------------------------------------------------------------" << endl;


        newComment("post8", "Thanks for the wishes");
        Post* current_post3 = SearchPostByID("post8");
        cout << "Command: Post comment(post8,Thanks for the wishes)" << endl;
        cout << "Command: ViewPost(Post8)" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        current_post3->printPost();
        cout << "-------------------------------------------------------------------------------" << endl;

        cout << "Command: SeeYourMemories" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        current_user->ViewMemory();
        cout << "-------------------------------------------------------------------------------" << endl;

        Post* current_post4 = SearchPostByID("post10");


        currentUser->shareMemory(current_post4);
        cout << "Command: ShareMemory(post10, 'Never thought I will be specialist in this field...')" << endl;
        cout << "Command: ViewTimeline" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        currentUser->UpdateTimeline();
        currentUser->ViewTimeline();
        cout << "-------------------------------------------------------------------------------" << endl;
    }
};

int main()
{
    Controller facebook;
    facebook.LoadData();
    facebook.run();
    return 0;
}