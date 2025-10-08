#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX 10
#define MAX_QUESTIONS 500

// ---------------- GLOBAL GAMIFICATION ----------------
int points = 0;

// ---------------- AUTHENTICATION ----------------
typedef struct {
    char username[50];
    char password[50];
    int points;
    int arrayLevel;
    int stackLevel;
    int queueLevel;
} User;

User currentUser;

// ---------------- FUNCTION PROTOTYPES ----------------
void registerUser();
int loginUser();
void saveUserProgress();
void logout();
int topicLevelIndex(char topic[]);
void loadQuestionsFromFile(const char *filename);
void quizModule(char topic[]);

// ---------------- USER DATA HANDLING ----------------
int loadUserData(const char *username, User *u) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;

    User temp;
    while (fscanf(fp, "%s %s %d %d %d %d", temp.username, temp.password, &temp.points,
                  &temp.arrayLevel, &temp.stackLevel, &temp.queueLevel) == 6) {
        if (strcmp(temp.username, username) == 0) {
            *u = temp;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void saveUserProgress() {
    FILE *fp = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!temp) { printf("Error saving data.\n"); return; }

    int found = 0;
    User u;
    if (fp) {
        while (fscanf(fp, "%s %s %d %d %d %d", u.username, u.password, &u.points,
                      &u.arrayLevel, &u.stackLevel, &u.queueLevel) == 6) {
            if (strcmp(u.username, currentUser.username) == 0) {
                fprintf(temp, "%s %s %d %d %d %d\n", currentUser.username, currentUser.password,
                        points, currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel);
                found = 1;
            } else {
                fprintf(temp, "%s %s %d %d %d %d\n", u.username, u.password, u.points,
                        u.arrayLevel, u.stackLevel, u.queueLevel);
            }
        }
        fclose(fp);
    }

    if (!found) {
        fprintf(temp, "%s %s %d %d %d %d\n", currentUser.username, currentUser.password,
                points, currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel);
    }

    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

void registerUser() {
    FILE *fp = fopen("users.txt", "a+");
    if (!fp) { printf("Cannot open file.\n"); return; }

    User u;
    printf("\n--- REGISTER ---\nEnter username: ");
    scanf("%s", u.username);
    printf("Enter password: ");
    scanf("%s", u.password);

    User temp;
    rewind(fp);
    while (fscanf(fp, "%s %s %d %d %d %d", temp.username, temp.password, &temp.points,
                  &temp.arrayLevel, &temp.stackLevel, &temp.queueLevel) == 6) {
        if (strcmp(temp.username, u.username) == 0) {
            printf("Username already exists! Try another.\n");
            fclose(fp);
            return;
        }
    }

    u.points = 0;
    u.arrayLevel = 1;
    u.stackLevel = 1;
    u.queueLevel = 1;

    fprintf(fp, "%s %s %d %d %d %d\n", u.username, u.password, u.points,
            u.arrayLevel, u.stackLevel, u.queueLevel);
    fclose(fp);
    printf("Registration successful!\n");
}

int loginUser() {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) { printf("No users registered yet. Please register first.\n"); return 0; }

    char username[50], password[50];
    printf("\n--- LOGIN ---\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    User temp;
    while (fscanf(fp, "%s %s %d %d %d %d", temp.username, temp.password, &temp.points,
                  &temp.arrayLevel, &temp.stackLevel, &temp.queueLevel) == 6) {
        if (strcmp(temp.username, username) == 0 && strcmp(temp.password, password) == 0) {
            currentUser = temp;
            points = temp.points;
            fclose(fp);
            printf("Login successful! Welcome, %s\n", currentUser.username);
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid username or password.\n");
    return 0;
}

void logout() {
    saveUserProgress();
    printf("Progress saved. Logged out successfully.\n");
    exit(0);
}

// ---------------- ARRAY MODULE ----------------
int arr[MAX], arrSize = 0;
void printArray(int highlightPos) {
    for (int i = 0; i < arrSize; i++) {
        if (i == highlightPos)
            printf("[%d] ", arr[i]);  // Highlight current element
        else
            printf("%d ", arr[i]);
    }
    printf("\n");
}

void arrayModule() {
    int choice, val, pos;
    do {
        printf("\n[ARRAY MODULE]\n1. Input Elements\n2. Traverse Array\n3. Insert Element\n4. Delete Element\n5. Exit Array Module\nChoice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) {
            case 1:
                printf("Enter number of elements (max 10): ");
                scanf("%d", &arrSize);
                printf("Enter %d elements separated by space:\n", arrSize);
                for(int i=0; i<arrSize; i++) scanf("%d", &arr[i]);
                break;

            case 2:
                printf("Traversing Array: ");
                for(int i=0;i<arrSize;i++){
                    printf("%d ", arr[i]);
                    Sleep(200); // animation effect
                }
                printf("\n");
                break;

            case 3: // INSERT
                printf("Enter position (0 to %d) to insert: ", arrSize);
                scanf("%d", &pos);
                if(pos<0 || pos>arrSize) { printf("Invalid position!\n"); break; }
                printf("Enter value: "); scanf("%d", &val);

                printf("\nShifting elements to the right:\n");
                for(int i=arrSize;i>pos;i--){
                    arr[i] = arr[i-1];
                    printArray(i);
                    Sleep(300);
                }

                arr[pos] = val;
                arrSize++;

                printf("\nInserted value %d at position %d:\n", val, pos);
                printArray(-1);
                break;

            case 4: // DELETE
                printf("Enter position (0 to %d) to delete: ", arrSize-1);
                scanf("%d",&pos);
                if(pos<0 || pos>=arrSize) { printf("Invalid position!\n"); break; }

                printf("\nDeleting element %d at position %d:\n", arr[pos], pos);
                for(int i=pos;i<arrSize-1;i++){
                    arr[i] = arr[i+1];
                    printArray(i);
                    Sleep(300);
                }
                arrSize--;
                printf("\nArray after deletion:\n");
                printArray(-1);
                break;

            case 5: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 5);
}

// ---------------- STACK MODULE ----------------
int stack[MAX], top=-1;

void showStack() {
    if(top==-1){printf("Stack is empty.\n"); return;}
    printf("Current Stack: "); for(int i=top;i>=0;i--) printf("%d ",stack[i]); printf("\n");
}

void push(int val){ if(top==MAX-1) printf("Stack Overflow!\n"); else {stack[++top]=val; showStack();}}
void pop(){ if(top==-1) printf("Stack Underflow!\n"); else {printf("Popped %d\n",stack[top--]); showStack();}}

void stackModule() {
    int choice, val;
    do {
        printf("\n[STACK MODULE]\n1. Push\n2. Pop\n3. Exit\nChoice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1: printf("Enter value: "); scanf("%d",&val); push(val); break;
            case 2: pop(); break;
        }
    } while(choice!=3);
}

// ---------------- QUEUE MODULE ----------------
int queue[MAX], front=-1, rear=-1;

void showQueue() {
    if(front==-1 || front>rear) { printf("Queue empty.\n"); return; }
    printf("Current Queue: "); for(int i=front;i<=rear;i++) printf("%d ",queue[i]); printf("\n");
}

void enqueue(int val){ if(rear==MAX-1) printf("Queue Full!\n"); else {if(front==-1) front=0; queue[++rear]=val; showQueue();}}
void dequeue(){ if(front==-1 || front>rear) printf("Queue Empty!\n"); else {printf("Dequeued %d\n",queue[front++]); showQueue();}}

void queueModule() {
    int choice, val;
    do {
        printf("\n[QUEUE MODULE]\n1. Enqueue\n2. Dequeue\n3. Exit\nChoice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1: printf("Enter value: "); scanf("%d",&val); enqueue(val); break;
            case 2: dequeue(); break;
        }
    } while(choice!=3);
}

// ---------------- QUIZ MODULE ----------------
typedef struct {
    char topic[20];
    int level;
    char question[200];
    char options[4][100];
    char correct;
} Question;

Question quizBank[MAX_QUESTIONS];
int totalQuestions=0;

void loadQuestionsFromFile(const char *filename){
    FILE *fp=fopen(filename,"r");
    if(!fp){ printf("Error: Cannot open %s\n",filename); return; }

    char line[256];
    Question q;
    totalQuestions=0;

    while(fgets(line,sizeof(line),fp)){
        if(strncmp(line,"topic=",6)==0) sscanf(line+6,"%s",q.topic);
        else if(strncmp(line,"level=",6)==0) sscanf(line+6,"%d",&q.level);
        else if(strncmp(line,"Q=",2)==0) strcpy(q.question,line+2);
        else if(strncmp(line,"A=",2)==0) strcpy(q.options[0],line+2);
        else if(strncmp(line,"B=",2)==0) strcpy(q.options[1],line+2);
        else if(strncmp(line,"C=",2)==0) strcpy(q.options[2],line+2);
        else if(strncmp(line,"D=",2)==0) strcpy(q.options[3],line+2);
        else if(strncmp(line,"ANS=",4)==0) q.correct = tolower(line[4]);
        else if(strncmp(line,"---",3)==0) quizBank[totalQuestions++] = q;
    }
    fclose(fp);
    printf("Loaded %d questions from %s\n", totalQuestions, filename);
}

int topicLevelIndex(char topic[]){
    if(strcmp(topic,"array")==0) return 0;
    else if(strcmp(topic,"stack")==0) return 1;
    else return 2;
}

int getUserLevel(char topic[]){
    if(strcmp(topic,"array")==0) return currentUser.arrayLevel;
    else if(strcmp(topic,"stack")==0) return currentUser.stackLevel;
    else return currentUser.queueLevel;
}

void setUserLevel(char topic[], int newLevel){
    if(strcmp(topic,"array")==0) currentUser.arrayLevel=newLevel;
    else if(strcmp(topic,"stack")==0) currentUser.stackLevel=newLevel;
    else currentUser.queueLevel=newLevel;
}

void quizModule(char topic[]){
    char ans;
    int correctCount=0;
    int levelQuestions=0;
    int tLevel = getUserLevel(topic);

    printf("\n[QUIZ MODULE] Topic: %s | Level %d\n", topic, tLevel);

    // Count questions in this level
    for(int i=0;i<totalQuestions;i++){
        if(strcmp(quizBank[i].topic,topic)==0 && quizBank[i].level==tLevel)
            levelQuestions++;
    }

    if(levelQuestions==0){ printf("No questions for this level.\n"); return; }

    // Loop through questions in current level
    for(int i=0;i<totalQuestions;i++){
        if(strcmp(quizBank[i].topic,topic)==0 && quizBank[i].level==tLevel){
            printf("\nQ: %s\n",quizBank[i].question);
            printf("a) %s\nb) %s\nc) %s\nd) %s\nYour Answer: ",
                quizBank[i].options[0], quizBank[i].options[1],
                quizBank[i].options[2], quizBank[i].options[3]);
            scanf(" %c",&ans);
            ans = tolower(ans);
            if(ans == quizBank[i].correct){ points+=10; correctCount++; printf("✅ Correct! +10 points\n"); }
            else { points-=5; printf("Wrong! -5 points (Correct: %c)\n", quizBank[i].correct); }
        }
    }

    printf("\nTotal Points: %d\n", points);

    if(correctCount==levelQuestions){
        printf("You cleared Level %d! Next level unlocked.\n", tLevel);
        setUserLevel(topic,tLevel+1);
    } else {
        printf("Clear all questions to unlock next level.\nYou can retry this level again.\n");
    }

    saveUserProgress();
}

// ---------------- MAIN MENU ----------------
int main(){
    int choice, loggedIn=0;

    loadQuestionsFromFile("quizdata.txt");

    while(!loggedIn){
        printf("\n====== Welcome to Gamified DS Learning Tool ======\n");
        printf("1. Register\n2. Login\n3. Exit\nChoice: ");
        scanf("%d",&choice);
        switch(choice){
            case 1: registerUser(); break;
            case 2: loggedIn=loginUser(); break;
            case 3: exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    while(1){
        printf("\n====== Hello, %s! ======\n",currentUser.username);
        printf("Points: %d | Array Level: %d | Stack Level: %d | Queue Level: %d\n",
               points, currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel);
        printf("1. Array Module\n2. Stack Module\n3. Queue Module\n4. Quiz on Array\n5. Quiz on Stack\n6. Quiz on Queue\n7. Logout\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1: arrayModule(); break;
            case 2: stackModule(); break;
            case 3: queueModule(); break;
            case 4: quizModule("array"); break;
            case 5: quizModule("stack"); break;
            case 6: quizModule("queue"); break;
            case 7: logout(); break;
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}
