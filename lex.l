%%

((\/\*(.|\n)*?(\*\/))|(\/\/(.)*))  
'\t'
' '	
'\n'							{Parser::linenum = Parser::linenum + 1;}
(([0][xX][0-9a-fA-F]+))|([0][bB][01]+)|(([1-9][0-9]*)|([0][0-7]*))      {Parser::d_val__ = std::string (matched());  return Parser::INT_CONSTANT;}
((([0-9]*[.][0-9]+))|([0-9]+[.][0-9]*))(([eE]([-+]?)[0-9]+)?)     {Parser::d_val__ = std::string (matched());  return Parser::FLOAT_CONSTANT;}
"int"							return Parser::INT;
"float"							return Parser::FLOAT;
"void"							return Parser::VOID;
"struct"						return Parser::STRUCT;
"->"							return Parser::PTR_OP;
"("								return '(';
")"								return ')';
","								return ',';
"["								return '[';
"]"								return ']';
"{"								return '{';
"&"								return '&';
"}"								return '}';
";"								return ';';
"return"						return Parser::RETURN;
"="								return '=';
"<"								return '<';
">"								return '>';
"+"								return '+';
"-"								return '-';
"*"								return '*';
"/"								return '/';
"."								return '.';
"!"								return '!';
"while"							return Parser::WHILE;
"for"							return Parser::FOR;
"if"							return Parser::IF;
"else"							return Parser::ELSE;
"&&"							return Parser::AND_OP;
"||"							return Parser::OR_OP;
"!="							return Parser::NE_OP;
"++"							return Parser::INC_OP;
"=<"							return Parser::LE_OP;
">="							return Parser::GE_OP;
"=="							return Parser::EQ_OP;
[[:alpha:]_][[:alpha:][:digit:]_]*			{Parser::d_val__ = std::string (matched());  return Parser::IDENTIFIER;}
\"(\\.|[^\\"])*\"				{Parser::d_val__ = std::string (matched());  return Parser::STRING_LITERAL;}
