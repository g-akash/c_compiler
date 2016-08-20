

#define apb ans.push_back

using namespace std;

globalsymtab::globalsymtab(string name1,string role,string addr,type* ret,vector<localsymtab>* sym)
  {
    name=name1;
    globalrole=role;
    address=addr;
    rettype = ret;
    symtab = sym;
  }

localsymtab::localsymtab(string name1, int offset1, type* typer1, string role1, int size1)
{
	name=name1;
	offset=offset1;
	typer=typer1;
	role=role1;
	size=size1;
}

vector <globalsymtab> gs;
//localsymtab *ls;
int offset=0;		//****
int size=0,size1=0,glosize;         //****
vector<localsymtab> gst;
vector<vector<localsymtab> > gst_vec;
string glotype,glotype1;	
bool is_int_constant=0;
type *temp1,*temp2,*temp3,*temp4;
string st1,st2,st3,st4;
int i1,i2,i3,i4,llval;
abstract_astnode* a1,*a2,*a3,*a4;
vector<type*> args;
vector<int> indexes;
int hasand;
int insidearglist=0;
vector<vector<type*> > mainargs;
vector<vector<int> > mainindexes;
vector<string> final_code;
vector<string> strings;
int stringindex;
int stringnumber=0;
int insideparameterlist=0;


/////laterchanges

int returnsize=0;
//////laterchangesend


/*
 * global variables for assignment 3
 * */

int labelcount;
vector<string> codestore,codepop;
/*ends here*/

/*code for assignment 3*/
string newlabel()
{
	string label="L"+to_string(labelcount);
	labelcount++;
	return label;
}

string newstringlabel()
{
	string stringlabel="str"+to_string(stringnumber);
	stringnumber++;
	return stringlabel;
}


int getparamsize(string val)
{
	int i;
	for(i=0;i<gs.size();i++)
	{
		if(gs[i].name==val&&gs[i].globalrole=="function")break;
	}
	//cout<<i<<" "<<gst_vec.size()<<" "<<val<<endl;
	int out=0;
	for (int j = 0; j < gst_vec[i].size(); j++)
	{
		if(gst_vec[i][j].role=="param"){
			if(gst_vec[i][j].typer->glotype=="array")out+=4;else out+=gst_vec[i][j].size;
		}
	}
	return out;
}

int getdeclsize()
{
	int declisize=0;
	for(int i=0;i<gst.size();i++)
	{
		if(gst[i].role=="var")declisize+=gst[i].size;
	}
	return declisize;
}

int getparamsize(){
	int out=0;
	for (int i = 0; i < gst.size(); ++i)
	{
		if(gst[i].role=="param"){
			if(gst[i].typer->glotype=="array")out+=4;else out+=gst[i].size;
		}
	}
	return out;
}


vector<string> storeregsmain()
{
	vector<string> ans;
	int decls = getdeclsize();
	int params = getparamsize();
	apb("addi $s7 $sp 0");
	apb("addi $sp $sp -800");
	apb("addi $sp $sp -4");
	for(int i=0;i<params; i+=4)
	{
		apb("lw $t0 "+to_string(i+4)+"($sp)");
		apb("sw $t0 "+to_string(i)+"($sp)");
	}
	apb("sw $fp "+to_string(params)+"($sp)");
	apb("addi $fp $sp "+to_string(params));
	return ans;
}



vector<string> storeregs(){	
	int decls = getdeclsize();
	vector<string> ans;
	apb("addi $sp, $sp, -4");
	apb("sw $ra, 0($sp)");
	apb("addi $sp, $sp, -"+to_string(decls));
	return ans;
}


vector<string> popregs(){
	int decls = getdeclsize();
	int params = getparamsize();
	params+=4;
	vector<string> ans;
	apb("addi $sp, $fp, -"+to_string(params));
	//apb(to_string(params)+" i am mmmmmmmmmmmm");
	apb("lw $ra, 0($sp)");
	apb("addi $sp, $fp, 0");
	return ans;
}


int getstructsize(string name){
	vector<localsymtab> curr_table;
	int retval=0;
	for (int i = 0; i < gs.size(); ++i)
	{
		
		if(gs[i].name==name){
			if(gst_vec.size()>i)
			curr_table = gst_vec[i];
			else{}
			break;
		}
	}
	
	for(int i=0;i<curr_table.size();i++)
	{
		
		retval+=(curr_table)[i].size;
		
	}
	return retval;
}


/*ends here*/

type* manufacturenode(int length, type* t,string s)
{
	
	if(t->glotype!="array"&&t->glotype!="pointer")
	{
		type* ans;
		if(s=="array")
		ans = new arraynode(length,t);
		else if(s=="pointer")
		ans = new pointernode(t);
		return ans;
	}
	t->typer=manufacturenode(length,t->typer,s);
	if(t->glotype=="array")t->typesize=t->typer->typesize*t->arraysize;
	else t->typesize=4;
	
	return t;
}






//////////////////////////////////////////////////////////////////////////////////////////////////

//printing function
void print_tab(int n){
	int i;
	for(i = 0; i < n; ++i)
	{
		cout<<"\t";
	}
}

declnode::declnode()
{
	name="";
	typer=0;
}

declnode::declnode(string name1,type* typer1){
	name=name1;
	typer=typer1;
}

arraynode::arraynode(int size1,type* typer1){
	arraysize=size1;
	typer=typer1;
	glotype="array";
	typesize=size1*typer1->typesize;
}

pointernode::pointernode(type* typer1){
	typer=typer1;
	glotype="pointer";
	typesize=4;
}

basenode::basenode(string glo){
	glotype=glo;
	typesize=size;
	typer=0;
}


void empty_node::print(int n){
	//print_tab(n);
	cout<<"(Empty)";	
}

vector<string> empty_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> v;
	return v;
}



explist_node::explist_node(abstract_astnode *a,abstract_astnode *b){
	exp1=a;
	exp2=b;
}

void explist_node::print(int n){
	//print_tab(n);
	if(exp2!=0){
	exp1->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp2->print(n+1);
	}
	else
	{
	exp1->print(n+1);
	}
}

vector<string> explist_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2;
	//~ if(exp1->typer!=0){
	//~ if(exp1->typer->glotype=="array")ans=exp1->codegenerator(2,"can'thappen1","can'thappen1","can'thappen1",2);
	//~ else ans=exp1->codegenerator(1,"can'thappen1","can'thappen1","can'thappen1",2);
	//~ }
	 ans=exp1->codegenerator(1,"can'thappen1","can'thappen1","can'thappen1",2);
	if(exp2!=0)
	{
		
		a2=exp2->codegenerator(1,"can'thappen2","can'thappen2","can'thappen2",2);
	}
	ans.insert(ans.end(),a2.begin(),a2.end());
	return ans;
}

stmtlist_node::stmtlist_node(abstract_astnode *a,abstract_astnode *b){
	exp1=a;
	exp2=b;
}

void stmtlist_node::print(int n){
	
	if(exp2!=0)
	{
	exp1->print(n);
	cout<<"\n";
	print_tab(n+1);
	exp2->print(n+1);
	}
	else
	{
		 //print_tab(n);
	cout<<"(Block [";
	exp1->print(n+1);
	cout<<"])";
	}
}

vector<string> stmtlist_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2;
	if(exp2!=0)
	{
		string neext=newlabel();
	ans=exp1->codegenerator(1,"shouldnothappen","shouldnothappen",neext,2);
	a2=exp2->codegenerator(1,"shouldnothappen1","shouldnothappen1",nextlabel,fall);
	apb(neext+":");
	ans.insert(ans.end(),a2.begin(),a2.end());
	}
	else
	{
		 //print_tab(n);
	ans=exp1->codegenerator(1,"shouldnothappen2","shouldnothappen2",nextlabel,fall);
	}
	
	return ans;
	
}


ass_node::ass_node(abstract_astnode *a,abstract_astnode *b){
	exp1=a;
	exp2=b;
}

void ass_node::print(int n){
	//print_tab(n);
	cout<<"(Ass ";
	exp1->print(n+1);
	cout<<" ";	
	exp2->print(n+1);
	cout<<")";
}

