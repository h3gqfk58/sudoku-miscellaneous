#include <stdio.h>
#include <iterator>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

int sq[9][9],sa[9][9],sw[9][9],nob;
vector<int> s3D[9][9];
vector<int>::iterator itr_s3D[9][9];

void allp() {
	int i,j,k,l,n[9];

	for(i=0;i<9;i++) {
		for(j=0;j<9;j++) {
			if(!sw[i][j]) {
				for(k=0;k<9;k++) n[k]=0;				
				for(k=0;k<9;k++) if(sw[i][k]) n[sw[i][k]-1]++;			
				for(k=0;k<9;k++) if(sw[k][j]) n[sw[k][j]-1]++;
				for(k=((i/3)*3);k<(((i/3)*3)+3);k++) 
					for(l=((j/3)*3);l<(((j/3)*3)+3);l++) 
						if(sw[k][l]) n[sw[k][l]-1]++;
	
				for(k=0;k<9;k++) if(!n[k]) s3D[i][j].push_back(k+1);
			}
		}
	}
}
int con(int a,int b) {
	int i,j;
	for(i=0;i<9;i++) if( (sw[a][i]==sw[a][b]) && (i!=b) ) return 1;	
	for(i=0;i<9;i++) if( (sw[i][b]==sw[a][b]) && (i!=a) ) return 1;	
	for(i=((a/3)*3);i<(((a/3)*3)+3);i++) 
		for(j=((b/3)*3);j<(((b/3)*3)+3);j++) 
			if( (sw[i][j]==sw[a][b]) && (i!=a) && (j!=b) ) return 1;
	return 0;
}

int nos_limited(int i,int j,int nos_p) {
	if(i==9) return 1;
	
	int I,J,nos=0,nos_c=0;

	if(j==8) { I=i+1; J=0; }
	else { I=i; J=j+1; }	

	if(sw[i][j]) {
		nos_c=nos_limited(I,J,(nos+nos_p));
		nos=nos+nos_c;
	}
	else {
    	for(itr_s3D[i][j] = s3D[i][j].begin(); itr_s3D[i][j] != s3D[i][j].end(); itr_s3D[i][j]++) {														
			sw[i][j]=*itr_s3D[i][j];
			if(con(i,j)) sw[i][j]=0;
			else if( !(nos_c=nos_limited(I,J,(nos+nos_p))) ) sw[i][j]=0;
			else {				
				nos=nos+nos_c;
				if((nos+nos_p)>=2) break;
			}			
		}
		sw[i][j]=0;
	}	
	return nos;	
}
int nos_unlimited(int i,int j) {
	if(i==9) return 1;
	
	int I,J,nos=0,nos_c=0;

	if(j==8) { I=i+1; J=0; }
	else { I=i; J=j+1; }	

	if(sw[i][j]) {
		nos_c=nos_unlimited(I,J);
		nos=nos+nos_c;
	}
	else {
    	for(itr_s3D[i][j] = s3D[i][j].begin(); itr_s3D[i][j] != s3D[i][j].end(); itr_s3D[i][j]++) {														
			sw[i][j]=*itr_s3D[i][j];			
			if(con(i,j)) sw[i][j]=0;
			else if( !(nos_c=nos_unlimited(I,J)) ) sw[i][j]=0;
			else nos=nos+nos_c;			
		}
		sw[i][j]=0;
	}	
	return nos;	
}
int get_answer(int i,int j) {
	if(i==9) return 0;
	
	int I,J;

	if(j==8) { I=i+1; J=0; }
	else { I=i; J=j+1; }	

	if(sw[i][j]!=0) {
		if(get_answer(I,J)) return 1;		
	}
	else {
    	for(itr_s3D[i][j] = s3D[i][j].begin(); itr_s3D[i][j] != s3D[i][j].end(); itr_s3D[i][j]++) {														
			sw[i][j]=*itr_s3D[i][j];
			if(con(i,j)) sw[i][j]=0;
			else if(get_answer(I,J)) sw[i][j]=0;
			else break;			
		}
		if(sw[i][j]==0) return 1;
	}	
	return 0;	
}

void print_qa() {
	int i,j;
	printf("\n         QUESTION                     ANSWER \n");
	printf(" -------------------------    -------------------------\n");
	for(i=0;i<9;i++) {
		printf(" |");
		for(j=0;j<9;j++) {
			if(sq[i][j]==0) printf(" .");
			else printf(" %d",sq[i][j]);			
			if(((j+1)%3)==0) printf(" |");
		}
		printf("    |");
		for(j=0;j<9;j++) {
			if(sa[i][j]==0) printf(" .");
			else printf(" %d",sa[i][j]);			
			if(((j+1)%3)==0) printf(" |");
		}
		printf("\n");	
		if(((i+1)%3)==0) printf(" -------------------------    -------------------------\n");
	}
	printf("\n");
}

