// Enter your code here.
// Please prepare the usual declaration statement with your name, SID, section and date

// You are expected to work and to debug on Code::Blocks.
// So you shall copy-and-paste your code here to test and submit your work finally.
 /*
 * I declare that the assignment here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   http://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name : Hung Yiu Pan
 * Student ID   : 1155108381
 * Class/Section: ENGG1110I
 * Date         : 7/2/2018
 */
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

void sid_check(long int *SID){
	int correct=0;
	printf("Input your SID [in form of 1166xxxxxx]: ");
	scanf("%ld",&*SID);
	while(!correct){
		if(*SID-1166000000<0||*SID-1166999999>0){
			printf("ERROR: Your SID is not conforming!\n");
			printf("Input your SID [in form of 1166xxxxxx]: ");
	        scanf("%ld",&*SID);}
		else correct=1;}}

int add(char A[],char B[], char C[], int s){
	int code;
	char CODE[10],temp[10];
    if(s<3){
        do{
            printf("Add course code [1000-9999]:\n");
            scanf("%d",&code);
            if(code<1000||code>9999) printf("ERROR: Invalid course code!\n");
        }while(code<1000||code>9999);
        sprintf(CODE,"%d",code);
        strcpy(temp,"ENGG");
        strcat(temp,CODE);
		if(!(strcmp(A,temp)==0||strcmp(B,temp)==0||strcmp(C,temp)==0)){
			if(strcmp(A,"nil")==0){
				strcpy(A,"ENGG");
				strcat(A,CODE);}
			else if(strcmp(B,"nil")==0){
				strcpy(B,"ENGG");
				strcat(B,CODE);}
			else{
				strcpy(C,"ENGG");
				strcat(C,CODE);}
			return s+1;}
        else{
            printf("ERROR: You cannot add again ENGG%d which is already in your shopping cart!\n",code);
            return s;
            }
        }
    else printf("ERROR: You have already selected 3 courses!  Please drop one before you add.\n");
	return s;}

int drop(char A[],char B[], char C[], int s){
	int code,exist[3]={0,0,0};
	char CODE[10];
	if(strcmp(A,"nil")!=0) exist[0]=1;
	if(strcmp(B,"nil")!=0) exist[1]=1;
	if(strcmp(C,"nil")!=0) exist[2]=1;
	if(s>0){
        do{
            printf("Drop course code [1000-9999]:\n");
            scanf("%d",&code);
            if(code<1000||code>9999) printf("ERROR: Invalid course code!\n");
        }while(code<1000||code>9999);
        sprintf(CODE,"%d",code);
		if(strncmp(&A[4],CODE,4)==0) {strcpy(A,"nil"); exist[0]=0;}
		else if (strncmp(&B[4],CODE,4)==0) {strcpy(B,"nil"); exist[1]=0;}
		else if (strncmp(&C[4],CODE,4)==0) {strcpy(C,"nil"); exist[2]=0;}
		else{
			printf("ERROR: Course code ENGG%d not found in your shopping cart!\n",code);
			return s;}
        if(exist[0]==1){
            if(exist[1]==0&&exist[2]==1) {strcpy(B,C); strcpy(C,"nil");}}
        else if(exist[1]==1){
            strcpy(A,B); strcpy(B,"nil");
            if(exist[2]==1) {strcpy(B,C); strcpy(C,"nil");}
        }
        else if(exist[2]==1) {strcpy(A,C); strcpy(B,"nil"); strcpy(C,"nil");}
		return s-1;}
	else{
		printf("ERROR: You have no course in your shopping cart to drop!\n",code);
		return s;
		}}

int clear(char A[],char B[], char C[]){
	strcpy(A,"nil");
	strcpy(B,"nil");
	strcpy(C,"nil");
	return 0;}

int action(int input,char A[],char B[], char C[],int s){
	if(input==1){
		s=add(A,B,C,s);
		return s;}
	else if(input==2){
			s=drop(A,B,C,s);
			return s;}
	else {
		s=clear(A,B,C);
		return s;}}

void conclude(int s,char A[],char B[], char C[], long int sid){
	printf("Dear student %ld\n",sid);
	printf("You have selected %d course(s), here is your shopping cart:\n",s);
	if(strcmp(A,"nil")!=0) printf("Course A): %s\n",A);
	if(strcmp(B,"nil")!=0) printf("Course B): %s\n",B);
	if(strcmp(C,"nil")!=0) printf("Course C): %s\n",C);}

int content(int *selected, char A[], char B[], char C[]){
	int input;
	printf("===============================================================================\n");
	printf("1234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
	printf("You have selected %d of 3 course(s), here is your shopping cart:\n",*selected);
	printf("Course A): %s\n",A);
	printf("Course B): %s\n",B);
	printf("Course C): %s\n",C);
	printf("Operation menu:\n1. Add a course\n2. Drop a course\n3. Clear shopping cart\n4. Check out\nAction [1-4]: \n");
	scanf("%d",&input);
	if(input<=0||input>=5)
    		printf("ERROR: Wrong action %d\n",input);
    	else if (input==1)
			printf("==> 1. Add a course\n");
    	else if (input==2)
			printf("==> 2. Drop a course\n");
    	else if (input==3)
			printf("==> 3. Clear shopping cart\n");
    	else if (input==4)
			printf("==> 4. Check out\n");
    if (input>=1 && input<=3)
			*selected=action(input,A,B,C,*selected);
    else if(*selected==0 && input==4){
			printf("ERROR: You must select at least one course!\n");
			input=5;}
	return input;}

void def(double input,char s[]){
	if (input>3.7) strcpy(s,"A");
	else if (input>3.3) strcpy(s,"A-");
	else if (input>3.0) strcpy(s,"B+");
	else if (input>2.7) strcpy(s,"B");
	else if (input>2.3) strcpy(s,"B-");
	else if (input>2.0) strcpy(s,"C+");
	else if (input>1.7) strcpy(s,"C");
	else if (input>1.3) strcpy(s,"C-");
	else if (input>1.0) strcpy(s,"D+");
	else strcpy(s,"D");}

int main(){
	printf("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n");
	printf("Course Registration Information System\n");
	printf("\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\n");
	long int SID;
	char A[9],B[9],C[9],s[3];
	sid_check(&SID);
	double gpa;
	do{
    	printf("Input your expected GPA [1.0-4.0]: ");
		scanf("%lf",&gpa);
		if(gpa<1.0||gpa>4.0) printf("ERROR: Your expected GPA is out of range!\n");
	}while(gpa<1.0||gpa>4.0);
	int input,selected;
	selected=clear(A,B,C);
	do{	input=content(&selected,A,B,C);
	}while(input!=4);
	conclude(selected,A,B,C,SID);
	def(gpa,s);
	printf("You shall get at least average grade %s to achieve GPA of %.2lf\n",s,gpa);
	return 0;
}
