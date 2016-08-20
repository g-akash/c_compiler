%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token INT FLOAT VOID
%token RETURN PTR_OP
%token WHILE FOR IF ELSE AND_OP OR_OP NE_OP 
%token INC_OP LE_OP GE_OP EQ_OP IDENTIFIER STRING_LITERAL INT_CONSTANT FLOAT_CONSTANT 
%token STRUCT 

%polymorphic Int : int; Float : float; String : std::string; abs: abstract_astnode*; decl1: declnode*;  

%type <String> STRING_LITERAL INT_CONSTANT FLOAT_CONSTANT unary_operator IDENTIFIER 
%type <abs> translation_unit function_definition constant_expression compound_statement statement_list statement assignment_statement expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression postfix_expression primary_expression l_expression expression_list selection_statement iteration_statement unary_expression logical_or_expression
%type <decl1> declarator fun_declarator
%type <Int> type_specifier
%%

translation_unit 
        :  struct_specifier
	 	| function_definition
	 	{
	 	int count=0;
	 	for(int i=0;i<gs.size();i++)if(gs[i].name=="main"&&gs[i].globalrole=="function")count++;
	 	if(count>1){cout<<"Line "<<linenum<<" error: More than one 'main' functions"<<endl; exit(0);}
	 	
	 	} 
	 	| translation_unit function_definition 
	 	{
	 	int count=0;
	 	for(int i=0;i<gs.size();i++)if(gs[i].name=="main"&&gs[i].globalrole=="function")count++;
	 	if(count>1){cout<<"More than one functions with name main"<<endl; exit(0);}
	 	
	 	} 
        | translation_unit struct_specifier
        {
	 	int count=0;
	 	for(int i=0;i<gs.size();i++)if(gs[i].name=="main"&&gs[i].globalrole=="function")count++;
	 	if(count>1){cout<<"More than one functions with name main"<<endl; exit(0);}
	 	} 
        ;

struct_specifier 
        : STRUCT IDENTIFIER 
        
        '{' 
        {
        
			st1=$2;
	        globalsymtab gsec(st1,"struct","",0,0);
	        st1=$2;
	        if(structpresent(st1))
	        {
	        cout<<"Line "<<linenum<<" error: struct "<<$2<<"has already been declared"<<endl;
	        exit(0);
	        }
	        
	        gs.push_back(gsec);
	        
        } 
        declaration_list '}' ';'
        {
			offsetadjusterforstruct();
			gst_vec.push_back(gst);
			gs[gs.size()-1].symtab=&gst_vec[gst_vec.size()-1];
			gst.clear();
			offset-=getstructsize(st1);
			//symprint();
			
		}
        ;

function_definition
		: type_specifier  fun_declarator 
		{
		
		st1=$2->name;
		//cout<<st1<<" "<<returnsize<<endl;
		temp1=$2->typer;
		//cout<<st1<<" st1 "<<temp1->typesize<<endl;
			globalsymtab gsec(st1,"function","",temp1,0);
			st1=$2->name;
			for(int i=0;i<gs.size();i++)
			{
			if(gs[i].name==st1&&gs[i].globalrole=="function")
			{
			cout<<"Line "<<linenum<<" error: Function '"<<st1<<"' already declared"<<endl;
			exit(0);
			}
			}
			gs.push_back(gsec);
			
		}
		compound_statement
		{
		
			
			
			$$=$4;
			
			vector<string> ans,mainstore;
			string neext=newlabel();
			ans.push_back(gs[gs.size()-1].name+":");
			vector<string> a2;
			mainstore=storeregsmain();
			a2=$$->codegenerator(1,"","",neext,2);
			if(gs[gs.size()-1].name=="main")ans.insert(ans.end(),mainstore.begin(),mainstore.end());
			ans.insert(ans.end(),codestore.begin(),codestore.end());
			ans.insert(ans.end(),a2.begin(),a2.end());
			ans.push_back(neext+":");
			ans.insert(ans.end(),codepop.begin(),codepop.end());
			ans.push_back("jr $ra");
			//for(int i=0;i<ans.size();i++)cout<<ans[i]<<endl;
			final_code.insert(final_code.end(),ans.begin(),ans.end());
			//gst_vec.push_back(gst);
			//cout<<gst_vec.size()<<endl;
			gs[gs.size()-1].symtab=&gst_vec[gst_vec.size()-1];
			
			
			
			gst.clear();
			//cout<<"The ast of the function "<<gs[gs.size()-1].name<<" is:"<<endl;
			//$$->print(0);
			std::cout<<"\n";
			//symprint();			
		}
		;

