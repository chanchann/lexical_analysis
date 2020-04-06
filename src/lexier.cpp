//
// Created by Frank on 2020-03-31.
//

#include "lexier.hpp"

Lexier::Lexier(string input_path)
    :input_path(input_path),token(new Token()) {}

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
        line.push_back('\n'); // 添加个结束符结尾
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
            if (isAlpha(c) || isDigital(c) || c == '_') {
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
        case Dfstate::Plus: // +
        case Dfstate::Minus: // -
        case Dfstate::GE:  // >=
        case Dfstate::Assignment:  // =
        case Dfstate::Star: // *
        case Dfstate::Slash: // /
        case Dfstate::SemiColon: // ;
        case Dfstate::LeftParen: // (
        case Dfstate::RightParen: // )
        case Dfstate::BigLeftParen: // {
        case Dfstate::BigRightParen: // }
            initToken(c);   // 退出当前状态，并保存Token
            break;
        case Dfstate::IntLiteral:
            if (isDigital(c)) {
                tokenText.push_back(c);  //继续保持在数字字面量状态
            } else if(c == '.'){
                tokenText.push_back(c);
                state = Dfstate::FloatLiteral;
                token->setType(TokenType::FloatLiteral);
            } else if(c == ')' || c == ';'){
                initToken(c);  //退出当前状态，并保存Token
            } else if(c == 'e' || c == 'E') {
                tokenText.push_back(c);
                state = Dfstate::ScienceLiteral1;
                token->setType(TokenType::ScienceLiteral);
            }else {
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::ScienceLiteral1:
            if(c == '-' || c == '+' || isDigital(c)){
                state = Dfstate::ScienceLiteral;
                tokenText.push_back(c);
            }else {
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::ScienceLiteral:
            if(isDigital(c)){
                tokenText.push_back(c);
            } else if(isBlank(c)|| c == ';' || c == ')' || c == ','){
                initToken(c);
            } else{
                errFunc(tokenText, c);
            }
            break;

        case Dfstate::FloatLiteral:
            if (isDigital(c)) {
                tokenText.push_back(c);  //继续保持在浮点数字面量状态
            } else if(c == 'e' || c == 'E') {
                tokenText.push_back(c);
                state = Dfstate::ScienceLiteral1;
                token->setType(TokenType::ScienceLiteral);
            }else {
                initToken(c);  // 退出当前状态，并保存Token
            }
            break;
        case Dfstate::Id_int_if1:  // i
            if (c == 'n') {    // in
                state = Dfstate::Id_int2;
                tokenText.push_back(c);
            } else if (c == 'f'){ // if
                state = Dfstate::Id_if2;
                tokenText.push_back(c);
            }else if (isDigital(c) || isAlpha(c)) {
                state = Dfstate::Id;    //切换回Id状态
                tokenText.push_back(c);
            } else {
                initToken(c);
            }
            break;
        case Dfstate::Id_if2:
            if(isBlank(c) || c == '('){
                token->setType(TokenType::If);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_int2:
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
        case Dfstate::Id_int3:  // int
            if (isBlank(c)) {
                token->setType(TokenType::Int);
                initToken(c);
            } else {
                state = Dfstate::Id;    //切换回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_else1:  // e
            if (c == 'l'){  //el
                state = Dfstate::Id_else2;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_else2:
            if(c == 's'){  // els
                state = Dfstate::Id_else3;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_else3:
            if(c == 'e') { // else
                state = Dfstate::Id_else4;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_else4:
            if(isBlank(c) || c == '{'){
                token->setType(TokenType::Else);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_while1:  // w
            if (c == 'h'){  // wh
                state = Dfstate::Id_while2;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_while2:  // wh
            if (c == 'i'){  // whi
                state = Dfstate::Id_while3;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_while3:  // whi
            if (c == 'l'){  // whil
                state = Dfstate::Id_while4;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_while4:  // whil
            if (c == 'e'){  // while
                state = Dfstate::Id_while5;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_while5:
            if(isBlank(c) || c == '('){
                token->setType(TokenType::While);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_float1:  // f
            if (c == 'l'){  // fl
                state = Dfstate::Id_float2;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_float2:  // fl
            if (c == 'o'){  // flo
                state = Dfstate::Id_float3;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_float3:  // flo
            if (c == 'a'){  // floa
                state = Dfstate::Id_float4;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_float4:  // floa
            if (c == 't'){  // float
                state = Dfstate::Id_float5;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_float5:
            if(isBlank(c)){
                token->setType(TokenType::Float);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_return1:  // r
            if (c == 'e'){  // re
                state = Dfstate::Id_return2;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_return2:  // re
            if (c == 't'){  // ret
                state = Dfstate::Id_return3;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_return3:  // ret
            if (c == 'u'){  // retu
                state = Dfstate::Id_return4;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_return4:  // retu
            if (c == 'r'){  // retur
                state = Dfstate::Id_return5;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_return5:  // retur
            if (c == 'n'){  // return
                state = Dfstate::Id_return6;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_return6:
            if(c == ';' || isBlank(c)){
                token->setType(TokenType::Return);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_break1:  // b
            if (c == 'r'){  // br
                state = Dfstate::Id_break2;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_break2:  // br
            if (c == 'e'){  // bre
                state = Dfstate::Id_break3;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_break3:  // bre
            if (c == 'a'){  // brea
                state = Dfstate::Id_break4;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_break4:  // brea
            if (c == 'k'){  // break
                state = Dfstate::Id_break5;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_break5:  // break
            if(c == ';' || isBlank(c)){
                token->setType(TokenType::Break);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Id_main1:  // m
            if (c == 'a'){  // ma
                state = Dfstate::Id_main2;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_main2:  // ma
            if (c == 'i'){  // mai
                state = Dfstate::Id_main3;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_main3:  // mai
            if (c == 'n'){  // main
                state = Dfstate::Id_main4;
                tokenText.push_back(c);
            }else if(isDigital(c) || isAlpha(c)){
                state = Dfstate::Id;   // 切回Id状态
                tokenText.push_back(c);
            }else{
                initToken(c);
            }
            break;
        case Dfstate::Id_main4:  // main
            if(c == '(' || isBlank(c)){
                token->setType(TokenType::Main);
                initToken(c);
            } else {
                state = Dfstate::Id; //切回Id状态
                tokenText.push_back(c);
            }
            break;
        case Dfstate::Hex_bin_oct1:  // 0
            if(c == 'b' || c == 'B'){  // 0b or 0B
                tokenText.push_back(c);
                state = Dfstate::Bin2;
                token->setType(TokenType::Binary);
            } else if(c == 'x' || c == 'X'){ // 0x or 0X
                tokenText.push_back(c);
                state = Dfstate::Hex2;
                token->setType(TokenType::HexDecimal);
            } else if(isDigital(c)){ //八进制
                tokenText.push_back(c);
                state = Dfstate::Oct2;
                token->setType(TokenType::Octal);
            }else {
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::Bin2:
            if(c == '1' or c == '0'){
                tokenText.push_back(c);
                state = Dfstate::Bin3;
            }else{
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::Bin3:
            if(c == '1' or c == '0'){
                tokenText.push_back(c);
            }else if(isBlank(c) || c == ')' || c == ';' || c == ',' || c == '}'){
                initToken(c);
            }else{
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::Hex2:
            if(isHex(c)){
                tokenText.push_back(c);
                state = Dfstate::Hex3;
            }else{
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::Hex3:
            if(isHex(c)){
                tokenText.push_back(c);
            }else if(isBlank(c) || c == ')' || c == ';' || c == ',' || c == '}'){
                initToken(c);
            }else{
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::Oct2:
            if(c >= '0' && c <='7'){
                tokenText.push_back(c);
                state = Dfstate::Oct3;
            }else{
                errFunc(tokenText, c);
            }
            break;
        case Dfstate::Oct3:
            if(c >= '0' && c <='7'){
                tokenText.push_back(c);
            }else if(isBlank(c) || c == ')' || c == ';' || c == ',' || c == '}'){
                initToken(c);
            }else{
                errFunc(tokenText, c);
            }
            break;
            //TODO fix ' 情况
        case Dfstate::charLiteral1:
            if(c == '\''){
                state = Dfstate::charLiteral2;
            }else if(c == '\\'){  // 支持转义
                state = Dfstate::charLiteral4;
            }else{
                state = Dfstate::charLiteral3;
            }
            tokenText.push_back(c);
            break;
        case Dfstate::charLiteral2:
            initToken(c);
            break;
        case Dfstate::charLiteral3:
            if(c == '\''){
                tokenText.push_back(c);
                state = Dfstate::charLiteral2;
            }else{
                cerr << "Line : " << line_number <<" charLiteral cannot contain too many bits" << endl;
                exit(1);
            }
            break;
        case Dfstate::charLiteral4:
            if(c == 't' || c == 'b' || c == '\\'    // 目前只支持这么多转义
            || c == '\'' || c == '\"' ||  c == 'n'){
                tokenText.push_back(c);
                state = Dfstate::charLiteral3;
            }else{
                cerr << "Line : " << line_number <<" charLiteral cannot contain too many bits" << endl;
                exit(1);
            }
            break;
        case Dfstate::stringLiteral1:
            if(c == '\"'){
                state = Dfstate::stringLiteral2;
                tokenText.push_back(c);
            }else{
                tokenText.push_back(c);  //保持
            }
            break;
        case Dfstate::stringLiteral2:
            initToken(c);
            break;
//        default: //这里如何处理？
    }

}

void Lexier::initToken(const char c) {
    if(tokenText.length() > 0) {
        token->setText(tokenText);
        token->setLineNumber(line_number);
        token_list.push_back(token);
        cout << token->getStateStr(token->getType()) << ":" << token->getText() << endl;
        tokenText.clear();
        token = new Token();  // need to check

    }

    state = Dfstate::Initial;

    if (isAlpha(c) || c == '_') {    // 第一个字符是字母或下划线
        if(c == 'i'){   //第一个字符是i
            state = Dfstate::Id_int_if1;
        } else if(c == 'e'){
            state = Dfstate::Id_else1;
        } else if(c == 'w'){
            state = Dfstate::Id_while1;
        } else if(c == 'f'){
            state = Dfstate::Id_float1;
        } else if(c == 'r'){
            state = Dfstate::Id_return1;
        } else if(c == 'b'){
            state = Dfstate::Id_break1;
        } else if(c == 'm'){
            state = Dfstate::Id_main1;
        }else{
            state = Dfstate::Id;
        }
        token->setType(TokenType::Identifier);
    } else if (isDigital(c)) {   // 第一个字符是数字
        if(c == '0') {
            state = Dfstate::Hex_bin_oct1;
        }else{
            state = Dfstate::IntLiteral;
            token->setType(TokenType::IntLiteral);
        }
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
    } else if (c == '{') {
        state = Dfstate::BigLeftParen;
        token->setType(TokenType::BigLeftParen);
    } else if (c == '}') {
        state = Dfstate::BigRightParen;
        token->setType(TokenType::BigRightParen);
    }else if (c == '='){
        state = Dfstate::Assignment;
        token->setType(TokenType::Assignment);
    }else if(c == '\''){
        state = Dfstate::charLiteral1;
        token->setType(TokenType::CharLiteral);
    }else if(c == '\"'){
        state = Dfstate::stringLiteral1;
        token->setType(TokenType::StringLiteral);
    }else if(c == '\n'){
        return;
    }
    else {
        state = Dfstate::Initial; // skip all unknown patterns
        return;
        // TODO : 错误处理
    }

    tokenText.push_back(c);
}