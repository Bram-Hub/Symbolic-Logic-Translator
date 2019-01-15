//Ben Consolati

#include <string>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

unsigned int COUNT = 0;


//Convert given statement into standard form for rest of program to read and check for syntax errors
//Conversion:	Conjunction: '&'	Disjunction: 'v'	Conditional: '$'	Biconditional: '%'	Negation: '~'
//				Program also accepts negations of vars A,B,C,D as: a,b,c,d and negation of '(' as '['   (these symbols are used internally)
bool Fix(string &sent){
	unsigned int i = 0;
	int pcount = 0;
	unsigned int lastchar = 0;  //0:null  1:'('   2:'~'  3:functor  4:literal  5:')'
	char c;
	while(i < sent.size()){
		c=sent[i];
		if(c=='A' || c=='B' || c=='C' || c=='D' || c=='a' || c=='b' || c=='c' || c=='d' || c=='T' || c=='F'){	//literals
			if(lastchar > 3)
				return false;
			lastchar = 4;
			i++;
		}
		else if(c==' '){		//remove spaces
			sent.erase(i,1);
		}
		else if(c=='(' || c=='['){
			if(lastchar > 3)
				return false;
			lastchar = 1;
			pcount++;
			i++;
		}
		else if(c==')'){
			if(lastchar < 4)
				return false;
			pcount--;
			if (pcount<0)
				return false;
			i++;
		}
		else if(c=='&' || c=='^'){	//convert to '&'
			if(lastchar < 4)
				return false;
			lastchar = 3;
			sent[i]='&';
			i++;
		}
		else if(c=='/'){
			if(sent[i+1]!='\\')
				return false;
			if(lastchar < 4)
				return false;
			lastchar = 3;
			sent[i]='&';
			i++;
			sent.erase(i,1);
		}
		else if(c=='|' || c=='v' || c=='V'){	//convert to 'v'
			if(lastchar < 4)
				return false;
			lastchar = 3;
			sent[i]='v';
			i++;
		}
		else if(c=='\\'){
			if(sent[i+1]!='/')
				return false;
			if(lastchar < 4)
				return false;
			lastchar = 3;
			sent[i]='v';
			i++;
			sent.erase(i,1);
		}
		else if(c=='!' || c=='~'){ //convert to '~'
			if(lastchar > 3)
				return false;
			if(lastchar == 2){
				lastchar = 0;
				i--;
				sent.erase(i,2);				
			}
			else{
				lastchar = 2;
				sent[i]='~';
				i++;
			}
		}
		else if(c=='$'){		//convert to '$'
			if(lastchar < 4)
				return false;
			lastchar = 3;
			i++;
		}
		else if(c=='-'){
			if(sent[i+1]!='>')
				return false;
			if(lastchar < 4)
				return false;
			lastchar = 3;
			sent[i]='$';
			i++;
			sent.erase(i,1);
		}
		else if(c=='%'){		//convert to '%'
			if(lastchar <4)
				return false;
			lastchar = 3;
			i++;
		}
		else if(c=='<'){
			if(sent[i+1]!='-' || sent[i+2]!='>')
				return false;
			if(lastchar < 4)
				return false;
			lastchar = 3;
			sent[i]='%';
			i++;
			sent.erase(i,2);
		}
		else return false;
	}
	if(pcount!=0)		//check parenthises
		return false;
	unsigned int cv=0;
	for(i=0;i<sent.size();i++){		//insert parenthises if user didnt specify any for a statement with more than two literals
		if(sent[i] == '(' || sent[i] == ')')
			i=sent.size();
		else if(sent[i] == 'A'  || sent[i] == 'B' || sent[i] == 'C' || sent[i] == 'D' || sent[i] =='a' || sent[i] =='b' || sent[i] =='c' || sent[i] =='d' || sent[i] =='T' || sent[i] =='F')
			cv++;
		else if(sent[i] == '&' || sent[i] == 'v' || sent[i] == '$' || sent[i] == '%'){
			if (cv>1){
				sent.insert(i,")");
				sent.insert(0,"(");
				i=i+2;
			}
		}
	}
	//Return true for a syntactically correct statement
	return true;
}

