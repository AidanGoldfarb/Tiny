/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
    printf("Running adder!\n");
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE]; //8192
    int n1=0, n2=0;

    /* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
    	p = strchr(buf, '&');
    	*p = '\0';
    	strcpy(arg1, buf);
    	strcpy(arg2, p+1);
    	n1 = atoi(arg1);
    	n2 = atoi(arg2);
    }

    /* Make the response body */
    //printf("Buf: %s\n", content);
    sprintf(content, "Welcome to add.com: ");
    //printf("Buf: %s\n", content);
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    //printf("Buf: %s\n", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", 
	    content, n1, n2, n1 + n2);
    //printf("Buf: %s\n", content);
    sprintf(content, "%sThanks for visiting!\r\n", content);
    //printf("Buf: %s\n", content);
  
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}
/* $end adder */