type_specifier                   // This is the information 
        : VOID 
        {
	        glotype="void";
	        $$=4;
	        returnsize=4;
        }	                 // that gets associated with each identifier
        | INT
        {
        
	        glotype="int";
	        $$=4;
	        returnsize=4;
        }
		| FLOAT 
		{
			glotype="float";
			$$=4;
			returnsize=4;
		}
        | STRUCT IDENTIFIER 
        {
        
        st1=$2;
	        if(!structpresent(st1))
	        {
	        cout<<"Line "<<linenum<<" error: struct "<<$2<<" has not been declared"<<endl;
	        exit(0);
	        }
	        st1=$2;
	        glotype=st1;
	        
	        $$=getstructsize(st1);
	        returnsize=getstructsize(st1);
	      
        }
        ;

fun_declarator
		: IDENTIFIER  '('{glotype1=glotype;size1=returnsize;} parameter_list ')' 
		{
		st1=$1;
			type* ident=new basenode(glotype1);
			ident->typesize=size1;
			temp1=ident;
			$$=new declnode(st1,temp1);
			
		}
		| IDENTIFIER  '(' ')'
		{
		st1=$1;
			type* ident=new basenode(glotype);
			ident->typesize=returnsize;
			$$=new declnode(st1,ident);
			
		} 
        | '*' fun_declarator  //The * is associated with the 
		{
		temp1 = $2->typer;
		st1=$2->name;
			$$=new declnode();
			$$->name=st1; 
			
	       
	        $$->typer = new pointernode(temp1);
	        returnsize=4;
        }
		;                      //function name


parameter_list
		: parameter_declaration 
		| parameter_list ',' parameter_declaration 
		;

parameter_declaration
		: type_specifier 
		{
			size=$1;
			glosize=size;
		}
		declarator 
		{
			st1=$3->name;temp1=$3->typer;i1=$3->typer->typesize;
			localsymtab ce(st1,offset,temp1,"param",i1);
			
			if(alreadydeclared(st1))
			{
				cout<<"Line "<<linenum<<"error: Already declared the variable "<<st1<<endl;
				exit(0);
			}
			gst.push_back(ce);offset+=size;
			
			if(compvoid(temp1))
			{
			cout<<"Line "<<linenum<<" error: cannot have void as a parameter in "<<st1<<endl;
			exit(0);
			}
			
		}
		;

declarator
		: IDENTIFIER
		{
			type* ident=new basenode(glotype);
			$$ = new declnode($1,ident);
			
		}
		| declarator '[' primary_expression']' // check separately that it is a constant
		{
			if(!is_int_constant){
				cout<<"Line :"<<linenum<<" array size is not an integer constant"<<endl;
				exit(0);
			}
			is_int_constant = 0;
			size*=$3->int_val;
			i1=$3->int_val;temp1=$1->typer;st1=$1->name;
			type* ype=manufacturenode(i1,temp1,"array");
			temp2 = ype;
			$$=new declnode(st1,temp2);
		}
	    | '*' declarator
	    {
	    temp1=$2->typer; 
		    $$=new declnode();
		    $$->name=$2->name;
		    
		    $$->typer=manufacturenode(0,temp1,"pointer");
		    
	    }
	    ;

primary_expression 			//type manufactured for all the rules here.
        : IDENTIFIER             ///////////////////change here
		{
			st1=$1;
			$$=new identifier_node(st1);
			$$->lval=1;
			
        	
	        if(!alreadydeclared(st1)){
	        	cout<<"Line :"<<linenum<<" The variable "<<st1<<" has not been declared but used."<<endl;
	        	exit(0);
        	
			}
		}
		| INT_CONSTANT
        {
	        $$ = new intconst_node($1);
	        is_int_constant=1;   
	        $$->typer=new basenode("int");
	        $$->lval=0;
        }
        | FLOAT_CONSTANT
        {
			$$=new floatconst_node($1);
			$$->typer=new basenode("float");
			$$->lval=0;
		}
        | STRING_LITERAL
        {
			st1=$1;
			st2=newstringlabel();
			strings.push_back(st2+": .asciiz "+st1);
			stringindex=strings.size()-1;
    	    $$ = new stringconst_node($1);
        }
        | '(' expression ')' 
		{
		temp1=$2->typer;
			$$=new primaryexp_node($2);
			$$->typer = temp1;
			$$->lval=$2->lval;
		}
        ;

