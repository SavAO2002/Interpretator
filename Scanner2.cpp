#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <cstring>

using namespace std;

enum Lex_type {
	Lex_NULL, //0
	Lex_AND, Lex_OR, Lex_NOT, Lex_IF, Lex_ELSE, Lex_WHILE, Lex_FOR, Lex_BREAK, Lex_GOTO, Lex_INT, Lex_BOOL, Lex_STRING, Lex_TRUE, Lex_FALSE, Lex_READ, Lex_WRITE, Lex_PROGRAM, Lex_STRUCT, Lex_STRUCT_DEF, //1...19
	Lex_FIN, //20
	Lex_PLUS, Lex_MINUS, Lex_SLASH, Lex_MULTIPLY, Lex_COMMA, Lex_DOT, //21...26
    Lex_LESS, Lex_GREATER, Lex_EQ, Lex_NEQ, Lex_GEQ, Lex_LEQ, Lex_ASSIGN, //27...33
	Lex_LBRACK, Lex_RBRACK, Lex_FLBRACK, Lex_FRBRACK, Lex_DQUAT, Lex_COLON, Lex_SEMICOLON,//34...40
	Lex_NUM, //41
	Lex_STR, //42
	Lex_ID, //43
	POLIZ_LABEL,//44
	POLIZ_ADDRESS,//45
	POLIZ_GO,//46
	POLIZ_FGO//47
};

const char* types[] {
	"Lex_NULL", 
	"Lex_AND", "Lex_OR", "Lex_NOT", "Lex_IF", "Lex_ELSE", "Lex_WHILE", "Lex_FOR", "Lex_BREAK", "Lex_GOTO", "Lex_INT", "Lex_BOOL", "Lex_STRING", "Lex_TRUE", "Lex_FALSE", "Lex_READ", "Lex_WRITE", "Lex_PROGRAM", "Lex_STRUCT", "Lex_STRUCT_DEF", 
	"Lex_FIN", 
	"Lex_PLUS", "Lex_MINUS", "Lex_SLASH", "Lex_MULTIPLY", "Lex_COMMA", "Lex_DOT", 
    "Lex_LESS", "Lex_GREATER", "Lex_EQ", "Lex_NEQ", "Lex_GEQ", "Lex_LEQ", "Lex_ASSIGN", 
	"Lex_LBRACK", "Lex_RBRACK", "Lex_FLBRACK", "Lex_FRBRACK", "Lex_DQUAT", "Lex_COLON", "Lex_SEMICOLON",
	"Lex_NUM", 
	"Lex_STR", 
	"Lex_ID", 
	"POLIZ_LABEL",
	"POLIZ_ADDRESS",
	"POLIZ_GO",
	"POLIZ_FGO"
};

class Lex {	
	Lex_type t_lex;
	int v_lex;
public:
	Lex(Lex_type t = Lex_NULL, int v = 0):t_lex(t),v_lex(v){}
	
	Lex_type get_type(){
		return t_lex;
	}
	int get_value(){
		return v_lex; 
	}
	friend ostream & operator << (ostream &s, Lex l);
};

class Ident{
	string name;
	Lex_type t_lex;
	string st_lex;
	bool assign;
	bool declare;
	int value;
	int goto_label;
	bool is_goto_label = false;
public:
	Ident(){
		declare = false;
		assign = false;
	}
	bool operator == (const string& s){
		return name == s;
	}
	Ident ( const string n) {
		name = n;
		declare = false;
		assign = false;
	}
	string get_name (){
		return name;
	}
	int get_value(){
		return value;
	}
	bool get_assign (){
		return assign;
	}
	bool get_declare (){
		return declare;
	}
	Lex_type get_type (){
		return t_lex;
	}
	string get_st_type (){
		return st_lex;
	}
	void put_value( int v ){
		value = v;
	}
	void put_assign(){
		assign = true;
	}
	void put_declare (){
		declare = true;
	}
	void put_type (Lex_type t){
		t_lex = t;
	}
	void put_st_type (string t){
		st_lex = t;
	}
	void set_goto_label (int l){
		goto_label = l;
	}
	int get_goto_label (){
		return goto_label;
	}
	void set_is_goto_label (){
		is_goto_label = true;
	}
	bool get_is_goto_label (){
		return is_goto_label;
	}
};

vector<Ident> table_ident;

int put_TID(const string & buf){
	vector<Ident>::iterator k;
	if ((k = find(table_ident.begin(), table_ident.end(), buf)) != table_ident.end())
		return k - table_ident.begin();
	table_ident.push_back(Ident(buf));
	return table_ident.size() - 1;
}

void get_TID(){
	for (auto& i : table_ident)
		//cout << i.get_name() << ' ' << types[i.get_type()] << ' ' << i.get_value()<<endl;
		cout << i.get_name() << ' ' << types[i.get_type()] << ' ' <<endl;
}

int in_TID( const string &buf){
	vector<Ident>::iterator k;
	if ((k = find(table_ident.begin(), table_ident.end(), buf)) != table_ident.end())
		return k - table_ident.begin();
	return -1;
}

vector<string> table_strconst;