vector<string> ass_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	
	vector<string> ans,a2;
	string lefttype=exp1->typer->glotype,righttype=exp2->typer->glotype;
	int rightsize=exp2->typer->typesize;
	//if(exp2->typer->glotype=="pointer"||exp2->typer->glotype=="int"||exp2->typer->glotype=="float")rightsize=exp2->typer->typesize;
	//else if(exp2->typer->glotype=="array")rightsize=4;
	//else rightsize=getstructsize(exp2->typer->glotype);
	
	if(lefttype=="int"&&righttype=="int")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(1,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		apb("lw $s0,0($sp)");
		apb("lw $s1,4($sp)");
		apb("sw $s0,0($s1)");
		apb("lw $s0,"+to_string(0)+"($sp)");
		apb("sw $s0,"+to_string(4)+"($sp)");
		apb("addi $sp, $sp 4");
		if(fall==0)
		{
			apb("bne $s0, $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("beq $s0, $0 "+falselabel);
		}
		}
	else if(lefttype=="int"&&righttype=="float")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(1,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		apb("l.s $f0,0($sp)");
		apb("lw $s1,4($sp)");
		apb("cvt.w.s $f0,$f0");
		apb("mfc1 $s0, $f0");
		apb("sw $s0,0($s1)");
		apb("sw $s0,4($sp)");
		apb("addi $sp,$sp,4");
		if(fall==0)
		{
			apb("bne $s0, $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("beq $s0, $0 "+falselabel);
		}
		}
	else if(lefttype=="float"&&righttype=="int")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(1,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		apb("lw $s0,0($sp)");
		apb("lw $s1,4($sp)");
		apb("mtc1 $s0, $f0");
		apb("cvt.s.w $f0, $f0");
		apb("s.s $f0,0($s1)");
		apb("s.s $f0, 4($sp)");
		apb("addi $sp,$sp,4");
		if(fall==0)
		{
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("bc1f "+truelabel);
		}
		else if(fall==1)
		{
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("bc1t "+falselabel);
		}
		}
	else if(lefttype=="float"&&righttype=="float")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(1,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		apb("l.s $f0,0($sp)");
		apb("lw $s1,4($sp)");
		apb("s.s $f0,0($s1)");
		apb("s.s $f0, 4($sp)");
		apb("addi $sp,$sp,4");
		if(fall==0)
		{
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("bc1f "+truelabel);
		}
		else if(fall==1)
		{
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("bc1t "+falselabel);
		}
		}
	else if(lefttype=="pointer"&&righttype=="pointer")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(1,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		apb("lw $s0,0($sp)");
		apb("lw $s1,4($sp)");
		apb("sw $s0,0($s1)");
		apb("sw $s0, 4($sp)");
		apb("addi $sp,$sp,4");
		if(fall==0)
		{
			apb("bne $s0, $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("beq $s0, $0 "+falselabel);
		}
		}
	else if(lefttype=="pointer"&&righttype=="array")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(2,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		apb("lw $s0,0($sp)");
		apb("lw $s1,4($sp)");
		apb("sw $s0,0($s1)");
		apb("sw $s0, 4($s0)");
		apb("addi $sp,$sp,8");
		if(fall==0)
		{
			apb("bne $s0, $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("beq $s0, $0 "+falselabel);
		}
		}
	else
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		a2=exp2->codegenerator(1,"shouldnothappen4","shouldnothappen4",nextlabel,2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		//apb(exp2->typer->glotype+" hhhhhhhhhhhhhhhhhhhhhhhhh "+to_string(exp2->typer->typesize));
		apb("lw $s1,"+to_string(rightsize)+"($sp)");
		for(int i=rightsize-4;i>=0;i-=4)
		{
			apb("lw $s0,"+to_string(i)+"($sp)");
			apb("sw $s0,-"+to_string(rightsize-i-4)+"($s1)");
		}
		for(int i=rightsize-4;i>=0;i-=4)
		{
			apb("lw $s0,"+to_string(i)+"($sp)");
			apb("sw $s0,"+to_string(i+4)+"($sp)");
		}
		apb("addi $sp, $sp 4");
		}
		return ans;
}

return_node::return_node(abstract_astnode *a){
	exp1=a;
}

void return_node::print(int n){
	//print_tab(n);
	cout<<"(Return ";
	exp1->print(n+1);
	cout<<")";
}


vector<string> return_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2,a3;
	string lefttype=exp1->typer->glotype,righttype=gs[gs.size()-1].rettype->glotype;//change righttype
	int leftsize=exp1->typer->typesize;
	if(lefttype=="int"&&righttype=="int")
	{
		ans=exp1->codegenerator(1,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		apb("lw $s0,0($sp)");
		apb("addi $s1,$fp,4");
		apb("sw $s0,0($s1)");
		apb("addi $sp,$sp,8");
		}
	else if(lefttype=="int"&&righttype=="float")
	{
		ans=exp1->codegenerator(1,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		apb("l.s $f0,0($sp)");
		apb("addi $s1,$fp,4");
		apb("cvt.w.s $f0,$f0");
		apb("mfc1 $s0, $f0");
		apb("sw $s0,0($s1)");
		apb("addi $sp,$sp,8");
		}
	else if(lefttype=="float"&&righttype=="int")
	{
		ans=exp1->codegenerator(1,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		apb("lw $s0,0($sp)");
		apb("addi $s1,$fp,4");
		apb("mtc1 $s0,$f0");
		apb("mtc1 $s0, $f0");
		apb("cvt.s.w $f0, $f0");
		apb("s.s $f0,0($s1)");
		apb("addi $sp,$sp,8");
		}
	else if(lefttype=="float"&&righttype=="float")
	{
		ans=exp1->codegenerator(1,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		apb("l.s $f0,0($sp)");
		apb("addi $s1,$fp,4");
		apb("s.s $f0,0($s1)");
		apb("addi $sp,$sp,8");
		}
	else if(lefttype=="pointer"&&righttype=="pointer")
	{
		ans=exp1->codegenerator(1,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		apb("lw $s0,0($sp)");
		apb("addi $s1,$fp,4");
		apb("sw $s0,0($s1)");
		apb("addi $sp,$sp,8");
		}
	else if(lefttype=="pointer"&&righttype=="array")
	{
		ans=exp1->codegenerator(2,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		apb("lw $s0,0($sp)");
		apb("addi $s1,$fp,4");
		apb("sw $s0,0($s1)");
		apb("addi $sp,$sp,8");
		}
	else
	{
		
		
		/**
		 * correct it.
		 * */
		
		ans=exp1->codegenerator(1,"shouldnothappen3","shouldnothappen3",nextlabel,2);
		//apb("addi $s1,$fp "+to_string(leftsize));
		//apb("lw $s1,"+to_string(leftsize)+"($sp)");
		for(int i=leftsize-4;i>=0;i-=4)
		{
			apb("lw $s0, "+to_string(i)+"($sp)");
			apb("sw $s0, "+to_string(i+4)+"($fp)");
			//~ apb("lw $s0, 0($sp)");
			//~ apb("addi $s1,$fp,"+to_string(4+leftsize));
			//~ apb("sw $s0, 0($s1)");
			//~ apb("addi $s1, $s1, -4");
		}
		}
		a3=popregs();
		ans.insert(ans.end(),a3.begin(),a3.end());
		apb("jr $ra");
		return ans;
}


if_node::if_node(abstract_astnode *a,abstract_astnode *b,abstract_astnode *c){
	exp1=a;	
	exp2=b;	
	exp3=c;	
}

void if_node::print(int n){
	//print_tab(n);
	cout<<"(If ";
	exp1->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp2->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp3->print(n+1);
	cout<<")";
}

vector<string> if_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2,a3;
	string trrue=newlabel(),faalse=newlabel();
	ans=exp1->codegenerator(1,trrue,faalse,nextlabel,1);//**************************
	a2=exp2->codegenerator(1,trrue,faalse,nextlabel,2);//***********************
	a3=exp3->codegenerator(1,trrue,faalse,nextlabel,2);//*********************
	apb(trrue+":");
	ans.insert(ans.end(),a2.begin(),a2.end());
	apb("j "+nextlabel);
	apb(faalse+":");
	ans.insert(ans.end(),a3.begin(),a3.end());
	return ans;
}


while_node::while_node(abstract_astnode *a,abstract_astnode *b){
	exp1=a;	
	exp2=b;		
}

void while_node::print(int n){
	//print_tab(n);
	cout<<"(While ";
	exp1->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp2->print(n+1);
	cout<<")";
}

vector<string> while_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a1,a2;
	string trrue=newlabel(),faalse=nextlabel,cond=newlabel();
	apb(cond+":");
	a1=exp1->codegenerator(1,trrue,faalse,nextlabel,1);
	a2=exp2->codegenerator(1,trrue,faalse,cond,2);
	ans.insert(ans.end(),a1.begin(),a1.end());
	apb(trrue+":");
	ans.insert(ans.end(),a2.begin(),a2.end());
	apb("j "+cond);
	return ans;
}

for_node::for_node(abstract_astnode *a,abstract_astnode *b,abstract_astnode *c,abstract_astnode *d){
	exp1=a;	
	exp2=b;	
	exp3=c;	
	exp4=d;
}


void for_node::print(int n){
	//print_tab(n);
	cout<<"(For ";
	exp1->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp2->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp3->print(n+1);
	cout<<"\n";
	print_tab(n+1);
	exp4->print(n+1);
	cout<<")";
}

vector<string> for_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2,a3,a4;
	string trrue=newlabel(),faalse=nextlabel,cond=newlabel(),chage=newlabel();
	ans=exp1->codegenerator(n,trrue,faalse,nextlabel,2);
	a2=exp2->codegenerator(n,trrue,faalse,nextlabel,1);
	a3=exp3->codegenerator(n,trrue,faalse,cond,2);
	a4=exp4->codegenerator(n,trrue,faalse,chage,2);
	apb(cond+":");
	ans.insert(ans.end(),a2.begin(),a2.end());
	apb(trrue+":");
	ans.insert(ans.end(),a4.begin(),a4.end());
	apb(chage+":");
	ans.insert(ans.end(),a3.begin(),a3.end());
	apb("j "+cond);
	
	return ans;
}

op1_node::op1_node(abstract_astnode *a,string b){
	exp1=a;
	val=b;	
}

void op1_node::print(int n){
	//print_tab(n);
	cout<<"("<<val<<" ";
	exp1->print(n+1);
	cout<<")";
}

vector<string> op1_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	string lefttype=exp1->typer->glotype;
	int i,j;
	
	if(val=="uminus")
	{
		ans=exp1->codegenerator(1,"","","",2);
		if(lefttype=="float")
		{
			apb("l.s $f0, 0($sp)");
			apb("li.s $f2, 0.0");
			apb("sub.s $f0, $f2, $f0");
			apb("s.s $f0,0($sp)");
			if(fall==0)
			{
				apb("li.s $f1 0.0");
				apb("c.eq.s $f0 $f1");
				apb("bc1f "+truelabel);
			}
			else if(fall==1)
			{
				apb("li.s $f1 0.0");
				apb("c.eq.s $f0 $f1");
				apb("bc1t "+falselabel);
			}
			}
		else
		{
			apb("lw $s0, 0($sp)");
			apb("sub $s0, $0, $s0");
			apb("sw $s0,0($sp)");
			if(fall==0)
			{
				apb("bne $s0 $0 "+truelabel);
			}
			else if(fall==1)
			{
				apb("beq $s0 $0 "+falselabel);
			}
			
			}
	}
	else if(val=="NOT")
	{
		ans=exp1->codegenerator(1,"","","",2);
		if(lefttype=="float")
		{
			apb("l.s $f0, 0($sp)");
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("cfc1 $s0 $25");
			apb("andi $s0 $s0 1");
			apb("sw $s0 0($sp)");
			if(fall==0)
			{
				apb("bne $s0 $0 "+truelabel);
			}
			else if(fall==1)
			{
				apb("beq $s0 $0 "+falselabel);
			}
			
			}/**NEEDS CORRECTION*/
		else
		{
			apb("lw $s0, 0($sp)");
			apb("li $s1 0");
			apb("nor $s0, $s0, $s1");
			apb("sw $s0,0($sp)");
			if(fall==0)
			{
				apb("bne $s0 $0 "+truelabel);
			}
			else if(fall==1)
			{
				apb("beq $s0 $0 "+falselabel);
			}
			}
	}
	else if(val=="AMP")
	{
		ans=exp1->codegenerator(2,"","","",2);
		if(n==2)
		{
			apb("lw $s0 0($sp)");
			apb("addi $s7 $s7 -4");
			apb("sw $s0 0($s7)");
			apb("sw $s7 0($sp)");
		}
		if(fall==0)
		{
			apb("lw $s0 0($sp)");
			apb("bne $s0 $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("lw $s0 0($sp)");
			apb("beq $s0 $0 "+falselabel);
		}
	}
	else if(val=="STAR")
	{
		if(n==1)
		{ans=exp1->codegenerator(1,"","","",2);}
		else if(n==2)
		{
			ans=exp1->codegenerator(2,"","","",2);
		}
		lefttype=exp1->typer->typer->glotype;
		if(lefttype=="float")
		{
			apb("lw $s0,0($sp)");
			apb("l.s $f0, 0($s0)"); 
			apb("s.s $f0, 0($sp)");
			if(fall==0)
			{
				apb("li.s $f1 0.0");
				apb("c.eq.s $f0 $f1");
				apb("bc1f "+truelabel);
			}
			else if(fall==1)
			{
				apb("li.s $f1 0.0");
				apb("c.eq.s $f0 $f1");
				apb("bc1t "+falselabel);
			}
			}
		else
		{
			apb("lw $s0,0($sp)"); 
			apb("lw $s0, 0($s0)"); 
			apb("sw $s0, 0($sp)");
			if(fall==0)
			{
				apb("bne $s0 $0 "+truelabel);
			}
			else if(fall==1)
			{
				apb("beq $s0 $0 "+falselabel);
			}
			}
	}
	else if(val=="PP")
	{
		ans=exp1->codegenerator(2,"","","",2);
		//cout<<"ohkay"<<endl;
		if(lefttype=="float")
		{
			apb("lw $s0 0($sp)");
			apb("l.s $f0 0($s0)");
			apb("li.s $f1 1.0");
			apb("add.s $f2 $f0 $f1");
			apb("sw $f2 0($s0)");
			apb("sw $f0 0($sp)");
			if(fall==0)
			{
				apb("li.s $f1 0.0");
				apb("c.eq.s $f0 $f1");
				apb("bc1f "+truelabel);
			}
			else if(fall==1)
			{
				apb("li.s $f1 0.0");
				apb("c.eq.s $f0 $f1");
				apb("bc1t "+falselabel);
			}
		}
		else
		{
		apb("lw $s0 0($sp)");
		apb("lw $s2 0($s0)");
		apb("addi $s1 $s2 1");
		apb("sw $s1 0($s0)");
		apb("sw $s2 0($sp)");
		if(fall==0)
		{
			apb("bne $s2 $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("beq $s2 $0 "+falselabel);
		}
		
	}
		
	}
	return ans;
}

op2_node::op2_node(abstract_astnode *a,abstract_astnode *b,string c){
	exp1=a;	
	exp2=b;
	
	val=c;
}

void op2_node::print(int n){
	//print_tab(n);
	cout<<"("<<val<<" ";
	exp1->print(n+1);
	cout<<" ";
	exp2->print(n+1);
	cout<<")";
}

vector<string> op2_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2;
	int i,j;
	string trrue,faalse;
	type* t1=exp1->typer,*t2=exp2->typer;
	if(val=="OR")
	{
		if(fall>=2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,fall);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,fall);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();l2=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("li.s $f2, 0.0");
			apb("li $s0, 0");
			apb("c.eq.s $f0, $f2");
			apb("bc1f "+l1);
			apb("c.eq.s $f1, $f2");
			apb("bc1f "+l1);
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 1");
			apb(l2+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();l2=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("li.s $f2, 0.0");
			apb("li $s0, 0");
			apb("c.eq.s $f0, $f2");
			apb("bc1f "+l1);
			apb("c.eq.s $f1, $f2");
			apb("bc1f "+l1);
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 1");
			apb(l2+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();l2=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("li.s $f2, 0.0");
			apb("li $s0, 0");
			apb("c.eq.s $f0, $f2");
			apb("bc1f "+l1);
			apb("c.eq.s $f1, $f2");
			apb("bc1f "+l1);
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 1");
			apb(l2+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			l1=newlabel();l2=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("bne $s0 0 "+l1);
			apb("bne $s1 0 "+l1);
			apb("li $s0 0");
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 1");
			apb(l2+":");
			//ans.push_back("or $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
		}
		else if(fall==0)
		{
			faalse=newlabel();
			ans=exp1->codegenerator(1,truelabel,faalse,nextlabel,0);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,0);
			apb(faalse+":");
			ans.insert(ans.end(),a2.begin(),a2.end());
			
		}
		else if(fall==1)
		{
			faalse=newlabel();
			ans=exp1->codegenerator(1,truelabel,faalse,nextlabel,0);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,1);
			apb(faalse+":");
			ans.insert(ans.end(),a2.begin(),a2.end());
		}
	}
	else if(val=="AND")
	{
		if(fall>=2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,fall);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,fall);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();l2=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("li.s $f2, 0.0");
			apb("li $s0, 1");
			apb("c.eq.s $f0, $f2");
			apb("bc1t "+l1);
			apb("c.eq.s $f1, $f2");
			apb("bc1t "+l1);
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 0");
			apb(l2+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();l2=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("li.s $f2, 0.0");
			apb("li $s0, 1");
			apb("c.eq.s $f0, $f2");
			apb("bc1t "+l1);
			apb("c.eq.s $f1, $f2");
			apb("bc1t "+l1);
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 0");
			apb(l2+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();l2=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("li.s $f2, 0.0");
			apb("li $s0, 1");
			apb("c.eq.s $f0, $f2");
			apb("bc1t "+l1);
			apb("c.eq.s $f1, $f2");
			apb("bc1t "+l1);
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 0");
			apb(l2+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			l1=newlabel(); l2=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("beq $s0 0 "+l1);
			apb("beq $s1 0 "+l1);
			apb("li $s0 1");
			apb("j "+l2);
			apb(l1+":");
			apb("li $s0 0");
			apb(l2+":");
			//ans.push_back("and $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
		}
		else if(fall==0)
		{
			trrue=newlabel();
			ans=exp1->codegenerator(1,trrue,falselabel,nextlabel,1);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,0);
			apb(trrue+":");
			ans.insert(ans.end(),a2.begin(),a2.end());
		}
		else if(fall==1)
		{
			trrue=newlabel();
			ans=exp1->codegenerator(1,trrue,falselabel,nextlabel,1);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,1);
			apb(trrue+":");
			ans.insert(ans.end(),a2.begin(),a2.end());
		}
		
	}
	else if(val=="EQ_OP")
	{	
		if(fall==2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.eq.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("c.eq.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("xor $s0,$s1,$s0");
			apb("sltu $s0,$s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
			
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.eq.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("xor $s0,$s1,$s0");
			apb("sltu $s0,$s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0, $0, "+truelabel);//some change made here
			}
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0");  //change made here
			apb("c.eq.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0"); //change made here
			apb("c.eq.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("xor $s0,$s1,$s0");
			apb("sltu $s0,$s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0, $0, "+falselabel);
			}
			
		}
	}
	else if(val=="NE_OP")
	{
		if(fall==2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0 $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.eq.s $f0 $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("c.eq.s $f0 $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("xor $s0,$s1,$s0");
			apb("sltu $s0,$0,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
			
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0, $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+truelabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0"); //small change made here
			apb("c.eq.s $f0, $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+truelabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0"); //change made here
			apb("c.eq.s $f0, $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+truelabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("xor $s0,$s1,$s0");
			apb("sltu $s0,$0,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0, $0, "+truelabel);
			}
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.eq.s $f0, $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+falselabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.eq.s $f0, $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+falselabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0"); //change made here
			apb("c.eq.s $f0, $f1");
			apb("li $s0 1");
			apb("bc1f "+l1);
			apb("li $s0 0");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+falselabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("xor $s0,$s1,$s0");
			apb("sltu $s0,$0,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0, $0, "+falselabel);//change made here
			}
			
		}
	}
	
	else if(val=="LT")
	{
		if(fall==2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.lt.s $f1 $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.lt.s $f1 $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("c.lt.s $f1 $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
			
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.lt.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0"); //change made here
			apb("c.lt.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0"); //change made here
			apb("c.lt.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else {
			//~ ans.push_back("lw $s0,0($sp)");
			//~ ans.push_back("lw $s1,4($sp)");
			//~ apb("slt $s2,$s1,$s0");
			//~ ans.push_back("sw $s2,4($sp)");
			//~ ans.push_back("addi $sp,$sp,4");
			//~ apb("slt $s0, $s1, $s0 ");
			//~ apb("bne $s0, $0, "+truelabel);	//change made here
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0, $0, "+truelabel);
			}
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.lt.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0"); changes made here
			apb("c.lt.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.lt.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			//apb("slt $s0,$s1, $s0");
			apb("beq $s0, $0 "+falselabel);
			}
			
		}
		
	}
	else if(val=="GT")
	{
		
		if(fall==2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.lt.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.lt.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("c.lt.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s0,$s1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
			
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.lt.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,04$sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.lt.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.lt.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s0,$s1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			//apb("slt $s0, $s0, $s1 ");
			apb("bne $s0, $0, "+truelabel);
			}
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.lt.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.lt.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.lt.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s0,$s1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			//apb("slt $s0,$s0, $s1");
			apb("beq $s0, $0 "+falselabel);
			}
			
		}
	}
	else if(val=="LE_OP")
	{
		if(fall==2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.le.s $f1 $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.le.s $f1 $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("c.le.s $f1 $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s0,$s1");
			apb("xori $s0, $s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
			
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.le.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.le.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.le.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s0,$s1");
			apb("xori $s0, $s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0, $0, "+truelabel);
			}
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.le.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.le.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.le.s $f1, $f0");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s0,$s1");
			apb("xori $s0, $s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0, $0 "+falselabel);
			}
			
		}
		
	}
	else if(val=="GE_OP")
	{	
		if(fall==2)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.le.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.le.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			apb("c.le.s $f0 $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s1,$s0");
			apb("xori $s0, $s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
			
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.le.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			apb("c.le.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.le.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1t "+truelabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s1,$s0");
			apb("xori $s0, $s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0, $0, "+truelabel);
			}
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,truelabel,falselabel,nextlabel,2);
			a2=exp2->codegenerator(1,truelabel,falselabel,nextlabel,2);
			ans.insert(ans.end(),a2.begin(),a2.end());
			string l1,l2;
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			l1=newlabel();
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			apb("c.le.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.le.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			l1=newlabel();
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			//ans.push_back("cvt.s.w $f0,$f0");//change made here
			apb("c.le.s $f0, $f1");
			apb("li $s0 0");
			apb("bc1f "+l1);
			apb("li $s0 1");
			apb(l1+":");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bc1f "+falselabel);
			
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			apb("slt $s0,$s1,$s0");
			apb("xori $s0, $s0,1");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0, $0 "+falselabel);
			}
			
		}
		
	}
	else if(val=="PLUS")
	{
		if(fall==2)
		{
		ans=exp1->codegenerator(1,"shouldnothappen5","shouldnothappen5","shouldnothappen5",2);
		a2=exp2->codegenerator(1,"shouldnothappen6","shouldnothappen6","shouldnothappen6",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("add $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
			
		}
		else if(fall==0)
		{
		ans=exp1->codegenerator(1,"shouldnothappen7","shouldnothappen7","shouldnothappen7",2);
		a2=exp2->codegenerator(1,"shouldnothappen8","shouldnothappen8","shouldnothappen8",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("add $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0,$0 "+truelabel);
			}
			
		}
		else if(fall==1)
		{
		ans=exp1->codegenerator(1,"shouldnothappen9","shouldnothappen9","shouldnothappen9",2);
		a2=exp2->codegenerator(1,"shouldnothappen10","shouldnothappen10","shouldnothappen10",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("add.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("add $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0,$0 "+falselabel);
			}
			
		}
	}
	else if(val=="MINUS")
	{
		if(fall==2)
		{
		ans=exp1->codegenerator(1,"shouldnothappen11","shouldnothappen11","shouldnothappen11",2);
		a2=exp2->codegenerator(1,"shouldnothappen12","shouldnothappen12","shouldnothappen12",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("sub $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		}
		else if(fall==0)
		{
		ans=exp1->codegenerator(1,"shouldnothappen13","shouldnothappen13","shouldnothappen13",2);
		a2=exp2->codegenerator(1,"shouldnothappen14","shouldnothappen14","shouldnothappen14",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("sub $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0,$0 "+truelabel);
			}
			
		}
		else if(fall==1)
		{
		ans=exp1->codegenerator(1,"shouldnothappen15","shouldnothappen15","shouldnothappen15",2);
		a2=exp2->codegenerator(1,"shouldnothappen16","shouldnothappen16","shouldnothappen16",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("sub.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("sub $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0,$0 "+falselabel);
			}
			
		}
	}
	else if(val=="MULT")
	{
		if(fall==2)
		{
		ans=exp1->codegenerator(1,"shouldnothappen17","shouldnothappen17","shouldnothappen17",2);
		a2=exp2->codegenerator(1,"shouldnothappen18","shouldnothappen18","shouldnothappen18",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mul $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		}
		else if(fall==0)
		{
		ans=exp1->codegenerator(1,"shouldnothappen19","shouldnothappen19","shouldnothappen19",2);
		a2=exp2->codegenerator(1,"shouldnothappen20","shouldnothappen20","shouldnothappen20",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mul $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0,$0 "+truelabel);
			}
			
		}
		else if(fall==1)
		{
		ans=exp1->codegenerator(1,"shouldnothappen21","shouldnothappen21","shouldnothappen21",2);
		a2=exp2->codegenerator(1,"shouldnothappen22","shouldnothappen22","shouldnothappen22",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mul.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mul $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0,$0 "+falselabel);
			}
			
		}
		
	}
	else if(val=="DIV")
	{
		
		if(fall==2)
		{
		ans=exp1->codegenerator(1,"shouldnothappen23","shouldnothappen23","shouldnothappen23",2);
		a2=exp2->codegenerator(1,"shouldnothappen24","shouldnothappen24","shouldnothappen24",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("div $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			}
		}
		else if(fall==0)
		{
			ans=exp1->codegenerator(1,"shouldnothappen25","shouldnothappen25","shouldnothappen25",2);
		a2=exp2->codegenerator(1,"shouldnothappen26","shouldnothappen26","shouldnothappen26",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1f "+truelabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("div $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("bne $s0,$0 "+truelabel);
			}
			
		}
		else if(fall==1)
		{
			ans=exp1->codegenerator(1,"shouldnothappen27","shouldnothappen27","shouldnothappen27",2);
		a2=exp2->codegenerator(1,"shouldnothappen28","shouldnothappen28","shouldnothappen28",2);
		ans.insert(ans.end(),a2.begin(),a2.end());
		if(t1->glotype=="float"&&t2->glotype=="int")
		{
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("mtc1 $s0,$f0");
			ans.push_back("cvt.s.w $f0,$f0");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="int"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("mtc1 $s1,$f1");
			ans.push_back("cvt.s.w $f1,$f1");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else if(t1->glotype=="float"&&t2->glotype=="float")
		{
			ans.push_back("l.s $f0,0($sp)");
			ans.push_back("l.s $f1,4($sp)");
			ans.push_back("div.s $f0,$f1,$f0");
			ans.push_back("s.s $f0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("li.s $f1, 0.0");
			apb("c.eq.s $f0, $f1");
			apb("bc1t "+falselabel);
			}
		else {
			ans.push_back("lw $s0,0($sp)");
			ans.push_back("lw $s1,4($sp)");
			ans.push_back("div $s0,$s1,$s0");
			ans.push_back("sw $s0,4($sp)");
			ans.push_back("addi $sp,$sp,4");
			apb("beq $s0,$0 "+falselabel);
			}
			
		}		
	}
	
	return ans;
}

funcall_node::funcall_node(string a,abstract_astnode *b,vector<type*> vec,vector<int> myindex){
	val=a;
	exp1=new identifier_node(val);
	exp2=b;
	argsofprintf=vec;
	indexesofstringsofprintf=myindex;
	for(int i=0;i<gs.size();i++)
	{
		if(gs[i].name==val&&gs[i].globalrole=="function"){typer=gs[i].rettype;}
	}
	//cout<<" indise gs "<<typer->typesize<<endl;
}

void funcall_node::print(int n)
{
	//print_tab(n);
	cout<<exp2<<endl;
	if(exp2!=0)
	{
	cout<<"(Func ";
	exp1->print(n+1);
	cout<<" ";
	exp2->print(n+1);
	cout<<")";
}
	else
	{
		cout<<"(Func ";
	exp1->print(n+1);
	cout<<" ";
	cout<<"(Empty)";
	cout<<")";
}
}

vector<string> funcall_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2;
	int datasize,i,paramsze;
	string returntype;
	if(val!="printf")insidearglist=2;
	if(exp2!=0)
	{
		a2=exp2->codegenerator(1,"shouldnothappenagain1","shouldnothappenagain1","shouldnothappenagain1",2);
	}
	insidearglist=0;
	if(val=="printf")
	{
		ans.insert(ans.end(),a2.begin(),a2.end());
		for(int i=0;i<argsofprintf.size();i++)
		{
		
		if(argsofprintf[i]->glotype=="float")
		{
			apb("l.s $f12 "+to_string(argsofprintf.size()*4-4-i*4)+"($sp)");
			apb("li $v0 2");
			apb("li $s0 4");
			//apb("sw $s0 0($sp)");
			apb("syscall");
			
		}
		else if(argsofprintf[i]->glotype=="string")
		{
			string thepointer,s;
			int theindex=indexesofstringsofprintf[i];
			s=strings[theindex];
			thepointer=s.substr(0,s.find(":"));
			apb("li $v0 4");
			apb("la $a0 "+thepointer);
			apb("syscall");
			
		}
		else 
		{
			apb("lw $a0 "+to_string(argsofprintf.size()*4-4-i*4)+"($sp)");
			apb("li $v0 1");
			apb("li $s0 4");
			//apb("sw $s0 0($sp)");
			apb("syscall");
			
		}
	}
	apb("addi $sp $sp "+to_string(argsofprintf.size()*4));
		return ans;
	}
	for(i=0;i<gs.size();i++)
	{
		if(gs[i].name==val&&gs[i].globalrole=="function"){paramsze=getparamsize(val);datasize=gs[i].rettype->typesize;returntype=gs[i].rettype->glotype;break;}
	}
	//apb(to_string(datasize)+" here "+returntype);
	apb("addi $sp, $sp  -"+to_string(datasize+4));
	ans.insert(ans.end(),a2.begin(),a2.end());
	apb("sw $fp "+to_string(paramsze)+"($sp)");
	apb("addi $fp $sp "+to_string(paramsze));
	int varsize=0;
	for(int j=argsofprintf.size()-1;j>=0;j--)
	{//cout<<args[j]->glotype<<endl;
		//apb(args[j]->glotype+"hhhhhhhhhhhhhhhhhh");
		if(argsofprintf[j]->glotype=="float"&&gst_vec[i][j].typer->glotype!="float")
		{
			apb("l.s $f0 "+to_string(varsize)+"($sp)");
			apb("cvt.w.s $f0 $f0");
			apb("mfc1 $s0 $f0");
			apb("sw $s0 "+to_string(varsize)+"($sp)");
			}
		else if(argsofprintf[j]->glotype!="float"&&gst_vec[i][j].typer->glotype=="float")
		{
			apb("lw $s0 "+to_string(varsize)+"($sp)");
			apb("mtc1 $s0 $f0");
			apb("cvt.s.w $f0 $f0");
			apb("s.s $f0 "+to_string(varsize)+"($sp)");
			}
		if(gst_vec[i][j].typer->glotype!="array")varsize+=gst_vec[i][j].size;
		else varsize+=4;
	}
	apb("jal "+gs[i].name);
	apb("lw $fp, 0($sp)");
	apb("addi $sp $sp, 4");
	if(returntype=="float")
	{
		apb("l.s $f0, 0($sp)");
		if(fall==0)
		{
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("bc1f "+truelabel);
		}
		else if(fall==1)
		{
			apb("li.s $f1 0.0");
			apb("c.eq.s $f0 $f1");
			apb("bc1t "+falselabel);
		}
	}
	else
	{
		apb("lw $s0 0($sp)");
		if(fall==0)
		{
			apb("bne $s0 $0 "+truelabel);
		}
		else if(fall==1)
		{
			apb("beq $s0 $0 "+falselabel);
		}
	}
	return ans;
}


floatconst_node::floatconst_node(string a){
float_val=stof(a);
typer=new basenode("float");
}

void floatconst_node::print(int n){
	//print_tab(n);
	cout<<"(FloatConst "<<val<<")";
}

vector<string> floatconst_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	ans.push_back("addi $sp,$sp,-4");
	ans.push_back("li.s $f0,"+to_string(float_val));
	ans.push_back("s.s $f0,0($sp)");
	return ans;
}

intconst_node::intconst_node(string a){
if(a.length()<=1)	
int_val=stoi(a);
else if(a[0]=='0'&&(a[1]=='x'||a[1]=='X'))
int_val=stoi(a.substr(2), nullptr, 16);
else if(a[0]=='0'&&(a[1]=='b'||a[1]=='B'))
int_val=stoi(a.substr(2), nullptr, 2);
else if(a[0]=='0')
int_val=stoi(a.substr(1), nullptr, 8);
else int_val=stoi(a);
typer = new basenode("int");
}

void intconst_node::print(int n){
	//print_tab(n);
	cout<<"(IntConst "<<val<<")";
}

vector<string> intconst_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	ans.push_back("addi $sp,$sp,-4");
	ans.push_back("li $s0,"+to_string(int_val));
	ans.push_back("sw $s0,0($sp)");
	return ans; 
}

stringconst_node::stringconst_node(string a){
	val=a;
	typer=new basenode("string");
}

void stringconst_node::print(int n){
	//print_tab(n);
	cout<<"(stringConst "<<val<<")";
}

vector<string> stringconst_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	ans.push_back("addi $sp,$sp,-4");
	ans.push_back("li $s0, 4");
	ans.push_back("sw $s0,0($sp)");
	return ans;
}

identifier_node::identifier_node(string a){
	val=a;
	identi=true;
	for(int i=0;i<gst.size();i++)
	{
		if(gst[i].name==a){typer=gst[i].typer;}
		
	}
}

void identifier_node::print(int n){
	//print_tab(n);
	cout<<"(Id \'\'"<<val<<"\'\')";
}

vector<string> identifier_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	
	vector<string> ans;
	int i,datasize;
	for(i=0;i<gst.size();i++)
		{
			if(gst[i].name==val)break;
		}
		datasize=gst[i].size;
		if(insidearglist==2&&typer->glotype=="array")n=2;
		
	if(n==1)
	{
		
			
			ans.push_back("addi $s1, $fp, -"+to_string(gst[i].offset));
			if(gst[i].role=="param"&&gst[i].typer->glotype=="array")apb("lw $s1 0($s1)");
			datasize=typer->typesize;
			ans.push_back("addi $sp,$sp,-"+to_string(datasize));
			for(int j=datasize-4;j>=0;j-=4)
			{
			apb("lw $s0, -"+to_string(datasize-4-j)+"($s1)");
			apb("sw $s0 "+to_string(j)+"($sp)");
			//ans.push_back("lw $s0,0($s0)");
			//ans.push_back("sw $s0,0($sp)");
			}
			
	}
	else if(n==2)
	{
		ans.push_back("addi $s0, $fp, -"+to_string(gst[i].offset));
		//apb(to_string(insidearglist));
		if(gst[i].role=="param"&&!insidearglist&&gst[i].typer->glotype=="array")apb("lw $s0 0($s0)");
		ans.push_back("addi $sp,$sp,-4");
		ans.push_back("sw $s0,0($sp)");
	}
	return ans;
}

pointer_node::pointer_node(abstract_astnode *a){
	exp1=a;
}

void pointer_node::print(int n){
	//print_tab(n);
	exp1->print(n+1);
}

vector<string> pointer_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	
	return ans;
}

deref_node::deref_node(abstract_astnode *a){
	exp1=a;
}

void deref_node::print(int n){
	//print_tab(n);
	cout<<"(Deref ";
	exp1->print(n+1);
	cout<<")";	

}

vector<string> deref_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	int datasize;
	ans=exp1->codegenerator(1,"neednothappen","neednothappen","neednothappen",2);
	if(n==1)
	{
		datasize=exp1->typer->typer->typesize;
		apb("lw $s0, 0($sp)");
		apb("addi $sp $sp "+to_string(4-datasize));
		for(int i=datasize-4;i>=0;i-=4)
		{
			apb("lw $s1 "+to_string(datasize-4-i)+"($s0)");
			apb("sw $s1 "+to_string(i)+"($sp)");
		}
	}
	else if(n==2)
	{
		//no need to generate code. -by shubham
	}
	return ans;
}


arrayref_node::arrayref_node(abstract_astnode *a,abstract_astnode *b){
	exp1=a;
	exp2=b;
}


// might need to add brackets...
void arrayref_node::print(int n){
	//print_tab(n);
	cout<<"(ArrayRef ";
	exp1->print(n+1);
	cout<<" ";	
	exp2->print(n+1);
	cout<<")";	
}


vector<string> arrayref_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a1,a2;
	int datasize,i;
	datasize=typer->typesize;
	ans=exp1->codegenerator(2,"","","",2);
	a2=exp2->codegenerator(1,"","","",2);
	ans.insert(ans.end(),a2.begin(),a2.end());
	ans.push_back("lw $s0, 0($sp)");
	ans.push_back("lw $s1, 4($sp)");
	apb("li $s2 "+to_string(datasize));
	if(exp1->typer->glotype=="pointer")apb("lw $s1 0($s1)");
	//identi=false;
	apb("mul $s0, $s0 $s2");////////////////////////////////////////marked for 
	ans.push_back("sub $s0,$s1,$s0");
	if(insideparameterlist==1)n=2;
	if(n==1)
	{
		datasize=typer->typesize;
		apb("addi $sp, $sp "+to_string(8-datasize));
		for(i=datasize-4;i>=0;i-=4)
		{
			apb("lw $s1 -"+to_string(datasize-4-i)+"($s0)");
			apb("sw $s1 "+to_string(i)+"($sp)");
		}
	}
	else
	{
		apb("addi $sp, $sp, 4");
		apb("sw $s0 0($sp)");
	}
	return ans;
}

ref_node::ref_node(abstract_astnode *a, string b){
	exp1=a;
	val=b;
}

void ref_node::print(int n){
	//print_tab(n);
	cout<<"(";
	exp1->print(n+1);
	cout<<"->"<<val<<")";
}

vector<string> ref_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	ans=exp1->codegenerator(1,"","","",2);
	string lefttype=exp1->typer->typer->glotype,righttype=val;
	int i,j,datasize,k;
	for(i=0;i<gs.size();i++)
	{
		if(gs[i].name==lefttype&&gs[i].globalrole=="struct")break;
	}
	for(j=0;j<gst_vec[i].size();j++)
	{
		if(gst_vec[i][j].name==righttype)
		{
			apb("lw $s0,0($sp)");
			apb("addi $s0,$s0,-"+to_string(gst_vec[i][j].offset));
			if(n==1)
			{
				datasize=typer->typesize;
				apb("addi $sp, $sp "+to_string(4-datasize));
				for(k=datasize-4;k>=0;k-=4)
				{
					apb("lw $s1 "+to_string(datasize-4-k)+"($s0)");
					apb("sw $s1 "+to_string(k)+"($sp)");
				}
				}
			else{apb("sw $s0 0($sp)");}
			break;
		}
	}
	return ans;
}

member_node::member_node(abstract_astnode *a, string b){
	exp1=a;
	val=b;
}

void member_node::print(int n){
	//print_tab(n);
	cout<<"(";
	exp1->print(n+1);
	cout<<".";
	cout<<"(Id \'\'"<<val<<"\'\')";
	cout<<")";
}

vector<string> member_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans,a2;
	ans=exp1->codegenerator(2,"","","",2);
	string lefttype=exp1->typer->glotype,righttype=val;
	int i,j,k,datasize;
	for(i=0;i<gs.size();i++)
	{
		if(gs[i].name==lefttype&&gs[i].globalrole=="struct")break;
	}
	for(j=0;j<gst_vec[i].size();j++)
	{
		if(gst_vec[i][j].name==righttype)
		{
			apb("lw $s0,0($sp)");
			apb("addi $s0,$s0,-"+to_string(gst_vec[i][j].offset));
			if(n==1)
			{
				datasize=typer->typesize;
				apb("addi $sp, $sp "+to_string(4-datasize));
				for(k=datasize-4;k>=0;k-=4)
				{
					apb("lw $s1 "+to_string(datasize-4-k)+"($s0)");
					apb("sw $s1 "+to_string(k)+"($sp)");
				}
				}
			else{apb("sw $s0 0($sp)");}
			break;
		}
	}
	

	return ans;
}

primaryexp_node::primaryexp_node(abstract_astnode *a){
	exp1=a;
}

void primaryexp_node::print(int n){
	//print_tab(n);
	cout<<"(";
	exp1->print(n+1);
	cout<<")";
}

vector<string> primaryexp_node::codegenerator(int n,string truelabel, string falselabel, string nextlabel, int fall)
{
	vector<string> ans;
	ans=exp1->codegenerator(n,truelabel,falselabel,nextlabel,fall);
	return ans;
}

void symprint()
{
	cout<<endl;
	int count=0;
	if(gst_vec.size()==0){cout<<"SYMBOLTABLE IS EMPTY"<<endl; return;}
	vector<localsymtab> vec= gst_vec[gst_vec.size()-1];
	if(vec.size()>0)count=vec[0].offset;
	
	if(gs.size()!=0)
	{
		cout<<"The symbol table of the "<<gs[gs.size()-1].globalrole<<" "<<gs[gs.size()-1].name<<" is:"<<endl;
		cout<<"Name"<<" : "<<"Offset"<<" : "<<"Size"<<" : "<<"Var/Param"<<endl;
	}
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i].name<<" : "<<vec[i].offset-count<<" : "<<vec[i].size<<" : "<<vec[i].role<<endl;
	}
	cout<<endl;
	
	
}


bool alreadydeclared(string name)
{
	for(int i=0;i<gst.size();i++)
	{
		if(gst[i].name==name)return true;
	}
	return false;
}

bool structpresent(string name)
{
	for(int i=0;i<gs.size();i++)
	{
		if(gs[i].name==name&&gs[i].globalrole=="struct")return true;
	}
	return false;
}

bool funcpresent(string name)
{
	if(name=="printf")return true;
	for(int i=0;i<gs.size();i++)
	{
		if(gs[i].name==name&&gs[i].globalrole=="function"){return true;}
	}
	return false;
}




//////////////////////////////////////////////////////////////////////////////////


perform::perform()
{
	voi=false;
	arraypoint=0;
	compatible=false;
	same=false;
}


/////////////////////////////////////////

//Functions: 
perform typecompare(type* t1, type* t2,string instruction)
{
	perform result,v1,v2;
	if(t1==0||t2==0){result.compatible=false; result.same=false; return result;}
	string x=t1->glotype, y=t2->glotype;
	
	
	if(instruction=="="){
	
	if(x==y&&(x=="int"||x=="float"||(x!="void"&&x!="pointer"&&x!="array"))){result.compatible=true;result.same=true;result.voi=false; result.arraypoint=0; return result;}
	if(x=="int"&&y=="float"||(x=="float"&&y=="int")){result.compatible=true;result.same=false;result.voi=false; result.arraypoint=0; return result;}
	if(x=="void"||y=="void")
	{
		result.compatible=false;
		if(x==y)
		result.same=true;
		else result.same=false;
		result.arraypoint=0;
		result.voi=true; 
		return result;
		}
	if(x=="array"){result.same=false;result.compatible=false;result.arraypoint=0;result.voi=false; return result;}
	if(x=="pointer"&&y=="pointer"){
		v1= typecompare(t1->typer,t2->typer,"=");
		if(v1.arraypoint>=2){result.compatible=false;result.same=false;result.arraypoint=v1.arraypoint;result.voi=v1.voi;return result;}
		if(!v1.voi)
		{
			result=v1;
			result.voi=false;
			if(!v1.same)
			result.compatible=false;
			else result.compatible=true;
			return result;
		}
		if(v1.voi)
		{
			result.voi=false;
			result.arraypoint=v1.arraypoint;
			result.compatible=true;
			result.same=v1.same;
			return result;
		}
		
		}
	if((x=="pointer"&&y=="array"))
	{v1= typecompare(t1->typer,t2->typer,"=");
		result.arraypoint=v1.arraypoint+1;
		result.voi=false;
		if(result.arraypoint>=2){result.compatible=false; result.same=false;return result; }
		if(v1.voi){result.compatible=true; result.same=false; return result;}
		if(v1.compatible){result.compatible=true; result.same=false; return result;}
		
		
	result.compatible=false; result.same=false; result.voi=false;result.arraypoint=false;return result;
	
		
	}
}
	else if(instruction=="||"||instruction=="&&")
	{
	if((x!="int"&&x!="float"&&x!="array"&&x!="pointer")||(y!="int"&&y!="float"&&y!="array"&&y!="pointer")){result.same=false;result.compatible=false;return result;}
	result.compatible=true; return result;
	}
	else if(instruction=="=="||instruction=="!="||instruction=="<"||instruction==">"||instruction=="<="||instruction==">=")
	{
		if(x==y&&(x=="int"||x=="float")){result.same=true;result.arraypoint=0;result.compatible=true;result.voi=false; return result;}
		if(x==y&&x=="void"){result.same=true;result.arraypoint=0;result.compatible=true;result.voi=true; return result;}
		
		if(x==y&&(x=="array"||x=="pointer"))
		{
			v1=typecompare(t1->typer,t2->typer,instruction);
			result.arraypoint=v1.arraypoint;
			result.same=v1.same;
			if(v1.arraypoint>=2||v1.same==false){result.compatible=false; return result;}
			result.compatible=true; return result;
		}
		if((x=="array"||x=="pointer")&&(y=="array"||y=="pointer")&&x!=y)
		{
			result.same=false;
			v1=typecompare(t1->typer,t2->typer,instruction);
			result.arraypoint=v1.arraypoint+1;
			if(result.arraypoint>=2||v1.same==false){result.compatible=false; return result;}
			result.compatible=v1.compatible; return result;
		}
		if((x=="float"||x=="int")&&(y=="float"||y=="int")){result.same=false;result.arraypoint=0;result.compatible=true;result.voi=false; return result;}
		if(x==y){result.same=true;result.compatible=false; result.arraypoint=0;result.voi=false; return result;}
			result.arraypoint=v1.arraypoint; 
			if(v1.voi)result.compatible=false; 
			return result;
			}
		
	
	else if(instruction=="PLUS"||instruction=="MINUS"||instruction=="+"||instruction=="-")
	{
		
		if((x=="int"&&y=="int")||(x=="float"&&y=="int")||(x=="float"&&y=="float")||(x=="int"&&y=="float")||(x=="pointer"&&y=="int")||(x=="int"&&y=="pointer")||(x=="array"&&y=="int")||(x=="int"&&y=="array")){result.compatible=true; return result;}
		result.compatible=false; return result;
	}
	else if(instruction=="*"||instruction=="/")
	{
		if((x=="float"||x=="int")&&(y=="int"||y=="float")){result.compatible=true; return result;}
		result.compatible = false;
		return result;
	}
	
	result.compatible=false;
	return result;
}
perform typecompare2(type* t1,string name, string instruction)
{
	
	perform result,v1,v2;
	vector<localsymtab> vec;
	
	string x=t1->glotype,y=name;
	if(instruction==".")
	{
		
		if(x=="int"||x=="array"||x=="pointer"||x=="float"||x=="void"){result.compatible=false; return result;}
		
		for(int i=0;i<gs.size();i++)
		if(gs[i].name==x)
		{
			if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				cout<<"error: variable "<<name<<" is not a valid data member"<<endl;
				exit(0);
			}
			break;
			}
		for(int i=0;i<vec.size();i++)   //change in localsymtab size could mess this up
		{
			if(vec[i].name==y){result.compatible=true; return result;}
		}
		result.compatible=false; return result;
	}
	else if(instruction=="->")
	{
		x=t1->typer->glotype;
		if(x=="int"||x=="array"||x=="pointer"||x=="float"||x=="void"){result.compatible=false; return result;}
		for(int i=0;i<gs.size();i++)if(gs[i].name==x)
		{
			if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				cout<<"Trying to access members of struct "<<x<<" which does not have symboltable"<<endl;
				exit(0);
			}
			break;
			}
		for(int i=0;i<vec.size();i++)   //change in localsymtab size could mess this up
		{
			if(vec[i].name==y){result.compatible=true; return result;}
		}
		result.compatible=false; return result;
	}
	result.compatible=false;
	return result;
}
perform typecompare3(type* t1, string instruction)
{
	perform result,v1,v2;
	string x=t1->glotype;
	if(instruction=="NOT"||instruction=="!")
	{
		if(x=="int"||x=="float"||x=="pointer"||x=="array"){result.compatible=true; return result;}
		result.compatible = false;
		return result;
	}
	else if(instruction=="uminus"||instruction=="-")
	{
		if(x=="int"||x=="float"){result.compatible=true; return result;}
		result.compatible = false;
		return result;
	}
	else if(instruction=="STAR"||instruction=="*")
	{
		
		if((x=="pointer"||x=="array")&&t1->typer->glotype!="void"){result.compatible=true; return result;}
		result.compatible=false;
		return result;
	}
	else if(instruction=="AMP"||instruction=="&")
	{
		result.compatible=true; return result;
	}
	else if(instruction=="++")
	{
		if(x=="pointer"||x=="int"||x=="float"){result.compatible=true; return result;}
		result.compatible=false;
		return result;
	}
	result.compatible=false;
	return result;
}