//Recursive function used by the Truth Table calculator
bool RTT(bool A, bool B, bool C, bool D, string sent, unsigned int &i){
	//this function treats statements as a two-variable statement in which either of the variables could recursively be a two-variable statement
	// ...yeah, that didnt make sense
	//statement is of the form:           tv1              tf            tv2
	//                              (truth value 1) (truth functor) (truth value 2)
	bool tv1,tv2;
	int tf;
	
	//get value of tv1
	while(sent[i] == ')')
		i++;
	if(sent[i] == '('){
		i++;
		tv1 = RTT(A,B,C,D,sent,i);	//tv1 is a statement in parentheses, call RTT recursively
	}
	else if(sent[i] == '['){
		i++;
		tv1 = !RTT(A,B,C,D,sent,i);
	}
	else if(sent[i] == 'A'){
		tv1 = A;
		i++;
	}
	else if(sent[i] == 'B'){
		tv1 = B;
		i++;
	}
	else if(sent[i] == 'C'){
		tv1 = C;
		i++;
	}
	else if(sent[i] == 'D'){
		tv1 = D;
		i++;
	}
	else if(sent[i] == 'a'){
		tv1 = !A;
		i++;
	}
	else if(sent[i] == 'b'){
		tv1 = !B;
		i++;
	}
	else if(sent[i] == 'c'){
		tv1 = !C;
		i++;
	}
	else if(sent[i] == 'd'){
		tv1 = !D;
		i++;
	}
	else if(sent[i] == 'T'){
		tv1 = 1;
		i++;
	}
	else if(sent[i] == 'F'){
		tv1 = 0;
		i++;
	}
	else if(sent[i] == '~'){
		i++;
		if(sent[i] == '('){
			i++;
			tv1 = !RTT(A,B,C,D,sent,i);
		}
		else if(sent[i] == 'A'){
			tv1 = !A;
			i++;
		}
		else if(sent[i] == 'B'){
			tv1 = !B;
			i++;
		}
		else if(sent[i] == 'C'){
			tv1 = !C;
			i++;
		}
		else if(sent[i] == 'D'){
			tv1 = !D;
			i++;
		}
		else if(sent[i] == 'T'){
			tv1 = 0;
			i++;
		}
		else if(sent[i] == 'F'){
			tv1 = 1;
			i++;
		}
	}
	else {cout<<"Error #1";}
	if(i >= sent.size())	//return tv1 if statement is only one variable
		return tv1;
	while(sent[i] == ')')
		i++;
	if(i >= sent.size())
		return tv1;

	//get value of tf
	if(sent[i] == '&')
		tf=1;
	else if(sent[i] == 'v')
		tf=2;
	else if(sent[i] == '$')
		tf=3;
	else if(sent[i] == '%')
		tf=4;
	i++;

	//get value of tv2
	if(sent[i] == '('){
		i++;
		tv2 = RTT(A,B,C,D,sent,i);
	}
	else if(sent[i] == '['){
		i++;
		tv2 = !RTT(A,B,C,D,sent,i);
	}
	else if(sent[i] == 'A'){
		tv2 = A;
		i++;
	}
	else if(sent[i] == 'B'){
		tv2 = B;
		i++;
	}
	else if(sent[i] == 'C'){
		tv2 = C;
		i++;
	}
	else if(sent[i] == 'D'){
		tv2 = D;
		i++;
	}
	else if(sent[i] == 'a'){
		tv2 = !A;
		i++;
	}
	else if(sent[i] == 'b'){
		tv2 = !B;
		i++;
	}
	else if(sent[i] == 'c'){
		tv2 = !C;
		i++;
	}
	else if(sent[i] == 'd'){
		tv2 = !D;
		i++;
	}
	else if(sent[i] == 'T'){
		tv2 = 1;
		i++;
	}
	else if(sent[i] == 'F'){
		tv2 = 0;
		i++;
	}
	else if(sent[i] == '~'){
		i++;
		if(sent[i] == '('){
			i++;
			tv2 = !RTT(A,B,C,D,sent,i);
		}
		else if(sent[i] == 'A'){
			tv2 = !A;
			i++;
		}
		else if(sent[i] == 'B'){
			tv2 = !B;
			i++;
		}
		else if(sent[i] == 'C'){
			tv2 = !C;
			i++;
		}
		else if(sent[i] == 'D'){
			tv2 = !D;
			i++;
		}
		else if(sent[i] == 'T'){
			tv2 = 0;
			i++;
		}
		else if(sent[i] == 'F'){
			tv2 = 1;
			i++;
		}
	}
	else {cout<<"Error #2";}

	//calculate value of tv1 tf tv2
	switch (tf){
		case 1:
			return (tv1 & tv2);		//conjunction
		case 2:
			return (tv1 | tv2);		//disjunction
		case 3:
			return (!tv1 | tv2);	//conditional
		case 4:
			return ( (tv1 & tv2) | (!tv1 & !tv2) );	//biconditional
		default:
			cout<<"ERROR!";
	}
	return false;
}

