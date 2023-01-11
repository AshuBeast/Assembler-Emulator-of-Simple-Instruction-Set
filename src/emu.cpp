/*****************************************************************************
TITLE: Claims																																
AUTHOR:  Ashutosh Kumar (2001CS11)
Declaration of Authorship
This txt file, claims.txt, is part of the assignment of CS322 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/

#include <bits/stdc++.h>
using namespace std;

map<int,string>mnemonics;

string int_to_hexa(int num){                  //convert integer to hexadecimal
    stringstream ss;
    ss<<setfill('0')<<setw(8)<<hex<<num;
    return ss.str();
}
int A,B,PC,SP;                  //program registers

int memory[20000];                //memory/memory array

void Instruction_Set_Architecture() {
    cout << "Opcode Mnemonic Operand\n";
    cout << "       memory     operand\n";
    cout << "0      ldc      operand\n";
    cout << "1      adc      operand\n";
    cout << "2      ldl      operand\n";
    cout << "3      stl      operand\n";
    cout << "4      ldnl     operand\n";
    cout << "5      stnl     operand\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      operand\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      operand\n";
}

pair<int,int>convert_instrct_to_op(int line_code){
	int opcode, operand;
	opcode=line_code&0xff;           //taking first 6 bits
	operand=line_code&0xffffff00;      //taking last 2 bits
  return {operand,opcode};
}

void dump_into_memory(int counter, ofstream& trc){
    trc<<"Dumping from memory";

    for (int i=0;i<counter;i++){
        if(i%4){
          cout<<int_to_hexa(memory[i])<< " ";
          trc<<int_to_hexa(memory[i])<< " ";
        }
        else{
          cout<<"\n\n"<< int_to_hexa(i)<< "\t"<<int_to_hexa(memory[i])<< " ";
          trc<<"\n\n"<<int_to_hexa(i)<<"\t"<<int_to_hexa(memory[i]) << " ";
        }
    }
    cout<<"\n";
}

void perform_operation(int operation,int operand,int count){
      if(operation==0){
        B = A;
        A = operand;
      }
      else if(operation==1){
        A += operand;
      }
      else if(operation==2){
        B = A;
        A = memory[SP+operand];
      }
      else if(operation==3){
        memory[SP+operand] = A;
        A = B;
      }
      else if(operation==4){
        A = memory[A+operand];
      }
      else if(operation==5){
         memory[A+operand] = B;
      }
      else if(operation==6){
        A += B;
      }
      else if(operation==7){
        A = B - A;
      }
      else if(operation==8){
        A = B << A;
      }
      else if(operation==9){
        A = B >> A;
      }
      else if(operation==10){
        SP += operand;
      }
      else if(operation==11){
        SP = A;
        A = B;
      }
      else if(operation==12){
        B = A;
        A = SP;
      }
      else if(operation==13){
        B = A;
        A = PC;
        PC += operand;
      }
      else if(operation==14){
        PC = A;
        A = B;
      }
      else if(operation==15){
        if(A == 0) 
        PC += operand;
      }
      else if(operation==16){
        if(A < 0) 
        PC += operand;
      }
      else if(operation==17){
        PC += operand;
      }
      else if(operation==18){
          cout<<"Number of instructions executed = "<<count<<"\n";
      }
}
int trace_code(int PC, int counter, ofstream& trc){
	int count = 0;
	while(1){
      int prePC=PC;

      int operand,operation;
      int line_code=memory[PC];

      pair<int,int>info=convert_instrct_to_op(line_code);
      operand=info.first;
      operation=info.second;

      cout<<"PC: "<<int_to_hexa(PC)<<"\tSP:"<<int_to_hexa(SP)<<"\tA: "<< int_to_hexa(A)<<"\tB: "<< int_to_hexa(B)<<"\t"<<mnemonics[operation]<<" "<<operand<<"\n\n";
      trc<<"PC: "<<int_to_hexa(PC)<<"\tSP: "<<int_to_hexa(SP)<<"\tA: "<<int_to_hexa(A)<<"\tB: "<< int_to_hexa(B)<<"\t" << mnemonics[operation]<<" "<<operand<<"\n";

      perform_operation(operation,operand,count);
      PC++;

      if(prePC == PC){
          cout<<"Detected Infinite Loop\n";
          break;
      }
		count++;
	}
	cout<<"Number of instructions executed = "<<count<<"\n";
}

int main(int argc, char* argv[]){
	if(argc<3){
		cout<<"Usage: ./emu.exe [option] file.o" << "\n";
    cout<<"Options:" << "\n";
    cout<<"\t-trace\tshow instruction trace_code\n";
    cout<<"\t-before\tshow memory dump before execution\n";
    cout<<"\t-after\tshow memory dump after execution\n";
    cout<<"\t-isa\tdisplay Instrcution_Set_Architecture\n";
    return 0;
	}

    //Memory operand table
	  mnemonics.insert(make_pair(0,"ldc"));
    mnemonics.insert(make_pair(1,"adc"));
    mnemonics.insert(make_pair(2,"ldl"));
    mnemonics.insert(make_pair(3,"stl"));
    mnemonics.insert(make_pair(4,"ldnl"));
    mnemonics.insert(make_pair(5,"stnl"));
    mnemonics.insert(make_pair(6,"add"));
    mnemonics.insert(make_pair(7,"sub"));
    mnemonics.insert(make_pair(8,"shl"));
    mnemonics.insert(make_pair(9,"shr"));
    mnemonics.insert(make_pair(10,"adj"));
    mnemonics.insert(make_pair(11,"a2sp"));
    mnemonics.insert(make_pair(12,"sp2a"));
    mnemonics.insert(make_pair(13,"call"));
    mnemonics.insert(make_pair(14,"return"));
    mnemonics.insert(make_pair(15,"brz"));
    mnemonics.insert(make_pair(16,"brlz"));
    mnemonics.insert(make_pair(17,"br"));
    mnemonics.insert(make_pair(18,"HALT"));
    mnemonics.insert(make_pair(19,"data"));
    mnemonics.insert(make_pair(20,"SET"));

	string input=argv[2];

  string file_name=input.substr(0,input.find(".", 0));
	string trace_file=file_name+".trace";

	ifstream input_file;

	ofstream trc; 

	input_file.open(input,ios::out|ios::binary);
	trc.open(trace_file);

	int mach_code;
	int counter=0;                                                                  //for storing number of instructions

	while(input_file.read((char*)&mach_code,sizeof(mach_code))){
      int operand, operation;
      pair<int,int>info=convert_instrct_to_op(mach_code);
      operand=info.first;
      operation=info.second;
      
      if(operation == 19 || operation == 20) memory[counter++] = operand;            //store operand in memory in case of SET/DATA
      else if(operation >= 0 && operation < 20) memory[counter++] = mach_code;       //store instructions in memory
      else if(operation<0 || operation>=20){                                       //Invalid Instruction
          cout<<"Invalid instruction!\n";
          return 0;
      }
	}

	A=0,B=0,PC=0,SP = sizeof(memory)/sizeof(memory[0])-1;                    
	
	if(string(argv[1])=="-before") dump_into_memory(counter,trc);         //dumping into memory before execution

	if(string(argv[1])=="-trace_code") trace_code(0, counter, trc);       //tracing for each instruction

	if(string(argv[1])=="-after"){                                        //memory dump after execution
      trace_code(0, counter, trc);
      dump_into_memory(counter, trc);
	}

  if(string(argv[1])=="-isa"){
        Instruction_Set_Architecture();                                       
  }

	input_file.close();
}