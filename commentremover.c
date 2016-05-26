/*
 ============================================================================
 Name        : commentremover.c
 Author      : Serguei Khasimkhanov
 Version     : 1.0
 Copyright   : You can copy all you like. You can only share if a) you share
				for free and credit me, or b) you share for money and give
				me 20% of revenue.
 Description : Prints java/js/c code from stdin without printing the comments
 Problems	 :
 ============================================================================
 */

#include <stdio.h>
#include <commentremover.h>

//shift 2-character buffer left by one
int shift(char * c) {
	c[0] = c[1];
	return 0;
}

//parse 2-character buffer for what to do
int parse(char * c) {
	if (c[1] == '\'') {
		return SINGLEQUOTATIONMARK;
	}
	else if (c[1] == '"') {
		return DOUBLEQUOTATIONMARK;
	}
	else if (c[1] == '/') {
		if (c[0] == '/') {
			return LINECOMMENT;
		}
		else if (c[0] == '*') {
			return BIGCOMMENTEND;
		}
		else {
			return POSSIBLECOMMENT;
		}
	}
	else if (c[1] == '*') {
		if (c[0] == '/') {
			return BIGCOMMENTSTART;
		}
		else {
			return POSSIBLECOMMENT;
		}
	}
	else {
		if (c[0] == '/') {
			return COMMENTSCARE;
		}
	}
	return NOCOMMENT;
}

int main(int argc, char** argv) {
	char c[2];
	int result;
	int inBigComment = 0; //Value of how deep in a big comment you are.
							//True for all but 0.

	for (c[0] = '\0'; (c[1] = getc(stdin)) != EOF;) {
		result = parse(c);

		if (result == SINGLEQUOTATIONMARK) {
			if (!inBigComment) {
				printf("%c", c[1]);
				while ((c[1] = getc(stdin)) != '\'') {
					if (c[1] == EOF)
						return 0;
					printf("%c", c[1]);
				}
				printf("%c", c[1]);
				c[0] = c[1];
			}
		} else if (result == DOUBLEQUOTATIONMARK) {
			if (!inBigComment) {
				printf("%c", c[1]);
				while ((c[1] = getc(stdin)) != '"') {
					if (c[1] == EOF)
						return 0;
					printf("%c", c[1]);
				}
				printf("%c", c[1]);
				c[0] = c[1];
			}
		}
		else if (result == NOCOMMENT) {
			if (!inBigComment)
				printf("%c", c[1]);
			shift(c);

		}
		else if (result == POSSIBLECOMMENT) {
			shift(c);
		}
		else if (result == COMMENTSCARE) {
			if (!inBigComment)
				printf("%c%c", c[0], c[1]);
			shift(c);
		}
		else if (result == LINECOMMENT) {
			while ((c[1] = getc(stdin)) != '\n') {
				//do nothing
				if (c[1] == EOF) return 0;
			}
			c[0] = '\0';
		}
		else if (result == BIGCOMMENTSTART) {
			inBigComment++; //one level deeper into a big comment
		}
		else if (result == BIGCOMMENTEND) {
			if (inBigComment) {
				inBigComment--; //one level out of a big comment

				// do not shift here, as then */* would be interpreted as
				// both a comment end and a comment start
			}
			else {
				printf("%c%c", c[0], c[1]);
			}

		}

	}

	return 0;
}
