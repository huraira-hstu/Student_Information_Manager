#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COURSE_COUNT 5
#define MAX_NAME_LENGTH 40
#define MAX_GRADE_NAME_LENGTH 40
#define ID_LENGTH 7
#define MAX_STUDENT_COUNT 100



typedef struct student {
    char name[MAX_NAME_LENGTH];
    char id[ID_LENGTH];
    struct course {
        char name[MAX_GRADE_NAME_LENGTH];
        float grade;
    }  courses[COURSE_COUNT];

} student;

int student_count = 0;
student students[MAX_STUDENT_COUNT];

void student_input(FILE *file, student *std) {
    if (file == stdin) {
        printf("Enter student name: ");
    }
    fscanf(file, " %[^\n]", std->name);

    if (file == stdin) {
        printf("Enter student ID: ");
    }
    fscanf(file, "%s", std->id);

    for (int i = 0; i < COURSE_COUNT; i++) {
        if (file == stdin) {
            printf("Enter Course %d: ", i + 1);
            fscanf(file, " %[^\n]", std->courses[i].name);
        }

        else fscanf(file, " %[^:]", std->courses[i].name);

        if (file == stdin) {
            printf("Enter Grade in Course %d: ", i + 1);
            fscanf(file, "%f", &std->courses[i].grade);
        }

        else fscanf(file, ": %f", &std->courses[i].grade);

    }
}


void student_output(FILE *file, student *std) {
    if(file == stdout){
        printf("Student Name: ");
    }
    fprintf(file, "\%s\n", std->name);

    if(file == stdout){
        printf("Student ID: ");
    }
    fprintf(file, "%s\n", std->id);

    for (int i = 0; i < COURSE_COUNT; i++) {
        fprintf(file, "  %s : %.2f\n", std->courses[i].name, std->courses[i].grade);
    }
    fprintf(file, "\n");
}


int compare_name(const student *std_param, const char* value) {
    return strcmp(std_param->name, value);
}

int compare_id(const student *std_param, const char* value) {
    return strcmp(std_param->id, value);
}

int (*compare_functions[])(const student*, const char*) = {
    compare_name,
    compare_id
};

int student_search(){
    printf("1. Search By Name\n");
    printf("2. Search By ID\n");

    int s;
    printf("Enter 1 or 2: ");
    scanf("%d", &s);

    char value[MAX_NAME_LENGTH];
    int max = student_count;
    printf("Enter %s : ", (s==1)? "Name":"ID");

    scanf(" %[^\n]", value);

    for (int i = 0; i < student_count; i++) {
        if (compare_functions[s-1](&students[i], value) == 0) {
            return i;
        }
    }

    return -1;
}

void display_help(){

    printf("\n0. Display Help.\n");
    printf("1. Student Info Input.\n");
    printf("2. View records of all student.\n");
    printf("3. Student Search.\n");
    printf("4. Generate Report.\n");
    printf("5. Load student Info.\n");
    printf("6. Save student Info.\n");
    printf("7. Clear screen.\n");
    printf("8. Exit program.\n\n");

}


void add_student(){
    student_input(stdin, &students[student_count]);
    student_count++;
}

void display_all_students(){
    for(int i = 0; i < student_count; i++){
        printf("\nStudent No. %d", i+1);
        printf("\n--------------\n", i+1);
        student_output(stdout, &students[i]);
        printf("\n");
    }
}

void find_student(){
    int st = student_search();
    if(st == -1) printf("NO students Found");
    else student_output( stdout,  &students[st] );
}

void generate_report() {
    printf("\nStudent Report\n");
    printf("--------------\n");
    printf("%-25s %-10s %-7s\n","Name","ID", "Grade");
    printf("%-25s %-10s %-7s\n","---------------------","-------", "-----");

    for (int i = 0; i < student_count; i++) {
        float grade=0;
        for (int j = 0; j < COURSE_COUNT; j++) {
            grade += students[i].courses[j].grade;
        }
        printf("%-25s %-10s %-7.2f", students[i].name, students[i].id, grade/COURSE_COUNT);

        printf("\n");
    }
    printf("\nEnd of Report Report\n");
    printf("---------------------\n\n");
}

void load_student_info(){
    FILE *fileptr = fopen("input.txt", "r");
    if (fileptr == NULL) {
        printf("Error Opening file.\n\a");
        exit(-1);
    }

    int std_cnt = 0;

    fscanf(fileptr, " %d", &std_cnt);

    for(int i = 0; i<std_cnt; i++){

        student_input(fileptr, &students[student_count+i]);
    }

    student_count += std_cnt;

    printf("\nData Load Successful.\n\n");

    fclose(fileptr);

}

void save_file(){
    FILE *fileptr2 = fopen("output.txt", "w");
    if (fileptr2 == NULL) {
        printf("Error Opening file.\n");
        exit(-1);

    }

    fprintf(fileptr2, " %d \n", student_count);
    for(int i= 0; i < student_count; i++){
        student_output(fileptr2, &students[i]);
    }

    fclose(fileptr2);

    printf("\nFile save successful.\n\n");

}

void clear_screen(){
    system("cls");
}


void quit_program(){
    printf("\n\nExiting Program.\n\n");
    exit(0);
}

void (*options[])(void) = {
    display_help,
    add_student,
    display_all_students,
    find_student,
    generate_report,
    load_student_info,
    save_file,
    clear_screen,
    quit_program
};


void option_picker(){
    int option_no, option_max = ( sizeof(options)/sizeof(options[0]) ) - 1;

    printf("Enter an option(0 - %d): ", option_max );

    scanf("%d", &option_no);
    if(option_no < 0 || option_no >8 ){
        printf("\nInvalid option selected.\n\n");
        option_no = 8;
    }

    options[option_no]();

}

int main() {

    printf("Student information management software.\n");

    display_help();

    while(1){
        option_picker();
    }

    return 0;
}
