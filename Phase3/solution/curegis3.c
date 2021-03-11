/*
 * ENGG1110 2017-18 Term 2 Project Phase 3
 * Sample solution
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// define symbolic constants for improving code readability
#define TRUE  1
#define FALSE 0

#define DEBUG TRUE
// functionally equivalent to defining this global variable:
int _DEBUG = 1;


typedef struct course {
    /* basic info to be read from courses.txt */
    char code[8+1];
    int  unit;
    char name[80+10+1];     // allow 10 more dummy char as a buffer

    /* extracted info during data processing */
    int  student_count;
    int  grade_record_index[100]; // index into grade records
} course_type;


typedef struct grade_record {
    /* basic info to be read from grades.txt */
    int  sid;
    char code[8+1];
    char grade[2+1];
} grade_record_type;


typedef struct student {
    /* extracted info during data processing */
    int  sid;
    int  course_count;
    int  course_index[9];  // a student can take at most 9 courses
    int  grade_index[9];  // a student can take at most 9 courses
    double gpa;
} student_type;


// function for opening file, common checking and reporting error too
// return proper file pointer
FILE *fopen_check(char *filename, char *mode)
{
    FILE *fp;
    fp = fopen(filename, mode);
    if (fp == NULL)
    {
        printf("Error!\n");
        exit(1);
    }
    return fp;
}


// function for removing all white-space characters ' ', '\t', '\r', '\n'
// at the end of a string, it modifies the string parameter
void remove_trailing_spaces(char *str)
{
    if (str == NULL) return;
    int len = strlen(str);
    // short-circuit && is evaluated even before --len
    while (len > 0 && isspace(str[--len]))
        str[len] = '\0';
}


// function for reading courses.txt into basic info of courses[] array of struct
// return number of courses read
int read_courses(FILE *fp, course_type  courses[], int max_course_count)
{
    int count = 0;
    for (count = 0; count < max_course_count; count++)
    {
        // see if reading first 2 items on a line is successful
        if (fscanf(fp, "%s%d ", courses[count].code, &courses[count].unit) != 2)
            break;
        // ok, read the rest of the line as course name, it may have spaces
        fgets(courses[count].name, sizeof(courses[count].name), fp);
        remove_trailing_spaces(courses[count].name);
    }
    return count;
}


// function for reading grades.txt into basic info of grades[] array of struct
// return number of grade records read
int read_grades(FILE *fp, grade_record_type  grades[], int max_grade_count)
{
    int count = 0;
    for (count = 0; count < max_grade_count; count++)
    {
        // see if reading 3 items on a line is successful
        if (fscanf(fp, "%d%s%s", &grades[count].sid, grades[count].code, grades[count].grade) != 3)
            break;
    }
    return count;
}


void sort_courses_by_code(course_type  courses[], int course_count)
{
    int i, j, winner;
    for (i = 0; i < course_count - 1; i++)
    {
        winner = i;
        for (j = i + 1; j < course_count; j++)
            if (strcmp(courses[winner].code, courses[j].code) > 0)
                winner = j;
        if (winner != i)
        {
            course_type tmp = courses[winner];
            courses[winner] = courses[i];
            courses[i] = tmp;
        }
    }
}


void sort_grades_by_sid_and_then_course(grade_record_type  grades[], int grade_record_count)
{
    int i, j, winner;
    for (i = 0; i < grade_record_count - 1; i++)
    {
        winner = i;
        for (j = i + 1; j < grade_record_count; j++)
            if (grades[winner].sid  > grades[j].sid ||
                grades[winner].sid == grades[j].sid && strcmp(grades[winner].code, grades[j].code) > 0)
                winner = j;
        if (winner != i)
        {
            grade_record_type tmp = grades[winner];
            grades[winner] = grades[i];
            grades[i] = tmp;
        }
    }
}


// function for extracting info from grades[] array into courses[] array
// e.g. fill in courses[0].student_count and courses[0].grade_record_index[...]
//      fill in courses[1].student_count and courses[1].grade_record_index[...]
void extract_classlists(course_type  courses[], int course_count,
                        grade_record_type  grades[], int grade_record_count)
{
    int g, c;
    for (g = 0; g < grade_record_count; g++)
        for (c = 0; c < course_count; c++)
            if (strcmp(grades[g].code, courses[c].code) == 0)
                // register the grade record index into the course classlist
                courses[c].grade_record_index[courses[c].student_count++] = g;
}


// function for processing and writing to classlists.txt
void write_classlists(FILE *fp, course_type  courses[], int course_count,
                      grade_record_type  grades[], int grade_record_count)
{
    int c, s;
    for (c = 0; c < course_count; c++)
    {
        fprintf(fp, "%d students in %s\n", courses[c].student_count, courses[c].code);
        for (s = 0; s < courses[c].student_count; s++)
        {
            int grade_index = courses[c].grade_record_index[s];
            fprintf(fp, "%d %s\n", grades[grade_index].sid, grades[grade_index].grade);
        }
    }
}