//Calculate Truth Table of given statement
vector<bool> TT(string sent){
	vector<bool> tt;
	bool A=0,B=0,C=0,D=0;
	bool truthvalue;
	unsigned int i,j=0;
	for(i=0;i<16;i++){
		D = i&1;
		C = i&2;
		B = i&4;
		A = i&8;
		truthvalue = RTT(A,B,C,D,sent,j);
		tt.push_back(truthvalue);
		j=0;
	}
	return tt;
}

//Translate statement from program's form to a more readable form and print it
void PrintPrettyForm(string sent){
	unsigned int i;

	//Convert symbols
	for(i=0;i<sent.size();i++){
		if(sent[i]=='&' || sent[i]=='v' || sent[i]=='$' || sent[i]=='%'){
			sent.insert(i+1," ");
			sent.insert(i," ");
			i++;
		}
		if(sent[i]=='&')
			sent[i] = '^';
		if(sent[i]=='$'){
			sent[i] = '-';
			i++;
			sent.insert(i,">");
		}
		if(sent[i]=='%'){
			sent[i] = '<';
			i++;
			sent.insert(i,"-");
			i++;
			sent.insert(i,">");
		}
		if(sent[i]=='a' || sent[i]=='b' || sent[i]=='c' || sent[i]=='d'){
			sent.insert(i,"~");
			i++;
			sent[i] = sent[i]-32;
		}
		if(sent[i]=='['){
			sent.insert(i,"~");
			i++;
			sent[i] = '(';
		}
	}

	//Print
	cout<<sent<<"\n";
}

//Print Truth Table of given statement
void Print(vector<bool> tt, bool A, bool B, bool C, bool D, string sent){
	bool Av=0,Bv=0,Cv=0,Dv=0;
	int i,ii;
	unsigned int k;

	//title row
	cout<<endl;
	if(A)
		cout<<" A";
	if(B)
		cout<<" B";
	if(C)
		cout<<" C";
	if(D)
		cout<<" D";
	cout<<" | ";
	PrintPrettyForm(sent);
	cout<<"-------------\n";

	//super complicated mess of an algorithm to print TT of correct size with correct literals
	ii = (A+1)*(B+1)*(C+1)*(D+1) - 1;
	for(i=ii;i>=0;i--){
		k=1;
		if(D){
			Dv = i&k;
			k = k*2;
		}
		if(C){
			Cv = i&k;
			k = k*2;
		}
		if(B){
			Bv = i&k;
			k = k*2;
		}
		if(A){
			Av = i&k;
			k = k*2;
		}
		cout<<" ";
		if(A)
			cout<<Av<<" ";
		if(B)
			cout<<Bv<<" ";
		if(C)
			cout<<Cv<<" ";
		if(D)
			cout<<Dv<<" ";
		cout<<"|  "<<tt[(16/(ii+1))*(i+1)-1]<<" \n";
	}
	cout<<"-------------\n";
}



//Increment functor  ( & -> v -> $ -> % -> &.. )
bool FInc(char &func){
	if(func=='&')
	func='v';
	else if(func=='v')
		func='$';
	else if(func=='$')
		func='%';
	else{
		func='&';
		return true;	// return true for rollover (ie, % -> &)
	}
	return false;	//no rollover
}

//Increment literal  (A -> B -> C -> D -> a -> b -> c -> d -> T -> F -> A ->... )
bool VInc(char &var, bool NOT){
	if(var=='A' || var=='B' || var=='C')
		var++;
	else if(var=='D'){
		if(NOT)				// only go to 'a' if '~' is allowed
			var = 'a';
		else
			var = 'T';
	}
	else if(var=='a' || var=='b' || var=='c')
		var++;
	else if(var == 'd')
		var = 'T';
	else if(var == 'T')
		var = 'F';
	else{
		var = 'A';
		return true;	// return true for rollover (ie, when F -> A)
	}
	return false;
}