compound_statement
		: '{' '}' 
		{
			offsetadjuster();
			codestore=storeregs();
			codepop=popregs();
			$$ =new empty_node();
		}
		| '{'
		{
		gst_vec.push_back(gst);
		offsetadjuster();
		codestore=storeregs();
		codepop=popregs();
		} statement_list '}'
		{
		a2=$3;
			$$ = new stmtlist_node(a2,0);
			
		} 
        | '{' declaration_list {gst_vec.push_back(gst);offsetadjuster();codestore=storeregs();codepop=popregs();}statement_list '}'
        {
        a2=$4;
	        $$=new stmtlist_node(a2,0);
	        
        } 
		;

statement_list
		: statement	
		{
			$$=$1;
			
		}	
        | statement_list statement
        {
	        $$=new stmtlist_node($1,$2); 
	        
        }	
		;

statement
        : '{' statement_list '}'  
        {
	        $$=new stmtlist_node($2,0);
        }
        | selection_statement 
        {
	        $$=$1;
        }	
        | iteration_statement
        {
	        $$=$1;
        }
		| assignment_statement	
		{
		
			$$=$1;
			
		}
        | RETURN {hasand=0;} expression ';'	
        {
        
			a2=$3;
			temp1=gs[gs.size()-1].rettype;
	        temp2=$3->typer;
	        $$ = new return_node(a2);
	        
	        
	        if(!typecompare5(temp1,temp2,0))
	        {
				cout<<"Line "<<linenum<<" error: Return type in function '"<<gs[gs.size()-1].name<<"' does not match"<<endl;
				exit(0);
	        }
	        if(hasand>0)
	        {
	        cout<<"Line "<<linenum<<" error: Returning pointer to locally declared variable"<<endl;
	        exit(0);
	        }
	        
	     // $$->code=combiner(a1->code,a2->code,"RETURN");  
        }
        ;

assignment_statement
		: ';' 
		{
		
			$$ = new empty_node();
			
		}								
		|  expression ';'  
		{
		temp1=$1->typer;
			$$ = $1;
			
			$$->typer = temp1;
		}
		;

expression             //assignment expressions are right associative
        :  logical_or_expression             ////////////////////change here
        {
        temp1=$1->typer;
	        $$=$1;
	        $$->typer=temp1;
				
        }
        |  unary_expression '=' expression
        {
        
	        temp1=$1->typer; temp2 = $3->typer;
	        llval=$1->lval;
	        a1=$1;a3=$3;
	        if(!typecompare(temp1,temp2,"=").compatible){
	        	cout<<"Line "<<linenum<<" error: Types are not compatible in '='"<<endl;
	        	exit(0);
	        }
	        
	        
	        $$ = new ass_node(a1,a3);
	        
	        $$->typer=typemanufacture(temp1,temp2,"=");
	        
	        
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	      
	      
	      if(llval==0)
	      {
	      cout<<"Line "<<linenum<<" error: lval required as left hand side of operand"<<endl;
	      exit(0);
	      }  
	      $$->lval=0; 
	      //$$->code=combiner(a1->code,a3->code,"="); 
        } 
        ;

logical_or_expression            // The usual hierarchy that starts here...
		: logical_and_expression
		{
			$$=$1;
			
		}
        | logical_or_expression OR_OP logical_and_expression
        {
        temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
	        $$ = new op2_node(a1,a3,"OR");
	        if(!typecompare(temp1,temp2,"||").compatible){
	        	cout<<"Line :"<<linenum<<"error: Types are not compatible in '||'"<<endl;
	        	exit(0);
	        }
	        
	        $$->typer=typemanufacture(temp1,temp2,"||");
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	      $$->lval=0;  
	    // $$->code=combiner(a1->code,a3->code,"||");   
        }
		;

