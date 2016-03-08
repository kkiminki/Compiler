//Kyler Kiminki
//kkiminki
//cmps104a
//symtable.cpp
//symbol table ADT

#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include"lyutils.h"
//#include"parser.y"

symbol_table* type_table;
vector<symbol_table*> symbol_stack;
size_t next_block;

symbols* sym_from_node(astree* node){
   symbols* sym = new symbols();
   sym->filenr = node->filenr;
   sym->linenr = node->linenr;
   sym->offset = node->offset;
   sym->block_nr = next_block-1;
   sym->attr = node->attr;
   return sym;
}

symbols* get_symbol_type(astree* ident,astree* type, symbols* sym){
   switch(type->symbol){
      case TOK_VOID:
         ident->attr.set(ATTR_void);
         ident->attr.set(ATTR_lval);
         sym = sym_from_node(ident);
         break;
      case TOK_BOOL:
         ident->attr.set(ATTR_bool);
         ident->attr.set(ATTR_lval);
         sym = sym_from_node(ident);
         break;
      case TOK_STRING:
         ident->attr.set(ATTR_string);
         ident->attr.set(ATTR_lval);
         sym = sym_from_node(ident);
         break;
      case TOK_INT:
         ident->attr.set(ATTR_int);
         ident->attr.set(ATTR_lval);
         sym = sym_from_node(ident);
         break;
      case TOK_CHAR:
         ident->attr.set(ATTR_char);
         ident->attr.set(ATTR_lval);
         sym = sym_from_node(ident);
         break;
      default:
         symbol_table::iterator iter = symbol_stack.back()->find(type->lexinfo);
         if(iter==symbol_stack.back()->end()){
            sym = NULL;
            fprintf(stderr, "Could not recognize symbol \"%s\" at %ld.%ld.%03ld\n",type->lexinfo->c_str(),type->filenr, type->linenr, type->offset);
         }else{
            ident->attr.set(ATTR_lval);
            ident->attr.set(ATTR_variable);
            sym = sym_from_node(ident);
            break;
      }
   }return sym;
}

void ident_symbol(astree* ident, astree* type){
   symbols* sym, got;
   sym = sym_from_node(ident);
   //symbol_table::iterator 
   symbol_table::iterator iter = symbol_stack.back()->find(ident->lexinfo);
   if(iter != symbol_stack.back()->end()){
      symbols* got = iter->second;
      fprintf(stderr, "Redeclaration of \"%s\" at line %ld.%ld.%03ld\nFound earlier at %ld.%ld.%03ld\n",ident->lexinfo->c_str(), ident->filenr, ident->linenr, ident->offset,got->filenr, got->linenr, got->offset);
      return;
   }
   
   sym = get_symbol_type(ident, type, sym);
   if(!ident->attr.test(ATTR_lval)){
      fprintf(stderr, "Could not identify type \"%s\" at %ld.%ld.%03ld\n",
              type->lexinfo->c_str(), type->filenr, type->linenr, type->offset);
      return;
   }
   symbol_entry entry(ident->lexinfo, sym);
   symbol_stack.back()->insert(entry);
   /*symbol_table::iterator iter2 = symbol_stack.back()->find(ident->lexinfo);
   if(iter2==symbol_stack.back()->end())
     fprintf(stdout, "YOU'RE FUCKED!\n");*/
}

void function_symbol(astree* ident){
   ident->children[0]->attr.reset(ATTR_lval);
   ident->children[0]->attr.reset(ATTR_variable);
   ident->children[0]->attr.set(ATTR_function);
   symbol_table::iterator got = symbol_stack.back()->find(ident->children[0]->lexinfo);
   /*if(got==symbol_stack.back()->end()){
      fprintf(stderr, "Error could not find identifier \"%s\" at %ld.%ld.%03ld\n",
              ident->lexinfo->c_str(), ident->filenr, ident->linenr, ident->offset);
      return;
   }*/
   symbols* temp = sym_from_node(ident->children[0]);;
   temp->attr.reset(ATTR_lval);
   temp->attr.reset(ATTR_variable);
   temp->attr.set(ATTR_function);
}

void func_param_symbol(astree* node){
   int i;
   symbols* sym = new symbols();
   for(i=0;i<(int)node->children.size();i++){
      /*symbol_table::iterator got = symbol_stack.back()->find(node->children[i]->children[0]->lexinfo);
      if(got != symbol_stack.back()->end()){
         fprintf(stderr, "Redeclaration of array \"%s\" at %ld.%ld.%03ld\n",
              node->lexinfo->c_str(), node->filenr, node->linenr, node->offset);
      }else{*/
         symbols* temp = get_symbol_type(node->children[i]->children[0], node->children[i], sym);
         temp->attr.set(ATTR_param);
         node->children[i]->children[0]->attr.set(ATTR_param);
      //}
   }
}

