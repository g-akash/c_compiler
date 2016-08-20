
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <stack>

using namespace std;

// ast classes start here


class type{
public:
int arraysize;
type* typer;
string name;
string glotype;
int typesize;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////

class symtable{
public:
  

}; 

class perform 
{
	public:
	bool compatible, same,voi;
	int arraypoint;
	perform();
};

class localsymtab{
  public:
  string name;
  int offset;
  type* typer;
  string role;
  int size;
  localsymtab(string name1, int offset1, type* typer1, string role1, int size1);
};

class globalsymtab{
public:
  string name;
  string globalrole;
  string address;
  type* rettype;
  vector<localsymtab> *symtab;
  //globalsymtab();
  globalsymtab(string name1,string role, string addr,type* ret,vector<localsymtab>* sym);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
class abstract_astnode
{
	public:
	string val;
	abstract_astnode *exp1,*exp2,*exp3,*exp4;
	int int_val;
	float float_val;
	type *typer; 
	int lval;
	bool identi;
	vector<string> code;
	vector<type*> argsofprintf;
	vector<int> indexesofstringsofprintf;
	virtual void print (int n) = 0;
	virtual vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)=0;
	// virtual std::string generate_code(const symbolTable&) = 0;
	// virtual basic_types getType() = 0;
	// virtual bool checkTypeofAST() = 0;
	// protected:
	// virtual void setType(basic_types) = 0;
	// private:
	// typeExp astnode_type;
};

class refast_astnode: public abstract_astnode{
public:
};

class expast_astnode: public abstract_astnode{
public:
};

class stmtast_astnode: public abstract_astnode{
public:
};


class declnode{
public:
  string name;
  type* typer;
  declnode();
  declnode(string name1,type* typer1);
};


class arraynode: public type{
public:
  //int size;
  //type* typer;
  arraynode(int size1,type* typer1);
};

class pointernode: public type{
public:
  //int size;
  //type* typer;
  pointernode(type* typer1);
};


class basenode:public type{
public:
//  string name;
//  string glotype;
  basenode(string glo);
};



////////////////////////prev ass2 part1 stuff///////////////////

class empty_node: public stmtast_astnode{
	public:
	void print(int n);
	vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};


class stmtlist_node: public stmtast_astnode{
public:
//  abstract_astnode *exp1,*exp2;
  stmtlist_node(abstract_astnode *a,abstract_astnode *b);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class explist_node: public expast_astnode{
public:
//  abstract_astnode *exp1,*exp2;
  explist_node(abstract_astnode *a,abstract_astnode *b);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class ass_node: public expast_astnode{
public:
//  abstract_astnode *exp1,*exp2;
  ass_node(abstract_astnode *a,abstract_astnode *b);
	void print(int n);
	vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class return_node: public stmtast_astnode{
public:
//  abstract_astnode *exp1;
  return_node(abstract_astnode *a);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class if_node: public stmtast_astnode{
public:
//  abstract_astnode *exp1,*exp2,*exp3;
  if_node(abstract_astnode *a,abstract_astnode *b,abstract_astnode *c);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class while_node: public stmtast_astnode{
public:
//  abstract_astnode *exp1,*exp2;
  while_node(abstract_astnode *a,abstract_astnode *b);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class for_node: public stmtast_astnode{
public:
//  abstract_astnode *exp1,*exp2,*exp3,*exp4;
  for_node(abstract_astnode* a,abstract_astnode* b,abstract_astnode* c,abstract_astnode* d);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class op1_node: public expast_astnode{
public:
//  string val; 
//  abstract_astnode *exp1;
  op1_node(abstract_astnode *a,string b);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class op2_node: public expast_astnode{
public:
//  string val;
//  abstract_astnode *exp1,*exp2;
  op2_node(abstract_astnode *a,abstract_astnode *b,string c);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class funcall_node: public expast_astnode{
public:
//  abstract_astnode *exp1, *exp2;
//  string val;
  funcall_node(string a,abstract_astnode *b,vector<type*> vec,vector<int> myindexes);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class floatconst_node: public expast_astnode{
public:
//  string val;
  floatconst_node(string a);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class intconst_node: public expast_astnode{
public:
//  string val;
  intconst_node(string a);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class stringconst_node: public expast_astnode{
public:
//  string val;
  stringconst_node(string a);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class identifier_node: public refast_astnode{
public:
//  string val;
  identifier_node(string a);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class pointer_node: public expast_astnode{
public:
//  abstract_astnode *exp1;
  pointer_node(abstract_astnode *a);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class deref_node: public refast_astnode{
public:
//  abstract_astnode *exp1;
  deref_node(abstract_astnode *a);
	void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class arrayref_node: public refast_astnode{
public:
//  abstract_astnode *exp1,*exp2;
  arrayref_node(abstract_astnode *a,abstract_astnode *b);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class ref_node:public expast_astnode{
public:
//  string val;
//  abstract_astnode *exp1;
  ref_node(abstract_astnode *a,string b);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class member_node:public refast_astnode{
public:
//  string val;
//  abstract_astnode *exp1;
  member_node(abstract_astnode *a,string b);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};

class primaryexp_node:public expast_astnode{
public:
//  abstract_astnode *exp1;
  primaryexp_node(abstract_astnode *a);
  void print(int n);
  vector<string> codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall);
};