//Checks for equivalence of truth table of a given sentence (test) to a given truth table (tt) and prints it if so
void Test(string test,vector <bool> tt){
	vector <bool> tttest;

	tttest = TT(test);				//Find TT and compare to original TT
	if(tttest == tt){
		PrintPrettyForm(test);
		COUNT++;
	}
}

//Inserts a two literal wide pair of parentheses
void TwoVPPlacer(string &test, unsigned int pos, int negate){
	if(!negate){
		test.insert(pos,"(");
		test.insert(pos+4,")");
	}
	else{
		test.insert(pos,"[");
		test.insert(pos+4,")");
	}
}

//Inserts a three literal wide pair of parentheses
void ThreeVPPlacer(string &test, unsigned int pos, int negate){
	if(!negate){
		test.insert(pos,"(");
		test.insert(pos+6,")");
	}
	else{
		test.insert(pos,"[");
		test.insert(pos+6,")");
	}
}

//Negates entire statement (ie, puts ~( ... ) around statement)
void NegPPlacer (string &test){
	test.insert(0,"[");
	test.push_back(')');
}


//Places parenthis in every possible configuration, calling the Test function each config.
void PPlacer(string otest, unsigned int depth, vector <bool> tt, bool NOT){
	string test;
	vector <bool> tttest;
	unsigned int i,j,k;

	if(depth==1){			//parentheses for 1 literal  (none)
		Test(otest,tt);
	}
	else if(depth==2){		//parentheses for 2 literals (only negation of entire statement)
		Test(otest,tt);
		if(NOT){
			NegPPlacer(otest);
			Test(otest,tt);
		}
	}
	else if(depth==3){		//prentheses for 3 literals
		for(i=0;i<3;i=i+2){
			k=0;
			while( k==0 || (NOT && k<2)){

				test = otest;
				TwoVPPlacer(test,i,k & 1);
				Test(test,tt);
				k++;
			}
		}
	}
	else if(depth==4){		//parentheses for 4 literals
		k=0;
		while( k==0 || (NOT && k<4 )){
			test = otest;
			TwoVPPlacer(test,4,k & 2);		//location of parenthesis for (A * A) * (A * A)
			TwoVPPlacer(test,0,k & 1);
			Test(test,tt);
			k++;
		}
		for(i=0;i<3;i=i+2){
			for(j=i+1;j<i+4;j=j+2){
				k=0;
				while( k==0 || (NOT && k<4)){
					test = otest;
					ThreeVPPlacer(test,i,k & 2);
					TwoVPPlacer(test,j,k & 1);
					Test(test,tt);
					k++;
				}
			}
		}
	}

}


//Exhaustively build statements and test them for equivalence with given truth table
void Translator(vector <bool> tt,bool AND, bool OR, bool CON, bool BIC, bool NOT, bool A, bool B, bool C, bool D, bool T, bool F, unsigned int depth, unsigned int cdepth){
	string test;						// string that is iteratively modified and compared to orginal sentence
	vector <bool> tttest;				// truth table of test sentence
	vector <char> vars(cdepth,'A');		// variables of test
	vector <char> funcs(cdepth-1,'&');	// functors of test
	unsigned int i,j;
	int k;
	bool rollover = false, rollover2 = false, check = false;
	char temp;

	if(!A){						//inccrement literal to first allowed
		if(B)
			temp='B';
		else if(C)
			temp='C';
		else if(D)
			temp='D';
		else if(T)
			temp='T';
		else
			temp='F';
		for(i=0;i<vars.size();i++)
			vars[i]=temp;
	}

	if(!AND){					//increment functor to first allowed
		if(OR)
			temp='v';
		else if(CON)
			temp='$';
		else
			temp='%';
		for(i=0;i<funcs.size();i++)
			funcs[i]=temp;
	}

	while(!rollover){
		//for(i=0;i<cdepth;i++){
			for(j=0;j<cdepth;j++){			//Build test sentance
				test.push_back(vars[j]);
				if(j<cdepth-1)
					test.push_back(funcs[j]);
			}
			PPlacer(test,cdepth,tt,NOT);		//Place Parenthesis and test truth vals against given truth vals
			if(cdepth>1){						//Increment functor
				rollover = true;
				for(k=cdepth-2;k>-1;k--){
					if(rollover){
						rollover = FInc(funcs[k]);	// When the functors roll over = 1 when a functor "slot" cycles through all functors
						check = false;
						while(!check){	// Make sure functor is allowed (chosen by user), otherwise increment further
							if ((funcs[k]=='&' && !AND) || (funcs[k]=='v' && !OR) || (funcs[k]=='$' && !CON) || (funcs[k]=='%' && !BIC)){
								rollover2 = FInc(funcs[k]);
								rollover = rollover || rollover2;
							}
							else 
								check=true;
						}
					}
				}
			}
			else
				rollover = true;
			if(rollover){					//Increment variable
				for(k=cdepth-1;k>-1;k--){
					if(rollover){
						rollover = VInc(vars[k],NOT);
						check = false;
						while(!check){	// Make sure variable is allowed (ie, in the original given statement)
							if((vars[k]=='A' && !A) || (vars[k]=='B' && !B) || (vars[k]=='C' && !C) || (vars[k]=='D' && !D) || (vars[k]=='a' && !A) || (vars[k]=='b' && !B) || (vars[k]=='c' && !C) || (vars[k]=='d' && !D) || (vars[k]=='T' && !T) || (vars[k]=='F' && !F)){
								rollover2 = VInc(vars[k],NOT);
								rollover = rollover || rollover2;
							}
							else
								check=true;
						}
					}
				}
			}
		//}
		test.clear();
	}
	if(++cdepth<=depth)									// recursively call Translator for more variables
		Translator(tt,AND,OR,CON,BIC,NOT,A,B,C,D,T,F,depth,cdepth);
}

