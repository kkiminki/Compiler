//Kyler Kiminki
//kkiminki
//cmps104a
//astree.cpp
//ADT for abstract synatx trees


#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"
#include "stringset.h"
#include "lyutils.h"

astree::astree (int symbol, int filenr, int linenr,
                int offset, const char* clexinfo):
        symbol (symbol), filenr (filenr), linenr (linenr),
        offset (offset), lexinfo (intern_stringset (clexinfo)) {
   DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
           (void*) this, filenr, linenr, offset,
           get_yytname (symbol), lexinfo->c_str());
}

astree* adopt1 (astree* root, astree* child) {
   root->children.push_back (child);
   DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
           root, root->lexinfo->c_str(),
           child, child->lexinfo->c_str());
   return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}

astree* adopt3(astree* root, astree* one, astree* two, astree* three){
   adopt1(root, one);
   adopt1(root, two);
   adopt1(root, three);
   return root;
}

astree* adopt4(astree* root, astree* one, astree* two, astree* three, astree* four){
   adopt1(root, one);
   adopt1(root, two);
   adopt1(root, three);
   adopt1(root, four);
   return root;
}

astree* adopt5(astree* root, astree* one, astree* two, astree* three, astree* four, astree* five){
   adopt1(root, one);
   adopt1(root, two);
   adopt1(root, three);
   adopt1(root, four);
   adopt1(root, five);
   return root;
}

astree* terminal_char(astree* root, astree* arg){
   root = new_parseroot();
   arg = new_parseroot();
   root = adopt1(root, arg);
   return root;
}

astree* adopt_children(astree* root, astree* node){
   while(!node->children.empty()){
      adopt1(root, node->children.back());
      node->children.pop_back();
   }
   return root; 
}

astree* set_func(astree* ident, astree* params, astree* block){
   astree* function = new astree((int)TOK_FUNCTION, (int)ident->filenr, (int)ident->linenr, (int)ident->offset, (const char*)ident->lexinfo);
   astree* par = new astree((int)TOK_PARAMLIST,(int) ident->filenr, (int)ident->linenr, (int)ident->offset, (const char*)ident->lexinfo);
   par = adopt_children(par, params);
   free_ast(params);
   function = adopt3(function, ident, par, block);
   return function;
}

void astree_out( astree* root){
   char* ast_str = strdup(get_execname());
   string ast_ext = ".ast";
   ast_str = strcat(ast_str, ast_ext.c_str());
   FILE* ast_file = fopen(ast_str, "w");
   dump_astree(ast_file, root);
   fclose(ast_file);
}

void print_attr(FILE* outfile, attr_bitset attr){
   if(attr.test(ATTR_void))
      fprintf(outfile, " void");
   if(attr.test(ATTR_bool))
      fprintf(outfile, " bool");
   if(attr.test(ATTR_char))
      fprintf(outfile, " char");
   if(attr.test(ATTR_int))
      fprintf(outfile, " int");
   if(attr.test(ATTR_null))
      fprintf(outfile, " null");
   if(attr.test(ATTR_string))
      fprintf(outfile, " string");
   if(attr.test(ATTR_struct))
      fprintf(outfile, " struct");
   if(attr.test(ATTR_array))
      fprintf(outfile, " array");
   if(attr.test(ATTR_function))
      fprintf(outfile, " function");
   if(attr.test(ATTR_field))
      fprintf(outfile, " field");
   if(attr.test(ATTR_typeid))
      fprintf(outfile, " typeid");
   if(attr.test(ATTR_lval))
      fprintf(outfile, " lval");
   if(attr.test(ATTR_param))
      fprintf(outfile, " param");
   if(attr.test(ATTR_const))
      fprintf(outfile, " const");
   if(attr.test(ATTR_variable))
      fprintf(outfile, " variable");
   if(attr.test(ATTR_vreg))
      fprintf(outfile, " vreg");
   if(attr.test(ATTR_vaddr))
      fprintf(outfile, " vaddr");
}


static void dump_node (FILE* outfile, astree* node) {
   char* str = strdup(get_yytname(node->symbol));
   if(strstr(str, "TOK_") == str) str += 4;
   fprintf (outfile, "%s \"%s\" %ld.%ld.%03ld",
            get_yytname (node->symbol), node->lexinfo->c_str(),
            node->filenr, node->linenr, node->offset);
   if(node->attr.any())
      print_attr(outfile, node->attr);
   /*bool need_space = false;
   for (size_t child = 0; child < node->children.size();
        ++child) {
      if (need_space) fprintf (outfile, " ");
      need_space = true;
      fprintf (outfile, "%p", node->children.at(child));
   }*/
   //fprintf (outfile, "]}");
}

static void dump_sym(FILE* outfile, astree* node){
   if(node->attr.any()){
   char* str = strdup(get_yytname(node->symbol));
   if(strstr(str, "TOK_") == str) str += 4;
   fprintf (outfile, "\"%s\" (%ld.%ld.%03ld) {0}",
            node->lexinfo->c_str(),node->filenr, node->linenr, node->offset);
   if(node->attr.any())
      print_attr(outfile, node->attr);
   } 
}

static void dump_table_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   for(int i = 0; i < depth; i++){
      fprintf(outfile, "\t");
   }
   dump_sym (outfile, root);
   fprintf (outfile, "\n");
   for (size_t child = 0; child < root->children.size();
        ++child) {
      dump_table_rec (outfile, root->children[child],
                       depth + 1);
   }
}

void dump_table (FILE* outfile, astree* root) {
   dump_table_rec (outfile, root, 0);
   fflush (NULL);
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   for(int i = 0; i < depth; i++){
      fprintf(outfile, "|\t");
   }
   dump_node (outfile, root);
   fprintf (outfile, "\n");
   for (size_t child = 0; child < root->children.size();
        ++child) {
      dump_astree_rec (outfile, root->children[child],
                       depth + 1);
   }
}

void dump_astree (FILE* outfile, astree* root) {
   dump_astree_rec (outfile, root, 0);
   fflush (NULL);
}

void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
   if (is_defined_token (toknum)) {
      dump_node (outfile, yyvaluep);
   }else {
      fprintf (outfile, "%s(%d)\n",
               get_yytname (toknum), toknum);
   }
   fflush (NULL);
}


void free_ast (astree* root) {
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
           root, root->filenr, root->linenr, root->offset,
           get_yytname (root->symbol), root->lexinfo->c_str());
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}


