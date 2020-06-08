#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif
#define ELEM(M,r,c) (M->elem[(M->cols)*r+c])

typedef struct{
	unsigned int rows;
	unsigned int cols;
	float *elem;
}MAT;

MAT *mat_create_with_type(unsigned int rows, unsigned int cols){
	MAT *a;
	float *elem;
	
	a =(MAT*)malloc(sizeof(MAT));
	if (a==0)
		return 0;
	elem = (float*)malloc(sizeof(float)*rows*cols);
	if (elem == 0){
		free(a);
		return 0;
	}
	
	a->rows = rows;
	a->cols = cols;
	a->elem = elem;
	
	return a;
}

MAT *mat_create_by_file(char * filename){
	MAT *a;
	unsigned int b,d;
	int fd;
	unsigned int *r;
	unsigned int *c;
	float *elem;
	
	a =(MAT*)malloc(sizeof(MAT));
	if (a==0)
		return 0;
	elem = (float*)malloc(sizeof(float)*b*d);
	if (elem == 0){
		free(a);
		return 0;
	}
	
	if( (fd = open(filename, O_BINARY | O_RDONLY)) < 0 )
		{
		fprintf(stderr, "File access problem.\n");
		exit(1);
		}
	
	r=&b;
	c=&d;
	lseek(fd,2,SEEK_SET);
	b=read(fd,r,sizeof(unsigned int));
	a->rows = *r;

	d=read(fd,c,sizeof(unsigned int));
	a->cols = *c;
	
	read(fd,elem,sizeof(float)*(a->cols)*(a->rows));
	a->elem = elem;
	close(fd);
	return(a);
		
}

char mat_save(MAT *mat,char *filename){
	int fd;
	char pole[2];
	
	pole[0] = 'M';
	pole[1] = '1';
	
	if( (fd = open(filename,O_BINARY | O_WRONLY | O_CREAT)) < 0 )
		{
		fprintf(stderr, "File access problem.\n");
		exit(1);
		}
	
	write(fd,pole,sizeof(char)*2);
	write(fd,&(mat->rows),sizeof(unsigned int));
	write(fd,&(mat->cols),sizeof(unsigned int));
	write(fd,&(mat->elem),sizeof(float)* (mat->rows)* (mat->cols));
	
	close(fd);
	return 0;
}


void mat_destroy(MAT *mat){
	free(mat);
	free(&(mat->elem));
}

void mat_unit(MAT *mat){
	int i,j;
	
	for (i=0;i<mat->rows*mat->cols;i++){
		if (i%(mat->cols+1)==0)
		mat->elem[i]=1;
		else mat->elem[i]=0;
	
	}
	
}

void mat_random(MAT *mat){
	int i;
	
	for (i=0;i<mat->rows*mat->cols;i++){
		mat->elem[i]=(((float)rand()/(float)(RAND_MAX))*2)-1;
	}
	
}
void mat_print(MAT *mat){
	int i;
	printf("%d\n",mat->rows);
	printf("%d\n",mat->cols);
	for (i=1;i<=mat->rows*mat->cols;i++){
		printf("%f ",mat->elem[i-1]);
		if (i!=0 && i%(mat->cols)==0)
		printf("\n");
	}
}

int main(){
	srand(time(NULL));
	MAT *a = mat_create_by_file("matica.dat");
	//MAT *a = mat_create_with_type(6,4);
	//mat_random(a);
	//mat_save(a,"matica.dat");
	mat_print(a);
	mat_destroy(a);
}
