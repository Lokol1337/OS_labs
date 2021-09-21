#include <dirent.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <dirent.h>
#include <stddef.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>



int main (int argc , char** argv){
int flag;
DIR *dir;
struct dirent *curDir;
struct stat statFile;
int dirFlag = 0;
int count = 1;
int countDir = 0;
while(count != argc){
	if(argv[count][0] != '-'){
		dir = opendir(argv[count]);
		dirFlag = 1;
		countDir = count;
		break;
	}
	else{
		dir = opendir("./");
	}
	count++;
}
if(dirFlag == 0)
	dir = opendir("./");


if (dir != NULL){
	if((flag = getopt(argc,argv,"l")) != -1){
		if(dirFlag != 0){
			char *bufTotal = malloc(strlen(argv[countDir]));
			for(int i = 0;i < strlen(argv[countDir])-1; i++){
				bufTotal[i] = argv[countDir][i];	
			}
			stat(bufTotal,&statFile);
			free(bufTotal);
			printf("total %ld\n", statFile.st_blocks);
		}
		else{
			stat("./", &statFile);
			printf("total %ld\n", statFile.st_blocks);
		}	
		while (curDir = readdir(dir)){
			if(curDir->d_name[0] != '.'){
				if(dirFlag == 1){
					char *bufDir = malloc(strlen(argv[countDir]) + strlen(curDir->d_name) + 1);
					strcpy(bufDir,argv[countDir]);
					strcat(bufDir,curDir->d_name);
					stat(bufDir,&statFile);
					free(bufDir);
				}
				else{
					stat(curDir->d_name,&statFile);
				}
					
					switch(S_IFMT & statFile.st_mode){
						case S_IFLNK: printf("l"); break;
						case S_IFBLK: printf("b"); break;
						case S_IFDIR: printf("d"); break;
						case S_IFCHR: printf("c"); break;
						case S_IFSOCK: printf("s"); break;
						case S_IFIFO: printf("p"); break; 
						default: printf("-"); break;
					}
					
					if(S_IRUSR & statFile.st_mode) printf("r"); else printf("-");
                                        if(S_IWUSR & statFile.st_mode) printf("w"); else printf("-");
                                        if(S_IXUSR & statFile.st_mode) printf("x"); else printf("-");

                                        if(S_IRGRP & statFile.st_mode) printf("r"); else printf("-");
                                        if(S_IWGRP & statFile.st_mode) printf("w"); else printf("-");
                                        if(S_IXGRP & statFile.st_mode) printf("x"); else printf("-");

                                        if(S_IROTH & statFile.st_mode) printf("r"); else printf("-");
                                        if(S_IWOTH & statFile.st_mode) printf("w"); else printf("-");
                                        if(S_IXOTH & statFile.st_mode) printf("x"); else printf("-");
					
					printf(" %d", statFile.st_nlink);					

					struct passwd* user = getpwuid(statFile.st_uid);
					if(user != 0)
						printf(" %s", user->pw_name);
					
					struct group* group = getgrgid(statFile.st_gid);
					if(group != 0)
						printf(" %s", group->gr_name);
					
					printf(" %ld", statFile.st_size);

					switch(localtime(&statFile.st_ctime)->tm_mon){
						case 0: printf(" Jan"); break;
						case 1: printf(" Feb"); break;
						case 2: printf(" Mar"); break;
						case 3: printf(" Apr"); break;
						case 4: printf(" May"); break;
						case 5: printf(" Jun"); break;
						case 6: printf(" Jul"); break;
						case 7: printf(" Aug"); break;
						case 8: printf(" Sep"); break;
						case 9: printf(" Oct"); break;
						case 10:printf(" Nov"); break;
						case 11:printf(" Dec"); break;
					}

					printf(" %d", localtime(&statFile.st_ctime)->tm_mday);

					time_t nowYear;
					time(&nowYear);
					
					if(localtime(&nowYear)->tm_year != localtime(&statFile.st_ctime)->tm_year)
						printf(" %d", localtime(&statFile.st_ctime)->tm_year + 1900);
						
					else{
						if(localtime(&statFile.st_ctime)->tm_hour/10 == 0)
                                                        printf(" 0%d:", localtime(&statFile.st_ctime)->tm_hour);
                                                else
                                                        printf(" %d:", localtime(&statFile.st_ctime)->tm_hour);

						if(localtime(&statFile.st_ctime)->tm_min/10 == 0)	
							printf("0%d", localtime(&statFile.st_ctime)->tm_min);
						else
							printf("%d", localtime(&statFile.st_ctime)->tm_min);
					}	
					
					printf(" %s\n",curDir->d_name);					
				
			}
       				
		}
		printf("\n");
		closedir(dir);
	}
	else{
		while (curDir = readdir(dir)){
                        if(curDir->d_name[0] != '.')
                                printf("%s  ",curDir->d_name);
                }
                printf("\n");
                closedir(dir);	
	}
}




return 0;
}