int put_TSC(const string & buf){
	vector<string>::iterator k;
	if ((k = find(table_strconst.begin(), table_strconst.end(), buf)) != table_strconst.end())
		return k - table_strconst.begin();
	table_strconst.push_back(buf);
	return table_strconst.size() - 1;
}

struct Address{
	string name;
	vector <int> mas;
	Address(string _name){
		name = _name;
		mas = {};
	}
};

vector<Address> table_label;

int put_TL(const string & buf){
	int i = 0;
	for(Address fld_count : table_label) {
		if (buf == fld_count.name) {
			return i;
		}
		i++;
	}
	table_label.push_back(Address(buf));
	return table_label.size() - 1;
}

int in_TL( const string &buf){
	int i = 0;
	for(Address fld_count : table_label) {
		if (buf == fld_count.name) {
			return i;
		}
		i++;
	}
	return -1;
}

struct StructArea{
		string name;
		Lex_type type;
		StructArea(string _name, Lex_type _type){
			name = _name;
			type = _type;
		}
	};
struct Struct{
	string name;
	vector <StructArea> areas;
	Struct(string _name){
		areas = {};
		name = _name;
	}
};

vector<Struct> table_struct;

int put_TST(const Struct & buf){
	int i = 0;
	for(Struct fld_count : table_struct) {
		if (buf.name == fld_count.name) {
			return i;
		}
		i++;
	}
	table_struct.push_back(buf);
	return table_struct.size() - 1;
}

class Scanner {
	FILE* f;
	char c;
	void get_c(){
		c = fgetc(f);
	}
	int find_l(const string buf, const char **list){
		int i = 0;
		while (list[i]){
			if (buf == list[i])
				return i;
			++i;
		}
		return 0;
	}
public:
	static const char* TW[], *TD[];

	Scanner(const char* name_p){
		f = fopen(name_p, "r");
		if (f == NULL){
			cout<< "Error opening the file"<<endl;
			exit(1);
		}
		get_c();	
	}
	~Scanner(){
		fclose(f);
	}
	Lex get_lex();
};
Lex Scanner::get_lex(){
	enum state{H, IDENT, NUM, COM, ALE, NEQ, STR };
	int d, j;
	string buf;
	state CS = H;
	do{
		switch (CS){
			case H:
				if ((c ==' ') || (c == '\n') || (c == '\t') || (c == '\r')) get_c();
				else if (isalpha(c)){
					buf.push_back(c);
					get_c();
					CS = IDENT;	
				}
				else if (isdigit(c)){
					d = c - '0';
					get_c();
					CS = NUM;
				}
				else if (c == '/') {
					buf.push_back(c);
					get_c();
					if (c == '*'){
						buf.pop_back();
						get_c();
						CS = COM;
					}
					else{
						get_c();
						j = find_l(buf, TD);
						return Lex(Lex_SLASH, j);	
					}	
				}
				else if ((c == '<') || (c == '>') || (c == '=')){
					buf.push_back(c);
					get_c();
					CS = ALE;
				}
				else if (c == '!'){
					buf.push_back(c);
					get_c();
					CS = NEQ;
				}
				else if (c == '"'){
					get_c();
					CS = STR;	
				}
				else if (c == EOF){
					return(Lex(Lex_FIN));
				}
				else{
					buf.push_back(c);
					if ((j = find_l(buf, TD))){
						get_c();
						return Lex((Lex_type)(j + (int)Lex_FIN), j);
					}
					else{
						cout << "Error: lex wasn't found. Symbol - "<< c <<endl;
						exit(1);
					}
				}
				break;
			case IDENT:
				if (isalpha(c) || isdigit(c) || c == '.'){
					buf.push_back(c);
					get_c();
				}
				else{
					if ((j = find_l(buf, TW))){
						return Lex((Lex_type)j, j);
					}
					else{
						j = put_TID(buf);
						return Lex(Lex_ID, j);
					}
				}
				break;
			case NUM:
				if (isdigit(c)){
					d = d * 10 + (c - '0');
					get_c();
				}
				else{
					return Lex(Lex_NUM, d);
				}
				break;
			case COM:
				if (c == '*'){
					get_c();
					if (c == '/'){
						get_c();
						CS = H;
					}
					else{
						cout << "Lex error in comments - "<< c <<endl;
						exit(1);
					}	
				}
				else if (c == EOF){
					cout << "Error: Not a closed comment."<<endl;
					exit(1);
				}
				else{
					get_c();
				}
				break;
			case ALE:
				if (c == '='){
					buf.push_back(c);
					get_c();
					j = find_l(buf, TD);
					return Lex((Lex_type)(j + (int)Lex_FIN), j);
				}
				else{
					j = find_l(buf, TD);
					return Lex((Lex_type)(j + (int)Lex_FIN), j);
				}
			case NEQ:
				if (c == '='){
					buf.push_back(c);
					get_c();
					j = find_l(buf, TD);
					return Lex(Lex_NEQ, j);
				}
				else{
					cout << "Error: lex wasn't found. Symbol - "<< c <<endl;
					exit(1);
				}
				break;
			case STR:
				if (c == '"'){
					j = put_TSC(buf);
					get_c();
					return Lex(Lex_STR, j);			
				}
				else if (c == EOF){
					cout << "Error: Not a closed quatation marks."<<endl;
					exit(1);
				}
				else{
					buf.push_back(c);
					get_c();
				}
				break;
		}
	} while(true);
};

