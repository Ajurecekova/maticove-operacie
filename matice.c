#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

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
	unsigned int i,j;
	int fd;
	unsigned int r;
	unsigned int c;
	float elem;
	
	
	
	if( (fd = open(filename,O_BINARY | O_RDONLY)) < 0 )
		{
		fprintf(stderr, "File access problem_CREATE.\n");
		exit(1);
		}

	lseek(fd,2*sizeof(char),SEEK_SET);
	read(fd,&r,sizeof(unsigned int));
	read(fd,&c,sizeof(unsigned int));
	MAT *a = mat_create_with_type(r,c);
	for(i=0;i<a->rows;i++){
		for(j=0;j<a->cols;j++){
			read(fd,&elem,sizeof(float));
			ELEM(a,i,j)=elem;
		}
	}
	
	close(fd);
	return(a);
		
}

char mat_save(MAT *mat,char *filename){
	int fd;
	char pole[2];
	
	pole[0] = 'M';
	pole[1] = '1';
	
	if( (fd = open(filename,O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0 )
		{
		fprintf(stderr, "File access problem_SAVE.\n");
		exit(1);
		}
	
	write(fd,pole,sizeof(char)*2);
	write(fd,&(mat->rows),sizeof(unsigned int));
	write(fd,&(mat->cols),sizeof(unsigned int));
	write(fd,(mat->elem),sizeof(float)* (mat->rows)* (mat->cols));
	
	
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

void mat_cele(MAT *mat){
	int i;
	
	for (i=0;i<mat->rows*mat->cols;i++){
		mat->elem[i]=rand()%6;
	}
}

void mat_print(MAT *mat){
	int i;
	printf("riadky: %d\n",mat->rows);
	printf("stlpce: %d\n",mat->cols);
	puts("");
	for (i=1;i<=(mat->rows)*(mat->cols);i++){
		printf("%5.2f ",mat->elem[i-1]);
		if (i!=0 && i%(mat->cols)==0)
		printf("\n");
	}
}

char mat_division (MAT *a, MAT *b ,MAT *c){
	int i,j,k,l,m,t;
	MAT *p = mat_create_with_type(b->rows*b->cols,a->cols*b->cols);
	
	for(i=0;i<a->rows;i++){
		t=0;
		for(j=0;j<a->cols;j++){
			t+=ELEM(a,i,j);
		}
		if (t==0)
		return 0;
	}
	for(i=0;i<a->cols;i++){
		t=0;
		for(j=0;j<a->rows;j++){
			t+=ELEM(a,j,i);
		}
		if (t==0)
		return 0;
	}
	
//matica na vypocet sustavy rovnic
	for (i=0;i<p->rows;i++){
		for (j=0;j<p->cols;j++){
			ELEM(p,i,j)=0;
		}
	}
	
	for(k=0;k<a->rows;k++){
		for (i=0;i<b->cols;i++){
			for (j=0;j<c->rows;j++){
				ELEM(p,(((b->cols)*k)+i),j*(b->cols)+i) = ELEM(a,k,j);
			}
		}
	}

    printf("\nMatica p:\n");
    mat_print(p);

//ak je nula na diagonale	
   float temp=0;
   for(i=0; i<p->rows; i++){
       if(ELEM(p,i,i)==0){
	   for(j=0; j<p->cols; j++){
	       if(j==i) continue;
	       if(ELEM(p,j,i) !=0 && ELEM(p,i,j)!=0){
		   for(k=0; k<p->rows; k++){
		       temp = ELEM(p,j,k);
		       ELEM(p,j,k) = ELEM(p,i,k);
		       ELEM(p,i,k) = temp;
		   }
		   temp = b->elem[j];
		   b->elem[j] = b->elem[i];
		   b->elem[i] = temp;
		   break;
	       }
	   }
       }
   }
//diagonalna matica	
	
   for(k=0; k<p->cols; k++){
       for(i=k+1; i<p->cols; i++){
		   float M = ELEM(p,i,k) / ELEM(p,k,k);
		   for(j=k; j<p->cols; j++){
		       ELEM(p,i,j) -= M * ELEM(p,k,j);
		   }
	   b->elem[i] -= M*b->elem[k];
       }
	   }
	
    printf("\nMatica p:\n");
    mat_print(p);
	
	for(k=p->cols-1; k>=0; k--){
	    for(i=k-1; i>=0; i--){
			float B = ELEM(p,i,k) / ELEM(p,k,k);
			for(j=p->cols-1; j>=k; j--){
			    ELEM(p,i,j) -= B * ELEM(p,k,j);
			   }
		    b->elem[i] -= B*b->elem[k];
        }
	}
	
    printf("\nMatica p:\n");
    mat_print(p);

    printf("\nMatica b:\n");
    mat_print(b);

//vypocet prvkov matice c
	for (i=0;i<p->rows;i++){
		for (j=0;j<p->cols;j++){
			if(i==j){
				c->elem[i] = b->elem[i]/ELEM(p,i,j);
			}
		}
	}
}

int main(){
	srand(time(NULL));
	//MAT *a = mat_create_by_file("matica.bin");
	MAT *a = mat_create_with_type(3,3);
	MAT *b = mat_create_with_type(3,3);
	MAT *c = mat_create_with_type(3,3);
	mat_cele(a);
	mat_print(a);
	mat_cele(b);
	mat_print(b);
	mat_division(a,b,c);
	//mat_random(a);
	//mat_print(a);
	//mat_save(a,"matica.bin");
	
	//MAT *b = mat_create_by_file("matica.bin");
	//mat_print(b);
	mat_print(c);
//	mat_destroy(a);
//	mat_destroy(b);
//	mat_destroy(c);
}
