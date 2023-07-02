#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct file {
	char tasks_name[200];
	int *tasks_value;    
    int tasks_number;  
    int page_num;        
    int *pageTableRandomValues; 
    char *file_values;    
    int tlb[5];
    int enoughSize;
    int indeks;
    int hit;
    int miss;
    int dispatch;
};

int findSize(char file_name[]) {
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("File not found!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    int res = ftell(fp);
    fclose(fp);
    return res;
}

void printPages(char *name, int pageNum, int *pageTableRandomValues ){
    printf("%s\n", name);
    for(int j = 0 ; j < pageNum ; j++){
         printf("%d -> %d\n",j,pageTableRandomValues[j]);
    }
    printf("-----------------\n");

}
void randomNumber(int count, int frame [], char *name, int *pageTableRandomValues) {
    int num;
    for(int i = 0 ; i < count ; i++){
        num = rand() % 250 ;
        if(frame[num] == -1){
            frame[num] = 1;
            pageTableRandomValues[i] = num;
        }
        else
            i--;
    }

}

int main(int argc, char *argv[]){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    int size = 1;
    struct file *fileStruct = (struct file *) malloc(size*sizeof(struct file));
   
    fp = fopen("tasks.txt", "r");
    if(fp == NULL) {
        perror("File not found!");
        exit(1);
    }

    int index = 0;
    int line_num = 0;
    char *p = NULL;
    int tasks_value_count = 1;

    while(getline(&line, &len, fp) != -1) {
        size++;
        fileStruct =  (struct file *)realloc(fileStruct,size*sizeof(struct file));
        index = 1;
        p = strtok(line," ");
        tasks_value_count = 1;
        (fileStruct+line_num)->tasks_value = (int*)malloc(tasks_value_count*sizeof(int));
        while(p != NULL){
            if(index == 1){
                strcpy((fileStruct+line_num)->tasks_name,p);
                index++;
            }
            else {
                (fileStruct+line_num)->tasks_value = (int*)realloc((fileStruct+line_num)->tasks_value, tasks_value_count*sizeof(int));
                (fileStruct+line_num)->tasks_value[index-2] = atoi(p);
                tasks_value_count++;
                (fileStruct+line_num)->tasks_number = tasks_value_count;
                index++;
            }
            p = strtok(NULL," ");
            
        }
        line_num++;
    }
    fclose(fp);
    free(line);

    //printf("----------- %d \n", (fileStruct+1)->tasks_value[5]);

    for(int i = 0; i< line_num; i++){
        int res = findSize((fileStruct+i)->tasks_name);
        if (res != -1)
            printf("Size of the file is %d bytes", res);

        (fileStruct+i)->page_num = ceil(res / 4096)+1;
        (fileStruct+i)->pageTableRandomValues = (int*)malloc((fileStruct+i)->page_num*sizeof(int));
        printf("        total page number:   %d\n", (fileStruct+i)->page_num);
    }


    int frame [250];
    for(int i = 0 ; i < 250 ; i++){
        frame[i] = -1;
    }


    for(int i = 0 ; i < line_num ; i++){
        for(int j = 0 ; j < 5 ; j++){
            (fileStruct+i)->tlb[j] = -1;
        }
    }
    int full = 0;
    int numberOfTask = 0;
    int sum_task_value = 0;
    
    for(int i = 0; i< line_num; i++){
            printf("%d\n",i);

        full = 0;
        for(int j = 0 ; j < 250 ; j++){
            //printf("%d -> %d\n",j,frame[j]);
            if(frame[j] != -1)
                full++;
        }
        if((fileStruct+i)->page_num > 250-full){
            printf("Not enough frame %s!\n", (fileStruct+i)->tasks_name);
            (fileStruct+i)->enoughSize = 0;
        }
        else{
            randomNumber((fileStruct+i)->page_num,frame,(fileStruct+i)->tasks_name, (fileStruct+i)->pageTableRandomValues);
            printPages( (fileStruct+i)->tasks_name,(fileStruct+i)->page_num, (fileStruct+i)->pageTableRandomValues);
            sum_task_value = sum_task_value + (fileStruct+i)->tasks_number -1;
            (fileStruct+i)->indeks = 0;
            (fileStruct+i)->enoughSize = 2;
            numberOfTask++;

        }
    }


    int text = 0;
    int value = 0;

    int girdi = 0;
    while( value < sum_task_value){
        if( (fileStruct+text)-> enoughSize == 2){
            if((fileStruct+text)->indeks < (fileStruct+text)->tasks_number-1){
                for(int i = 0; i< 5; i++){
                    int pageVal = ceil((fileStruct+text)->tasks_value[(fileStruct+text)->indeks]/4096);
                    for(int s = 0; s< 5; s++){
                        if((fileStruct+text)->tlb[s] == pageVal){
                            (fileStruct+text)->hit++;
                            (fileStruct+text)->indeks++;
                            value++;
                            girdi = 1;
                            break; 
                        }
                        if((fileStruct+text)->tlb[s] == -1){
                            (fileStruct+text)->tlb[s] = pageVal;
                            (fileStruct+text)->miss++;
                            (fileStruct+text)->indeks++;
                            value++;
                            girdi =1;
                            break;
                        }
                    }
                    if(girdi != 1){
                        (fileStruct+text)->tlb[((fileStruct+text)->indeks)%5] = pageVal;
                        (fileStruct+text)->miss++;
                        (fileStruct+text)->indeks++;
                        value++;
                    }
                    girdi = 0;

                    if((fileStruct+text)->indeks >= (fileStruct+text)->tasks_number-1){
                        break;
                    }
                }
            }
            else{
                text++;
                continue;
            }
            (fileStruct+text)->dispatch++;
        }
        text++;
        if(text == line_num){
            text = 0;
        }
    }

    for(int i = 0; i< line_num; i++){
        printf("%s : HIT-> %d    MISS-> %d    DISPATCH-> %d\n", (fileStruct+i)->tasks_name,(fileStruct+i)->hit,(fileStruct+i)->miss,(fileStruct+i)->dispatch );
    }


    return 0;
}