const char* Scanner::TW[] = {"", "and", "or", "not", "if", "else", "while", "for", "break", "goto", "int", "boolean", "string", "true", "false", "read", "write", "program", "struct", NULL};
const char* Scanner::TD[] = {"", "+", "-", "/", "*", ",", ".", "<", ">", "==", "!=", ">=", "<=", "=", "(", ")", "{", "}", "\"", ":", ";", NULL};

ostream & operator << (ostream &s, Lex l){
	string t;
	if ( l.t_lex <= Lex_STRUCT )
	    t = Scanner::TW[l.t_lex];
	else if ( (l.t_lex >= Lex_FIN) && (l.t_lex <= Lex_SEMICOLON))
	    t = Scanner::TD[ l.t_lex - Lex_FIN ];
	else if ( l.t_lex == Lex_NUM )
	    t = "NUM";
	else if ( l.t_lex == Lex_ID )
	    t = table_ident[l.v_lex].get_name();
	else if ( l.t_lex == Lex_STR )
		//t = table_strconst[l.v_lex].get_name();
		t = table_strconst[l.v_lex];
	else if (l.t_lex == Lex_STRUCT_DEF)
		t = table_ident[l.v_lex].get_name();
	/*else if (l.t_lex > Lex_STR)
		t = table_ident[l.v_lex].get_name();*/
	else if (l.t_lex == POLIZ_LABEL)
		t = "Label";
	else if (l.t_lex == POLIZ_GO)
		t = "!";
	else if (l.t_lex == POLIZ_ADDRESS)
		t = "Addr";
	else if (l.t_lex == POLIZ_FGO)
		t = "!F";
	else {
	    cout << "Error: can't print a lex"<<endl;
	    cout<< l;
	    exit(1);
	}
	s << '(' << t << " , " << l.v_lex << ");" << endl;
	return s;
}
/*---------------------------------------------------------------------------------------*/

template <class T, class T_EL>
void from_st(T &st, T_EL &i){
	i = st.top();
	st.pop();
}

class Parser{
	Scanner scan;
	Lex curr_lex;
	Lex_type c_type;
	int c_val;
	int c_break = -1;
	stack <int>	st_int;
	stack <Lex_type> st_lex;
	stack <Lex> st_fail;
	void P(); //program{}
	void ST();//struct
	void D1();// куча описаний
	void D(Lex s_lex);//описание
	void B(bool in_loop);//блок с операторами
	void S(bool in_loop);//один оператор
	void E();//сравнение
	void E1();//+-или
	void E2();
	void E3();
	void T();//*/и
	void F();//переменная
	void gl(){
		if (!st_fail.empty()){
        	from_st(st_fail, curr_lex);
   		} 
   		else {
        	curr_lex = scan.get_lex();
    	}
		c_type = curr_lex.get_type();
		c_val = curr_lex.get_value();
		//cout << curr_lex << endl;
	}
	vector <StructArea> tmp;
	void Struct_Area_Def();
	void dec(Lex_type c_type);
	void check_id();
	void check_label(Lex s_lex);
	void check_semicolon();
	void check_op_for_comparison();
	void check_op_for_multiplication();
	void check_op_for_summation();
	void check_op_for_or_and();
	void check_not();
	void eq_bool();
	void eq_type();
	void check_declare_in_read();
	bool is_struct(Lex curr_lex);
	void add_break(int in_loop);
	void VAR(Lex s_lex);
	void INIT(Lex s_var);
public:
	Parser(const char* name_p): scan(name_p){}
	void analyze();
	vector <Lex> poliz;
};

void Parser::analyze(){
	gl();
	P();
	if (c_type != Lex_FIN){
		cout << "Error: expect FIN"<<endl;
		exit(1);
	}
	for (auto& i : table_ident){
		if (i.get_type() == POLIZ_LABEL && !i.get_is_goto_label()){
			cout<< "label was not declared - " << i.get_name()<<endl;
			exit(1);
		}
	}	
	int i = 0;
	for (Lex L: poliz){
		cout <<i << ") " << L;
		i++;
	}
	cout << endl << "Yes!!!" << endl;
}

void Parser::P(){
	if (c_type == Lex_PROGRAM){
		gl();
	}
	else{
		cout << "Error: expect PROGRAM"<<endl;
		exit(1);
	}
	if (c_type == Lex_FLBRACK){
		gl();
	}
	else{
		cout << "Error: expect {"<<endl;
		exit(1);
	}
	if (c_type == Lex_STRUCT){
		ST();
	}
	D1();
	B(false);
	gl();
}; //program{}

