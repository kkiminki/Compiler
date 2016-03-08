#include<unordered_map>
#include<bitset>

struct symbols;

enum{ ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
      ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
      ATTR_field, ATTR_typeid, ATTR_lval, ATTR_param,ATTR_const,
      ATTR_variable, ATTR_vreg, ATTR_vaddr, ATTR_bitset_size
};

using symbol_table = unordered_map<const string*, symbols*>;
using symbol_entry = symbol_table::value_type;

using attr_bitset = bitset<ATTR_bitset_size>;

struct symbols{
   symbol_table* fields = NULL;
   size_t filenr, linenr, offset;
   size_t block_nr;
   attr_bitset attr;
   vector<symbols*>* params = NULL;
};
