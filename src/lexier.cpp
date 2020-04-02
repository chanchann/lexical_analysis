//
// Created by Frank on 2020-03-31.
//

#include "lexier.h"

Lexier::Lexier(string input_path)
    :input_path(input_path) {}

Lexier::~Lexier() {
    //vector中存储了对象的指针，调用clear后，并不会调用这些指针所指对象析构函数，因此要在clear之前调用delete；
    for(auto it = token_list.begin(); it!= token_list.end(); it++){
        delete *it;
    }
    token_list.clear();
}

bool Lexier::startParseTokens() {
    int line_number = 1;
    bool parseResult = true;
    string tokenCategory = " ";
    string token;
    // input & output stream
//    ifstream& input = getInput();
//    ofstream& output = getOfstream();

    /* loop find each token in each line for DFA check */
    for (string line; getline(input, line); ){
        this->state = Dfstate::INITIAL;
        // output the line number
//        output << right <<  "Line" <<  setw(4) << line_number << " :" << endl;
        cout << "Line" << line_number << ":" << endl;
        handle(line);

        line_number += 1;
    }

//    input.close();
//    output.close();
    return parseResult;
}


/* open file and return input stream
 * */
ifstream& Lexier::getInput(){

    /* input souce code and reutrn ifstream object */
    static ifstream inputFile( input_path.c_str() ,ios::in);
    if ( !inputFile ){
        cerr << "File could not be opened11" << endl;
        exit(1);
    }else{
        cout << "input file opened!" << endl;
    }

    return inputFile;
}

/* open file and return output stream
 * */
ofstream& Lexier::getOfstream(){
    string output_path = "../output/token.txt";
    int ret = create_dir_not_exist(output_path);
    if(ret == -1) cout << "Directory create error" << endl;
    /* return the output stream for write the result */
    static ofstream outputFile( output_path, ios::out);
    if (!outputFile){
        cerr << "output file could not be opened" << endl;
        exit(1);
    }
    return outputFile;
}

int Lexier::create_dir_not_exist(string path){
    int len = path.length();
    char tmpDirPath[256] = { 0 };
    for(int i = 0 ; i < len; i++){
        tmpDirPath[i] = path[i];
        if(tmpDirPath[i] == '/'){
            if (access(tmpDirPath, F_OK) == -1){
                int ret = mkdir(tmpDirPath, 0777);
                if(ret == -1){
                    return ret;
                }
            }
        }
    }
    return 0;
}


/* for other class get parse result
 * */
vector<Token*> Lexier::getTokenList(){
    return token_list;
}

void Lexier::handle(const std::string& src){
    for (int i = 0; i < src.length(); i++) {
        handleState(src[i]);
    }
}

void Lexier::handleState(const char c){
    switch (this->state) {
        case Dfstate::INITIAL:
            initToken(c, state);
            break;
        case Dfstate::Indentifier:
            if (isAlpha(c) || isDigital(c)) {
                res.back().val += c;
            } else {
                state = Dfstate::Initial;
                initToken(c, state, res);
            }
            break;
        case Dfstate::GT:
            if (c == '=') {
                state = Dfstate::GE;
                res.back().type = TokenType::GE_Type;
                res.back().val += c;
            } else {
                state = Dfstate::Initial;
                initToken(c, state, res);
            }
            break;
        case Dfstate::GE:
            state = Dfstate::Initial;
            initToken(c, state, res);
            break;
        case Dfstate::IntLiteral:
            if (isDigital(c)) {
                res.back().val += c;
            } else {
                state = Dfstate::Initial;
                initToken(c, state, res);
            }
            break;
    }
}

void Lexier::initToken(const char c) {
    if (state != Dfstate::INITIAL)
        return;
    Token token;
    if (isAlpha(c)) {
        state = Dfstate::IDENTIFIER;
        token.setType()
        token.type = TokenType::Identifier_Type;
    } else if (isDigital(c)) {
        state = Dfstate::IntLiteral;
        token.type = TokenType::IntLiteral_Type;
    } else if (c == '>') {
        state = Dfstate::GT;
        token.type = TokenType::GT_Type;
    } else {
        return;
    }

    token.val = c;
    res.push_back(token);
}