bool typecompare4(type* t1,type* t2,int depth){  // t1 is expected type, t2 is argument's type
	
	string s1=t1->glotype,s2=t2->glotype;
	int a1=t1->arraysize,a2=t2->arraysize;
	
	if(depth==0){
		if(s1==s2 && s1!="pointer" && s1!="array"){
			//s1 and s2 both are either int/float/struct. Assuming they are not void, which
			// would have handled while parsing construction/declaration
			return true;
		}
		else if(s1==s2 && (s1=="pointer" || s1=="array") )
			return typecompare4(t1->typer,t2->typer,depth+1);
		// equal cases have been handled

		else if((s1=="float" && s2=="int") || (s1=="int" && s2=="float"))
			return true;
		else if( (s1=="pointer" && s2=="array") || (s1=="array" && s2=="pointer"))
			return typecompare4(t1->typer,t2->typer,depth+1);
		else
			return false;
	}
	else{
		if(depth==1 && (s1=="void" || s2=="void")){	// came here means original is void* only
			return true;
		}
		//depth>0
		//here onwards, s1==s2
		if(s1==s2 && s1!="pointer" && s1!="array")
			return true;
		else if(s1=="array" && s2=="array"){
			if(a1!=a2)		// both are arrays after depth 0, then the arraysizes should match
				return false;
			return typecompare4(t1->typer,t2->typer,depth+1);
		}
		else if(s1=="pointer" && s2=="pointer")
			return typecompare4(t1->typer,t2->typer,depth+1);
		else
			return false;
	}
	return false;
}


