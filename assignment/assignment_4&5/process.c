#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
struct info{
        int start;
        int end;
        int file_size;
        char type;
};
char * find_file(int n,int file_size,char type){
	int res = (n/file_size)+1;
	
	char index[32];
	sprintf(index,"%d",res);
        char *file_name = malloc(sizeof(char)*40);
        if(type=='a')
	        strcat(file_name,"a_");
	else
	        strcat(file_name,"b_");
	strcat(file_name,index);
	strcat(file_name,".txt");
	return file_name; 
}
int parent_sum=0;
int child_sum=0;
int main_sum=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
void *fun(void* r)
{
        struct info* range=(struct info*)r;
        fflush (stdout);
        printf("%c %d %d\n",range->type,range->start,range->end);
	FILE* fptr;
	const char* file_name;
	for(int i=range->start;i<=range->end;i++){
		if(i%range->file_size==0){
		        fflush (stdout);
		        printf("---%c %d---\n",range->type,i);
		        file_name=(const char*)find_file(i,range->file_size,range->type);
		        if(i!=range->start)  fclose(fptr);
			fptr=fopen(file_name,"r");
			fflush (stdout);
                        printf("%s\n",file_name);
                        printf("%d %d\n", range->start,range->file_size); 
		        if(i==range->start) fseek(fptr, (range->start)%(range->file_size), SEEK_SET);
		}
		int t;
		t=getw(fptr);
		if(range->type=='a'){
                        pthread_mutex_lock( &mutex1 );
                        parent_sum+=t;
                        fflush (stdout);
                        printf("from file %s parent sum t : %u\n",file_name,parent_sum);
                        pthread_mutex_unlock( &mutex1 );
                }
                else{
                        pthread_mutex_lock( &mutex2 );
                        child_sum+=t;
                        fflush (stdout);
                        printf("from file %s child sum t : %u\n",file_name,child_sum);
                        pthread_mutex_unlock( &mutex2 );
                }
	
       }
       
      fclose(fptr);
   // 
       
}

int main(int argc,char* argv[]){

        int a_count = atoi(argv[1]);
        int b_count = atoi(argv[2]);
        int num_count = atoi(argv[3]);
        char afile_name[]="a_";
        
        srand(getpid());
        //creating a type files
        for(int i=0;i<a_count;i++){
	        FILE * fptr;
	        char index[32];
	        sprintf(index,"%d",i+1);
	        char file_name[40]="";
	        strcat(file_name,afile_name);
	        strcat(file_name,index);
	        strcat(file_name,".txt");
	        fptr=fopen(file_name,"w");
	        printf("parent data : ");
	        for(int j=0;j<num_count;j++){
	                int num=rand()%10;
	                main_sum+=num;
	                printf("%d ",num);
	                putw(num,fptr);
	        }
	        printf("| total: %d\n",main_sum);
	        fclose(fptr);
        }
        char bfile_name[]="b_";
        int tem=0;
        for(int i=0;i<b_count;i++){
	        FILE * fptr;
	        char index[32];
	        sprintf(index,"%d",i+1);
	        char file_name[40]="";
	        strcat(file_name,bfile_name);
	        strcat(file_name,index);
	        strcat(file_name,".txt");
	        fptr=fopen(file_name,"w");
	        printf("child data : ");
	        
	        for(int j=0;j<num_count;j++){
	                int num=rand()%10;
	                main_sum+=num; tem+=num;
	                printf("%d ",num);
	                putw(num,fptr);
	        }
	        printf("| total: %d\n",tem);
	        fclose(fptr);
        }
fflush (stdout);
int fd[2];
pipe(fd);
int parent=fork();
int p_thread_count=atoi(argv[4]);
int c_thread_count=atoi(argv[5]);
if(parent){
	int temp=0;
	close(fd[1]);
	int work=(a_count*num_count)/p_thread_count;
	pthread_t* thread_id = (pthread_t*) malloc(sizeof(pthread_t)*p_thread_count);
	for(int i=0;i<p_thread_count;i++){
		struct info *range = malloc(sizeof(range));
		range->start=temp;
		if(i!=p_thread_count-1)
		range->end=temp+work-1;
		else range->end=(a_count*num_count)-1;
		temp+=work;
		range->type='a';
		range->file_size=num_count;
		//printf("pa %d %d\n",range->start,range->end);
		pthread_create(&thread_id[i],NULL,&fun,(void*)range);
	}
	for(int j=0; j < c_thread_count; j++)
        {
         pthread_join( thread_id[j], NULL);
         printf("parent sum : %d\n",parent_sum);
        }
        free(thread_id);
	wait(NULL);
	int child_msg=0;
	read(fd[0],&child_msg,sizeof(child_sum));
	close(fd[0]);
	printf("child_msg : %d  parent_sum : %d\n",child_msg,parent_sum);
	printf("%d\n",parent_sum+child_msg);
	printf("main_sum : %d",main_sum);
}
else if(parent==0){
	int temp=0;
	close(fd[0]);
	int work=(b_count*num_count)/c_thread_count;
	pthread_t* thread_id = (pthread_t*) malloc(sizeof(pthread_t)*p_thread_count);
	for(int i=0;i<c_thread_count;i++){
		struct info *range = malloc(sizeof(range));
		range->start=temp;
		if(i!=p_thread_count)
		range->end=temp+work-1;
		else range->end=temp+(b_count*num_count)-1;
		temp+=work;
		range->type='b';
		range->file_size=num_count;
		//printf("ch -%d %d\n",range->start,range->end);
		pthread_create(&thread_id[i],NULL,&fun,(void*)range);
	}
	for(int j=0; j < c_thread_count; j++)
        {
         pthread_join( thread_id[j], NULL);
         printf("child sum : %d\n",child_sum);
        }
        free(thread_id);
	write(fd[1],&child_sum,sizeof(child_sum));
	close(fd[1]);
	exit(0);
}
else{

        printf("Fork Error\n");

}
return 0;
}
