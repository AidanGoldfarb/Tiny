#include "csapp.h"
int main(void){
	char buf [1];
	// printf("buf: %s \nsize of buf: %ld\n", buf, sizeof(buf));
	//char *str = "Helloooo";
	//Hellooooooooooooo
	char source [5000] = "helloioooooooooooooooooooooooo";
	// printf("size of source: %ld\n", sizeof(source));
	//sscanf(source,"%s",buf);
	//printf("done\n");
	sscanf(source,"%s",buf);
	printf("done\n");
	return 0;
	// printf("buf: %s \nsize of buf: %ld\n", buf, sizeof(buf));

	 // char str[2];
	 // sscanf (argv[1],"%s",str);
	 // return 0;
	// char *str1 = (char *)malloc(100);
	// //str1 = "string------------";
	// char *str2 = "string";
	// //char str3 [17] = {"string\\0overflow"};
	// printf("%s\n", str1);
	// printf("%s\n", str2);
	// //printf("%s\n", str3);
	// strcpy(str1,str2);
	// printf("%s\n", str1);
	// char *str1 = "buffer";
	// char str2[6] = {"overfl"};
	// printf("%s\n", str2);
	// strcpy(str2,str1);
	// printf("%s\n", str2);
	// // char *method = "ZeET";
	// printf("%d\n",strcasecmp(method, "GET"));


	// char *method = "GET blah\n\r";
	// if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
 //        printf("You suck\n");
 //    }
 //    else{
 //    	printf("Success\n");
 //    }
	// if (-1) {                     //line:netp:doit:beginrequesterr
 //        printf("You suck\n");
 //    }
 //    else{
 //    	printf("Success\n");
 //    }

	// char *str = "cgdi-bin";
	// if (!strstr(str, "cgi-bin")) {  /* Static content */ //line:netp:parseuri:isstatic
	// 	printf("YES\n");
 //    }
 //    else{
 //    	printf("NO\n");
 //    }
}