/////////////////////////////////////


//returns true if t1 is compatible with t2 

//returns pointer to type manufactured from the other two types.
type* typemanufacture(type* t1, type* t2, string instruction)
{
	
	if(instruction=="=")
	{
		if(t1->glotype=="pointer"&&t2->glotype=="array")t1->typer=t2->typer;
		return t1;
	}
	else if(instruction=="||"||instruction=="&&")
	{
		type* ans = new basenode("int");
		return ans;
	}
	else if(instruction=="=="||instruction=="!=")
	{type* ans = new basenode("int");
		return ans;
	}
	else if(instruction=="<="||instruction==">="||instruction=="<"||instruction==">")
	{type* ans = new basenode("int");
		return ans;
	}
	else if(instruction=="+"||instruction=="-")
	{
	if(t1->glotype=="float"||t2->glotype=="float"){type* ans = new basenode("float");return ans;}	
	if(t1->glotype=="array"){return t1;}	
	if(t2->glotype=="array"){return t2;}	
	if(t1->glotype=="pointer"){return t1;}	
	if(t2->glotype=="pointer"){return t2;}	
	type* ans = new basenode("int");
		return ans;
	}
	else if(instruction=="*"||instruction=="/")
	{
		if(t1->glotype=="float"||t2->glotype=="float"){type* ans=new basenode("float");return ans;}
		type* ans = new basenode("int"); return ans;
	}
	else if(instruction=="arrayref")
	{
		
		return t1->typer;
	}
	
	type* ans = new basenode("int"); return ans;
	return ans;
}
type* typemanufacture2(type* t1, string name,  string instruction)
{
	
	type* ans = new basenode("int");
	vector<localsymtab> vec;
	string x=t1->glotype,y=name;
	if(instruction=="++")return t1;
	else if(instruction==".")
	{
		for(int i=0;i<gs.size();i++)
		{
			if(gs[i].name==t1->glotype)
			{
				if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				cout<<"error: Trying to access variable "<<name<<" of struct which does not have a symboltable"<<endl;
				exit(0);
			}
				break;
				}
	}
	for(int i=0;i<vec.size();i++)
	{
		if(vec[i].name==y){return vec[i].typer;}
		
	}
	return 0;
}	
else if(instruction=="->")
{
	
	x=t1->typer->glotype;y=name;
	for(int i=0;i<gs.size();i++)
		{
			if(gs[i].name==x)
			{
				if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				cout<<"error: Trying dereference in struct "<<x<<" which does not have a symboltable"<<endl;
				exit(0);
			}
				break;
				}
	}
	for(int i=0;i<vec.size();i++)
	{
		if(vec[i].name==y){return vec[i].typer;}
		
	}
}
else if(instruction=="*"||instruction=="STAR")
{
	
	//do womething for dereferencing void
	if(t1->typer->glotype=="void"){cout<<"error: Cannot dereferece void"<<endl; exit(0);}
	
	return t1->typer;
}
else if(instruction=="AMP")
{
	type *ans= new pointernode(t1);
	return ans;
}
else if(instruction=="uminus")
{
	return t1;
}
else if(instruction=="NOT")
{
	type *ans= new basenode("int");
	return ans;
	
}
	return 0;
}