logical_and_expression
        : equality_expression
        {
	        $$=$1;
        }
        | logical_and_expression AND_OP equality_expression 
        {
			temp1=$1->typer; temp2 = $3->typer;
			a1=$1;a3=$3;
	        $$=new op2_node(a1,a3,"AND");
	        if(!typecompare(temp1,temp2,"&&").compatible){
	        	cout<<"Line :"<<linenum<<" Types are not compatible in '&&'"<<endl;
	        	exit(0);
        	}
	        $$->typer=typemanufacture(temp1,temp2,"&&"); 
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
	       // $$->code=combiner(a1->code,a3->code,"&&");
        }
		;

equality_expression
		: relational_expression 
		{
			$$=$1;
		}
        | equality_expression EQ_OP relational_expression 
        {
        temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
	        $$ = new op2_node(a1,a3,"EQ_OP");
	        if(!typecompare(temp1,temp2,"==").compatible){
		        cout<<"Line :"<<linenum<<" error: Types are not compatible in '=='"<<endl;
		        exit(0);
	        }
	        
	        $$->typer=typemanufacture(temp1,temp2,"==");
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
	       // $$->code=combiner(a1->code,a3->code,"==");
	        }	
		| equality_expression NE_OP relational_expression
		{
		temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
			$$ = new op2_node(a1,a3,"NE_OP");
			if(!typecompare(temp1,temp2,"!=").compatible){
				cout<<"Line :"<<linenum<<" error: Types are not compatible in '!='"<<endl;exit(0);
			}
			
	        $$->typer=typemanufacture(temp1,temp2,"!=");
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	     $$->lval=0;  
	     //$$->code=combiner(a1->code,a3->code,"!="); 
		}
		;

relational_expression
		: additive_expression
		{
			$$=$1;
		}
        | relational_expression '<' additive_expression 
        {
        temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
	        $$=new op2_node(a1,a3,"LT");
	        if(!typecompare(temp1,temp2,"<").compatible){
	        	cout<<"Line :"<<linenum<<" Types are not compatible in '<' operator"<<endl;
	        	exit(0);
	        }
	        
	        $$->typer=typemanufacture(temp1,temp2,"<");
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
	       // $$->code=combiner(a1->code,a3->code,"<");
        }
		| relational_expression '>' additive_expression 
		{
		temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
			$$=new op2_node(a1,a3,"GT");
			if(!typecompare(temp1,temp2,">").compatible){
				cout<<"Line :"<<linenum<<" Types are not compatible in '>'"<<endl;
				exit(0);
			}
	        $$->typer=typemanufacture(temp1,temp2,">");
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
	        //$$->code=combiner(a1->code,a3->code,">");
		}
		| relational_expression LE_OP additive_expression 
		{
		temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
			$$=new op2_node(a1,a3,"LE_OP");
			if(!typecompare(temp1,temp2,"<=").compatible){
				cout<<"Line :"<<linenum<<" Types are not compatible in '<='"<<endl;
				exit(0);
			}
	        $$->typer=typemanufacture(temp1,temp2,"<=");
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        
	        $$->lval=0;
	       // $$->code=combiner(a1->code,a3->code,"LE_OP");
		
		}
        | relational_expression GE_OP additive_expression 
        {
        temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
	        $$=new op2_node(a1,a3,"GE_OP");
	        if(!typecompare(temp1,temp2,">=").compatible){
	        	cout<<"Line :"<<linenum<<" Types are not compatible in '>='"<<endl;
	        	exit(0);
	        }
	        $$->typer=typemanufacture(temp1,temp2,">=");
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
        $$->lval=0;
        //$$->code=combiner(a1->code,a3->code,"GE_OP");
        }
		;

additive_expression 
		: multiplicative_expression
		{
			$$=$1;
			
		}
		| additive_expression '+' multiplicative_expression 
		{
			temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
			if(!typecompare(temp1,temp2,"+").compatible){
				cout<<"Line :"<<linenum<<" Types are not compatible in '+'"<<endl;
				exit(0);
			}
			
			$$=new op2_node(a1,a3,"PLUS");
	        $$->typer=typemanufacture(temp1,temp2,"+");
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
	        //$$->code=combiner(a1->code,a3->code,"+");
	        
		}
		| additive_expression '-' multiplicative_expression 
		{
		temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
			$$=new op2_node(a1,a3,"MINUS");
			if(!typecompare(temp1,temp2,"-").compatible){
				cout<<"Line :"<<linenum<<" Types are not compatible in '-'"<<endl;
				exit(0);
			}
			
	        $$->typer=typemanufacture(temp1,temp2,"-");
		
			
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
	        //$$->code=combiner(a1->code,a3->code,"-");
		}
		;

