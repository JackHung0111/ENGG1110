/**
 * ENGG1110 Problem Solving by Programming
 *
 * Project Phase 3
 *
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
 * Class/Section: I
 * Date         : 15/4/2018
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct course {
    /* basic info to be read from courses.txt */
    char code[8+1];
    int unit;
    char name[80+10+1]; // allow 10 more dummy char as a buffer
    /* extracted info during data processing */
    int student_count;
    int grade_record_index[100]; // index into grade records
} course_type;

typedef struct grade_record {
    /* basic info to be read from grades.txt */
    int sid;
    char code[8+1];
    char grade[2+1];
} grade_record_type;

typedef struct student {
    /* extracted info during data processing */
    int sid;
    int course_count;
    char course_index[9][8+1]; // a student can take at most 9 courses
    double gpa;
} student_type;

// function for reading courses.txt into basic info of courses[] array of struct
// return number of courses read
int read_courses ( course_type courses[] ) {
    FILE *fp;
    int i = 0;// i is a looping variable, as well as be the count of courses
    char input[100];
    fp = fopen("courses.txt","r");
    if( fp == NULL ){ // No file is found
        printf("ERROR!\n");
        exit(1); // terminate the program
    }
    do{
        fgets(input,100,fp); // get the string from the file line by line
        if(strcmp(input,"END\n") != 0){
            strncpy (courses[i].code,&input[0],8); // copy the courses code from file
            courses[i].code[8] = '\0';
            courses[i].unit = input[9] - 48; // copy the courses unit from file
            strncpy (courses[i].name,&input[11],80); // copy the courses name from file
            courses[i].name[strlen(courses[i].name) - 1] = '\0';
            i++;
        }
    }while(strcmp(input,"END\n")!=0); // if the string is equal to "END\n", loop ends
    fclose(fp);
    return i;
}

// function for reading grades.txt into basic info of grades[] array of struct
// return number of grade records read
int read_grades(grade_record_type grades[]){
    FILE *fp;
    int i = 0;// i is a looping variable, as well as be the count of grade records
    int j; // looping variable
    char input[100], SID[11];
    fp = fopen("grades.txt","r");
    if(fp == NULL){ //no file is found
        printf("ERROR!\n");
        exit(1); //terminate the program
    }
    do{
        fgets(input,100,fp); // get the string from the file line by line
        if(strncmp(input,"END",3) != 0){
            strncpy(SID,&input[0],10); // copy the SID from file
            grades[i].sid = atoi(SID); // atoi is a defined function to convert char array into integer
            strncpy(grades[i].code,&input[11],8); // copy the corresponding courses code from file
            grades[i].code[8] = '\0';
            strncpy(grades[i].grade,&input[20],3); // copy the corresponding grade from file
            if(strlen(grades[i].grade) == 2) grades[i].grade[1] = '\0';
                else grades[i].grade[2] = '\0'; // if the grade consists of +,- sign
            for (j=0;j<i;j++) // avoid students taking same courses more than once
                if(grades[i].sid == grades[j].sid
                        && strcmp(grades[i].code,grades[j].code) == 0)
                   i--;
            i++;
        }
    }while(strncmp(input,"END",3)!=0); // if the string is equal to "END\n", loop ends
    fclose(fp);
    return i;
}

void integer_sort(int record[], int count){
    //selection sort
    int i, j, minpos, temp;// i and j are looping variables
    for(i=0;i<count;i++){
        minpos = i;
        for(j=i+1;j<count;j++)
            if(record[j] < record[minpos]) // comparing the 2 integers
                minpos = j;
        if(minpos != i){
            //swapping
            temp = record[i];
            record[i] = record[minpos];
            record[minpos] = temp;
        }
    }
}

void course_sort(course_type record[], int count){
    //selection sort
    int i, j, minpos;// i and j are looping variables
    course_type temp;
    for(i=0;i<count;i++){
        minpos = i;
        for(j=i+1;j<count;j++)
            if(strcmp(record[j].code,record[minpos].code) < 0) // comparing the 2 codes
                minpos = j;
        if (minpos != i){
            //swapping
            temp = record[i];
            record[i] = record[minpos];
            record[minpos] = temp;
        }
    }
}

