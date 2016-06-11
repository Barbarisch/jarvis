#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h> //malloc
#include <stdio.h> //popen, pclose

#include "parser.h"

#define CMD_FIRSTPART "cd "INSTALL_DIR"; echo \""
#define CMD_ENDPART "\" | "INSTALL_DIR"/syntaxnet/my_demo.sh 2>/dev/null"

/**
 *	parse_sentence - call parsey mcparseface to parse sentence
 *
 * TODO sanatize sentence string (command injection currently)
 */
int parse_sentence(char *sentence)
{
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	std::string cmd;

	cmd.assign(CMD_FIRSTPART);
	cmd.append(sentence);
	cmd.erase(cmd.length()-1, 1); //remove newline
	cmd.append(CMD_ENDPART);

	// open pipe stream.
	fp = popen(cmd.c_str(), "r");
	if(!fp) {
		return -1;
	}   

	while(getline(&line, &len, fp) !=  -1) {
		std::cout << "testing " << line << std::endl;
	}

	// close the stream.
	pclose(fp);	

	return 0;
}

int conll_parser(char *line, std::map<char*, char*> words)
{
	int idx = 0;
	char *pch = NULL, *word = NULL, *arc = NULL;
	
	pch = line;
	while((pch = strtok(line, "\t"))) {
		if(idx == 0)
			word == pch;
		if(idx == 6)
			arc = pch;
		idx++;
	}

	if(word && arc)
		words.insert(std::pair<char*,char*>(word,arc));

	return 0;
}
