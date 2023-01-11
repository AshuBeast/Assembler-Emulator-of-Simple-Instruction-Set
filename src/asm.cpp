#include <bits/stdc++.h>
using namespace std;

map<string,pair<string,int>>mnemonics;
map<string,int>label_table;
map<int,string>errors;
vector<string>machineCode;

void Mnemonic_Opcode(){
  mnemonics.insert({"ldc",{"00",1}});
  mnemonics.insert({"adc",{"01",1}});
  mnemonics.insert({"ldl",{"02",2}});
  mnemonics.insert({"stl",{"03",2}});
  mnemonics.insert({"ldnl",{"04",2}});
  mnemonics.insert({"stnl",{"05",2}});
  mnemonics.insert({"add",{"06",0}});
  mnemonics.insert({"sub",{"07",0}});
  mnemonics.insert({"shl",{"08",0}});
  mnemonics.insert({"shr",{"09",0}});
  mnemonics.insert({"adj",{"0a",1}});
  mnemonics.insert({"a2sp",{"0b",0}});
  mnemonics.insert({"sp2a",{"0c",0}});
  mnemonics.insert({"call",{"0d",2}});
  mnemonics.insert({"return",{"0e",0}});
  mnemonics.insert({"brz",{"0f",2}});
  mnemonics.insert({"brlz",{"10",2}});
  mnemonics.insert({"br",{"11",2}});
  mnemonics.insert({"HALT",{"12",0}});
  mnemonics.insert({"data",{"13",1}});
  mnemonics.insert({"SET",{"14",1}});
}

string remove_spaces(string s){
  reverse(s.begin(),s.end());
  while(s.back()==' ' || s.back()=='\t') s.pop_back();
  reverse(s.begin(),s.end());
  while(s.back()==' ' || s.back()=='\t') s.pop_back();

  return s;
}

bool present(string s,char ch){
  for(int i=0;i<s.size();i++){
    if(s[i]==ch){
      return true;
    }
  }
  return false;
}

int getIndex(string s,char ch){
  for(int i=0;i<s.size();i++){
    if(s[i]==ch){
      return i;
    }
  }
  return -1;
}

bool duplicate(string s){
  for(auto i:label_table){
    if(i.first==s) return true;
  }
  return false;
}

bool validLabel(string s){
  for(int i=0;i<s.size();i++){
    if(!((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z') || (s[i]>='0' && s[i]<='9'))){
      return false;
    }
  }
  if((s[0]>='0' && s[0]<='9')){
    return false;
  }
  return true;
}

bool isDecimal(string s){
  if(s[0]=='+' || s[0]=='-'){
    s=s.substr(1);
  }
  for(int i=0;i<s.size();i++){
    if(s[i]<'0' || s[i]>'9') return false;
  }
  return true;
}

bool isOctal(string s){
  for(int i=1;i<s.size();i++){
    if(s[i]<'0' || s[i]>'7') return false;
  }
  if(s.size()>1 && s[0]=='0') return true;
  return false;
}

bool isHexa(string s){
  for(int i=2;i<s.size();i++){
    if(!((s[i]>='a' && s[i]<='f') || (s[i]>='A' && s[i]<='F') || (s[i]>='0' && s[i]<='9'))) return false;
  }
  if((s.size()>2) && (s[0]=='0' && s[1]=='x')) return true;
  return false;
}

int inDecimal(string s){
  if(s[0]=='0' && s[1]=='x'){
    return stoul(s,NULL,16);
  }
  else if(s[0]=='0'){
    return stoul(s,NULL,8);
  }
  else if(s[0]=='+'){
    return stoul(s.substr(1),NULL,10);
  }
  else if(s[0]=='-'){
    return -stoul(s.substr(1),NULL,10);
  }
  return stoul(s,NULL,10);
}

string hex_to_bin(string s){
  map<char,string>m;
  m['0']="0000",m['1']="0001",m['2']="0010",m['3']="0011",m['4']="0100",
  m['5']="0101",m['6']="0110",m['7']="0111",m['8']="1000",m['9']="1001",
  m['a']="1010",m['b']="1011",m['c']="1100",m['d']="1101",m['e']="1110",
  m['f']="1111";

  string binary="";
  for(int i=0;i<s.size();i++){
    binary+=m[s[i]];
  }

  return binary;
}

string toHex(int decimal){
  stringstream ss;
  ss<<setfill('0')<<setw(8)<<hex<<decimal;
  return ss.str();
}

void parsing_instruct_1st(string main_instruct,int line,int loc){
  int gap=getIndex(main_instruct,' ');

  string mnemo=main_instruct.substr(0,gap);
  mnemo=remove_spaces(mnemo);

  string operand=main_instruct.substr(gap+1);
  operand=remove_spaces(operand);

  if(mnemonics.find(mnemo)==mnemonics.end()){
    errors.insert({line,"Error at line: "+to_string(line)+" | Error type: Bogus mnemonic"});
  }

  else if(mnemonics[mnemo].second==0){
    if(mnemo.length()!=main_instruct.length()){ 
      errors.insert({line,"Error at line: "+to_string(line)+" | Error type: Unexpected operand"});  
    }
  }
  
  else{          
    if(mnemo.length()==main_instruct.length()){ 
      errors.insert({line,"Error at line: "+to_string(line)+" | Error type: Missing operand"});  
    }
    else if(present(operand,',')){
      errors.insert({line,"Error at line: "+to_string(line)+" | Error type: Extra operand"});
    }
  }
}

void parsing_instruct_2nd(string main_instruct,string instruct,int line,int loc){
  int gap=getIndex(main_instruct,' ');

  string mnemo=main_instruct.substr(0,gap);
  mnemo=remove_spaces(mnemo);

  string operand=main_instruct.substr(gap+1);
  operand=remove_spaces(operand);

  string encoding;

  if(mnemonics.find(mnemo)==mnemonics.end()){
    return;
  }

  if(mnemonics[mnemo].second==0){
    encoding=toHex(loc)+" 000000"+mnemonics[mnemo].first;
  }

  else if(mnemonics[mnemo].second==1){
    if(isDecimal(operand) || isOctal(operand) || isHexa(operand)){
      string temp=toHex(inDecimal(operand));
      encoding=toHex(loc)+" "+temp.substr(temp.length()-6)+mnemonics[mnemo].first;
    }

    else{
      if(label_table.find(operand)==label_table.end()){
        if(errors.find(line)==errors.end()){
          errors.insert({line,"Error at line: "+to_string(line)+" | Error type: No such label"});
          return;
        }
      }

      string temp=toHex(label_table[operand]);         
      encoding=toHex(loc)+" "+temp.substr(temp.length()-6)+mnemonics[mnemo].first;
    }
  }

  else if(mnemonics[mnemo].second==2){
    if(isDecimal(operand) || isOctal(operand) || isHexa(operand)){
      string temp=toHex(inDecimal(operand));
      encoding=toHex(loc)+" "+temp.substr(temp.length()-6)+mnemonics[mnemo].first;
    }

    else{
      if(label_table.find(operand)==label_table.end()){
        if(errors.find(line)==errors.end()){
          errors.insert({line,"Error at line: "+to_string(line)+" | Error type: No such label"});
          return;
        }
      }

      if(mnemo=="br" || mnemo=="brz" || mnemo=="brlz"|| mnemo=="call"){  
        string temp=toHex(label_table[operand]-(loc+1));         
        encoding=toHex(loc)+" "+temp.substr(temp.length()-6)+mnemonics[mnemo].first;
      }
      else{
        string temp=toHex(label_table[operand]);
        encoding=toHex(loc)+" "+temp.substr(temp.length()-6)+mnemonics[mnemo].first;
      }
    }
  }
  machineCode.push_back(encoding+" "+instruct+"\n");
}

void table_form(string instruct,int &pc,int line){
  int loc=pc;
  if(present(instruct,':')){
    int colon=getIndex(instruct,':');
    string label=instruct.substr(0,colon);

    if(duplicate(label)){
      errors.insert({line,"Error at line: "+to_string(line)+" | Error type: Duplicate label definition"});
    }

    if(!validLabel(label)){  
      errors.insert({line,"Error at line: "+to_string(line)+" | Error type: Bogus label name"});  
    }

    if(instruct.back()!=':'){
      string main_instruct=instruct.substr(colon+1);
      main_instruct=remove_spaces(main_instruct);

      parsing_instruct_1st(main_instruct,line,loc);

      int gap=getIndex(main_instruct,' ');

      string mnemo=main_instruct.substr(0,gap);
      mnemo=remove_spaces(mnemo);

      string operand=main_instruct.substr(gap+1);
      operand=remove_spaces(operand);

      if(mnemo=="SET"){
        if(isDecimal(operand) || isOctal(operand) || isHexa(operand)){
          int temp=inDecimal(operand);
          label_table.insert({label,temp});
        }
      }
      else{
        label_table.insert({label,loc});
      }
    }
    else{
      label_table.insert({label,loc});
      pc=pc-1;
    }
  }  
  else{
    parsing_instruct_1st(instruct,line,loc);
  }
}


void first_Pass(string input,ofstream &logFile){
  int line=1,pc=0;
  string s;

  ifstream infile;
  infile.open(input);

  cout<<"\n------ Instructions ------\n";

  while(getline(infile,s)){
    int semi=getIndex(s,';');
    string instruct=s.substr(0,semi);
    instruct=remove_spaces(instruct);
    cout<<instruct<<"\n";

    if(instruct==""){
      line++;
      continue;
    }

    else{
      table_form(instruct,pc,line);
      line++;
      pc++;
    }
  }
  infile.close();
}

void generate_machineCode(string instruct,int &pc,int line){
  int loc=pc;
  if(present(instruct,':')){
    int colon=getIndex(instruct,':');

    if(instruct.back()!=':'){
      string main_instruct=instruct.substr(colon+1);
      main_instruct=remove_spaces(main_instruct);

      parsing_instruct_2nd(main_instruct,instruct,line,loc);
    }

    else{
      string encoding=toHex(loc)+"          "+instruct+"\n";
      machineCode.push_back(encoding);
      pc=pc-1;
    }
  }
  else{
    parsing_instruct_2nd(instruct,instruct,line,loc);
  }
}

void second_Pass(string input,ofstream &logFile,string input_name){
  cout<<"\n------ Labels and PC------\n";
  for(auto i:label_table){
    cout<<i.first<<"->"<<i.second<<"\n";
  }

  int line=1,pc=0;
  string s;

  ifstream infile;
  infile.open(input);

  while(getline(infile,s)){
    int semi=getIndex(s,';');
    string instruct=s.substr(0,semi);
    instruct=remove_spaces(instruct);

    if(instruct==""){
      line++;
      continue;
    }

    else{
      generate_machineCode(instruct,pc,line);
      line++;
      pc++;
    }
  }
  
  if(errors.empty()){
    logFile<<"No errors found!\n";
  }
  else{
    for(auto i:errors){
      logFile<<i.second<<"\n";
    }
  }

  if(!errors.empty()) return;

  string out_file,obj_file;
  out_file=input_name+".l";
  obj_file=input_name+".o";

  ofstream outFile(out_file);
  ofstream objFile(obj_file);

  for(int i=0;i<machineCode.size();i++){
    outFile<<machineCode[i];

    string machine_code=machineCode[i].substr(9,8);
    if(machine_code!="        "){
      objFile<<hex_to_bin(machine_code)<<"\n";
    }
  }

  infile.close();
  outFile.close();
  objFile.close();
}

int main(int argc,char* argv[]){
  Mnemonic_Opcode();

  string inputFile,log_file;

  inputFile=argv[1];
  string input_name=inputFile.substr(0,getIndex(inputFile,'.'));

  log_file=input_name+".log";
  ofstream logFile(log_file);

  first_Pass(inputFile,logFile);
  second_Pass(inputFile,logFile,input_name);

  logFile.close();
}