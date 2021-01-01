#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <map>
#include <array>
#include <set>
#include <stdio.h>
#include <algorithm>

using namespace std;

map<int, vector<string>> my_database;
vector<string> v;

//indirect-ordering-maps
map<string, vector<int>> name_ordered;
map<string, vector<int>> subject_ordered;
map<int, vector<int>> score_ordered;


//because map automatically sorts the key, i choose maps, instead of the other containers,
//to sort each data( name, subject, score) indirectly and refer the value of these
//indirect-ordering-maps back to the key of main map(my_databse).

//In this implimentation, i dont need to worry about sorting the data as the map does it itself.
//However, i used a comparator functor for the comparision of the scores.
//As maps has unique keys. Student names and scores are not repeted.

//I used vectors to store the sorted index in indirect-ordering-maps because vectors dynamically resize according to need.
//I used vectors to store the string in main map(my_database) because vectors dynamically resize according to need.



class functorCompare{
public:
    functorCompare(int p_limit, int p_score) : limit(p_limit), score(p_score){}
    bool operator()(){
        return limit < score;
    }
private:
    int limit;
    int score;
};

class functorCompare_string{
public:
    functorCompare_string(string p_soc, string p_s) : string_of_choice(p_soc), string_(p_s){}
    bool operator()(){
        return string_ == string_of_choice;
    }
private:
    string string_of_choice;
    string string_;
};

