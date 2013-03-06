# -*- encoding=utf-8 -*-
import sys

code_name = "url_extern_type_code"
inc_code = ""
c_code=""
ext_num = 0
ext_enum = []
ext_type = []
type_code = []
fun_macros = []
ext_macros = []
fun_declares = []
c_includes = []


c_includes.append('#include <stdio.h>')
c_includes.append('#include <stdlib.h>')
c_includes.append('#include <string.h>')
c_includes.append('#include "%s.h"\n' %(code_name))

fun_declares.append('int get_url_extern_type(char *uri, int uri_len, unsigned int *extern_type, int *has_para);')

fun_macros.append('''#define _STR_2_64U(data, len, _val) \\\n{\\\n\tint i; \\\n\tfor (i=len-1; i>=0; i--) {\\\n\t\t_val = _val << 8; \\\n\t\t_val |= data[i]; \\\n\t}\\\n}\n''')

data = ""
line = ""
for line in sys.stdin:
	line = line.rstrip("\n").split()
	if len(line) < 3:
		continue
	if line[0] == "string_data":
		if len(data) > 0:
			type_code.append(data)
		data = ""
		ext_enum.append("EXT_%s" %(str.upper(line[2])))
		ext_type.append("%s" %(str.upper(line[2])))
		data = "/* %s */\n" %(line[2])
		ext_macros.append("EXTERN_%s_CODE()" %(str.upper(line[2])))
		data += "#define EXTERN_%s_CODE() \\\n" %(str.upper(line[2]))
		#type_code.append(data)
		ext_num = ext_num + 1
		continue
	data += "case %s:\\\n" %(line[2])

if len(data) > 0:
	type_code.append(data)
data = ""
type_code.append("#define EXTERN_UNKNOWN_CODE() default:\n")
ext_enum.append("EXT_%s" %("UNKNOWN"))
ext_macros.append("EXTERN_UNKNOWN_CODE()")
ext_type.append("UNKNOWN")
ext_enum.append("EXT_%s" %("NUM"))

# generate include file
inc_code = inc_code + "#ifndef __%s_H\n" %(str.upper(code_name))
inc_code = inc_code + "#define __%s_H\n" %(str.upper(code_name))

for i in range(0, len(fun_macros)):
	inc_code += "%s\n" %(fun_macros[i])
inc_code += "\n"

inc_code += "enum {\n"
inc_code += "\t%s = 0x00,\n" %(ext_enum[0])
for i in range(1, len(ext_enum)):
	inc_code += "\t%s,\n" %(ext_enum[i])
inc_code = inc_code + "};\n"

for i in range(0, len(type_code)):
	inc_code += "%s\n" %(type_code[i])

inc_code += "\n"

for i in range(0, len(fun_declares)):
	inc_code += "%s\n" %(fun_declares[i])
inc_code += "\n"

inc_code = inc_code + "#endif // __%s_H\n" %(str.upper(code_name))

# write to include header file
open(code_name + ".h", "w").write(inc_code)

fun_definitions = []
struct_declares = []
static_glb_data = []
static_funcs = []

struct_declares.append("""
typedef struct _ext_def_s {
    unsigned int type;
    char *type_name;
}ext_def_t;
""")

glb_data = """static ext_def_t extern_type[] = {\n"""
for i in range(0, len(ext_enum)-1):
	if i % 4 == 0:
		glb_data += "\t"
	glb_data += '{%s, "%s"}' %(ext_enum[i], ext_type[i])
	if i < len(ext_type)-1:
		glb_data += ", "
	if (i+1) % 4 == 0:
		glb_data += "\n"
glb_data += "\n};\n"
static_glb_data.append(glb_data)
glb_data = ""

static_funcs.append("""
static char * find_ext_name(unsigned int type)
{
    int i = 0;

    while (extern_type[i].type != EXT_UNKNOWN) {
        if (extern_type[i].type == type) return extern_type[i].type_name;
        i++;
    }

    return "UNKNOWN_TYPE";
}
""")

# define: get_url_extern_type
func = """
int get_url_extern_type(char *uri, int uri_len, unsigned int *extern_type, int *has_para)
{
    int len = 0;
    unsigned long long type_value = 0;
    const int res_type_size = 8;
    char * uri_end;
    char *p, *p2;
    char *p_dot; // pointer to "."

    *extern_type = EXT_UNKNOWN;
    p = uri;
    uri_end = uri + uri_len;
    p2 = uri_end - 1;

    if (has_para) *has_para = 0;

    // traverse from back to front for character '/'
    while (p2 > p && '/' != *p2) p2--;

    p = p2 = p2 + 1; // skip to resource name
    // skip to '?', after that, p2 may point at uri_end.
    while (p2 < uri_end && '?' != *p2) p2++;
    if (p2 - p <= 0) return 0;
    // check there is any parameter in uri.
    if (uri_end - p2 >= 2 && has_para) *has_para = 1;

    // skip to '.'
    p_dot = NULL;
    while (p < p2) {
        if (*p == '.') p_dot = p;
        p++;
    }
	    if (!p_dot || p2 - p_dot <= 0) return 0;
    p_dot++; // skip '.'

    // copy the resource type.
    if (res_type_size <= p2-p_dot) return 1;
    //memcpy(type_buf, p_dot, p2-p_dot);
    len = p2 - p_dot;

    _STR_2_64U(p_dot, len, type_value);
    switch(type_value) {
"""

for i in range(0, len(ext_enum)-1):
	func += "\t\t%s\n\t\t*extern_type = %s;\n\t\tbreak;\n" %(ext_macros[i], ext_enum[i])
func += "\t}\n"
func += "}\n"
fun_definitions.append(func)
func = ""

# generate c source code file
for i in range(0, len(c_includes)):
	c_code += "%s\n" %(c_includes[i])
c_code += "\n"

for i in range(0, len(struct_declares)):
	c_code += "%s\n" %(struct_declares[i])
c_code += "\n"

for i in range(0, len(static_glb_data)):
	c_code += "%s\n" %(static_glb_data[i])
c_code += "\n"

for i in range(0, len(static_funcs)):
	c_code += "%s\n" %(static_funcs[i])
c_code += "\n"

for i in range(0, len(fun_definitions)):
	c_code += "%s\n" %(fun_definitions[i])
c_code += "\n"

# write to source code file
open(code_name+".c", "w").write(c_code)