void student_sid_sort(student_type record[], int count){
    //selection sort
    int i, j, minpos;
    student_type temp;
    for(i=0;i<count;i++){
        minpos = i;
        for(j=i+1;j<count;j++)
            if(record[j].sid < record[minpos].sid) //comparing the 2 SIDs in ascending order
                minpos = j;
        if (minpos != i){
            //swapping
            temp = record[i];
            record[i] = record[minpos];
            record[minpos] = temp;
        }
    }
}

// function for extracting info from grades[] array into courses[] array
// e.g. fill in courses[0].student_count and courses[0].grade_record_index[...]
// fill in courses[1].student_count and courses[1].grade_record_index[...]
// this is just one of the many possible ways for extracting the classlists
// you may solve this problem in your own way
void extract_classlists(course_type courses[], grade_record_type grades[],
                        int course_count, int grade_record_count){
    int i, j; // looping variables
    //initialization
    for(i=0;i<course_count;i++)
        courses[i].student_count = 0;
    //extraction of SID from grades[] to courses[]
    for(i=0;i<course_count;i++)
        for(j=0;j<grade_record_count;j++)
            if(strcmp(grades[j].code,courses[i].code) == 0){
                courses[i].grade_record_index[courses[i].student_count] = grades[j].sid;
                courses[i].student_count++;
            }
}

// function for sorting, processing and writing to classlists.txt
void write_classlists(course_type courses[], grade_record_type grades[],
                      int course_count, int grade_record_count){
    FILE *fp;
    course_type record[course_count];
    int i, j, k; // looping values
    //Substitution
    for(i=0;i<course_count;i++)
        record[i] = courses[i];
    course_sort(record,course_count);
    fp = fopen("classlists.txt","w");
    for(i=0;i<course_count;i++){
        fprintf(fp,"%d students in %s\n",record[i].student_count,record[i].code);
        integer_sort(record[i].grade_record_index,record[i].student_count);
        for(j=0;j<record[i].student_count;j++){
            k = 0;
            while(k < grade_record_count){
                if(record[i].grade_record_index[j] == grades[k].sid &&
                                        strcmp(record[i].code,grades[k].code) == 0)
                    fprintf(fp,"%d %s\n",record[i].grade_record_index[j],grades[k].grade);
                k++;
            }
        }
    }
    fclose(fp);
}

// function for extracting info from grades[] and courses[] array into students[]
// e.g. fill in students[0].course_count and students[0].course_index[...]
// fill in students[1].course_count and students[1].course_index[...]
// this is just one of the many possible ways for extracting the transcripts
// you may solve this problem in your own way
void extract_transcripts(course_type courses[], grade_record_type grades[],student_type students[],
                         int grade_record_count, int *student_count){
    int i, j, k;
    *student_count = grade_record_count;
    int record[grade_record_count];
    // Substitution
    for(i=0;i<grade_record_count;i++)
        record[i] = grades[i].sid;
    integer_sort(record,grade_record_count);
    // count number of students
    // counting number of students (counting for distinct records)
    for(i=1;i<grade_record_count;i++)
        if(record[i] == record[i-1])
            *student_count = *student_count - 1;
    // Shorten the record[] which only includes the distinct records
    for(i=0;i<*student_count;i++)
        while(record[i] == record[i+1] && i != *student_count - 1)
            for(j=i;j<grade_record_count-1;j++)
                record[j] = record[j+1];
    // Count the number of courses that the students[i] studies
    for(i=0;i<*student_count;i++){
        students[i].sid = record[i]; // substitution
        students[i].course_count = 0; // initialization
        for(j=0;j<grade_record_count;j++)
            if(students[i].sid == grades[j].sid)
                students[i].course_count++;
    }
    // Count the courses codes that the students[i] studies
    for(i=0;i<*student_count;i++){
        k = 0;
        for(j=0;j<grade_record_count;j++)
            if(students[i].sid == grades[j].sid){
                strcpy(students[i].course_index[k],grades[j].code);
                k++;
            }
    }
}

