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
void mat_destroy(MAT *mat){
	free(mat);
	free(mat->elem);
}

MAT *mat_create_by_file(char * filename){
	unsigned int i,j;
	int fd;
	char pole[2];
	unsigned int r;
	unsigned int c;
	float elem;
    MAT *a;
	
	if( (fd = open(filename,O_BINARY | O_RDONLY)) < 0 )
		return 0;

	if (read(fd,pole,sizeof(char)*2) == 0){
		close(fd);
		return 0;
	}
	if(pole[0]!='M' || pole[1]!='1'){
		close(fd);
		return 0;
	}
	
	if(read(fd,&r,sizeof(unsigned int))== 0){
		close(fd);
		return 0;
	}
	
	if (read(fd,&c,sizeof(unsigned int))== 0){
		close(fd);
		return 0;
	}
	
    a = mat_create_with_type(r,c);
	if (a==NULL){
		close(fd);
		return 0;
	}

	for(i=0;i<a->rows;i++){
		for(j=0;j<a->cols;j++){
			if (read(fd,&elem,sizeof(float))==0){
				mat_destroy(a);
				close(fd);
				return 0;
			}
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
		return 0;
		}
	
	if(write(fd,pole,sizeof(char)*2)==0){
		close(fd);
		return 0;
	}
	if(write(fd,&(mat->rows),sizeof(unsigned int))==0){
		close(fd);
		return 0;
	}
	if(write(fd,&(mat->cols),sizeof(unsigned int))==0){
		close(fd);
		return 0;
	}
	if(write(fd,(mat->elem),sizeof(float)* (mat->rows)* (mat->cols))==0){
		close(fd);
		return 0;
	}
	
	
	close(fd);
	return 0;
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

void mat_rows_swap(MAT *p, MAT *b, int i, int j ,float temp,int k){
	if(ELEM(p,j,i) !=0 && ELEM(p,i,j)!=0){
		for(k=0; k<p->rows; k++){
			temp = ELEM(p,j,k);
			ELEM(p,j,k) = ELEM(p,i,k);
			ELEM(p,i,k) = temp;
		}
        if(ELEM(p,i,i)==0){
            mat_destroy(p);
        }
        temp = b->elem[j];
        b->elem[j] = b->elem[i];
        b->elem[i] = temp;	
    }
}

char nula_na_diagonale(MAT *p,MAT *b){
	int i,j,k;
	float temp;
	   for(i=0; i<p->rows; i++){
	       if(ELEM(p,i,i)==0){
			   for(j=0; j<p->cols; j++){
			   	if(j==i)
			   	continue;
			    mat_rows_swap(p,b,i,j,temp,k);
		   		}
	       }
   }
   	
}

char gaussova_eliminacia(MAT *p,MAT *b){
	int i,j,k;
	float M,B;
	
    //na horny trojuholnikovy
    for(k=0; k<p->cols; k++){
        nula_na_diagonale(p,b);
        for(i=k+1; i<p->cols; i++){
            M = ELEM(p,i,k) / ELEM(p,k,k);
		    for(j=k; j<p->cols; j++){
		       ELEM(p,i,j) -= M * ELEM(p,k,j);
		    }
		    if(ELEM(p,k,k)==0){
				return 0;
			}
		    
	        b->elem[i] -= M*b->elem[k];
        }
    }
	
    //cisla len na diagonale	
	for(k=p->cols-1; k>=0; k--){
	    for(i=k-1; i>=0; i--){
			B = ELEM(p,i,k) / ELEM(p,k,k);
			for(j=p->cols-1; j>=k; j--){
			    ELEM(p,i,j) -= B * ELEM(p,k,j);
			   }
		    b->elem[i] -= B*b->elem[k];
        }
	}

	return 1;
}

char mat_division (MAT *a, MAT *b ,MAT *c){
	int i,j,k,l,m,t;
	float B,M, temp=0;
	MAT *p = mat_create_with_type(b->rows*b->cols,a->cols*b->cols);
	if (p == NULL){
		return 0;
	}
	for(i=0;i<c->rows;i++){
		for(j=0;j<c->cols;j++){
			ELEM(c,i,j)=0;
		}
	}
	if (c->rows!=a->cols || c->cols!=b->cols || c->rows!=b->rows){
		return 0;
		mat_destroy(p);
	}
	for(i=0;i<a->rows;i++){
		t=0;
		for(j=0;j<a->cols;j++){
			if(ELEM(a,i,j)!=0)
				t++;
		}
		if (t==0){
		
		return 0;
		mat_destroy(p);}
	}
	for(i=0;i<a->cols;i++){
		t=0;
		for(j=0;j<a->rows;j++){
			if(ELEM(a,j,i)!=0)
				t++;
		}
		if (t==0){
		
		return 0;
		mat_destroy(p);}
	}
//vytvorenie p	
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
//uprava p   
	nula_na_diagonale(p,b);
	gaussova_eliminacia(p,b);

//vytvorenie c		
	for (i=0;i<p->rows;i++){
		for (j=0;j<p->cols;j++){
			if(i==j){
				c->elem[i] = b->elem[i]/ELEM(p,i,j);
			}
		}
	}

	return 0;
	mat_destroy(p);
}

int main(){
	srand(time(NULL));
	//MAT *a = mat_create_by_file("matica.bin");
	MAT *a = mat_create_with_type(4,4);
	MAT *b = mat_create_with_type(4,4);
	MAT *c = mat_create_with_type(4,4);
	mat_cele(a);
	mat_print(a);
	mat_cele(b);
	mat_print(b);
	mat_division(a,b,c);
	mat_print(c);
	//mat_random(a);
	//mat_print(a);
	//mat_save(a,"matice.bin");
	
	//MAT *b = mat_create_by_file("matice.bin");
	//mat_print(b);
	
	
	mat_destroy(a);
	mat_destroy(b);
	mat_destroy(c);
}
