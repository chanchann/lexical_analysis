//
// Created by Frank on 2020-03-31.
//

#include "lexier.h"

Lexier::Lexier(string input_path)
    :input_path(input_path){}

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
    ifstream& input = getInput();
    ofstream& output = getOfstream();

    /* loop find each token in each line for DFA check */
    for (string line; getline(input, line); ){

        // output the line number
        output << right <<  "Line" <<  setw(4) << line_number << " :" << endl;

        // special case: for string splite replace ' ' to '\s' ensure ' ' can be accept
        int char_space_pos = 0;
        while(  (char_space_pos = line.find('\'', char_space_pos)) != string::npos  ){
            if(line[char_space_pos+1] == ' ' && line[char_space_pos+2] == '\'' ){
                line.erase(char_space_pos+1, 1);
                line.insert(char_space_pos+1, "\\s");
            }
            char_space_pos += 2;
            if ( char_space_pos >= line.length() ){
                break;
            }
        }

        // convert input string to char* for split
        char* tokenPtr = new char[line.length() + 1];
        char* token_list_ptr = tokenPtr;
        strcpy(tokenPtr, line.c_str() );

        // splite by tab and white space
        tokenPtr = strtok(tokenPtr, " \t");

        // whike has next token
        while( tokenPtr != NULL ){

            token = tokenPtr;
            //enter DFA  if error
            if( !enterDfa(token, tokenCategory )){
                cout << "Error! Line " << left << setw(3) << line_number \
							<< "token:" << "\""  << token << "\"" << endl;
                parseResult = false;
            }

            //is comment (no nedd to print)
            if( tokenCategory.compare("Comment") == 0 ){
                break;
            }
            // reduce '\s' to ' '
            // TODO: What does this judge mean?
            if (token.compare("\'\\s\'") == 0){
                cout << " I am here " << endl;
                token = "\' \'";
            }
            // output the lexier result
            output << left <<  "\t"     << setw(16) << "<" + tokenCategory + ">" << ":"  << token << endl;

            //save result
            token_list.push_back( new Token(token, tokenCategory, line_number) );

            // get next token
            tokenPtr = strtok(NULL, " \t");
        }
        delete [] token_list_ptr;
        line_number += 1;
    }

    input.close();
    output.close();
    return parseResult;
}

bool Lexier::enterDfa(string token, string& tokenCategory){

    const int keyword_size = 8;
    const static string keyword[keyword_size]  = {" ","if", "int", "char", "return", "else", "while", "break"};

    for(int i = 0; i < keyword_size; ++i){
        if ( token.compare(keyword[i]) == 0){
            tokenCategory = "<Kerword>";
            return true;
        }
    }
    //starting state
    int state = 0;
    /* implement a DFA */
    for( int i = 0; i < token.length(); ++i ){
        move(state, token[i], tokenCategory);
    }
    // check state is accept or not accept
    if( state == Token::NOT_ACCEPT || state == Token::CHAR || state == Token::CHAR){
        tokenCategory = "Error";
        return false;
    }
    return true;
}

/*
 *  initial state
 * */
void Lexier::startState(int& state, char next_char, string& tokenCategory){

    switch(next_char){

        //operators type1
        case '+': case '-': case '*': case '/':
            state = Token::OPERATORS1;
            tokenCategory = "Operator";
            break;
            //operators type2
        case '=': case '!': case '>': case '<':
            state = Token::OPERATORS2;
            tokenCategory = "Operator";
            break;
            //operators type3
        case '&':
            state = Token::OPERATORS3;
            tokenCategory = "Operator";
            break;
            //operators type4
        case '|':
            state = Token::OPERATORS4;
            tokenCategory = "Operator";
            break;
            //char state
        case '\'':
            state = Token::CHAR;
            tokenCategory = "Char";
            break;
            //special symbols
        case '[': case ']': case '(': case ')': case '{': case '}': case ';': case ',':
            state = Token::SYMBOLS;
            tokenCategory = "Special";
            break;
        case '_':
            tokenCategory = "Identifier";
            break;
        default:
            // Number state
            if ( isdigit(next_char) ){
                state = Token::NUMBER;
                tokenCategory = "Number";
            }// identifier
            else if( isalpha(next_char) ){
                state = Token::IDENTIFIER;
                tokenCategory = "Identifier";
            }//
            else{
                state = Token::NOT_ACCEPT;
            }
    }
}


/*
 * DFA state move
 * */
void Lexier::move(int& state, char next_char, string& tokenCategory){

    switch(state){
        // start state
        case 0:
            startState(state, next_char, tokenCategory);
            break;
            // '+' , '-' , '*', '/'
        case Token::OPERATORS1:
            //comment state
            if (next_char == '/'){
                state = Token::COMMENT;
                tokenCategory = "Comment";
            }
            else{
                state = Token::NOT_ACCEPT;
            }
            break;
            // '=', '!',  '<', '>'
        case Token::OPERATORS2:
            state = next_char == '=' ? Token::ACCEPT : Token::NOT_ACCEPT;
            break;
            // '&'
        case Token::OPERATORS3:
            state = next_char == '&' ? Token::ACCEPT : Token::NOT_ACCEPT;
            break;
            // '|'
        case Token::OPERATORS4:
            state = next_char == '|' ? Token::ACCEPT : Token::NOT_ACCEPT;
            break;

            // '/'
        case Token::COMMENT:
            state = next_char == '/' ? Token::ACCEPT : Token::NOT_ACCEPT;
            break;

            // first '\''
        case Token::CHAR:
            if ( next_char == '\\'  ){
                state = Token::SPECIAL_CHAR;
            }
            else{
                state = Token::WAIT_QUOTE;
            }
            break;

            // '\t' '\n' ' '
        case Token::SPECIAL_CHAR:
            state = next_char == 't' || next_char == 'n' || next_char == 's'? Token::WAIT_QUOTE : Token::NOT_ACCEPT;
            break;

            // second '\''
        case Token::WAIT_QUOTE:
            state = next_char == '\'' ? Token::ACCEPT : Token::NOT_ACCEPT;
            break;

            // '[', ']', '{', '}', '(', ')', ';', ','
        case Token::SYMBOLS:
            state = Token::NOT_ACCEPT;
            break;

            //0-9
        case Token::NUMBER:
            state = isdigit(next_char) ? Token::NUMBER : Token::NOT_ACCEPT;
            break;

            //[a-z A-z '_'][a-z A-Z 0-9 _]
        case Token::IDENTIFIER:
            if(next_char == '_' || isdigit(next_char)\
								|| isalpha(next_char) ){
                state = Token::IDENTIFIER;
            }
            else{
                state = Token::NOT_ACCEPT;
            }
            break;

            // accept
        case  Token::ACCEPT:
            state = next_char == ' ' ? Token::ACCEPT : Token::NOT_ACCEPT;
            break;

            // notAccept
        case Token::NOT_ACCEPT:
            break;
    }
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
    // TODO：判断文件是否存在并创建
    /* return the output stream for write the result */
    static ofstream outputFile( "../output/token.txt", ios::out);
    if (!outputFile){
        cerr << "output file could not be opened" << endl;
        exit(1);
    }
    return outputFile;
}

/* for other class get parse result
 * */
vector<Token*> Lexier::getTokenList(){
    return token_list;
}