perform member(type* t1, string name, string instruction)
{
	perform result,v1,v2;
	vector<localsymtab> vec;
	string x=t1->glotype,y=name;
	if(instruction==".")
	{
		
		if(x=="int"||x=="array"||x=="pointer"||x=="float"||x=="void"){result.compatible=false; return result;}

		for(int i=0;i<gs.size();i++)if(gs[i].name==x)
		{
			if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				cout<<"error: variable "<<name<<" not a valid data member type"<<endl;
				exit(0);
			}
			break;
			}
		for(int i=0;i<vec.size();i++)   //change in localsymtab size could mess this up
		{
			if(vec[i].name==y){result.compatible=true; return result;}
		}
		result.compatible=false; return result;
	}
	else if(instruction=="->")
	{

		if(t1->typer==0){result.compatible=false; return result;}
		x=t1->typer->glotype;
		
		if(x=="int"||x=="array"||x=="pointer"||x=="float"||x=="void"){result.compatible=false; return result;}
		for(int i=0;i<gs.size();i++)if(gs[i].name==x)
		{
			if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				cout<<"error: Trying dereference "<<x<<" struct which does not have a symboltable"<<endl;
				exit(0);
			}
			break;
			}
		for(int i=0;i<vec.size();i++)   //change in localsymtab size could mess this up
		{
			if(vec[i].name==y){result.compatible=true; return result;}
		}
		result.compatible=false; return result;
	}
	result.compatible=false;
	return result;
}

