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
    line_number = 1;
    bool parseResult = true;
    // input & output stream
    ifstream& input = getInput();
    ofstream& output = getOfstream();

    /* loop find each token in each line for DFA check */
    for (string line; getline(input, line); ){
        this->state = Dfstate::Initial;
        // output the line number
//        output << right <<  "Line" <<  setw(4) << line_number << " :" << endl;
        cout << "Line" << line_number << ":" << endl;
        handle(line);
        line_number += 1;
    }

    input.close();
    output.close();
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
vector<Token*>& Lexier::getTokenList(){
    return token_list;
}

void Lexier::handle(const std::string& src){
    for (int i = 0; i < src.length(); i++) {
        handleState(src[i]);
    }
}

void Lexier::handleState(const char c){
    switch (this->state) {
        case Dfstate::Initial:
            initToken(c);  // 重新确定后续状态
            break;
        case Dfstate::Id:
            if (isAlpha(c) || isDigital(c)) {
                tokenText.push_back(c);  // 保持标识符状态
            } else {
                initToken(c);   // 退出标识符状态，并保存Token
            }
            break;
        case Dfstate::GT:
            if (c == '=') {
                token->setType(TokenType::GE);  // 转化为GE
                state = Dfstate::GE;
                tokenText.push_back(c);
            } else {
                initToken(c);  // 退出标识符状态，并保存Token
            }
            break;
        case GE:  // >=
        case Assignment:  // =
        case Plus: // +
        case Minus: // -
        case Star: // *
        case Slash: // /
        case SemiColon: // ;
        case LeftParen: // (
        case RightParen: // )
            initToken(c);   // 退出当前状态，并保存Token
            break;
        case IntLiteral:
            if (isDigital(c)) {
                tokenText.push_back(c);  //继续保持在数字字面量状态
            } else {
                initToken(c);  //退出当前状态，并保存Token
            }
            break;
        case Id_int1:
            if (c == 'n') {    // in
                state = Dfstate::Id_int2;
                tokenText.push_back(c);
            } else if (isDigital(c) || isAlpha(c)) {
                state = Dfstate::Id;    //切换回Id状态
                tokenText.push_back(c);
            } else {
                initToken(c);
            }
            break;
        case Id_int2:
            if (c == 't') {   // int
                state = Dfstate::Id_int3;
                tokenText.push_back(c);
            } else if (isDigital(c) || isAlpha(c)) {
                state = Dfstate::Id;    //切换回id状态
                tokenText.push_back(c);
            } else {
                initToken(c);
            }
            break;
        case Id_int3:
            if (isBlank(c)) {
                // TODO:增添 keyword标识
                token->setType(TokenType::Int);
                initToken(c);
            } else {
                state = Dfstate::Id;    //切换回Id状态
                tokenText.push_back(c);
            }
            break;
//        default: //这里如何处理？
    }
    //把最后一个token送进去
    if(tokenText.length() > 0) initToken(c);

}

void Lexier::initToken(const char c) {

    if(tokenText.length() > 0) {
        token->setText(tokenText);
        token->setLineNumber(line_number);
        token_list.push_back(token);

        tokenText.clear();
//        token = new Token;

    }

    state = Dfstate::Initial;

    if (isAlpha(c)) {    // 第一个字符是字母
        if(c == 'i'){   //第一个字符是i
            state = Dfstate::Id_int1;
        }else{
            state = Dfstate::Id;
        }
        token->setType(TokenType::Identifier);
    } else if (isDigital(c)) {   // 第一个字符是数字
        state = Dfstate::IntLiteral;
        token->setType(TokenType::IntLiteral);
    } else if (c == '>') {
        state = Dfstate::GT;
        token->setType(TokenType::GT);
    } else if (c == '+'){
        state = Dfstate::Plus;
        token->setType(TokenType::Plus);
    } else if (c == '-'){
        state = Dfstate::Minus;
        token->setType(TokenType::Minus);
    } else if (c == '*'){
        state = Dfstate::Star;
        token->setType(TokenType::Star);
    } else if (c == '/'){
        state = Dfstate::Slash;
        token->setType(TokenType::Slash);
    } else if (c == ';'){
        state = Dfstate::SemiColon;
        token->setType(TokenType::SemiColon);
    } else if (c == '('){
        state = Dfstate::LeftParen;
        token->setType(TokenType::LeftParen);
    } else if (c == ')'){
        state = Dfstate::RightParen;
        token->setType(TokenType::RightParen);
    } else if (c == '='){
        state = Dfstate::Assignment;
        token->setType(TokenType::Assignment);
    }
    else {
        state = Dfstate::Initial; // skip all unknown patterns
        return;
        // TODO : 错误处理
    }

    tokenText.push_back(c);
}