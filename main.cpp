//Kyler Kiminki
//kkiminki
//cmps104a
//main.cpp
//Client file for the oc compiler

#include <string>
#include <fstream>
#include <vector>

using namespace std;

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stringset.h"
#include "lyutils.h"
#include "symtable.h"

#define USG_ERR -1
#define R_ERR -2
#define W_ERR -3


//Calls cpp at command line
int cpp(char* file, int d_flag, const char* d_string){
   string CPP = "/usr/bin/cpp";
   string command;
   //check $d_flag and append $d_string
   //if necessary
   if(d_flag){
      command = CPP + " " + d_string + " " + file;
   }else{
      command = CPP + " " + file;
   }
   FILE* pipe = popen(command.c_str(), "r");
   //Returns -1 to call read error
   if(pipe==NULL) return -1;
   yyin = pipe;
   yyparse();
   //do{
      //yylex();
   //}while(yyparse()!=YYEOF);
   return 1;
}

//Prints out usage message
void print_usage(){
   cerr << "oc -@[debug_flags] -D[cpp_argument] -l -y [input_program].oc" << endl;
}

int main(int argc, char** argv){
   sym_init();
   yy_flex_debug = 0;
   int c, d_flag=0;
   string d_string = "-D";
   string f_string = "@:D:ly";
   //Checks if any flag arguments match $f_string
   while((c=getopt(argc, argv, f_string.c_str()))!=-1){
      switch(c){
         //Passes argument to set_debugflags() in auxlib
         case '@':
            if(optarg==NULL){
               cerr << "USAGE ERROR: The -@ flag takes a string argument" << endl;
               print_usage();
               set_exitstatus(USG_ERR);
               break;
            }
            set_debugflags(optarg);
            DEBUGF('f', "Flag f was activated\n");
            break;
         //Sets $d_flag to one and $d_string to its argument
         //and is passed to cpp()
         case 'D':
            if(optarg==NULL){
               cerr << "USAGE ERROR: The -D flag takes a string argument" << endl;
               print_usage();
               set_exitstatus(USG_ERR);
               break;
            }
            d_string = d_string + optarg;
            d_flag = 1;
            break;
         //Currently do nothing for either flag
         case 'l':
            yy_flex_debug = 1;
            break;
         case 'y':
            yydebug = 1;
            break;
         //Deal with unrecognized flags
         case'?':
            cerr << "USAGE ERROR: Could not recognize symbol -" << optopt << endl;
            print_usage();
            set_exitstatus(USG_ERR);
            break;
         default:
            break;
      }
   }
   //Set the executable name to the last command line argument
   set_execname(strtok(strdup(argv[argc-1]), "."));
   const char* basename = strdup(get_execname());
   char* tok_str = strdup(basename);
   string tok_ext = ".tok";
   strcat(tok_str, tok_ext.c_str());
   tok_file = fopen(tok_str, "w");
   if(tok_file==NULL){
      cerr << "FILE ERROR: COULD NOT WRITE TO TRACE FILE \"" << tok_str << "\"" << endl;
      set_exitstatus(R_ERR);
      return get_exitstatus();
   }
   //Call cpp and test its result code for a read error
   if(cpp(argv[argc-1], d_flag, d_string.c_str())!=1){
      cerr << "FILE ERROR: COULD NOT READ INPUT FILE \"" << argv[argc-1] << "\"" << endl;
      set_exitstatus(R_ERR);
      return get_exitstatus();
   }
   fclose(tok_file);
   //dump_astree(tok_file, yylval);
   //Set $basename to its prefix and append ".str" suffix
   //then create the file and open an iostream
   char* str_file = strdup(get_execname());
   string str_ext = ".str";
   strcat(str_file, str_ext.c_str());
   filebuf fb;
   fb.open(str_file, ios::out);
   ostream os(&fb);
   //If the osteam couldn't be opened exit with write error
   if(!os){
      cerr << "FILE ERROR: COULD NOT WRITE TO TRACE FILE \"" << str_file << "\"" << endl;
      set_exitstatus(R_ERR);
      return get_exitstatus();
   }
   os.exceptions(os.exceptions()|ios_base::badbit);
   //Dump the stringset into the ".str" file
   dump_stringset(os);
   fb.close();
   char* ast_str = strdup(basename);
   string ast_ext = ".ast";
   ast_str = strcat(ast_str, ast_ext.c_str());
   FILE* ast_file = fopen(ast_str, "w");
   if(ast_file==NULL)
     cerr << "FILE ERROR: COULD NOT WRITE TO TRACE FILE \"" << ast_file << "\"" << endl;
   dump_astree(ast_file, yyparse_astree);
   char* sym_str = strdup(basename);
   string sym_ext = ".sym";
   sym_str = strcat(sym_str, sym_ext.c_str());
   FILE* sym_file = fopen(sym_str, "w");
   if(sym_file==NULL)
      cerr << "FILE ERROR: COULD NOT WRITE TO TRACE FILE \"" << sym_file << "\"" <<endl;
   dump_table(sym_file, yyparse_astree);
   return get_exitstatus();
}