bool compvoid(type* temp)
{
	if(temp->glotype=="array")return compvoid(temp->typer);
	if(temp->glotype=="void")return true;
	return false;
	
}



bool compareargs(string name, int n)
{
	vector<localsymtab> vec;
	type* tem1,*tem2;
	for(int i=0;i<gs.size();i++)
	{
		if(gs[i].name==name&&gs[i].globalrole=="function")
		{
			if(gst_vec.size()>i)
			{vec=gst_vec[i];}
			else
			{
				vec=gst;
				
			}
		}
	}
	for(int i=0;i<args.size();i++)
	{
		if(i>=vec.size())
		{
			cout<<"Line :"<<n<<" error: too many arguments provided to the function '"<<name<<"', provided "<<args.size()<<endl;
			exit(0);
		}
	if(vec[i].role!="param")
		{
			cout<<"Line :"<<n<<" error: too many arguments provided to the function '"<<name<<"', provided "<<args.size()<<endl;
			exit(0);
		}
	tem1=vec[i].typer;
	tem2=args[i];
	if(!typecompare4(tem1,tem2,0))
	{
		cout<<"Line :"<<n<<" error: argument "<<i+1<<" in function '"<<name<<"' does not match"<<endl;
		exit(0);
	}
	}
	if(vec.size()>args.size()&&vec[args.size()].role=="param")
	{
		cout<<"Line :"<<n<<" error: too few arguments to function '"<<name<<"', provided "<<args.size()<<endl;
		exit(0);
	}
	return true;
}