///////MAIN FUNCTION///////
int main(){
	time_t starttime,endtime;
	double elapsedtime;
	string sent;			//Statement to translate from
	vector <bool> tt;		//truth table of this statement
	string input;
	bool AND,OR,CON,BIC,NOT,A,B,C,D,T,F;	//functors to be used in translations
	unsigned int i,depth;		//number of literals to be in translations
	bool again = 1;

	while(again){
		AND=false,OR=false,CON=false,BIC=false,NOT=false,A=false,B=false,C=false,D=false,T=false,F=false;

		//Get statement to translate from (STTF)
		cout<<"Enter logical statement:";
		getline(cin, sent);					
		while(sent=="")
			getline(cin,sent);

		//Put it in a form readible by the rest of the program
		if(!Fix(sent)){
			cout<<"Syntax error!"<<endl;
			return 0;
		}

		// Determine what literals are in STTF
		for(i=0;i<sent.size();i++){	
			if(sent[i]=='A')
				A=true;
			else if(sent[i]=='B')
				B=true;
			else if(sent[i]=='C')
				C=true;
			else if(sent[i]=='D')
				D=true;
		}

		//Get truth table of STTF (STTF TT)
		tt = TT(sent);	

		//Print truth table
		Print(tt,A,B,C,D,sent);

		//Get functors to translate to
		cout<<"\nFunctors to translate to (Enter 'x' when done): ";	
		while (input != "x"){
			cin>>input;
			if(input=="&" || input=="^" || input=="/\\")
				AND=true;
			else if(input=="v" || input=="V" || input=="|" || input=="\\/")
				OR=true;
			else if(input=="$" || input=="->")
				CON=true;
			else if(input=="%" || input=="<->")
				BIC=true;
			else if(input=="~" || input=="!")
				NOT=true;
			else if(input=="T")
				T=true;
			else if(input=="F")
				F=true;
		}
		if(!(AND || OR || CON || BIC)){
			cout<<"You need at least one functor!"<<endl;
			return(0);
		}

		//Get number of literals
		cout<<"Max number of literals in output (less than 5):";
		cin>>depth;
		cout<<endl<<endl;

		//Get current time
		time(&starttime);	

		//Run translator
		Translator(tt,AND,OR,CON,BIC,NOT,A,B,C,D,T,F,depth,1);

		//Get time and calculate elapsed time
		time(&endtime);										
		elapsedtime = difftime(endtime,starttime);

		//Print count and elapsed time
		cout<<"\n"<<COUNT<<" translations found in "<<elapsedtime<<" seconds!\n";	

		COUNT=0;
		cout<<endl<<"Play again?";
		cin>>input;
		if(input!="y")
			again = 0;
	}
	return 0;
}