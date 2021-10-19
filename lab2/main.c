#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>


int main(int argc, char** argv){
	char* file = NULL;
	int rez;
	int flag = 0;
	char group = ' ';
	char addRem = ' ';
	int r = 0;
	int w = 0;
	int x = 0;
	mode_t changes = 00000; 
	
	if(argv[argc - 1][0] != '-')
		file = argv[argc - 1];
	
	while((rez = getopt(argc,argv,"ugoarwxh")) != -1){
		switch(rez){
			case 'h':
				if(argc != 2){
					printf("Неправильный запрос!\n");
					return 0;
				}		
				printf("Введите Флаг группы, флаг удаления или добавления, и флаги атрибутов.\nПосле введите путь к файлу\n");
				printf("Пример: ./main -uarw test\n");
				return 0;
				break;
			case 'u':
				if(argv[1][1] != 'u'){
					printf("Неправильный запрос!\n");
					return 1;
				}
				else
					group = 'u';
				break;
			case 'g':
				if(argv[1][1] != 'g'){
                                        printf("Неправильный запрос!\n");
					return 1;
				}
                                else
                                        group = 'g';
                                break;
			case 'o':
				if(argv[1][1] != 'o'){
                                        printf("Неправильный запрос!\n");
					return 1;
				}
                                else
                                        group = 'o';
                                break;
			case 'a':
				if(argv[1][2] != 'a' || group == ' ' || addRem != ' '){
					printf("Неправильный запрос!\n");
					return 1;
				}
				else
					addRem = 'a';
				break;
			case 'r':
				if(argv[1][2] == 'r' && group != ' '){
					addRem = 'r';
				}
				else if((argv[1][3] == 'r' || argv[1][4] == 'r' || argv[1][5] == 'r') && group != ' ' && r == 0 && addRem != 'r'){
					if(group == 'u')
						changes += 00400;
					if(group == 'g')
						changes += 00040;
					if(group == 'o')
						changes += 00004;
					r = 1;
				}
				else{
					printf("Неправильный запрос!\n");
					return 1;
				}
				break;
			case 'w':
				if((argv[1][3] == 'w' || argv[1][4] == 'w' || argv[1][5] == 'w') && group != ' ' && w == 0){
					if(group == 'u')
                                                changes += 00200;
                                        if(group == 'g')
                                                changes += 00020;
                                        if(group == 'o')
                                                changes += 00002;	
					w = 1;
				}
				else{
					printf("Неправильный запрос!\n");
					return 1;
				}
				break;
			case 'x':
				if((argv[1][3] == 'x' || argv[1][4] == 'x' || argv[1][5] == 'x') && group != ' ' && x == 0){
                                        if(group == 'u')
                                                changes += 00100;
                                        if(group == 'g')
                                                changes += 00010;
                                        if(group == 'o')
                                                changes += 00001;
					x = 1;
                                }
                                else{
                                        printf("Неправильный запрос!\n");
					return 1;
				}
				break;
	
		}
	}
	

	struct stat stFile;
	if(file != NULL){
		if(access(file,0) == 0){
			stat(file,&stFile);
			if(changes == 00000){
				printf("Неправильный запрос!\n");
				return 1;
			}
		}
		else{
			printf("Файл не существует!\n");
			return 1;
		}
	}
	else{
		printf("Введите название файла!\n");
		return 1;
	}
	
	mode_t curMode = stFile.st_mode;

	if(addRem == 'a')
		chmod(file,changes | curMode);
	else if(addRem == 'r')
		chmod(file,(~changes) & curMode);
	


	





	
	return 0;

}
