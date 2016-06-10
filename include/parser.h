#ifndef PARSER_H
#define PARSER_H

#include <map>

#define INSTALL_DIR "/home/user/Desktop/workspace/models/syntaxnet"

//Coarse-grained Part-of-Speech
#define OTHER	0
#define NOUN	1
#define VERB	2
#define DET		3
#define ADP		4

//Find-grained Part-of-Speech
//https://www.ling.upenn.edu/courses/Fall_2003/ling001/penn_treebank_pos.html

//word significance in sentence
#define OTHER	0
#define ROOT	1
#define dep		2
#define det		3
#define dobj	4
#define prt		5

class ConllData {
	public:
	private:
		int tokenNum;
		char *word;
		int field3;
		int cg_pos;
		int fg_pos;
		int field6;
		int tokenRelation;
		int key;
		int field9;
		int field10;
};

int conll_parser(char *, std::map<char*, char*>);
int parse_sentence(char *);

#endif //PARSER_H