void Parser::ST(){
	while (c_type == Lex_STRUCT){
		gl();
		if (c_type != Lex_ID){
			cout << "Error: wrong type of lex - "<< curr_lex <<endl;
			exit(1);
		}
		if (table_ident[c_val].get_declare()){
			cout << "Redefenition of structure "<< curr_lex <<endl;
			exit(1);
		}
		table_ident[c_val].put_type(Lex_STRUCT_DEF);
		table_ident[c_val].put_value(table_ident.size());
		Struct st(table_ident[c_val].get_name());
		table_struct.push_back(st);
		gl();
		if (c_type != Lex_FLBRACK){
			cout << "Error: expect { "<< curr_lex <<endl;
			exit(1);
		}
		gl();
		Struct_Area_Def();
		if (c_type != Lex_FRBRACK){
			cout << "Error: expect } "<< curr_lex <<endl;
			exit(1);
		}
		gl();
		check_semicolon();
	}
};

void Parser::Struct_Area_Def(){
	while (c_type != Lex_FRBRACK){
		Lex_type l = c_type;
		do{
			gl();
			if (c_type != Lex_ID){
				cout << "Error: wrong type of lex"<<endl;
				exit(1);
			}
			StructArea field(table_ident[c_val].get_name(), l);
			table_ident.pop_back();
			for(StructArea fld_count : tmp) {
				if (field.name == fld_count.name) {
					cout << "Error: similar fields "<<endl;
					exit(1);
				}
			}
			tmp.push_back(field);
			gl();
		}while (c_type == Lex_COMMA);
		if (c_type != Lex_SEMICOLON){
			cout << "Error: expect ; "<< curr_lex <<endl;
			exit(1);
		}
		gl();
	}
	table_struct[table_struct.size() - 1].areas = tmp;
};
	
void Parser::D1(){
	while ((c_type == Lex_INT) || (c_type == Lex_BOOL) || (c_type == Lex_STRING) || (is_struct(curr_lex))){
		D(curr_lex);
		gl();
	};
};// куча описаний

void Parser::D(Lex s_lex){
	gl();
	if (c_type != Lex_ID){
		cout << "Error: wrong type of lex"<<endl;
		exit(1);
	}
	else{
		VAR(s_lex);
	}
	while (c_type == Lex_COMMA){
		gl();
		if (c_type != Lex_ID){
			cout << "Error: expect Lex_ID"<<endl;
			exit(1);
		}
		VAR(s_lex.get_type());
	}
	if (c_type != Lex_SEMICOLON){
		cout << "Error: expect ;"<<endl;
		exit(1);
	}		
};//описание
void Parser::B(bool in_loop){
	do{
		S(in_loop);
	}
	while (c_type != Lex_FRBRACK);
};//блок с операторами

void Parser::S(bool in_loop){
	int pl0, pl1, pl2, pl3;
	if (c_type == Lex_IF){
		gl();
		E();
		eq_bool();
		pl2 = poliz.size();
        poliz.push_back (Lex());
        poliz.push_back (Lex(POLIZ_FGO));
		S(in_loop);
		pl3 = poliz.size();
        poliz.push_back (Lex());
        poliz.push_back (Lex (POLIZ_GO));
        poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
		if (c_type == Lex_ELSE){
			gl();
			S(in_loop);
			//poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
		}
		poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
	}
	else if (c_type == Lex_FOR){
		gl();
		if (c_type != Lex_LBRACK){
			cout<< "Error - no bracket "<< curr_lex;
			exit(1);
		}
		gl();
		if (c_type != Lex_SEMICOLON){
			E();
		}
		pl1 = poliz.size();
		if (c_type != Lex_SEMICOLON){
			E();
			eq_bool();
		}
		else{
			poliz.push_back(Lex(Lex_TRUE));
		}
		pl0 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_FGO));
		pl3 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_GO));
		if (c_type != Lex_RBRACK){
			E();
		}
		poliz.push_back(Lex(POLIZ_LABEL, pl1));
		poliz.push_back(Lex(POLIZ_GO));
		if (c_type != Lex_RBRACK){
			cout<< "Error - not closed bracket "<< curr_lex;
			exit(1);
		}
		gl();
		poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
		S(true);
		poliz.push_back(Lex(POLIZ_LABEL, pl3 + 2));
		poliz.push_back(Lex(POLIZ_GO));
		poliz[pl0] = Lex(POLIZ_LABEL, poliz.size());
		add_break(poliz.size());
	}
	else if (c_type == Lex_WHILE){
		pl0 = poliz.size();
		gl();
		E();
		eq_bool();
		pl1 = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_FGO));
		S(true);
		poliz.push_back(Lex(POLIZ_LABEL, pl0));
		poliz.push_back(Lex(POLIZ_GO));
		poliz[pl1] = Lex(POLIZ_LABEL, poliz.size());
		add_break(poliz.size());
	}
	else if (c_type == Lex_BREAK){
		if (!in_loop){
			cout<< "No loop "<< curr_lex;
			exit(1);
		}
		c_break = poliz.size();
		poliz.push_back(Lex());
		poliz.push_back(Lex(POLIZ_GO));
		gl();
		check_semicolon();
	}
	else if (c_type == Lex_GOTO){
		gl();
		if (!table_ident[c_val].get_declare()){
			st_int.push(c_val);
			dec(POLIZ_LABEL);
			int l = poliz.size();
			int k = put_TL(table_ident[c_val].get_name());
			table_label[k].mas.push_back(l);
			poliz.push_back(Lex());
			poliz.push_back(Lex(POLIZ_GO));
		}
		else{
			poliz.push_back(Lex(POLIZ_LABEL, table_ident[c_val].get_goto_label()));
			poliz.push_back(Lex(POLIZ_GO));
		}
        gl();
        check_semicolon();
	}
	else if (c_type == Lex_READ){
		gl();
		if (c_type == Lex_LBRACK){
			gl();
			if (c_type == Lex_ID){
				check_declare_in_read();
				poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
				poliz.push_back(Lex(Lex_READ));
				gl();
			}
			else{
				cout<< "Error - S()"<< curr_lex;
				exit(1);
			}
			if (c_type == Lex_RBRACK){
				gl();
				check_semicolon();
			}
			else{
				cout<< "Error - not closed bracket"<< curr_lex;
				exit(1);
			}
		}
		else{
			cout<< "Error - no brackets with read"<< curr_lex;
			exit(1);
		}
	}
	else if (c_type == Lex_WRITE){
		gl();
		if (c_type == Lex_LBRACK){
			gl();
			E1();
			poliz.push_back(Lex(Lex_WRITE));
			if (c_type == Lex_RBRACK){
				gl();
				check_semicolon();
			}
			else{
				cout<< "Error - not closed bracket"<< curr_lex;
				exit(1);
			}
		}
		else{
			cout<< "Error - no brackets with read"<< curr_lex;
			exit(1);
		}
	}
    else if( c_type == Lex_NOT || c_type == Lex_MINUS || c_type == Lex_TRUE || c_type == Lex_FALSE || c_type == Lex_NUM || c_type == Lex_STR || c_type == Lex_LBRACK || (c_type == Lex_ID) || (c_type == Lex_STRUCT_DEF)){
		E();
	}
	else if (c_type == Lex_FLBRACK){
		gl();
		B(in_loop);
		gl();
	}
    else{
    	cout<< "Error - no operator"<< curr_lex;
		exit(1);
    }

};//один оператор

void Parser::E(){
	if (c_type == Lex_ID){
        Lex s_lex = curr_lex;
		int l = c_val;
        gl();
        if (c_type == Lex_COLON){
        	check_label(s_lex);
        	gl();
        }
		else if (c_type == Lex_DOT){
			int k;
			gl();
			string n = table_ident[l].get_name() + "." + table_ident[c_val].get_name();
			if ((k = in_TID(n)) != -1){
				st_fail.push(Lex(Lex_ID, table_ident[k].get_value()));
				gl();
				E();
				poliz.push_back(Lex(Lex_SEMICOLON));
				check_semicolon();
			}
		}
        else{
			if (c_type != Lex_ASSIGN) {
				st_fail.push(curr_lex);
				st_fail.push(s_lex);
				gl();
				E1();
				check_semicolon();
				return;
			}
			if (s_lex.get_type() == Lex_STRUCT_DEF){
				st_lex.push(table_ident[s_lex.get_value()].get_type());
				poliz.push_back(Lex(POLIZ_ADDRESS, s_lex.get_value()));
				gl();
				if (table_ident[s_lex.get_value()].get_st_type() != table_ident[c_val].get_st_type()){
					cout<< "Error - different types of structures "<< curr_lex;
					exit(1);
				}
				E1();
				eq_type();
				poliz.push_back(Lex(Lex_ASSIGN));
				check_semicolon();
			}
			else{
				st_lex.push(table_ident[s_lex.get_value()].get_type());
				poliz.push_back(Lex(POLIZ_ADDRESS, s_lex.get_value()));
				gl();
				E1();
				eq_type();
				poliz.push_back(Lex(Lex_ASSIGN));
				check_semicolon();
			}
        }
    }
    else {
        E1();
    }
};//сравнение

void Parser::E1(){
	E2();
	while (c_type == Lex_AND || c_type == Lex_OR){
		st_lex.push(c_type);
		gl();
		E2();
		check_op_for_or_and();
	}
};//сравнение

