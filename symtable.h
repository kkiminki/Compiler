#include<vector>
#include"lyutils.h"
#include"auxlib.h"
#include"astree.h"
//#include"symbol.h"

symbols* sym_from_node(astree* node);

symbols* get_symbol_type(astree* ident, symbols* sym);

void ident_symbol(astree* ident, astree* type);

void array_symbol(astree* ident, astree* type);

void function_symbol(astree* node);

void func_param_symbol(astree* node);

void struct_symbol(astree* id);

void struct_fields_symbol(astree* id, astree* fields);

void const_symbol(astree* ident);

void sym_init();

symbol_table* new_sym_table();

void insert_structure(astree* node);

void traverse_tree(astree* root);

void var_symbol(astree* node);

void check_func(astree* node);

void check_param(astree* node);