void array_symbol(astree* ident, astree* type){
   ident->attr.set(ATTR_array);
   symbols* sym;
   get_symbol_type(type, ident, sym);
   if(!ident->attr.test(ATTR_lval)){
      fprintf(stderr, "Error could not recognize type \"%s\" at %ld.%ld.%03ld\n",
              type->lexinfo->c_str(), type->filenr, type->linenr, type->offset);
   }
   symbol_entry entry(ident->lexinfo, sym);
   symbol_stack.back()->insert(entry);
}

void struct_symbol(astree* id){
   id->attr.set(ATTR_struct);
   symbols* sym = sym_from_node(id);
   symbol_entry(id->lexinfo, sym);
}

void struct_fields_symbol(astree* id, astree* fields){
   id->attr.set(ATTR_struct);
   int i;
   symbols* sym = new symbols();
   symbol_table* field_table = new symbol_table();
   for(i=0; i<(int)fields->children.size()-1;i++){
      symbols* temp = new symbols();
      symbol_table::iterator got = symbol_stack.back()->find(fields->children[i]->lexinfo);
      /*if(got == symbol_stack.back()->end()){
         fprintf(stderr,"Unrecognized field \"%s\" %ld.%ld.%03ld\n",
                 fields->children[i]->lexinfo->c_str(), fields->children[i]->filenr,
                  fields->children[i]->linenr, fields->children[i]->offset);
      }*/
      get_symbol_type(fields->children[i]->children[0], fields->children[i], temp);
      symbol_entry(fields->children[i]->lexinfo, temp);
   }
   sym->fields = field_table;
   symbol_entry entry(id->lexinfo, sym);
   symbol_stack.back()->insert(entry);
}

void const_symbol(astree* ident){
   switch(ident->symbol){
      case TOK_INTCON:
         ident->attr.set(ATTR_int);
         ident->attr.set(ATTR_const);
         break;
      case TOK_CHARCON:
         ident->attr.set(ATTR_char);
         ident->attr.set(ATTR_const);
         break;
      case TOK_STRINGCON:
         ident->attr.set(ATTR_string);
         ident->attr.set(ATTR_const);
         break;
      case TOK_FALSE:
      case TOK_TRUE:
         ident->attr.set(ATTR_bool);
         ident->attr.set(ATTR_const);
         break;
      case TOK_NULL:
          ident->attr.set(ATTR_null);
          ident->attr.set(ATTR_const);

   }
}
         
void sym_init(){
   type_table = new symbol_table();
   symbol_stack.push_back(new symbol_table());
   next_block = 1;
}

symbol_table* new_sym_table(){
   symbol_table* table = new symbol_table();
   return table;
}

void insert_structure(astree* node){
   symbols* sym = sym_from_node(node); 
   sym->block_nr = 0;
   symbol_entry entry(node->children[0]->lexinfo, sym);
   symbol_stack.back()->insert(entry);
}

void traverse_tree(astree* root){
   int child;
   for(child = 0; child < (int)root->children.size();child++){
      traverse_tree(root->children[child]);
   }
}

void var_symbol(astree* node){
   symbol_table::iterator got = symbol_stack.back()->find(node->lexinfo);
   if(got==symbol_stack.back()->end()){
      fprintf(stderr, "Variable \"%s\"  at %ld.%ld.%03ld has not been declared\n",
      node->lexinfo->c_str(), node->filenr, node->linenr, node->offset);
      return;
   }
   symbols* sym = got->second;
   node->attr = sym->attr;
   node->attr.set(ATTR_variable);
   sym->attr.set(ATTR_variable);
}

void check_func(astree* node){
   symbol_table::iterator got = symbol_stack.back()->find(node->lexinfo);
   if(got==symbol_stack.back()->end()){
   fprintf(stderr, "Function \"%s\"  at %ld.%ld.%03ld has not been declared\n",
      node->lexinfo->c_str(), node->filenr, node->linenr, node->offset);
      return;
   }
   symbols* sym = got->second;
   node->attr = sym->attr;
   node->attr.set(ATTR_function);
   sym->attr.set(ATTR_function);
}
void check_param(astree* node){
   int i;
   for(i=0;i<(int)node->children.size()-1;i++){
      symbol_table::iterator iter = symbol_stack.back()->find(node->children[i]->lexinfo);
      if(iter==symbol_stack.back()->end()){
         fprintf(stderr, "Function \"%s\"  at %ld.%ld.%03ld has not been declared\n",
         node->lexinfo->c_str(), node->filenr, node->linenr, node->offset);
         return;
      }else{
         symbols* sym = iter->second;
         node->children[i]->attr.set(ATTR_param);
         sym->attr.set(ATTR_param);
      }
   }
}