multiplicative_expression
		: unary_expression
		{
			$$=$1;
		}
		| multiplicative_expression '*' unary_expression 
		{
		temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
			$$ = new op2_node(a1,a3,"MULT");
			if(!typecompare(temp1,temp2,"*").compatible){
				cout<<"Line :"<<linenum<<" Types are not compatible in '*'"<<endl;
				exit(0);
			}
			
	        $$->typer=typemanufacture(temp1,temp2,"*");
	        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
		} 
		| multiplicative_expression '/' unary_expression 
		{
		temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
		$$ = new op2_node(a1,a3,"DIV");
		if(!typecompare(temp1,temp2,"/").compatible){
			cout<<"Line :"<<linenum<<" Types are not compatible in '/'"<<endl;
			exit(0);
		}
		
        $$->typer=typemanufacture(temp1,temp2,"/");
        
        
	        if(compvoid(temp1))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
		}
		;

unary_expression
		: postfix_expression
		{
		$$=$1;
		}
		| unary_operator unary_expression
		{
		 temp2 = $2->typer;
        llval=$2->lval;
		st1=$1;a2=$2;
		
		if(!typecompare3(temp2,st1).compatible){
			cout<<"Line :"<<linenum<<" Types are not compatible in "<<st1<<endl;
			exit(0);
		}
		
		$$=new op1_node(a2,st1);
		
        
        
        $$->typer=typemanufacture2(temp2,"",st1);
        if(st1=="AMP"&&llval!=1)
        {
			cout<<"Line :"<<linenum<<" error: lval required for '&' operator"<<endl;
			exit(0);
        }
        
	        
	        if(compvoid(temp2))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        
	       
	        
	        if(st1!="STAR"){$$->lval=0;}               //////////////////////I think we might need some change
	        else if(st1=="STAR"){$$->lval=1;}
	        
        
		}
	;

