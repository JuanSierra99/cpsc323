#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

std::string reserved[] = {"while", "for", "switch", "do", "return"};

// this struct will allow us to keep track if a token is a number, reserved identifier, or valid identifer.
struct{
    std::string str;
    bool number;
    bool reserved;
    bool identifier;
}Token;

std::string number(std::string token){
    //checks to see if token is a number
    for (char const &ch : token){
        if (isdigit(ch) == 0){
            return "no";
        }
    }
    Token.number = true;  // if it is a number set the struct value to true
    return "YES";
}

std::string reserve(std::string token){
    // checks to see if token is a reserved word
    for (int i = 0; i < reserved->length(); i++){
        if(token == reserved[i]){
            Token.reserved = true;  //if it is reserved word, set the struct value to true
            return "YES";
        }
    }
    return "no";
}

std::string identifier(std::string token){
    //if the token is not a number, and not reserved, then it may be an identifier.
    if(Token.number == true || Token.reserved == true){
        return "no";
    }
    //if it is not an alphabet character, and not an underscore, it is not identifier
    else if(isalpha(token[0]) == 0 && token[0] != '_'){
            return "no";
    }
    else{
        // for token to be identifier it has to be an underscore, alphabet char, or number.
        for (char const &ch : token){
            if (ch != '_' && isalpha(ch)==0 && isnumber(ch)==0){
                return "no";
                 }
        }
    }
    Token.identifier = true;  // it is an identifier. set it to true.
    return "YES";
}

int main(){
std::ifstream File;  // to read from our sample txt file.
std::string str;
File.open("sample.txt");
std::cout << "Token        number      reserved      identifier" << std::endl;
std::cout << std::setfill('-') << std::setw(50) << "-" << std::endl;
std::cout << std::setfill(' ');
if(File.is_open()){
    while(File.good()){
        //  we set every struct variable to false in each iteration of the loop so that other 
        //  tokens do not take the previous tokens values.
        File >> Token.str;
        Token.number = false;
        Token.identifier = false;
        Token.reserved = false;
        std::cout << std::setw(8) << Token.str;
        std::cout << std::setw(10) << number(Token.str);
        std::cout << std::setw(12) << reserve(Token.str);
        std::cout << std::setw(16) << identifier(Token.str);
        std::cout << std::endl;
    }
}
return 0;
}
