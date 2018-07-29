/*
The MIT License (MIT)

Copyright (c) 2018 Pierre Lindenbaum

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

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_BLACK "\x1b[0m"
#define ANSI_COLOR_RESET ANSI_COLOR_BLACK

#define OUT_OF_MEMORY do { fprintf(stderr,"%s: %d : OUT_OF_MEMORY.\n",__FILE__,__LINE__); exit(EXIT_FAILURE);} while(0)

#define VERSION_NO  "1.0.1"

typedef struct LineAndTokens_t
	{
	char*	line;
	size_t	line_capacity;
	size_t	line_len;
	size_t*	tokens;
	size_t	tokens_capacity;
	size_t	tokens_len;
	} LineAndTokens,*LineAndTokensPtr;


#define TokensSize(ptr) (1 + (ptr)-> tokens_len )
#define TokensAt(ptr,index) (char*)(index == 0UL ? (ptr)-> line :  &((ptr)-> line[ (ptr)->tokens[ index -1 ] ]))

#define PRINT_ROW(format) fprintf(out,"%s" format " %zu%s\n",(is_a_tty?ANSI_COLOR_CYAN:""),nLines,(is_a_tty?ANSI_COLOR_RESET:""))
#define START_ROW PRINT_ROW(">>>")
#define END_ROW PRINT_ROW("<<<")

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
			c=0;
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
	fprintf(out,"Version: " VERSION_NO  "\nCompilation:\n\t%s %s\n",__DATE__,__TIME__);
	fputs("Author:\n\tPierre Lindenbaum PhD @yokofakun\n",out);
	fputs("WWW:\n\thttps://github.com/lindenb/verticalize\n",out);
	fputs("Usage:\n\tverticalize (option) (file|stdin)\n",out);
	fputs("Options:\n"
		"\t-h help (this screen)\n"
		"\t-v print version and exit\n",out);
	fputs("\t-n first line is NOT header\n",out);
	fputs("\t-d (char) delimiter. Default is tabulation.\n",out);
	fputs("\n",out);
	}

int main(int argc,char** argv)
	{
	char delim='\t';
	size_t nLines=0;
	int c,first_line_header=1;
	FILE* in=stdin;
	FILE* out=stdout;
	int is_a_tty=1;
	size_t i;
	char out_format[200];
	
	while ((c = getopt (argc, argv, "vhnd:")) != -1)
	 {
         switch (c)
           	{
		case 'v': fputs(VERSION_NO "\n",stdout);return EXIT_SUCCESS;
           	case 'h': usage(stderr);return EXIT_SUCCESS;
           	case 'n': first_line_header=0;break;
           	case 'd':
           		{
           		if(strcmp(optarg,"\\t")==0)
		   		{
		   		delim='\t';
		   		}
		   	else if(strlen(optarg)!=1)
		   		{
		   		fprintf (stderr, "Bad delimiter. Expected one character.\n");
               			return EXIT_FAILURE;
		   		}
		   	else
		   		{
		   		delim=optarg[0];
		   		}
           		break;
           		}
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
	
	/* someone is redirecting me elsewhere ? */
	is_a_tty=isatty(fileno(out));
	
	if(first_line_header)
		{
		LineAndTokensPtr header=readLine(in,NULL,delim);
		LineAndTokensPtr prev=NULL,next=NULL;
		if(header==NULL)
			{
			fprintf(stderr,"%s: No header line found.\n",argv[0]);
			return EXIT_FAILURE;
			}
		nLines++;
		
		/* determine max column size */
		if(is_a_tty)
			{
			size_t max_column_length=1;
			for(i=0;i< TokensSize(header);++i)
				{
				size_t len=strlen(TokensAt(header,i));
				if(len > max_column_length) max_column_length=len;
				}
			sprintf(out_format,
				ANSI_COLOR_MAGENTA
				"$%%-03zu"
				ANSI_COLOR_RESET
				" "
				ANSI_COLOR_GREEN
				"%%%zus"
				ANSI_COLOR_RESET
				" : %%s\n",max_column_length);
			}
		else
			{
			sprintf(out_format,"$%%zu%c%%s%c%%s\n",delim,delim);
			}
		
		while((next=readLine(in,prev,delim))!=NULL)
			{
			if(feof(in) && next->line_len==0UL) break;
			nLines++;

			
			if(nLines>1) fputc('\n',out);
			
			START_ROW;
			for(i=0;i< TokensSize(header) && i< TokensSize(next);++i)
				{
				fprintf(out,out_format,i+1,TokensAt(header,i),TokensAt(next,i));
				}
			if( TokensSize(header) < TokensSize(next) )
				{
				for(i=TokensSize(header);i< TokensSize(next);++i)
					{
					fprintf(out,out_format,i+1,"???",TokensAt(next,i));
					}
				}
			else if( TokensSize(header) > TokensSize(next) )
				{
				for(i=TokensSize(next);i< TokensSize(header);++i)
					{
					fprintf(out,out_format,i+1,TokensAt(header,i),"???");
					}
				}
			
			
			prev=next;
			if(END_ROW<0) break;
			}
		LineAndTokensFree(header);
		LineAndTokensFree(prev);
		}
	else
		{
		size_t header_count=0;
		LineAndTokensPtr prev=NULL,next=NULL;
		while((next=readLine(in,prev,delim))!=NULL)
			{
			if(feof(in) && next->line_len==0UL) break;
			nLines++;
			if(nLines==1)
				{
				header_count=TokensSize(next);
				if(is_a_tty)
					{
					sprintf(out_format,
						ANSI_COLOR_MAGENTA
						"$%%-03zu"
						ANSI_COLOR_RESET
						" : %%s\n"
						);
					}
				else
					{
					sprintf(out_format,"$%%zu%c%%s\n",delim);
					}
				}
			if(nLines>1) fputc('\n',out);

			START_ROW;
			for(i=0;i< TokensSize(next);++i)
				{
				fprintf(out,out_format,i+1,TokensAt(next,i));
				}
			
			if( header_count > TokensSize(next) )
				{
				for(i=TokensSize(next);i< header_count;++i)
					{
					fprintf(out,out_format,i+1,"???");
					}
				}
			prev=next;
			if(END_ROW<0) break;
			}
		LineAndTokensFree(prev);
		}
	if(optind+1==argc) fclose(in);

	return EXIT_SUCCESS;
	}