postfix_expression
		: primary_expression  
		{
		$$=$1;
		}				
        | IDENTIFIER '(' ')' 
        {
	    //$$ = new empty_node();
	    mainargs.push_back(args);
	    args.clear();
	    mainindexes.push_back(indexes);
	    indexes.clear();
	    st1=$1;
	    if(!funcpresent(st1)||!compareargs(st1,linenum))
	    {
	    cout<<"Line :"<<linenum<<" error: Function '"<<st1<<"' invoked but not defined"<<endl;
	    exit(0);
	    }
	    if(funasvar(st1))
	    {
	    cout<<"Line :"<<linenum<<" error: called object '"<<st1<<"' is not a function or function pointer"<<endl;
	    exit(0);
	    }
	    
	    $$ = new funcall_node(st1,0,args,indexes);
	    args=mainargs[mainargs.size()-1];mainargs.resize(mainargs.size()-1);
	    indexes=mainindexes[mainindexes.size()-1];mainindexes.resize(mainindexes.size()-1);
	    
	    
	    $$->lval=0;
	   
	    } 				
		| IDENTIFIER 
		'(' 
		{mainargs.push_back(args);mainindexes.push_back(indexes);args.clear();indexes.clear();insideparameterlist=1;}
		expression_list ')'
		{
		insideparameterlist=0;
		st1=$1;
		if(st1!="printf"){
		if(!funcpresent(st1)||!compareargs(st1,linenum))
	    {
	    cout<<"Line :"<<linenum<<" error: Function '"<<st1<<"' invoked but not defined"<<endl;
	    exit(0);
	    }
	    
	    if(funasvar(st1))
	    {
	    cout<<"Variable name collides with function"<<endl;
	    exit(0);
	    }
	    }
	    st1=$1;
	    a4=$4;
	    $$ = new funcall_node(st1,a4,args,indexes);
	    args=mainargs[mainargs.size()-1];mainargs.resize(mainargs.size()-1);
	    indexes=mainindexes[mainindexes.size()-1];mainindexes.resize(mainindexes.size()-1);
	    //cout<<" hohohohoohohohohoohoh "<<$$->typer->typesize<<" "<<st1<<endl;
	    $$->lval=0;
		}
        | postfix_expression '[' expression ']'
        {
        temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
        llval=$1->lval;
        $$ = new arrayref_node(a1,a3);
        if(temp1->glotype!="array"&&temp1->glotype!="pointer")
        {cout<<"Line :"<<linenum<<" error: Types are not compatible in array reference"<<endl;
	        exit(0);
        }
        
        $$->typer=typemanufacture(temp1,temp2,"arrayref");
        temp4=$$->typer;
        
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	      
	      $$->lval=llval;
	      if(temp2->glotype!="int")
	      {
	      cout<<"Line "<<linenum<<" array subscript is not an 'integer const'"<<endl;
	      exit(0);
	      }  
        
        }
        | postfix_expression '.' IDENTIFIER
        {
        temp1=$1->typer; 
        a1=$1;st3=$3;
        llval=$1->lval;
        $$=new member_node(a1,st3);
        a1=$1;st3=$3;
        if(!typecompare2(temp1,st3,".").compatible){
	        cout<<"Line :"<<linenum<<" variable "<<st3<<" is not a data member of postfix expression"<<endl;
	        exit(0);
        }
        
       
        $$->typer=typemanufacture2(temp1,st3,".");
        
        temp4 = $$->typer;
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        
	        $$->lval=llval;
        }
        | postfix_expression PTR_OP IDENTIFIER 
        {
        temp1=$1->typer; 
        a1=$1;st3=$3;
        llval=$1->lval;
        
        $$=new ref_node(a1,st3);
        
        if(!typecompare2(temp1,st3,"->").compatible){
        	cout<<"Line :"<<linenum<<" Types are not compatible in '->'"<<endl;
        	exit(0);
        }
         
        
        $$->typer=typemanufacture2(temp1,st3,"->");
        temp4 = $$->typer;
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
        
        $$->lval=llval;
        }
		| postfix_expression INC_OP 	// ... and ends here
		{
		llval=$1->lval;
		if(llval==0)
		{
		cout<<"Line "<<linenum<<" error: Does not have lval for incrementing"<<endl;
		exit(0);
		}
		a1=$1;
		temp1=$1->typer;
		$$=new op1_node(a1,"PP");
		if(!typecompare3(temp1,"++").compatible){
			cout<<"Line :"<<linenum<<" Types are not compatible in '++'"<<endl;
			exit(0);
		}
		
		
        $$->typer=typemanufacture2(temp1,"","++");
        temp4 = $$->typer;
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
	        $$->lval=0;
		}
		;

l_expression                // A separate non-terminal for l_expressions   /////////////////change needed here
        : IDENTIFIER
        {
        st1=$1;
	        if(!alreadydeclared(st1)){
	        	cout<<"Line :"<<linenum<<" The variable "<<st1<<" has not been declared but used."<<endl;
	        	exit(0);
        }
        
        
        $$= new identifier_node(st1);
               
        }
        | l_expression '[' expression ']'  
        {
        temp1=$1->typer; temp2 = $3->typer;
        a1=$1;a3=$3;
	        $$ = new arrayref_node(a1,a3);
	        if(temp1->glotype!="array"&&temp1->glotype!="pointer")
	        {
	        cout<<"Line :"<<linenum<<" Types are not compatible in array reference"<<endl;
	        exit(0);
	        }
	        
	        $$->typer=typemanufacture(temp1,0,"arrayref");
	        temp4=$$->typer;
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
        }	
        | '*' l_expression
        {
        a2=$2;
        temp2=$2->typer;
    	    
    	    if(!typecompare3(temp2,"*").compatible){
    	    	cout<<"Line :"<<linenum<<" Types are not compatible in dereferencing operator"<<endl;
    	    	exit(0);
    	    }
    	    $$=new deref_node(a2);
    	    
    	    $$->typer=typemanufacture2(temp2,"","*");
    	    temp4=$$->typer;
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }
        }
        | l_expression '.' IDENTIFIER
        {
        	
        	temp1=$1->typer;st3=$3;
        	a1=$1;
        	if(!member(temp1,st3,".").compatible){
        		cout<<"Line :"<<linenum<<" variable "<<st3<<" is not a valid member"<<endl;
        		exit(0);
        	}
        	
        	
        	$$=new member_node(a1,st3);
        	
        	$$->typer=typemanufacture2(temp1,st3,"."); 
        	temp4=$$->typer;
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        }   
	        }
        | l_expression PTR_OP IDENTIFIER
     	   {
     	   
        	temp1=$1->typer; st3=$3;
        	a1=$1; 
        	if(!member(temp1,st3,"->").compatible){
        		cout<<"Line :"<<linenum<<" Types are not compatible in '->' "<<endl;
        		exit(0);
        	}
        	
        	
        	
        	$$=new ref_node(a1,st3);
        	$$->typer=typemanufacture2(temp1,st3,"->"); 
        	
        	temp4 = $$->typer;
        	
	        if(compvoid(temp4))
	        {
	        cout<<"Line : "<<linenum<<" error: Trying to generate void"<<endl;
	        exit(0);
	        } 
	        
    	    }
        | '(' l_expression ')'
    	    {
        	$$=$2;
    	    }	
        ;

