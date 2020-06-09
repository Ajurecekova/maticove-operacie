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
	unsigned int r;
	unsigned int c;
	float *elem;
	
	a =(MAT*)malloc(sizeof(MAT));
	if (a==0)
		return 0;
	elem = (float*)malloc(sizeof(float)*b*d);
	if (elem == 0){
		free(a);
		return 0;
	}
	
	if( (fd = open(filename, O_RDONLY)) < 0 )
		{
		fprintf(stderr, "File access problem.\n");
		exit(1);
		}

	lseek(fd,2*sizeof(char),SEEK_SET);
	read(fd,&r,sizeof(unsigned int));
	printf("%d\n",r);
	a->rows = r;

	read(fd,&c,sizeof(unsigned int));
	a->cols = c;
	
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
	
	if( (fd = open(filename, O_WRONLY | O_CREAT)) < 0 )
		{
		fprintf(stderr, "File access problem.\n");
		exit(1);
		}
	
	write(fd,pole,sizeof(char)*2);
	write(fd,&mat,(MAT*)malloc(sizeof(MAT)));
	
	
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
	printf("%d\n",mat->rows);
	printf("%d\n",mat->cols);
	for (i=1;i<=mat->rows*mat->cols;i++){
		printf("%f ",mat->elem[i-1]);
		if (i!=0 && i%(mat->cols)==0)
		printf("\n");
	}
}

char mat_division (MAT *a, MAT *b ,MAT *c){
	int i,j,k,l,m;
	MAT *p = mat_create_with_type(b->rows*b->cols,a->cols*b->cols);
	
	
//matica na vypocet sustavy rovnic
	for (i=0;i<p->rows;i++){
		for (j=0;j<p->cols;j++){
			ELEM(p,i,j)=0;
		}
	}
	
	for(k=0;k<b->rows;k++){
		for (i=0;i<b->cols;i++){
			for (j=0;j<c->cols;j++){
				ELEM(p,(((b->cols)*k)+i),j*b->cols+i) = ELEM(a,k,j);
			}
		}
	}
		for (i=1;i<=p->rows*p->cols;i++){
		printf("%f ",p->elem[i-1]);
		if (i!=0 && i%(p->cols)==0)
		printf("\n");
	}
	printf("\n");
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
	
	for (i=1;i<=p->rows*p->cols;i++){
		printf("%f ",p->elem[i-1]);
		if (i!=0 && i%(p->cols)==0)
		printf("\n");
	}
	printf("\n");
	
	for(k=p->cols-1; k>=0; k--){
	    for(i=k-1; i>=0; i--){
			float B = ELEM(p,i,k) / ELEM(p,k,k);
			for(j=p->cols-1; j>=k; j--){
			    ELEM(p,i,j) -= B * ELEM(p,k,j);
			   }
		    b->elem[i] -= B*b->elem[k];
        }
	}
	
	for (i=1;i<=p->rows*p->cols;i++){
		printf("%f ",p->elem[i-1]);
		if (i!=0 && i%(p->cols)==0)
		printf("\n");
	}
	printf("\n");
	for (i=1;i<=b->rows*b->cols;i++){
		printf("%f ",b->elem[i-1]);
	}
//vypocet prvkov matice c
	for (i=0;i<p->rows;i++){
		for (j=0;j<p->cols;j++){
			if(i==j){
				c->elem[i]=b->elem[i]/ELEM(p,i,j);
			}
		}
	}
	printf("\n");
	for (i=1;i<=c->rows*c->cols;i++){
		printf("%f ",c->elem[i-1]);
		if (i!=0 && i%(c->cols)==0)
		printf("\n");
	}
	return c;

}

int main(){
	srand(time(NULL));
	//MAT *a = mat_create_by_file("matica.dat");
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
	//mat_save(a,"matica.txt");
	mat_print(c);
	mat_destroy(a);
	mat_destroy(b);
	mat_destroy(c);
}