bool samestruct(type* temp)
{
	if(temp->glotype==gs[gs.size()-1].name){return true;}
	if(temp->glotype=="array"){return samestruct(temp->typer);}
	return false;
}



/////////////////////////////////////////////////////////////////

bool typecompare5(type* t1, type* t2,int depth){
	//t1 is rettype and t2 is the passed type
	string s1=t1->glotype,s2=t2->glotype;
	if(depth==0){
		if(s1==s2 && s1!="pointer")
			return true;
		else if(s1==s2 && s1=="pointer")
			return typecompare5(t1->typer, t2->typer, depth+1);
		else if(s1=="pointer" && s2=="array")
			return typecompare5(t1->typer, t2->typer, depth+1);
		else if((s1=="float" && s2=="int") || (s1=="int" && s2=="float"))
			return true;
		return false;
	}
	else{
		if(depth==1 && (s1=="void" || s2=="void")){	// came here means original is void* only
			return true;
		}
		else if(s1==s2 && s1!="pointer")
			return true;
		else if(s1==s2 && s1=="pointer")
			return typecompare5(t1->typer,t2->typer,depth+1);
		return false;
	}
	return false;
}


bool funasvar(string s)
{
	for(int i=0;i<gst.size();i++)
	{
		if(gst[i].name==s)return true;
	}
	return false;
}
























