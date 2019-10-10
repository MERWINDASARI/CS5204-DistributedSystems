#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int abs(int d){
        if(d<0) return -1*d;
        else return d;
}

int main (int argc, char *argv[]) {
        int t_id, n_threads, i, j, k;
        int n,m,p;
        time_t start, end; 
        scanf("%d%d%d",&n,&m,&p);

        int **a = (int **)malloc(n * sizeof(int *)); 
        for (i=0; i<n; i++) 
         a[i] = (int *)malloc(m * sizeof(int));
  
        int **b = (int **)malloc(m * sizeof(int *)); 
        for (i=0; i<m; i++) 
         b[i] = (int *)malloc(p * sizeof(int));
         
        int **c = (int **)malloc(n * sizeof(int *)); 
        for (i=0; i<n; i++) 
         c[i] = (int *)malloc(p * sizeof(int));
              
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			a[i][j]=i+j;
		}
	 }
	for(i=0;i<m;i++){
	  	for(j=0;j<p;j++){
	        	b[i][j]=abs(i-j);
		 }
	  }
	for(i=0;i<n;i++){
	  	for(j=0;j<p;j++){
	        	c[i][j]=0;
		 }
	  }
	time(&start);
	{
                  #pragma omp parallel for collapse(2)
                  for (int i=0; i<n; i++){
                    for(int j=0; j<p; j++){       
                      for (int k=0; k<m; k++){
                        c[i][j] += a[i][k] * b[k][j];
                      }
                    }
                  }
        }      
        time(&end);
         
         double time_taken = (double)(end - start); 
         printf("Time taken by openmp program : %lf sec\n",time_taken);
         for(i=0;i<n;i++){
	  	for(j=0;j<p;j++){
	        	c[i][j]=0;
		 }
	  }
         time(&start);
         for (int i=0; i<n; i++){
                for(int j=0; j<p; j++)       
                      for (int k=0; k<m; k++)
                        c[i][j] += a[i][k] * b[k][j];
         }
         time(&end);
         time_taken = (double)(end - start); 
         printf("Time taken by normal program is : %lf sec\n",time_taken);
         for (i=0; i<n; i++) 
         free(a[i]);
         free(a);
         for (i=0; i<m; i++) 
         free(b[i]);
         free(b);
         for (i=0; i<n; i++) 
         free(c[i]);
         free(c);
         /*
                printf("\nMatrix A:\n");
                for (i=0; i<n; i++)
                  {
                        for (j=0; j<m; j++) 
                                printf("%d ", a[i][j]);
                        printf("\n"); 
                  }
                printf("Matrix B:\n");
                for (i=0; i<m; i++)
                  {
                        for (j=0; j<p; j++) 
                                printf("%d ", b[i][j]);
                        printf("\n"); 
                  }

                printf("Matrix C:\n");
                for (i=0; i<n; i++)
                  {
                  for (j=0; j<p; j++) 
                    printf("%d ", c[i][j]);
                  printf("\n"); 
                  }
          */
        
}
