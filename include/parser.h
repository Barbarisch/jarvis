#ifndef PARSER_H
#define PARSER_H

#include <map>

int conll_parser(char *, std::map<char*, char*>);
int parse_sentence(char *);

#endif //PARSER_H
