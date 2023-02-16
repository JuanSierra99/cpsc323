#include <iostream>
#include <string>

struct {
    bool a;
    bool b;
    bool c;
    bool d;
}myStruct;

int checkGrammar(std::string str){
    for(const char &ch :str){
        if(ch == 'a'){

        }
        std::cout << myStruct.a;
    }
    return 0;
}

int main(){
    myStruct.a = myStruct.b = myStruct.c = true;
    myStruct.d = false;
    std::string str;
    std::cout << "Enter a string: ";
    std::cin >> str;
    checkGrammar(str);
    return 0;
}

// S[a=S, b=B, c=C, ]
// S->aS
// S->bB
// S->cC
// B->bB
// B->aC
// B->cD
// B->/
// C->aS
// C->bD
// C->cD
// C->/
// D->bD
// D->aB
// D->cC