#include <iostream>
#include <map>
#include <string.h>
#include <stdlib.h> //malloc
#include <stdio.h> //popen, pclose

#include "parser.h"

int parse_sentence(char *sentence)
{
	char *cmd1 = "cd /home/user/Desktop/workspace/models/syntaxnet;";
	char *cmd2 = "echo \"";
	char *cmd3 = "\" | /home/user/Desktop/workspace/models/syntaxnet/syntaxnet/my_demo.sh 2>/dev/null";
	char *cmd;
	int len1, len2, len3, len4;
	FILE *fp = NULL;
	char *line = NULL;
	int len = 1;

	len1 = strlen(cmd1);
	len2 = strlen(cmd2);
	len3 = strlen(cmd3);
	len4 = strlen(sentence);

	cmd = (char *)malloc(4096);

	if(!cmd)
		return -1;

	memset(cmd, 0, 4096);

	memcpy(cmd, cmd1, len1);
	memcpy(cmd+len1, cmd2, len2);
	memcpy(cmd+len1+len2, sentence, len4);
	memcpy(cmd+len1+len2+len4, cmd3, len3);

	// open pipe stream.
	fp = popen(cmd,"r");

	// error checking.
	if(!fp) {
		return -1;
	}   

	while(getline(&line, (size_t *)&len, fp) !=  -1) {
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
