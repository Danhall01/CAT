#ifndef _INCLUDE_CAT_INTERNAL_MACRO_H_
#define _INCLUDE_CAT_INTERNAL_MACRO_H_

// Used to expand the name
#define _CAT_MACRO_VAR_CONCAT(prefix, suffix) preffix##suffix
#define _CAT_MACRO_VAR_EXPAND(prefix, suffix) _CAT_MACRO_VAR_CONCAT(prefix, suffix)

// Create a variable inside of a macro, this creates a safe variable name that can be reused on
// other lines
#define CAT_MACRO_VAR(name) _CAT_MACRO_VAR_EXPAND(name, __LINE__)

#endif /* _INCLUDE_CAT_INTERNAL_MACRO_H_ */

