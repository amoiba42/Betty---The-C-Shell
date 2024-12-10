#ifndef __MANPAGES_H__
#define __MANPAGES_H__
void remove_triple_newline(char *str);
void strip_html(char *html, char **text, size_t *text_size);
void showpages(char **args);
#endif