//Kyler Kiminki
//kkiminki
//cmps104a
//astree.h
//Header file for astree ADT

#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <string>
#include <vector>
using namespace std;

#include "auxlib.h"
#include "symbol.h"

struct astree {
   int symbol;               // token code
   size_t filenr;            // index into filename stack
   size_t linenr;            // line number from source code
   size_t offset;            // offset of token with current line
   const string* lexinfo;    // pointer to lexical information
   vector<astree*> children; // children of this n-way node
   astree (int symbol, int filenr, int linenr,
           int offset, const char* clexinfo);
   attr_bitset attr;
};

// Append one child to the vector of children.
astree* adopt1 (astree* root, astree* child);

// Append two children to the vector of children.
astree* adopt2 (astree* root, astree* left, astree* right);

//Append three children to the vector of children.
astree* adopt3 (astree* root, astree* one, astree* two, astree* three);

//Appends four children to the vector of children.
astree* adopt4 (astree* root, astree* one, astree* two, astree* three, astree* four);

//Appends five children to the vector of children.
astree* adopt5 (astree* root, astree* one, astree* two, astree* three, astree* four, astree* five);

//Generates terminal chars
astree* terminal_char(astree* root, astree* arg);

//Appends the children of node to the children of root.
astree* adopt_children(astree* root, astree* node);

//Sets functions
astree* set_func(astree* ident, astree* params, astree* block);

//Generates output
void astree_out( astree* root);

// Dump an astree to a FILE.
void dump_astree (FILE* outfile, astree* root);

// Debug print an astree.
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);

// Recursively free an astree.
void free_ast (astree* tree);

// Recursively free two astrees.
void free_ast2 (astree* tree1, astree* tree2);

//void dump_sym(FILE* outfile, astree* node);

//void dump_table_rec(FILE* outfile, astree* root, int depth);

void dump_table(FILE* outfile, astree* root);

#endif