int main() {

   char package[100];
   char name[100];
   char subject[100];
   char score[10];
   char *contents;
   int i=0;
   string r_subject;
   string r_name;
   int total_students=0;


    FILE *input = fopen("C:\\Users\\omsus\\CLionProjects\\Assignments\\Trimester2 project4\\gendata\\cmake-build-debug\\database.txt","r");
    while(!feof(input)){
       ++i;
       fgets(package,100,input);
       contents = strtok(package, " ");
       strcpy(name, contents);
       v.push_back(name);
       contents = strtok(NULL, " ");
       strcpy(subject, contents);
       v.push_back(subject);
       contents = strtok(NULL," ");
       strcpy(score,contents);
       v.push_back(score);
       my_database[i] = v;
       v.clear();
   }


    for(auto it = my_database.begin();it != my_database.end();++it){
        ++total_students;
        name_ordered[it->second[0]].push_back(it->first);
    }
    total_students /= 4;

    //re-calculating and adding the total marks
    int total = 0;
    string temp;
    for(auto it = name_ordered.begin();it != name_ordered.end();++it) {
        total = 0;
        for(auto ite = it->second.begin();ite != it->second.end();++ite) {
            total +=  stoi(my_database[*ite][2]);
        }
        v.clear();
        v.push_back(it->first);
        v.push_back("Total");
        v.push_back(to_string(total)+'\n');
        my_database[++i]=(v);
    }

    name_ordered.clear();
    for(auto it = my_database.begin();it != my_database.end();++it){
        name_ordered[it->second[0]].push_back(it->first);
        subject_ordered[it->second[1]].push_back(it->first);
        score_ordered[stoi(it->second[2])].push_back(it->first);
    }
    int repeat =1;
    while(repeat==1) {

        cout << "-----------------------------WELCOME TO THE PROGRAM-----------------------------" << endl << endl;
        cout << "CHOOSE ONE OF THE PRINTING OPTIONS" << endl;
        cout << "1.   PRINT BY ORDER OF NAME" << endl;
        cout << "2.   PRINT BY ORDER OF SUBJECT" << endl;
        cout << "3.   PRINT BY ORDER OF RAW SCORE" << endl;
        cout << "4.   PRINT BY ORDER OF TOTAL/EVALUATED SCORE" << endl;
        cout << "5.   PRINT INFO ABOUT ONLY ONE PERSON" << endl;
        cout << "6.   PRINT BY ORDER OF MARKS HIGHER THAN IN A PERTICULAR SUBJECT" << endl;
        cout << "--------------------------------------------------------------------------------" << endl;
        int reply;
        cin >> reply;

        switch (reply) {
            case 1: {
                //printing name by order
                for (auto it = name_ordered.begin(); it != name_ordered.end(); ++it) {
                    cout << "---------------------------------" << endl;
                    for (auto ite = it->second.begin(); ite != it->second.end(); ++ite) {
                        cout << my_database[*ite][0] << " " << my_database[*ite][1] << " " << my_database[*ite][2];
                    }
                }
                cout << "---------------------------------" << endl;
                cout<<" THERE WERE "<<total_students<<" STUDENTS"<<endl;
                break;
            }
            case 2: {
                cout << endl << "--------------------------------------------------------------------------------"
                     << endl;
                cout << "     CHOOSE A SUBJECT  1[physics] , 2[mathematics], 3[chemistry], and 4[biology]" << endl;
                cin >> reply;

                switch (reply) {
                    case 1: {
                        r_subject = "Physics";
                    }
                        break;
                    case 2: {
                        r_subject = "Mathematics";
                    }
                        break;
                    case 3: {
                        r_subject = "Chemistry";
                    }
                        break;
                    case 4: {
                        r_subject = "Biology";
                    }
                        break;
                }

                //Printing the data of particular subject

                for (auto it = score_ordered.begin(); it != score_ordered.end(); ++it) {
                    for (auto ite = it->second.begin(); ite != it->second.end(); ++ite) {
                        functorCompare_string cs(r_subject, my_database[*ite][1]);
                        bool compare = cs();
                        if (compare) {
                            cout << my_database[*ite][0] << " " << my_database[*ite][1] << " " << my_database[*ite][2];
                        }

                    }
                }
                break;
            }
            case 3: {
                //printing by order of (raw) score
                for (auto it = score_ordered.begin(); it != score_ordered.end(); ++it) {
                    for (auto ite = it->second.begin(); ite != it->second.end(); ++ite) {
                        cout << my_database[*ite][0] << " " << my_database[*ite][1] << " " << my_database[*ite][2];
                    }
                }
                break;
            }
            case 4: {
                //printing by order of total/evaluated score
                for (auto it = score_ordered.begin(); it != score_ordered.end(); ++it) {
                    for (auto ite = it->second.begin(); ite != it->second.end(); ++ite) {
                        functorCompare_string cs("Total", my_database[*ite][1]);
                        bool compare = cs();
                        if (compare) {
                            r_name = my_database[*ite][0];
                            cout << "---------------------------------" << endl;
                            for (auto ite = name_ordered.find(r_name)->second.begin();
                                 ite != name_ordered.find(r_name)->second.end(); ++ite) {
                                cout << my_database[*ite][0] << " " << my_database[*ite][1] << " "
                                     << my_database[*ite][2];
                            }
                        }

                    }
                }
                break;
            }

            case 5: {
                cout << endl << "--------------------------------------------------------------------------------"
                     << endl;
                cout << "     ENTER THE NAME YOU WANT TO SEARCH THE DATA FOR" << endl;
                cin >> r_name;
                for (auto ite = name_ordered.find(r_name)->second.begin();
                     ite != name_ordered.find(r_name)->second.end(); ++ite) {
                    cout << my_database[*ite][0] << " " << my_database[*ite][1] << " " << my_database[*ite][2];
                }
                break;
            }

            case 6: {
                cout << endl << "--------------------------------------------------------------------------------"
                     << endl;
                cout << "     CHOOSE A SUBJECT  1[physics] , 2[mathematics], 3[chemistry], and 4[biology]" << endl;
                cin >> reply;

                switch (reply) {
                    case 1: {
                        r_subject = "Physics";
                    }
                        break;
                    case 2: {
                        r_subject = "Mathematics";
                    }
                        break;
                    case 3: {
                        r_subject = "Chemistry";
                    }
                        break;
                    case 4: {
                        r_subject = "Biology";
                    }
                        break;
                }

                cout << "     ENTER THE MARKS " << endl;
                cin >> reply;
                int count = 0;
                //Printing the data of particular subject
                for (auto it = score_ordered.begin(); it != score_ordered.end(); ++it) {
                    for (auto ite = it->second.begin(); ite != it->second.end(); ++ite) {
                        functorCompare_string cs(r_subject, my_database[*ite][1]);
                        bool compare = cs();
                        if (compare) {
                            functorCompare c(reply,stoi(my_database[*ite][2]));
                            bool compare_score = c();
                            if(compare_score){
                                cout << my_database[*ite][0] << " " << my_database[*ite][1] << " " << my_database[*ite][2];
                                ++count;
                            }
                        }
                    }
                }
                cout << "--------------------------------------------" << endl;
                cout<<"Out of "<<total_students<<" students"<<endl;
                cout << "The number of students that have scored more than " << reply
                     << " in this particular subject is: " << count << endl<<endl;
                count = 0;
                break;
            }
        }
    cout<<"----------------- RUN THE PROGRAM AGAIN ? (0. no/1. yes)------------------"<<endl;
    cin>> repeat;
    }
    return 0;
}