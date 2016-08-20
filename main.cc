#include <iostream>
#include "Scanner.h"
#include "Parser.h"
using namespace std;

extern vector<globalsymtab> gs;
extern vector<string> final_code;
extern vector<string> strings;
int main (int argc, char** arg)
{
	
  Parser parser;
  parser.parse();
  bool mainispresent=false;
  for(int i=0;i<gs.size();i++)
  {
	  if(gs[i].name=="main"&&gs[i].globalrole=="function")mainispresent=true;
  }
  if(mainispresent){
  cout<<".data"<<endl;
  for(int i=0;i<strings.size();i++)
  cout<<strings[i]<<endl;
  cout<<".text"<<endl;
  //cout<<endl<<endl<<"The global symbol table is: "<<endl;
  //cout<<"Name"<<" : "<<"type"<<endl;
  //~ for(int i=0;i<gs.size();i++)
  //~ {
	  //~ cout<<gs[i].name<<" : "<<gs[i]<<endl;
  //~ }
  for(int i=0;i<final_code.size();i++)
  {
	  if(final_code[i][final_code[i].size()-1]==':')cout<<final_code[i]<<endl;
	  else cout<<"	"<<final_code[i]<<endl;
  }
	}
	else cout<<"Main function is not present so the code will not work on qtspim. Please add a main function."<<endl;
}