/*
 * The code for code generation
 * 
 * */



string c1,c2,c3,c4,c5,c6;






//~ vector<string> storeregsmain(){	

	//~ vector<string> ans;
	//~ int decls = getdeclsize();
	//~ int params = getparamsize();
	//~ apb("addi $sp, $sp, -4");
	//~ for (int i = 0; i < params; i+=4)
	//~ {
		//~ apb("lw $t0, "+to_string(i+4)+"($sp)");
		//~ apb("sw $t0, "+to_string(i)+"($sp)");
	//~ }
	//~ apb("sw $fp, "+to_string(params)+"($sp)");
	//~ apb("addi $fp, $sp, "+to_string(params));

	//~ return ans;
//~ }

//~ vector<string> popregsmain(){

	//~ vector<string> ans;
	//~ int decls = getdeclsize();
	//~ int params = getparamsize();
	//~ apb("lw $fp, "+to_string(4*params)+"($sp)");
	//~ for (int i = params-4; i >=0; i-=4)
	//~ {
		//~ apb("lw $t0, "+to_string(i)+"($sp)");
		//~ apb("sw $t0, "+to_string(i+4)+"($sp)");
	//~ }
	//~ apb("addi $sp, $sp, 4");

	//~ return ans;
//~ }





//~ void storeregs()
//~ {
	//~ funcode.push_back("addi $sp, $sp, -36");
	//~ funcode.push_back("sw $ra, 32($sp)");
	//~ funcode.push_back("sw $s0, 28($sp)");
	//~ funcode.push_back("sw $s1, 24($sp)");
	//~ funcode.push_back("sw $s2, 20($sp)");
	//~ funcode.push_back("sw $s3, 16($sp)");
	//~ funcode.push_back("s.s $f0, 12($sp)");
	//~ funcode.push_back("s.s $f1, 8($sp)");
	//~ funcode.push_back("s.s $f2, 4($sp)");
	//~ funcode.push_back("s.s $f3, 0($sp)");
	//~ return;
//~ }
//~ void popregs(){
	//~ funcode.push_back("l.s $f3, 0($sp)");
	//~ funcode.push_back("l.s $f2, 4($sp)");
	//~ funcode.push_back("l.s $f1, 8($sp)");
	//~ funcode.push_back("l.s $f0, 12($sp)");
	//~ funcode.push_back("lw $s3, 16($sp)");
	//~ funcode.push_back("lw $s2, 20($sp)");
	//~ funcode.push_back("lw $s1, 24($sp)");
	//~ funcode.push_back("lw $s0, 28($sp)");
	//~ funcode.push_back("lw $ra, 32($sp)");
	//~ funcode.push_back("addi $sp, $sp, 36");
	//~ funcode.push_back("addi $sp,$fp,4");
	//~ funcode.push_back("lw $fp,0($sp)");
	//~ funcode.push_back("addi $sp,$sp,8");
	//~ funcode.push_back("lw $ra,-4($sp)");
	//~ funcode.push_back("jr $ra");
	//~ return;
//~ }




void offsetadjuster()
{
	int base,curroffset=0;
	if(gst.size()>0){base=0;}
	
	for(int i=0;i<gst.size();i++)
	{
		if(gst[i].role=="var")
		{
			gst[i].offset=curroffset+8-base;
			curroffset+=gst[i].typer->typesize;
		}
		else {
			gst[i].offset=curroffset+4-base;
			if(gst[i].typer->glotype=="array")curroffset+=4;else curroffset+=gst[i].typer->typesize;
			}
		//cout<<gst[i].offset<<"   omygod"<<endl;
	}	
}

void offsetadjusterforstruct()
{
	int base;
	if(gst.size()>0)base=gst[0].offset;
	
	for(int i=0;i<gst.size();i++)
	{
		if(gst[i].role=="var")
		{
			gst[i].offset=gst[i].offset-base;
		}
		//cout<<gst[i].offset<<"   omygod"<<endl;
	}	
}