expression_list
        : expression
        {
			a1=$1;
	        $$=a1;
	        temp1=a1->typer;
	        //cout<<temp1->glotype<<endl;
	        args.push_back(temp1);
	        indexes.push_back(stringindex);
	        
        }
        | expression_list ',' expression
        {
        a1=$1;a3=$3;
        temp2=$3->typer;
        a1=$1;a3=$3;
	        $$ = new explist_node(a1,a3);
	        
	        
	        args.push_back(temp2);
	        indexes.push_back(stringindex);
        }
		;

unary_operator
        : '-'	
        {
        $$="uminus";
        }		
        | '!'
		{
		$$="NOT";
		}
        | '&'
		{
		$$="AMP";
		hasand++;
		}
        | '*' 	
		{
		$$="STAR";
		hasand--;
		}
		;

selection_statement
        : IF '(' expression ')' statement ELSE statement 
        {
	        $$=new if_node($3,$5,$7); 
        }
		;

iteration_statement
		: WHILE '(' expression ')' statement 
		{
			$$=new while_node($3,$5);
		}	
		| FOR '(' expression ';' expression ';' expression ')' statement  //modified this production
		{
			$$ = new for_node($3,$5,$7,$9);
		}
        ;

declaration_list
        : declaration  					
        | declaration_list declaration
		;

declaration
		: type_specifier
		{
		size=$1;
		glosize=size;
		
		
		}
		declarator_list';'
		;

declarator_list
		: declarator
			{
			
			st1=$1->name;temp1=$1->typer;
			i1=$1->typer->typesize;
			
			localsymtab ce(st1,offset,temp1,"var",i1);
			if(alreadydeclared(st1))
			{
			cout<<"Line :"<<linenum<<" error: Already declared the variable "<<st1<<endl;
			exit(0);
			}
			gst.push_back(ce);offset+=size;
			if(compvoid(temp1))
			{
			cout<<"Line "<<linenum<<" error: Cannot declare void variable "<<st1<<endl;
			exit(0);
			}
			if(gs[gs.size()-1].globalrole=="struct"&&samestruct(temp1))
			{
			cout<<"Line "<<linenum<<" error: Trying to declare member same as parent"<<endl;
			exit(0);
			}
			
			
			}
		| declarator_list ',' {size=glosize;} declarator 
			{
			st3=$4->name;temp2=$4->typer;
			i3=$4->typer->typesize;
			localsymtab ce(st3,offset,temp2,"var",i3);
			
			if(alreadydeclared(st3))
			{
			cout<<"Line :"<<linenum<<" error: Already declared the variable "<<st3<<endl;
			//exit(1);
			}

			gst.push_back(ce);offset+=size;
			
			if(compvoid(temp2))
			{
			cout<<"Line :"<<linenum<<" cannot declare void variable"<<endl;
			exit(0);
			}
			
			if(gs[gs.size()-1].globalrole=="struct"&&samestruct(temp2))
			{
			cout<<"Line "<<linenum<<" error: Trying to declare member same as parent"<<endl;
			exit(0);
			}
			
		
		}
	 	;
