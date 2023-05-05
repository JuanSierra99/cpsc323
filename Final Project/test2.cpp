#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cstring>
#include <stack>
#include <vector>

using namespace std;

// Size of the predictive parsing table, update as required
#define MAX_EXPRESSIONS 23
#define MAX_LANGUAGE 30

// Predictive Parsing Table
std::string parsingTable[MAX_EXPRESSIONS][MAX_LANGUAGE] = 
{
/*<prog>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "program <identifier> ; var <dec-list> begin <stat-list> end.", "", "", "", "" }, // CHECKED
/*<identifier>*/ { "", "", "", "", "", "", "", "", "", "", "<letter> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<X>*/         { "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<digit> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "<letter> <X>", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "NULL", "NULL", "", "", "", "", "", "" }, // CHECKED
/*<dec-list>*/  { "", "", "", "", "", "", "", "", "", "", "<dec> : <type> ;", "<dec> : <type> ;", "<dec> : <type> ;", "<dec> : <type> ;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<dec>*/       { "", "", "", "", "", "", "", "", "", "", "<identifier> , <dec>", "<identifier> , <dec>", "<identifier> , <dec>", "<identifier> , <dec>", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<D>*/         { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "NULL", "", "", ", <dec>", "", "", "", "", "", "", "" }, // CHECKED
/*<type>*/       { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "integer", "" }, // CHECKED
/*<stat-list>*/  { "", "", "", "", "", "", "", "", "", "", "<stat> <L>", "<stat> <L>", "<stat> <L>", "<stat> <L>", "", "", "", "", "", "", "", "", "", "", "","", "", "<stat> <L>", "", "" }, // CHECKED
/*<L>*/         { "", "", "", "", "", "", "", "", "", "", "<stat-list>", "<stat-list>", "<stat-list>", "<stat-list>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<stat-list>", "", "NULL" }, // CHECKED
/*<stat>*/       { "", "", "", "", "", "", "", "", "", "", "<assign>", "<assign>", "<assign>", "<assign>", "", "", "", "", "", "", "", "", "", "", "", "", "", "<write>", "", "" }, // CHECKED
/*<write>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "display ( <identifier> ) ; display( \" value \" , <identifier> ) ;", "", "" }, // CHECKED
/*<W>*/         { "", "", "", "", "", "", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "", "", "", "", "\" value \" ,", "", "", "", "", "" }, // CHECKED
/*<assign>*/    { "", "", "", "", "", "", "", "", "", "", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "<identifier> = <expr> ;", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<expr>*/      { "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "<term> <expr'>", "", "", "<term> <expr'>", "<term> <expr'>", "", "", "", "<term> <expr'>", "", "", "", "", "", "", "", "" }, // CHECKED
/*<expr'>*/     { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "- <term> <expr'> ", "- <term> <expr'> ", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" }, // CHECKED, but recheck for the + sign
/*term>*/       { "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "<factor> <term'>", "", "", "<factor> <term'>", "<factor> <term'>", "", "", "", "<factor> <term'>", "", "", "", "", "", "", "", "" }, // CHECKED
/*<term'>*/      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "/ <factor> <term'> ", "* <factor> <term'> ", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" }, // CHECKED
/*<factor>*/     { "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", "<number>", " <identifier>", " <identifier>", " <identifier>", " <identifier>", "", "", "<number>", "<number>", "", "", "", "( <expr> )", "", "", "", "", "", "", "", "" }, //CHECKED
/*<number>*/    { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "<sign> <digit> <Y>", "<sign> <digit> <Y>", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<Y>*/         { "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "<digit><Y>", "", "", "", "", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "NULL", "", "", "", "", "", "" },
/*<sign>*/      { "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "", "", "", "", "", "", "+", "-", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<digit>*/     { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
/*<letter>*/     { "", "", "", "", "", "", "", "", "", "", "p", "q", "r", "s", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // CHECKED
};      

// Predicting Parsing Table Headers
std::string language[MAX_LANGUAGE] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "p", "q", "r", "s", "/", "*", "+", "-", ";", ":", "=", "(", ",", ")", "\"", "program", "begin", "display", "integer", "end."};
std::string expressions[MAX_EXPRESSIONS] = {
    "<prog>", 
    "<identifier>",
    "<X>", 
    "<dec-list>", 
    "<dec>", 
    "<D>", 
    "<type>",
    "<stat-list>",
    "<L>", 
    "<stat>", 
    "<write>", 
    "<W>" 
    "<assign>", 
    "<expr", 
    "<expr'>",
    "<term>", 
    "<term'>", 
    "<factor>", 
    "<number>", 
    "<Y>", 
    "<sign>", 
    "<digit>", 
    "<letter>"
};

int main() {
    
    for (int i=0; i<MAX_LANGUAGE; i++){
        cout << "(" << i << " " << language[i] << ") \n";
    }

    cout << endl;

    for (int i=0; i<MAX_EXPRESSIONS; i++){
        cout << "(" << i << " " << expressions[i] << ") \n";
    }

}