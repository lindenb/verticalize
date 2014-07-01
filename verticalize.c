/*
The MIT License (MIT)

Copyright (c) 2014 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define OUT_OF_MEMORY do { fprintf(stderr,"%s: %d : OUT_OF_MEMORY.\n",__FILE__,__LINE__); exit(EXIT_FAILURE);} while(0)

typedef struct LineAndTokens_t
	{
	char*	line;
	size_t	line_capacity;
	size_t	line_len;
	size_t*	tokens;
	size_t	tokens_capacity;
	size_t	tokens_len;
	} LineAndTokens,*LineAndTokensPtr;


static LineAndTokensPtr LineAndTokensNew()
	{
	LineAndTokensPtr ptr=(LineAndTokensPtr)calloc(1,sizeof(LineAndTokens));
	if(ptr==NULL) OUT_OF_MEMORY;
	ptr->line_capacity = BUFSIZ;
	ptr->line = (char*)malloc(sizeof(char)*ptr->line_capacity);
	if(ptr->line==NULL) OUT_OF_MEMORY;
	ptr->line_len=0UL;
	
	ptr->tokens_capacity = 100;
	ptr->tokens = (size_t*)malloc(sizeof(size_t)*ptr->tokens_capacity);
	if(ptr->tokens==NULL) OUT_OF_MEMORY;
	ptr->tokens_len = 0UL;
	
	return ptr;
	}

static void LineAndTokensFree(LineAndTokensPtr ptr)
	{
	if(ptr==NULL) return;
	free(ptr->line);
	free(ptr->tokens);
	free(ptr);
	}
	

static LineAndTokensPtr readLine(FILE* in,LineAndTokensPtr ptr,int delim)
	{
	int c;
	if(feof(in)) return NULL;
	if(ptr==NULL) ptr=LineAndTokensNew();
	ptr->line_len=0;
	ptr->tokens_len=0;
	while((c=fgetc(in))!=EOF && c!='\n')
		{
		if( ptr->line_len +2 >= ptr->line_capacity )
			{
			ptr->line_capacity *= 2;
			ptr->line = (char*)realloc(ptr->line,sizeof(size_t)*ptr->line_capacity);
			if(ptr->line==NULL) OUT_OF_MEMORY;
			}
		if(c==delim)
			{
			if( ptr->tokens_len +1 >= ptr->tokens_capacity )
				{
				ptr->tokens_capacity *= 2;
				ptr->tokens = (size_t*)realloc(ptr->tokens,sizeof(size_t)*ptr->tokens_capacity);
				if(ptr->tokens==NULL) OUT_OF_MEMORY;
				}
			ptr->tokens[ ptr->tokens_len++ ]=ptr->line_len+1;
			}
		ptr->line[ptr->line_len++]=c;
		}
	ptr->line[ptr->line_len]=0;
	return ptr;
	}


/** print usage */
static void usage(FILE* out)
	{
	fprintf(out,"Compilation:\n\t%s %s\n",__DATE__,__TIME__);
	fputs("Author:\n\tPierre Lindenbaum PhD @yokofakun\n",out);
	fputs("WWW:\n\thttps://github.com/lindenb/makefile2graph\n",out);
	fputs("Usage:\n\tverticalize (option) (file|stdin)\n",out);
	fputs("Options:\n"
		"\t-h help (this screen)\n",out);
	fputs("\t-c first line is NOT header\n",out);
	fputs("\n",out);
	}

int main(int argc,char** argv)
	{
	size_t nLines=0;
	int c,first_line_header=1;
	FILE* in=stdin;
	
	while ((c = getopt (argc, argv, "hn")) != -1)
	 {
         switch (c)
           	{
           	case 'h': usage(stderr);return EXIT_SUCCESS;
           	case 'n': first_line_header=1;break;
		case '?':
               		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
               		return EXIT_FAILURE;
   	        default:
   	        	fprintf (stderr, "Bad input.\n");
               		return EXIT_FAILURE;
		}
	  }
	if(optind==argc)
		{
		in=stdin;
		}
	else if(optind+1==argc)
		{
		in=fopen(argv[optind],"r");
		if(in==NULL)
			{
			fprintf(stderr,"Cannot open \"%s\" : \"%s\".\n",
				argv[optind],strerror(errno));
			return EXIT_FAILURE;
			}
		}
	else
		{
		fprintf(stderr,"Illegal number of arguments.\n");
		return EXIT_FAILURE;
		}
	
	if(first_line_header)
		{
		LineAndTokensPtr header=readLine(in,NULL,'\t');
		LineAndTokensPtr prev=NULL,next=NULL;
		if(header==NULL)
			{
			fprintf(stderr,"%s: No Line found.\n",argv[0]);
			return EXIT_FAILURE;
			}
		nLines++;
		
		while((next=readLine(stdin,prev,'\t'))!=NULL)
			{
			nLines++;
			printf("%s %d\n",next->line,next->tokens_len);
			
			prev=next;
			}
		LineAndTokensFree(header);
		LineAndTokensFree(prev);
		}
	else
		{
		LineAndTokensPtr prev=NULL,next=NULL;
		while((next=readLine(stdin,prev,'\t'))!=NULL)
			{
			nLines++;
			prev=next;
			}
		LineAndTokensFree(prev);
		}
	if(optind+1==argc) fclose(in);

	return EXIT_SUCCESS;
	}

