const char *token_next(const char *str, int *pos) {
    const char *ret;

    while (str[*pos] == ' ') {
        (*pos)++;
    }

    ret = str+*pos;

    /* Advance to the next space or end of string. */
    while (str[*pos] != ' ' && str[*pos] != '\0') {
        (*pos)++;
    }

    return ret;
}
