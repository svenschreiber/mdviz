#ifndef STRING_H
#define STRING_H

// The String struct is sometimes used as memory container
// and sometimes as just a string view.
typedef struct String {
    u8 *data;
    u32 size;
} String;

#define Str(literal) ((String){.data = (u8*)literal, .size = sizeof(literal) - 1})

// ---- UTILITY
u8 to_lowercase(u8 c);
b32 is_whitespace_or_eol(u8 c);
s32 str_to_s32(String str);
f32 str_to_f32(String str);

// ---- STRING API
String str_push(char *str);
String str_pushf(char *format, ...);
b32 str_compare_ignorecase(String a, String b);
b32 str_compare(String a, String b);
String str_copy(String str);
b32 str_has_prefix(String str, String prefix);
b32 str_has_suffix(String str, String suffix);
String str_substring(String str, u32 from, u32 to);
String *str_split(String str, String sep);
String str_trim(String str);
b32 str_read_line(String *str, String *result);
b32 str_is_substring(String str, String substr);
String str_find_next(String str, String to_find);
void str_replace(String *str, String rep, String with);
s32 str_replace_all(String *str, String rep, String with);

#endif // STRING_H