void Parser::E2(){
	E3();
	while (c_type == Lex_EQ  || c_type == Lex_LESS || c_type == Lex_GREATER || c_type == Lex_LEQ || c_type == Lex_GEQ || c_type == Lex_NEQ ){
		st_lex.push(c_type);
		gl();
		E3();
		check_op_for_comparison();
	}
};//сравнение
void Parser::E3(){
	T();
	while (c_type == Lex_PLUS || c_type == Lex_MINUS){
        st_lex.push(c_type);
        gl();
        T();
        check_op_for_summation();
    }
};//+-или
void Parser::T(){
	F();
	while (c_type == Lex_MULTIPLY || c_type == Lex_SLASH) {
        st_lex.push(c_type);
        gl();
        F();
        check_op_for_multiplication();
    }
};//*/и
void Parser::F(){
	if ( c_type == Lex_ID ) {
        check_id ();
        poliz.push_back(Lex(Lex_ID, c_val));
        gl ();
    }
    else if ( c_type == Lex_NUM ) {
    	st_lex.push(Lex_INT);
    	poliz.push_back(curr_lex);
        gl ();
    }
    else if (c_type == Lex_STR){
    	st_lex.push(Lex_STRING);
    	poliz.push_back(Lex(Lex_STR, c_val));
    	gl();
    }
    else if ( c_type == Lex_TRUE ) {
    	st_lex.push(Lex_BOOL);
    	poliz.push_back(Lex(Lex_TRUE, 1));
        gl ();
    }
    else if ( c_type == Lex_FALSE) {
    	st_lex.push(Lex_BOOL);
    	poliz.push_back(Lex(Lex_FALSE, 0));
        gl ();
    }
    else if ( c_type == Lex_NOT ) {
        gl (); 
        F (); 
        check_not ();
    }
    else if ( c_type == Lex_MINUS ) {
        gl (); 
        st_lex.push(Lex_INT);
        poliz.push_back(Lex(Lex_NUM, 0));
        st_lex.push(Lex_MINUS);
        F();
        check_op_for_summation(); 
    }
    else if ( c_type == Lex_LBRACK ) {
        gl (); 
        E1 ();
        if ( c_type == Lex_RBRACK){
            gl ();
        }
        else{
        	cout<< "Error - not closed bracket "<< curr_lex;
			exit(1);
        }
    }
	else if (c_type == Lex_STRUCT_DEF){
		st_lex.push(Lex_STRUCT_DEF);
		poliz.push_back(Lex(Lex_STRUCT_DEF, c_val));
		gl();
	}
    else{
    	cout<< "Error - wrong lex "<< curr_lex;
		exit(1);
    }
};//переменная

void Parser::VAR(Lex s_lex){
	if (is_struct(s_lex)){
		table_ident[c_val].put_type(Lex_STRUCT_DEF);
		table_ident[c_val].put_st_type(table_ident[s_lex.get_value()].get_name());
        table_ident[c_val].put_value(s_lex.get_value());
        table_ident[c_val].put_declare();
		string struct_name = table_ident[c_val].get_name();
        int num_struct = table_ident[s_lex.get_value()].get_value() - 1;
        for(auto &fld : table_struct[num_struct].areas) {
            string name = struct_name + '.' + fld.name;
            int k = put_TID(name);
            Ident& new_id = table_ident[k];
            new_id.put_declare();
            new_id.put_type(fld.type);
            new_id.put_value(0);
        }
		gl();
	}
	else{
		st_int.push(c_val);
		dec(s_lex.get_type());
		st_lex.push(table_ident[c_val].get_type());
		Lex s_var = curr_lex;
		gl();
		INIT(s_var);
	}
}

void Parser::INIT(Lex s_var){
	if (c_type != Lex_ASSIGN)
		return;
	poliz.push_back(Lex(POLIZ_ADDRESS, s_var.get_value()));
    gl();
    E1();
    eq_type();
    poliz.push_back(Lex(Lex_ASSIGN));
}


void Parser::dec(Lex_type type){
	int i;
	while (!st_int.empty()){
		from_st(st_int, i);
		if (table_ident[i].get_declare()){
			cout<< "Error - " << curr_lex <<" was declared twice";
			exit(1);
		}
		else{
			table_ident[i].put_declare();
			table_ident[i].put_type(type);

		}
	}
}

void Parser::check_label(Lex s_lex){
	int l = s_lex.get_value();
	if (!table_ident[l].get_assign()){
		table_ident[l].set_is_goto_label();
		table_ident[l].put_assign();
		int i = in_TL(table_ident[l].get_name());
		if (i != -1){
			vector <int> m = table_label[i].mas;
			//cout << m.size() << ' ' << poliz[m[0]] << endl;
			for (size_t j = 0; j < m.size(); j++){
				poliz[m[j]] = Lex(POLIZ_LABEL, poliz.size());
			}
		}
		else{
			st_int.push(l);
			dec(POLIZ_LABEL);
			table_ident[l].set_goto_label(poliz.size());
			put_TL(table_ident[l].get_name());
		}
	}
	else{
		cout<< "Error - " << curr_lex <<" - label assigned twice";
		exit(1);
	}	
}

void Parser::check_id(){
	if (table_ident[c_val].get_declare())
		st_lex.push((table_ident[c_val].get_type()));
	else{
		cout<< "Error - " << curr_lex <<" not declared";
		exit(1);
	}	
}

void Parser::check_op_for_comparison(){
	Lex_type t1, t2, op;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
 
    if (op == Lex_EQ && t1 == t2 && t1 == Lex_STRUCT_DEF){
		Lex l2 = poliz[poliz.size()];
		Lex l1 = poliz[poliz.size() - 1];
		if (l1.get_value() == l2.get_value()){
			st_lex.push(Lex_BOOL);
			poliz.push_back(Lex(op));
		}
		else{
			cout << "wrong types are in operation";
        	exit(1);
		}
	}
	else if (op == Lex_LESS || op == Lex_GREATER || op == Lex_EQ || op == Lex_NEQ){
    	if (t1 == t2 && (t1 == Lex_INT || t1 == Lex_STR  || t1 == Lex_STRING || t1 == Lex_BOOL)){
    		st_lex.push(Lex_BOOL);
    		poliz.push_back(Lex(op));
    	}
    }
    else if (t1 == t2 && (t1 == Lex_INT || t1 == Lex_BOOL) && (op == Lex_LEQ || op == Lex_GEQ)){
    		st_lex.push(Lex_BOOL);
    		poliz.push_back(Lex(op));
    }
    else{
        cout << "wrong types are in operation";
        exit(1);
    }
}

