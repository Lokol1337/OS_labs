#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void input(char* pathArch,char* pathFile){
	FILE *arch, *file;
	
	long long int sizeFile = 0;
	char *buf = NULL;
	struct stat statFile;
	mode_t curmode;
	int flag = 0;
	long long int bufNameSize = 0;
	char* bufName;
	long long int bufCon = 0;
	mode_t bufMode;
	if(((arch = fopen(pathArch, "r")) != NULL)){
		while((fscanf(arch, "%d", &bufNameSize)) != EOF){	
			fgetc(arch);
			bufName = malloc(bufNameSize);
			fread(bufName, 1, bufNameSize,arch);
			fgetc(arch);
			if(strcmp(bufName,pathFile) == 0){
				printf("%s : Файл уже существует в архиве %s\n",pathFile,pathArch);
				flag = 1;
				fclose(arch);
				break;
			}
			fscanf(arch, "%d", &bufMode);
			fgetc(arch);
			fscanf(arch, "%d", &bufCon);
			fgetc(arch);
			for(int i = 0;i < bufCon; i++){
				fgetc(arch);
			}
			
		}
			
	}
	else{
		if(pathArch == 0)
			printf("Введите название архива!\n");
		exit(1);
	}

	if(flag == 0){
		if(((arch = fopen(pathArch, "a")) != NULL)){
			if(((file = fopen(pathFile, "r")) != NULL)){
				fseek (file, 0, SEEK_END);
				sizeFile = ftell(file);
				fseek (file, 0, SEEK_SET);
				buf = malloc (sizeFile);
				if(buf)
				{
					fread (buf, 1, sizeFile, file);
				}
		
				stat(pathFile, &statFile);
				
				fprintf(arch, "%d\n", strlen(pathFile));
				fprintf(arch, "%s\n", pathFile);
				fprintf(arch, "%d\n", statFile.st_mode);
				fprintf(arch, "%d\n", sizeFile);
				fprintf(arch, "%s", buf);
	
				fclose (file);	
			}
			else{
				printf("Не удалось открыть файл %s.\n" , pathFile);
			}
			fclose(arch);
		}
	
		else
			exit(1);
	}
}

void extract(char* pathArch, char* pathFile){
	FILE *arch, *file;
	char* after = 0;
	char* before = 0;
	long long int beforeSize = 0;
	long long int afterSize = 0;
	long long int fileSize = 0;
	long long int delSize = 0;

	mode_t bufMode;
	long long int bufNameSize = 0;
	char* bufName;
	long long int bufConSize = 0;
	char* bufCon;
	int flag = 0;
	if((arch = fopen(pathArch, "r")) != NULL){
		fseek (arch, 0, SEEK_END);
		fileSize = ftell(arch);
		fseek (arch, 0, SEEK_SET);
		while(!feof(arch)){
			if((fscanf(arch, "%d", &bufNameSize)) == EOF){
				break;
			}
			long long int buf = bufNameSize;
			int count = 0;
			while(buf!=0){
				count++;
				buf /= 10;
			}
			beforeSize = ftell(arch) - count;
			fgetc(arch);
			bufName = malloc(bufNameSize);
			fread(bufName, 1, bufNameSize, arch);
			fgetc(arch);		
                         
			fscanf(arch, "%d", &bufMode);
			fgetc(arch);          
			fscanf(arch, "%d", &bufConSize);
			fgetc(arch);
			bufCon = malloc(bufConSize);
			fread(bufCon, 1, bufConSize, arch);
			if(strcmp(bufName,pathFile) == 0){
				file = fopen(pathFile, "w");
				chmod(pathFile, bufMode);
				fprintf(file, "%s", bufCon);
				flag = 1;
				fclose(file);
				break;
			}
			bufNameSize = 0;
			bufMode = 0;
			bufConSize = 0;
			bufName = 0;
			
		}
		
		if(flag == 1){
			afterSize = fileSize - ftell(arch);
			delSize = ftell(arch) - beforeSize;
		}		
		else{
			printf("Файл %s не существует в архиве %s.\n",pathFile,pathArch);
			fclose(arch);
			exit(1);
		}
		
	}

	fseek(arch, 0, SEEK_SET);
	
	
	if(beforeSize != 0){
		before = malloc(beforeSize);	
		fread(before, 1, beforeSize, arch);

	}

	for(int i = 0; i < delSize; i++){
		fgetc(arch);
	}
	if(afterSize != 0){
		after = malloc(afterSize);
		fread(after, 1, afterSize, arch);
	}
	
	fclose(arch);
	
	if((arch = fopen(pathArch, "w")) != NULL){
		if(before){
			fwrite(before, beforeSize, 1, arch);
		}
		if(after){;
			fwrite(after, afterSize, 1, arch);
		}
		fclose(arch);
	}
	
	
	
	
	
}

void stats(char* pathArch){
	FILE* arch;
	long long int bufCap = 0;
	int countAll = 0;
	long long int bufNameSize = 0;
	long long int bufCount = 0;
	long long int bufConSize = 0;
	mode_t bufMode;
	char* bufCon;
	char* bufName;
	if(((arch = fopen(pathArch, "r")) !=  NULL)){
		printf("Файлы в архиве: %s\n", pathArch);
		while(!feof(arch)){
			fscanf(arch,"%d",&bufNameSize);
			fgetc(arch);
			bufName = malloc(bufNameSize);	
			fread(bufName, 1, bufNameSize, arch);
			fgetc(arch);
			fscanf(arch, "%d", &bufMode);
			fgetc(arch);
			fscanf(arch, "%d", &bufConSize);
			fgetc(arch);
			bufCon = malloc(bufConSize);
			fread(bufCon, 1, bufConSize, arch);
			if(bufNameSize){
                                bufCount++;
				printf("%d. %s\n",bufCount, bufName);
				bufCap += bufConSize;
                        }
			bufConSize = 0;
			bufNameSize = 0;
			bufMode = 0;
			bufName = 0;
		}
		fclose(arch);
		printf("Всего файлов: %d\nС общим весом: %d\n", bufCount, bufCap);

	}
}

void help(){
	printf("Доступные команды:\n-i: ./main arch -i test.txt\n-e: ./main arch -e test.txt\n-s: ./main arch -s\n");
}

int main(int argc, char** argv){
	int result = 0;
	char* arch;
	
	if(argc > 1){
		if(argv[1][0] != '-')
			arch = argv[1];
	}
	while((result = getopt(argc, argv, "i:e:sh")) != -1){
		switch(result){
			case 'i':
				input(arch, optarg);
				break;
			case 'e':
				extract(arch, optarg);
				break;
			case 's':
				stats(arch);
				break;
			case 'h':
				help();
				break;
			default:
				help();
				break;
		}
	}
	return 0;

}

