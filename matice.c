# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
#include <fcntl.h>
# include <time.h>

typedef struct{
	unsigned int rows;
	unsigned int cols;
	float *elem;
}MAT;

MAT *mat_create_with_type(unsigned int rows, unsigned int cols){
	MAT *a;
	float *elem;
	a =(MAT*)malloc(sizeof(MAT));
	elem = (float*)malloc(sizeof(float)*rows*cols);
	a->rows = rows;
	a->cols = cols;
	a->elem = elem;
	if (a == 0 || elem == 0){
		free(a);
		free(elem);
		return 0;
	}
	else
	return a;
}

MAT *mat_create_by_file(char * filename){
	MAT *a;
	unsigned int b,d,e;
	int fd;
	int i;
	unsigned int *r;
	unsigned int *c;
	float *elem;
	a =(MAT*)malloc(sizeof(MAT));
	elem = (float*)malloc(sizeof(float)*b*d);
	if( (fd = open(O_BINARY | O_RDONLY, filename)) < 0 )
		{
		fprintf(stderr, "File access problem.\n");
		exit(1);
		}
	lseek(fd,2,SEEK_SET);
	b=read(fd,r,1);
	a->rows=b;
	d=read(fd,c,1);
	a->cols=d;
	for (i=0;i<b*d;i++)
		a->elem[i]=read(fd,elem,1);
	if (a == 0 || elem == 0){
		free(a);
		free(elem);
		close(fd);
		return 0;
	}
	else{
	close(fd);
	return(a);}
}

char mat_save(MAT *mat,char *filename){
	int fd;
	char pole[2];
	pole[0] = 'M';
	pole[1] = '1';
	if( (fd = open(filename, O_BINARY | O_RDONLY)) < 0 )
		{
		perror("c1");
		exit(1);
		}
	printf("file opened");
	write(fd,"M",sizeof(char));
	write(fd,"1",sizeof(char));
	write(fd,mat->rows,sizeof(unsigned int));
	write(fd,mat->cols,sizeof(unsigned int));
	write(fd,mat->elem,sizeof(float)* (mat->rows)* (mat->cols));
	close(fd);
	return 0;
}


void mat_destroy(MAT *mat){
	free(mat);
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
		mat->elem[i]=(rand()%3)-1;
	}
	
}
void mat_print(MAT *mat){
	int i;
	for (i=1;i<=mat->rows*mat->cols;i++){
		printf("%f ",mat->elem[i-1]);
		if (i!=0 && i%(mat->cols)==0)
		printf("\n");
	}
}

int main(){
	srand(time(NULL));
	MAT *a = mat_create_with_type(7,7);
	mat_random(a);
	mat_save(a,"matica.dat");
	mat_print(a);
	mat_destroy(a);
}
