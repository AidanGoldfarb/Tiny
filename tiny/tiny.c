/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh 
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
	        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
	                    port, MAXLINE, 0);
	        printf("Accepted connection from (%s, %s)\n", hostname, port);
		doit(connfd);                                             //line:netp:tiny:doit
		Close(connfd);                                            //line:netp:tiny:close
    }
}
/* $end tinymain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    //setvbuf(stdout, NULL, _IONBF,0);

    /* Read request line and headers */
    Rio_readinitb(&rio, fd); //sets up empty read buffer
    if (!Rio_readlineb(&rio, buf, MAXLINE)){  //line:netp:doit:readrequest
    	printf("62");
        return;
    }
    //printf("%s\n", buf);
    sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
    // printf("method: %s\n", method);
    // printf("uri: %s\n", uri);
    // printf("version: %s\n", version);
    if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
    	printf("failed 'GET' test\n");
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return; //0x5555555578c3
    }
    printf("78\n");                                                   //line:netp:doit:endrequesterr
    read_requesthdrs(&rio);                              //line:netp:doit:readrequesthdrs
    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs);       //line:netp:doit:staticcheck
    if (stat(filename, &sbuf) < 0) {                     //line:netp:doit:beginnotfound //STAT fills sbuf. Need to pass in regular file to sbuf to beat ISREG
    	printf("SBUF not filled with %s\n", uri);
		clienterror(fd, filename, "404", "Not found",
			    "Tiny couldn't find this file");
		return;
    }                                                    //line:netp:doit:endnotfound
    printf("88\n");
    if (is_static) { /* Serve static content */
    	printf("UR_static\n");          
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { //line:netp:doit:readable
			printf("Fucked by clienterror\n");
			printf("S_ISREG: %d\n", S_ISREG(sbuf.st_mode));
		    clienterror(fd, filename, "403", "Forbidden",
				"Tiny couldn't read the file");
		    return;
		}
		printf("95\n");
		serve_static(fd, filename, sbuf.st_size);        //line:netp:doit:servestatic
    }
    else { /* Serve dynamic content */
    	printf("UR_dynamic!\n");
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { //line:netp:doit:executable
		    clienterror(fd, filename, "403", "Forbidden",
				"Tiny couldn't run the CGI program");
		    return;
		}
	printf("104\n");
	serve_dynamic(fd, filename, cgiargs);            //line:netp:doit:servedynamic
    }
}
/* $end doit */

/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
void read_requesthdrs(rio_t *rp) 
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
		Rio_readlineb(rp, buf, MAXLINE);
		printf("%s", buf);
    }
    printf("You beat this level\n");
    return;
}
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
int parse_uri(char *uri, char *filename, char *cgiargs) 
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
		strcpy(cgiargs, "");                             //line:netp:parseuri:clearcgi
		strcpy(filename, ".");                           //line:netp:parseuri:beginconvert1
		strcat(filename, uri);                           //line:netp:parseuri:endconvert1
		if (uri[strlen(uri)-1] == '/')                   //line:netp:parseuri:slashcheck
		    strcat(filename, "home.html");               //line:netp:parseuri:appenddefault
		return 1;
    }
    else {  /* Dynamic content */                        //line:netp:parseuri:isdynamic
		ptr = index(uri, '?');                           //line:netp:parseuri:beginextract
		if (ptr) {
		    strcpy(cgiargs, ptr+1);
		    *ptr = '\0';
		}
		else 
		    strcpy(cgiargs, "");                         //line:netp:parseuri:endextract
		strcpy(filename, ".");                           //line:netp:parseuri:beginconvert2
		printf("Copying buffer of size %ld into filename (size: %ld)\n", sizeof(uri), sizeof(filename));
		strcat(filename, uri);                           //line:netp:parseuri:endconvert2
		return 0;
    }
}
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize)
{
	//printf("In serve_static, filename: %s\nfilesize: %d\n", filename, filesize);
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);    //line:netp:servestatic:getfiletypee
    printf("filetype: %s\n\n", filetype);
    printf("[1]buf: %s\n", buf);
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); //line:netp:servestatic:beginserve
    printf("[2]buf: %s\n", buf);
    Rio_writen(fd, buf, strlen(buf));
    printf("[3]buf: %s\n", buf);
    sprintf(buf, "Server: Tiny Web Server\r\n");
    printf("[4]buf: %s\n", buf);
    Rio_writen(fd, buf, strlen(buf));
    printf("[5]buf: %s\n", buf);
    sprintf(buf, "Content-length: %d\r\n", filesize);
    printf("[6]buf: %s\n", buf);
    Rio_writen(fd, buf, strlen(buf));
    printf("[7]buf: %s\n", buf);
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype); //OVERFLOW (14 byte prefix)
    printf("[8]buf: %s\n", buf);
    Rio_writen(fd, buf, strlen(buf));    //line:netp:servestatic:endserve
    printf("[9]buf: %s\n", buf);
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0); //line:netp:servestatic:open
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0); //line:netp:servestatic:mmap
    Close(srcfd);                       //line:netp:servestatic:close
    Rio_writen(fd, srcp, filesize);     //line:netp:servestatic:write
    Munmap(srcp, filesize);             //line:netp:servestatic:munmap
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) 
{
    if (strstr(filename, ".html"))
		strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
		strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
		strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg")){
		strcpy(filetype, "image/jpeg");
		// char lethal[8180];
		// for(int i = 0; i<8180; i++){
		// 	lethal[i] = *"0";
		// }
		// strcpy(filetype, lethal);
	}
    else
		strcpy(filetype, "text/plain");
}  
/* $end serve_static */

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs) 
{
	printf("In dynamic, filename: %s\ncgiargs: %s\n", filename, cgiargs);
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    printf("ihere1\n");
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    printf("ihere2\n");
	if (Fork() == 0) { /* Child */ //line:netp:servedynamic:fork
    	printf("in child\n");
		/* Real server would set all CGI vars here */
		printf("setenv: %d\n",setenv("QUERY_STRING", cgiargs, 1)); //line:netp:servedynamic:setenv
		//Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ //line:netp:servedynamic:dup2
		Execve(filename, emptylist, environ); /* Run CGI program */ //line:netp:servedynamic:execve
    }
    printf("not in child\n");
    Wait(NULL); /* Parent waits for and reaps child */ //line:netp:servedynamic:wait
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
	printf("in clienterror\n");
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    //printf("buf: %s\n", buf);
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    printf("should see this\n");
    //Rio_writen(fd, buf, strlen(buf)); //broken pipe
    printf("should not see this\n");
    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    //Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    //Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    //Rio_writen(fd, buf, strlen(buf));
    printf("buf right before overflow: %s\n", buf);
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause); //OVERFLOW
    printf("buf after: %s\n", buf);
    //Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    //Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */
