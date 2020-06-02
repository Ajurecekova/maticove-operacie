# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <fnctl.h>
# include <time.h>
typedef struct{
	unsigned int rows;
	unsigned int cols;
	float *elem;
}MAT;

MAT *mat_create_with_type(unsigned int rows, unsigned int cols){
	int *a;
	int *r;
	int *c:
	a = malloc(rows * cols);
	return(a)
}

MAT *mat_create_by_file(char * filename){
	MAT mat
	unsigned int b,d,e;
	int fd;
	if( (fd = open(O_BINARY | O_RDONLY, "filename")) < 0 )
		{
		fprintf(std_err, "File access problem.\n");
		exit(1);
		}
	lseek(fd,2,SEEK_SET);
	b=read(fd,r,1);
	mat.rows=b;
	d=read(fd,c,1);
	mat.cols=c;
	for (i=0;i<b*d;i++)
		mat.elem[i]=read(fd,a,1);
	
}

char mat_save(MAT *mat,char *filename){
	int fd;
	if( (fd = open(O_BINARY | O_RDONLY, "filename")) < 0 )
		{
		fprintf(std_err, "File access problem.\n");
		exit(1);
		}
	write(fd,1	52`)
}


void mat_destroy(MAT *mat){
	free(a);
}

void mat_unit(MAT *mat){
	int *a;
	a=malloc(mat->rows*mat->cols);
	a=&mat;
	for (i=0;i<mat->rows*mat->cols;i++){
		if i%(mat->cols+1)==0
		a[i]=1;
		else a[i]=0;
	
	}
	free(a)
}

void mat_random(MAT *mat){
	int *a;
	a=malloc(mat->rows*mat->cols);
	a=&mat;
	for (i=0;i<mat->rows;i++){
		a[i]=rand()%3-1;
	}
	free(a);
}
void mat_print(MAT *mat){
	int *a;
	a=malloc(mat->rows*mat->cols);
	a=&mat;
	for (i=0;i<mat->rows;i++){
		for (i=0;i<mat->cols;i++){
			printf("%d",a[i][j]);
		}
	}
	free(a);
}
