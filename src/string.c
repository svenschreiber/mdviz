String str_push(char *str) {
    u32 len = strlen(str);
    String result = {0};
    result.data = malloc(sizeof(u8) * len);
    result.size = len;
    memcpy(result.data, str, len);
    return result;
}

String str_pushf(char *format, ...) {
    char buffer[2048];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 2048, format, args);
    String result = str_push(&buffer[0]);
    va_end(args);
    return result;
}

u8 to_lowercase(u8 c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

b32 str_compare_ignorecase(String a, String b) {
    if (a.size != b.size) return 0;
    for (u32 i = 0; i < a.size; ++i) {
        if (to_lowercase(a.data[i]) != to_lowercase(b.data[i])) return 0;
    }
    return 1;
}

b32 str_compare(String a, String b) {
    if (a.size != b.size) return 0;
    for (u32 i = 0; i < a.size; ++i) {
        if (a.data[i] != b.data[i]) return 0;
    }
    return 1;
}

String str_copy(String str) {
    String result = {0};
    result.data = malloc(sizeof(u8) * str.size);
    result.size = str.size;
    memcpy(result.data, str.data, str.size);
    return result;
}

b32 str_has_prefix(String str, String prefix) {
    if (str.size < prefix.size) return 0;
    for (u32 i = 0; i < prefix.size; ++i) {
        if (str.data[i] != prefix.data[i]) return 0;
    }
    return 1;
}

b32 str_has_suffix(String str, String suffix) {
    if (str.size < suffix.size) return 0;
    for (s32 i = 0; i < suffix.size; ++i) {
        if (str.data[str.size - suffix.size + i] != suffix.data[i]) return 0;
    }
    return 1;
}

String str_substring(String str, u32 from, u32 to) {
    String result = {0};
    if (from > str.size) from = str.size;
    if (to > str.size) to = str.size;
    if (from > to) {
        u32 tmp = from;
        from = to;
        to = tmp;
    }
    result.data = str.data + from;
    result.size = to - from;
    return result;
}

String *str_split(String str, String sep) {
    assert(sep.size > 0 && "String seperator is empty");
    String *list = 0;
    u32 start = 0;
    String current = str;
    for (u32 i = 0; i < str.size; ++i) {
        if (str_has_prefix(current, sep)) {
            arrput(list, str_substring(str, start, i));
            start = i + sep.size;
            i += sep.size - 1;
            current = str_substring(current, sep.size - 1, current.size);
        }
        current = str_substring(current, 1, current.size);
    }
    arrput(list, str_substring(str, start, str.size));
    return list;
}

b32 is_whitespace_or_eol(u8 c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

String str_trim(String str) {
    if (!str.size) return str;

    String result = {0};

    for (u32 i = 0; i < str.size; ++i) {
        if (is_whitespace_or_eol(str.data[i])) continue;

        result.data = str.data + i;
        result.size = str.size - i;
        break;
    }

    for (u32 i = 0; i < result.size; ++i) {
        if (is_whitespace_or_eol(result.data[result.size - 1 - i])) continue;

        result.size -= i;
        break;
    }

    return result;
}

b32 str_read_line(String *str, String *result) {
    if (str->size == 0) return 0;

    u32 line_size = 0;
    while (line_size < str->size && str->data[line_size] != '\n') {
        ++line_size;
    }
    result->data = str->data;
    result->size = line_size;
    
    // advance original string
    if (line_size < str->size - 1) {
        str->data += line_size + 1;
        str->size -= line_size + 1;
    } else {
        str->data += str->size;
        str->size = 0;
    }

    return 1;
}

b32 str_is_substring(String str, String substr) {
    return substr.data >= str.data && substr.data + substr.size <= str.data + str.size;
}

String str_find_next(String str, String to_find) {
    for (u32 i = 0; i < str.size; ++i) {
        String sub = str_substring(str, i, str.size);
        if (str_has_prefix(sub, to_find)) return str_substring(sub, 0, to_find.size);
    }

    return (String){0};
}

// str->data could get realloced, so be sure to make a copy
// if you want to keep the old string
// TODO: don't know if this is the right way
void str_replace(String *str, String rep, String with) {
    assert(str_is_substring(*str, rep));
    assert(!str_is_substring(*str, with));

    // Need to save index of substring start, because
    // rep.data could also get invalidated by realloc
    u32 rep_start = rep.data - str->data;

    u32 new_size = str->size - rep.size + with.size;

    if (new_size > str->size) {
        str->data = realloc(str->data, new_size);
        u32 rep_end = rep_start + rep.size;
        memmove(str->data + rep_start + with.size, str->data + rep_end, str->size - rep_end);
    } else if (new_size < str->size) {
        u32 rep_end = rep_start + rep.size;
        memmove(str->data + rep_start + with.size, str->data + rep_end, str->size - rep_end);
        str->data = realloc(str->data, new_size);
    }

    memcpy(str->data + rep_start, with.data, with.size);
    str->size = new_size;
}

// Contrary to replace, when using replace_all, rep should not be a string view of str.
// Returns the number of replacements that happened. 
s32 str_replace_all(String *str, String rep, String with) {
    assert(rep.size);
    assert(!str_is_substring(*str, rep));

    s32 replacements = 0;

    String find = {0};
    String iter = *str;
    while ((find = str_find_next(iter, rep)).data) {
        u32 start = find.data - str->data;
        str_replace(str, find, with);
        iter = str_substring(*str, start + with.size, str->size);
        ++replacements;
    }
    return replacements;
}

s32 str_to_s32(String str) {
    char buffer[64];
    memmove(buffer, str.data, str.size);
    buffer[str.size] = '\0';
    return atoi(buffer);
}

f32 str_to_f32(String str) {
    char buffer[64];
    memmove(buffer, str.data, str.size);
    buffer[str.size] = '\0';
    return (f32)atof(buffer);
}