// function for mapping a grade to a grade point
// return a corresponding grade point in 0.0 – 4.0
double map_grade_point(char grade[])
{
    char * grades[] = {"A", "A-", "B+", "B", "B-", "C+", "C", "C-", "D+", "D", "F"};
    double points[] = {4.0,  3.7,  3.3, 3.0,  2.7,  2.3, 2.0,  1.7,  1.3, 1.0, 0.0};
    int g;
    for (g = 0; g < sizeof(grades) / sizeof(grades[0]); g++)
        if (strcasecmp(grades[g], grade) == 0) // case-insensitive comparison
            return points[g];
    printf("ERROR: invalid grade %s in grades.txt, treated as Fail\n", grade);
    return 0;
}


int lookup_course(course_type courses[], int course_count, char code[])
{
    int c;
    for (c = 0; c < course_count; c++)
        if (strcmp(courses[c].code, code) == 0)
            return c;
    printf("ERROR: course code %s not found!\n", code);
    return 0;
}


// function for extracting info from grades[] and courses[] array into students[]
// e.g. fill in students[0].course_count and students[0].course_index[...]
//      fill in students[1].course_count and students[1].course_index[...]
// return number of students (unique sid)
int extract_transcripts(course_type  courses[], int course_count,
                        grade_record_type  grades[], int grade_record_count,
                        student_type students[], int max_student_record_count)
{
    int student_record_count = -1;
    int g;
    for (g = 0; g < grade_record_count; g++)
    {
        if (student_record_count == -1 || students[student_record_count].sid != grades[g].sid)
            // the first student OR encountered another unique student
            student_record_count++;

        if (students[student_record_count].course_count > 9)
        {
            printf("ERROR: student %d has taken too many courses in grades.txt!\n", grades[g].sid);
            continue;
        }

        students[student_record_count].sid = grades[g].sid;
        int c = students[student_record_count].course_count;
        students[student_record_count].course_index[c] = lookup_course(courses, course_count, grades[g].code);
        students[student_record_count].grade_index[c] = g;
        students[student_record_count].course_count++;
    }
    student_record_count++;

    if (student_record_count > max_student_record_count)
        printf("ERROR: too many unique students in grades.txt!\n");

    return student_record_count;
}


// function for processing and writing to transcripts.txt
void write_transcripts(FILE *fp, course_type  courses[], int course_count,
                       grade_record_type  grades[], int grade_record_count,
                       student_type students[], int student_record_count)
{
    int c, g, s;
    for (s = 0; s < student_record_count; s++)
    {
        fprintf(fp, "%d took %d courses\n", students[s].sid, students[s].course_count);
        double total_point = 0;
        int    total_unit = 0;
        for (c = 0; c < students[s].course_count; c++)
        {
            course_type        course       = courses[students[s].course_index[c]];
            grade_record_type  grade_record = grades[students[s].grade_index[c]];

            fprintf(fp, "%s %s x%d [%s]\n", grade_record.code, grade_record.grade, course.unit, course.name);

            total_point += map_grade_point(grade_record.grade) * course.unit;
            total_unit  += course.unit;
        }
        fprintf(fp, "GPA = %.2f\n", total_point / total_unit);
    }
}

int main(void) {
    printf("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\\n");
    printf("Course Registration Information System Phase 3\n");
    printf("\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\n");

    // declare and initialize all data structures to 0

    int           course_count;
    course_type   courses[20] = {0};  // at most 20 courses in the array

    int                grade_record_count;
    grade_record_type  grades[100] = {0};  // at most 100 grade records in the array

    int           student_record_count;
    student_type  students[100] = {0};  // at most 100 students!

    // call functions to complete the tasks
    FILE *course_file, *grade_file, *classlist_file, *transcript_file;

    printf("Reading courses.txt\n");
    course_file = fopen_check("courses.txt", "r");
    course_count = read_courses(course_file, courses, sizeof(courses) / sizeof(courses[0]));
    fclose(course_file);
    printf("Read %d courses\n", course_count);

    printf("Reading grades.txt\n");
    grade_file = fopen_check("grades.txt", "r");
    grade_record_count = read_grades(grade_file, grades, sizeof(grades) / sizeof(grades[0]));
    fclose(grade_file);
    printf("Read %d records\n", grade_record_count);

    // DEBUG verification
    if (DEBUG) {
        int i;
        for (i = 0; i < course_count; i++)
            printf("DEBUG courses[%d]: [%s][%d][%s]\n", i, courses[i].code, courses[i].unit, courses[i].name);
        for (i = 0; i < grade_record_count; i++)
            printf("DEBUG grades[%d]: [%d][%s][%s]\n", i, grades[i].sid, grades[i].code, grades[i].grade);
    }

    sort_courses_by_code(courses, course_count);
    sort_grades_by_sid_and_then_course(grades, grade_record_count);

    extract_classlists(courses, course_count, grades, grade_record_count);

    printf("Writing classlists.txt\n");
    classlist_file = fopen_check("classlists.txt", "w");
    write_classlists(classlist_file, courses, course_count, grades, grade_record_count);
    fclose(classlist_file);
    printf("Wrote %d classlists\n", course_count);

    student_record_count = extract_transcripts(courses, course_count, grades, grade_record_count, students, sizeof(students) / sizeof(students[0]));

    printf("Writing transcripts.txt\n");
    transcript_file = fopen_check("transcripts.txt", "w");
    write_transcripts(transcript_file, courses, course_count, grades, grade_record_count, students, student_record_count);
    fclose(transcript_file);
    printf("Wrote %d transcripts\n", student_record_count);

    printf("Done!\n");

    return 0;
}