void Parser::check_op_for_summation(){
	Lex_type t1, t2, op;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
    if (op == Lex_PLUS || op == Lex_MINUS){
    	if (t1 == t2 && t1 == Lex_INT){
    		st_lex.push(Lex_INT);
    		poliz.push_back(Lex(op));
    	}
    	else if ((t1 == Lex_STRING || t1 == Lex_STR) && (t2 == Lex_STRING || t2 == Lex_STR) && op == Lex_PLUS){
    		st_lex.push(Lex_STR);
    		poliz.push_back(Lex(op));
    	}
    }
    else{
        cout << "wrong types are in operation";
        exit(1);
    }
}

void Parser::check_op_for_multiplication(){
	Lex_type t1, t2, op, r = Lex_INT;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
 
    if (op == Lex_SLASH || op == Lex_MULTIPLY){
    	if (t1 == t2 && t1 == Lex_INT){
    		st_lex.push(r);
    		poliz.push_back(Lex(op));
    	}
    }
    else{
        cout << "wrong types are in operation";
        exit(1);
    }
}

void Parser::check_op_for_or_and(){
	Lex_type t1, t2, op;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
    if ((t1 == t2) && ( t1 == Lex_BOOL) && (op == Lex_AND || op == Lex_OR)){
    	st_lex.push(Lex_BOOL);
    	poliz.push_back(Lex(op));
    }
   	else{
        cout << "wrong types are in operation";
        exit(1);
    }
}

void Parser::check_not(){
	if (st_lex.top() != Lex_BOOL){
		cout<< "Error - " << curr_lex <<" - wrong type in not";
		exit(1);
	}
	else{
		poliz.push_back(Lex(Lex_NOT));
	}
}

void Parser::eq_type(){
	Lex_type type;
	from_st(st_lex, type);
	if (!((type == Lex_STR || type == Lex_STRING) && (st_lex.top() == Lex_STR || st_lex.top() == Lex_STRING)))
		if (type != st_lex.top()){
			cout<< "Error - " << curr_lex <<" wrong type in :=";
			exit(1);
		}
	st_lex.pop();
}

void Parser::eq_bool(){
	if ( st_lex.top() != Lex_BOOL){
		cout<< "Not boolean expression " << curr_lex;
		exit(1);
	}
	st_lex.pop();
}

void Parser::check_semicolon(){
	if (c_type != Lex_SEMICOLON){
		cout<< "Error - no semicolon - "<< curr_lex;
		exit(1);
	}
	gl();
}

void Parser::check_declare_in_read(){
	if( !table_ident[c_val].get_declare()){
		cout<< "not declared while reading - "<< curr_lex;
		exit(1);
	}	
}

bool Parser::is_struct(Lex s_lex){
	return (s_lex.get_type() == Lex_ID) && (table_ident[s_lex.get_value()].get_type() == Lex_STRUCT_DEF);
}

void Parser::add_break(int in_loop){
	if (c_break != -1){
		poliz[c_break] = Lex(POLIZ_LABEL, in_loop);
		c_break = -1;
	}
}
/*--------------------------------------------------------------------------------------*/

class Executer{
public:
	void _execute ( vector<Lex> & poliz );
	bool is_assign( string name_m );
};

bool Executer::is_assign( string name_m){
	string s = name_m + ".";
	for(Ident fld_count : table_ident) {
		if (fld_count.get_name().find(s) == 0) {
			if (!fld_count.get_assign())
				return false;
		}
	}
	return true;
}