void have_question() {
	char se[9][10];
	int i,j,nos=0,con_q=0;

	printf("Enter your SUDOKU Question :\n\n");	
	for(i=0;i<9;i++) scanf("%s",se[i]);

	for(i=0;i<9;i++) {
		for(j=0;j<9;j++) {
			sq[i][j]=sa[i][j]=sw[i][j]=0;
			s3D[i][j].clear();			
		}
	}

	for(i=0;i<9;i++) {
		for(j=0;j<9;j++) {						
            sq[i][j]=sa[i][j]=sw[i][j]=(((int)(se[i][j]))-48);			
			if(sw[i][j]) {
				s3D[i][j].push_back(sw[i][j]);
				if(!con_q) con_q=con(i,j);
			}			
		}
	}	
	if(!con_q) {
		allp();		
		nos=nos_limited(0,0,0);
		get_answer(0,0);
	}
	for(i=0;i<9;i++) for(j=0;j<9;j++) sa[i][j]=sw[i][j];
	print_qa();		
	printf("Number of solutions = %d\n",nos);
}

int generate_solved_puzzle(int i,int j) {
	if(i==9) return 0;	
	
	int I,J,k,l,m;

	if(j==8) { I=i+1; J=0; }
	else { I=i; J=j+1; }

	if(sw[i][j]!=0) { if(generate_solved_puzzle(I,J)) return 1; }
	else {			
		for(l=s3D[i][j].size();l;l--) {			
			k=l*(rand()/(RAND_MAX+0.0));		
			sw[i][j]=s3D[i][j][k];
			s3D[i][j].erase(s3D[i][j].begin()+k);

			s3D[I][J].clear(); for(m=0;m<9;m++) s3D[I][J].push_back(m+1);

			if(con(i,j)) sw[i][j]=0;	
			else if(generate_solved_puzzle(I,J)) sw[i][j]=0;			
			else break;		
		}
		if(sw[i][j]==0) return 1;
	}	
	return 0;	
}
int remove_numbers(vector<int> N,int x) {
	if(x==nob) return 1;

	int I,J,nos,i,j,k,l;
	
	for(i=N.size();i;i--) {		
		j=i*(rand()/(RAND_MAX+0.0));
		I=(N[j]-1)/9; J=(N[j]-1)%9;

		sw[I][J]=0;
		for(k=0;k<9;k++) for(l=0;l<9;l++) s3D[k][l].clear();			
		allp();		
		nos=nos_limited(0,0,0);		
		if(nos==1) {
			N.erase(N.begin()+j);	
			if(remove_numbers(N,x+1)) return 1;
		}
		sw[I][J]=sa[I][J];	
	}
	return 0;
}

void generate_puzzle(int level) {
	if(level==1) nob=30;
	else if(level==2) nob=45;
	else if(level==3) nob=55;

	int i,j;
	vector<int> N;

	printf("Level %d Sudoku Puzzle :\n\n",level);

	for(i=0;i<9;i++) {
		for(j=0;j<9;j++) {
			sq[i][j]=sa[i][j]=sw[i][j]=0;
			s3D[i][j].clear();
		}
	}
	allp();
	generate_solved_puzzle(0,0);
	for(i=0;i<81;i++) N.push_back(i+1); 
	for(i=0;i<9;i++) for(j=0;j<9;j++) sa[i][j]=sw[i][j];
	remove_numbers(N,0);
	for(i=0;i<9;i++) for(j=0;j<9;j++) sq[i][j]=sw[i][j];
	print_qa();
}

int main() {	
	srand((unsigned)time(0)); rand(); rand(); rand();		
	int choice=0;

	while(choice!=5) {
		printf("\nEnter : \n");
		printf(" 1 to generate level-1 Sudoku puzzle\n");
		printf(" 2 to generate level-2 Sudoku puzzle\n");
		printf(" 3 to generate level-3 Sudoku puzzle\n");
		printf(" 4 if you have your own question\n");
		printf(" 5 to Exit\n");

		printf("\nEnter your choice : ");
		scanf("%d",&choice);
		printf("\n");

		switch (choice)	{
			case 1:
				generate_puzzle(1);
				break;

			case 2:
				generate_puzzle(2);
				break;

			case 3:
				generate_puzzle(3);
				break;

			case 4:
				have_question();
				break;

			case 5:
                printf("Exit\n");
				break;
	
			default:
				printf("Please Enter correct choice.\n");				
			    break;
		}		
		printf("\n--------------------------------------------------\n");
	}	
	return 0;
}