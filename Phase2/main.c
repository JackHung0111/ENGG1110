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
 * Date         : 7/3/2018
 */

#include <stdio.h>
#include <string.h>

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

void printBanner(){
    printf("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n");
    printf("Course Registration Information System Phase 2\n");
    printf("\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\n");
}

void printRuler(){
    printf("===============================================================================\n");
    printf("1234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
}

// print course codes or an empty shopping cart
void printShoppingCart(int courses[9], int count){
    int i;
    if(count==0)
        printf("Your shopping cart is empty!\n");
    else{
        printf("Here is your shopping cart:\n");
        for(i=0;i<count;i++)
            printf("Course %d): ENGG%04d\n",i+1,courses[i]);
    }
}

// try to find a course code in courses[] shopping cart array which has count items
// return the index of the target in the shopping cart
int findCourse(int courses[9], int count, int code){
    int i, place=-1;
    for(i=0;i<count;i++){
        if(courses[i]==code)
            place=i;
    }
    if(place==-1)
        return -1;     // on not found
    else
        return place;  // on found
}

void sorting(int courses[],int count){
    int i, j, minPos, tmp;
    for (i=0;i<count-1;i++) {
        minPos = i;
        for (j=i+1;j<count;j++)
            if (courses[j]<courses[minPos])
                minPos = j;
        if (minPos != i) {
            tmp = courses[i];
            courses[ i ] = courses[ minPos ];
            courses[ minPos ] = tmp;
            }
    }
}

// try to add a course code to courses[] shopping cart array which has count items
// return the updated item count in the shopping cart
int addCourse(int courses[9], int count, int code){
    int i,j;
    if (findCourse(courses, count, code) != -1)
        printf("ERROR: You cannot add again ENGG%04d which is already in your shopping cart!\n", code);
    else{
        courses[count]=code;
        sorting(courses,count+1);
        return count + 1;   // on successful
    }
    return count;  // on failure
}

// try to drop a course code from courses[] shopping cart array which has count items
// return the updated item count in the shopping cart
int dropCourse(int courses[9], int count, int code){
    int i,find;
    find=findCourse(courses, count, code);
    if (find == -1)
        printf("ERROR: Course code ENGG%04d not found in your shopping cart!\n",code);
    else{
        for(i=find;i<count-1;i++)
            courses[i]=courses[i+1];
        courses[count]=0;
        return count - 1;   // on successful
    }
    return count;  // on failure

}

int add(int courses[],int count){
    int code;
    if(count==9)
        printf("ERROR: You have already selected 9 courses!  Please drop one before you add.\n");
    else {
        do{
            printf("Add course code [1000-9999]:\n");
            scanf("%d",&code);
            if(code<1000||code>9999) printf("ERROR: Invalid course code!\n");
        }while(code<1000||code>9999);
    count=addCourse(courses,count,code);}
    return count;
}

int drop(int courses[],int count){
    int code;
    if(count==0)
        printf("ERROR: You have no course in your shopping cart to drop!\n");
    else{
            do{
                printf("Drop course code [1000-9999]:\n");
                scanf("%d",&code);
                if(code<1000||code>9999) printf("ERROR: Invalid course code!\n");
            }while(code<1000||code>9999);
        count=dropCourse(courses,count,code);}
    return count;
}

void clear(int courses[]){
    int i;
    for(i=0;i<9;i++)
        courses[i]=0;
}

int action(int input,int courses[],int count){
    if(input==1)
        count=add(courses,count);
    else if (input==2)
        count=drop(courses,count);
    else if (input==3){
        clear(courses);
        count=0;
    }
    return count;
}

int content(int courses[],int *count){
    int input;
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
			*count=action(input,courses,*count);
    return input;
}

// read the expected grade of a given course code from the user
// return the converted grade point [0.0 - 4.0] from the input grade
double readGrade(int code){
    printf("ENGG%04d [A/A-/B+/B/B-/C+/C/C-/D+/D]: ", code);
    char aLine[3];
    scanf("%2s",aLine);
    if (strcasecmp(aLine, "A" ) == 0)   return 4.0;
    if (strcasecmp(aLine, "A-") == 0)   return 3.7;
    if (strcasecmp(aLine, "B+" ) == 0)  return 3.3;
    if (strcasecmp(aLine, "B") == 0)    return 3.0;
    if (strcasecmp(aLine, "B-" ) == 0)  return 2.7;
    if (strcasecmp(aLine, "C+") == 0)   return 2.3;
    if (strcasecmp(aLine, "C" ) == 0)   return 2.0;
    if (strcasecmp(aLine, "C-") == 0)   return 1.7;
    if (strcasecmp(aLine, "D+" ) == 0)  return 1.3;
    if (strcasecmp(aLine, "D") == 0)    return 1.0;
    return -1; // impossible case, internal error
}

double check_out(int courses[],int count){
    double cgpa=0.0;
    double gpa[count];
    int i;
    printf("Assume all courses carry 3 credit units, enter your expected grade for:\n");
    for(i=0;i<count;i++){
        gpa[i]=readGrade(courses[i]);
        while(gpa[i]==-1.00){
            printf("ERROR: Invalid grade input!\n");
            gpa[i]=readGrade(courses[i]);
        }
        cgpa+=gpa[i];
    }
    for(i=0;i<count;i++){
        printf("Expected grade point for ENGG%04d = ",courses[i]);
        printf("%.2lf\n",gpa[i]);
    }
    cgpa=cgpa/count;
    return cgpa;
}

int main(){
    int courses[9];
    int count = 0;
    long int SID;
    int choice;
    int i;
    for(i=0;i<9;i++) courses[i]=0;
    printBanner();
    sid_check(&SID);
    do{
        printRuler();
        printShoppingCart(courses,count);
        choice=content(courses,&count);
        if(count==0&&choice==4)
            printf("ERROR: You must select at least one course!\n");
    }while(!(choice==4&&count>0));
    double cgpa;
    printf("Dear student %ld\n",SID);
    printShoppingCart(courses,count);
    cgpa=check_out(courses,count);
    printf("Your expected GPA is %.2lf",cgpa);
    return 0;
}