void Executer::_execute ( vector<Lex> & poliz) {
    Lex pc_el;
    stack < int > args;
	stack < int > s_args;
	stack < int > st_args;
    int i, j, index = 0, size = poliz.size();
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case Lex_TRUE: 
            case Lex_FALSE: 
            case Lex_NUM: 
            case POLIZ_ADDRESS: 
            case POLIZ_LABEL:
                args.push ( pc_el.get_value () );
                break;
			case Lex_STR:
			case Lex_STRING:
				s_args.push ( pc_el.get_value () );
                break;
			case Lex_STRUCT_DEF:
				st_args.push ( pc_el.get_value () );
                break;
            case Lex_ID:
                i = pc_el.get_value();
				if ((table_ident[i].get_type() == Lex_STRUCT_DEF) && (is_assign(table_ident[i].get_name()))){
					table_ident[i].put_assign();
					st_args.push(i);
					break;
				}
                else if ( table_ident[i].get_assign () ) {
					if (table_ident[i].get_type() == Lex_STRING || table_ident[i].get_type() == Lex_STR)
						s_args.push(table_ident[i].get_value ());
					else{
                  		args.push ( table_ident[i].get_value () );
					}
                 	break;
				}
                else{
					cout << "POLIZ: indefinite identifier" << endl;
					exit(1);
				}
            case Lex_NOT:
                from_st ( args, i );
                args.push( !i );
                break;
 
            case Lex_OR:
                from_st ( args, i ); 
                from_st ( args, j );
                args.push ( j || i );
                break;
 
            case Lex_AND:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j && i );
                break;
 
            case POLIZ_GO:
                from_st ( args, i );
                index = i - 1;
                break;
 
            case POLIZ_FGO:
                from_st ( args, i );
                from_st ( args, j );
                if ( !j ) index = i - 1;
                break;
 
            case Lex_WRITE:
				if (!s_args.empty()){
					from_st(s_args, i);
					cout << table_strconst[i] << endl;
				}
				else{
					from_st ( args, j );
					cout << j << endl;
				}
				break;
 
            case Lex_READ:
                int k;
				from_st(args, i);
				if ( table_ident[i].get_type() == Lex_INT ) {
					string s1;
					cout << "Input int value for" << table_ident[i].get_name () << endl;
					cin >> s1;
					try{
						k = stoi(s1);
					}
					catch(...){cout << "Wrong value"<<endl; exit(1);};
				}
				else if ( table_ident[i].get_type() == Lex_STRING ) {
					string s1;
					cout << "Input int value for " << table_ident[i].get_name () << endl;
					cin >> s1;
					k = put_TSC(s1);
				}
				else {
					string j;
					while (1) {
						cout << "Input boolean value (true or false) for" << table_ident[i].get_name() << endl;
						cin >> j;
						if ( j != "true" && j != "false" ) {
							cout << "Error in input:true/false" << endl;
							continue;
						}
						k = ( j == "true" ) ? 1 : 0;
						break;
					}
				}
                table_ident[i].put_value(k);
                table_ident[i].put_assign();
                break;
 
            case Lex_PLUS:
                if (!s_args.empty()){
					from_st ( s_args, i );
                	from_st ( s_args, j );
					k = put_TSC(table_strconst[j] + table_strconst[i]);
					s_args.push ( k );
                }
                else{
					from_st ( args, i );
                	from_st ( args, j );
                    args.push ( i + j );
                }
                break;
 
            case Lex_MULTIPLY:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i * j );
                break;
 
            case Lex_MINUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j - i );
                break;
 
            case Lex_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                if (i) {
                    args.push ( j / i );
                    break;
                }
                else{
					cout << "POLIZ:divide by zero" << endl;
					exit(1);
				}
            case Lex_EQ:
				if (!s_args.empty()){
					from_st ( s_args, i );
                	from_st ( s_args, j );
					args.push ( table_strconst[i] == table_strconst[j] );
                }
                else{
					from_st ( args, i );
                	from_st ( args, j );
                    args.push ( i == j );
                }
                break;
 
            case Lex_LESS:
                if (!s_args.empty()){
					from_st ( s_args, i );
                	from_st ( s_args, j );
					args.push ( table_strconst[i] < table_strconst[j] );
                }
                else{
					from_st ( args, i );
                	from_st ( args, j );
                    args.push ( j < i );
                }
                break;
 
            case Lex_GREATER:
                if (!s_args.empty()){
					from_st ( s_args, i );
                	from_st ( s_args, j );
					args.push ( table_strconst[i] > table_strconst[j] );
                }
                else{
					from_st ( args, i );
                	from_st ( args, j );
                    args.push ( j > i );
                }
                break;
 
            case Lex_LEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j <= i );
                break;
 
            case Lex_GEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j >= i );
                break;
 
            case Lex_NEQ:
                if (!s_args.empty()){
					from_st ( s_args, i );
                	from_st ( s_args, j );
					args.push ( table_strconst[i] != table_strconst[j] );
                }
                else{
					from_st ( args, i );
                	from_st ( args, j );
                    args.push ( i != j );
                }
                break;
 
            case Lex_ASSIGN:
				if (!st_args.empty()){
					from_st(st_args, i);
					from_st ( args, j );
					vector <StructArea> v1 = table_struct[table_ident[j].get_value()].areas;
					for (size_t l = 0; l < v1.size(); l++){
						string s_1 = table_ident[j].get_name() + "." + v1[l].name;
						string s_2 = table_ident[i].get_name() + "." + v1[l].name;
						table_ident[put_TID(s_1)] = table_ident[put_TID(s_2)];
						table_ident[put_TID(s_1)].put_assign();
					}
					break;
				}
                else if (!s_args.empty()){
					from_st ( s_args, i );
                }
                else{
					from_st ( args, i );
                }
				from_st ( args, j );
                table_ident[j].put_value (i);
                table_ident[j].put_assign (); 
                break;
 
            default:
				cout <<"POLIZ: unexpected elem"<< endl;
				exit(1);
        }
        ++index;
    };
    cout << "Finish of executing!!!" << endl;
};

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};
 
void Interpretator::interpretation () {
    pars.analyze ();
	get_TID();
    E._execute ( pars.poliz );
}

int main(int argc, char** argv){
	if (argc < 2){
		printf("Error: No file\n");
		return 0;
	}
	Interpretator I(argv[1]);
	I.interpretation();

	return 0;
}

