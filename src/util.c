String read_entire_file(char *path) {
    String result = {0};
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    result.size = ftell(f);
    fseek(f, 0, SEEK_SET);
    result.data = ALLOC(u8, result.size);
    fread(result.data, result.size, 1, f);
    fclose(f);
    return result;
}