// function for mapping a grade to a grade point
// return a corresponding grade point in 0.0 – 4.0
double map_grade_point(char grade[]){
    // copied from Project Phase 2
    if (strcmp(grade, "A" ) == 0)   return 4.0;
    else if (strcmp(grade, "A-") == 0)   return 3.7;
    else if (strcmp(grade, "B+" ) == 0)  return 3.3;
    else if (strcmp(grade, "B") == 0)    return 3.0;
    else if (strcmp(grade, "B-" ) == 0)  return 2.7;
    else if (strcmp(grade, "C+") == 0)   return 2.3;
    else if (strcmp(grade, "C" ) == 0)   return 2.0;
    else if (strcmp(grade, "C-") == 0)   return 1.7;
    else if (strcmp(grade, "D+" ) == 0)  return 1.3;
    else if (strcmp(grade, "D") == 0)    return 1.0;
    else return 0.0;
}

// function for sorting, processing and writing to transcripts.txt
void write_transcripts(course_type courses[], grade_record_type grades[],student_type students[],
                       int student_count, int grade_record_count, int course_count){
    FILE *fp;
    student_type record[student_count];
    course_type course_record[9];
    int i, j, k, l; // looping values
    int total_unit[student_count]; // the total course unit that the student[] studies
    // initialization and substitution
    for(i=0;i<student_count;i++){
        students[i].gpa = 0.0;
        total_unit[i] = 0;
        record[i] = students[i];
    }
    student_sid_sort(record,student_count);
    fp = fopen("transcripts.txt","w");
    for(i=0;i<student_count;i++){
        fprintf(fp,"%d took %d courses\n",record[i].sid,record[i].course_count);
        for(j=0;j<record[i].course_count;j++) // Substitution
            strcpy(course_record[j].code,record[i].course_index[j]);
        course_sort(course_record,record[i].course_count);
        for(j=0;j<record[i].course_count;j++){
            k = 0;
            while(k < grade_record_count){
                if(strcmp(grades[k].code,course_record[j].code) == 0
                                        && grades[k].sid == record[i].sid){
                    l = 0;
                    while(l < course_count){
                        if(strcmp(courses[l].code,course_record[j].code) == 0){
                            fprintf(fp,"%s %s x%d [%s]\n",course_record[j].code,grades[k].grade,courses[l].unit,courses[l].name);
                            students[i].gpa = students[i].gpa + map_grade_point(grades[k].grade)*courses[l].unit;
                            total_unit[i] = total_unit[i] + courses[l].unit;
                        }
                        l++;
                    }
                }
                k++;
            }
        }
        students[i].gpa = students[i].gpa/total_unit[i]; // calculating the GPA
        fprintf(fp,"GPA = %.2f\n",students[i].gpa);
    }
    fclose(fp);
}

void printBanner(){
    printf("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n");
    printf("Course Registration Information System Phase 3\n");
    printf("\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\n");
}

int main(){
    int course_count;
    course_type courses[20]; // at most 20 courses in the array
    int grade_record_count;
    grade_record_type grades[100]; // at most 100 grade records in the array
    int student_count;
    student_type students[100]; // at most 100 students!
    printBanner(); // modify from project phase 2
    // call functions to complete the task

    course_count = read_courses(courses);
    printf("Reading courses.txt\nRead %d course\n",course_count);

    grade_record_count = read_grades(grades);
    printf("Reading grades.txt\nRead %d records\n",grade_record_count);
    extract_classlists(courses,grades,course_count,grade_record_count);
    write_classlists(courses,grades,course_count,grade_record_count);
    printf("Writing classlists.txt\nWrote %d classlists\n",course_count);

    extract_transcripts(courses,grades,students,grade_record_count,&student_count);
    write_transcripts(courses,grades,students,student_count,grade_record_count,course_count);
    printf("Writing transcripts.txt\nWrote %d transcripts\n",student_count);

    printf("Done!\n");
    return 0;
}
