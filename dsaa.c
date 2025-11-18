// main.c
// Upgraded gamified DSA learning tool with many DSAs: heap, graph (adj list), linked lists, queue, files, etc.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <conio.h> 


#define MAX 10
#define MAX_QUESTIONS 500
#define MAX_USERS 200
#define FORUM_SIZE 200

int points = 0;

typedef struct {
    char username[50];
    char password[50];
    int points;
    int arrayLevel;
    int stackLevel;
    int queueLevel;
    int linkedListLevel;
    int treeLevel;

} User;

User currentUser;

void registerUser();
int loginUser();
void saveUserProgress();
void logout();
int topicLevelIndex(char topic[]);
void loadQuestionsFromFile(const char *filename);
void quizModule(char topic[]);

void animatePrint(const char* msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        printf("%c", msg[i]);
        Sleep(30);
    }
}


int loadUserData(const char *username, User *u) {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;

    User temp;
    while (fscanf(fp, "%s %s %d %d %d %d %d %d",temp.username, temp.password, &temp.points, &temp.arrayLevel, &temp.stackLevel, &temp.queueLevel, &temp.linkedListLevel, &temp.treeLevel) == 8) {

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
        while (fscanf(fp, "%s %s %d %d %d %d %d %d",
                      u.username, u.password, &u.points,
                      &u.arrayLevel, &u.stackLevel, &u.queueLevel,
                      &u.linkedListLevel, &u.treeLevel) == 8) {

            if (strcmp(u.username, currentUser.username) == 0) {
                fprintf(temp, "%s %s %d %d %d %d %d %d\n",
                        currentUser.username, currentUser.password, points,
                        currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel,
                        currentUser.linkedListLevel, currentUser.treeLevel);

                found = 1;
            } else {
                fprintf(temp, "%s %s %d %d %d %d %d %d\n",
                        u.username, u.password, u.points,
                        u.arrayLevel, u.stackLevel, u.queueLevel,
                        u.linkedListLevel, u.treeLevel);
            }
        }
        fclose(fp);
    }

    if (!found) {
        fprintf(temp, "%s %s %d %d %d %d %d %d\n",
                currentUser.username, currentUser.password, points,
                currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel,
                currentUser.linkedListLevel, currentUser.treeLevel);
    }

    fclose(temp);
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

// ---------------- registration / login ----------------
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

// ---------------- Array / Stack / Queue modules (unchanged) ----------------
int arr[MAX], arrSize = 0;
void printArray(int highlightPos) {
    for (int i = 0; i < arrSize; i++) {
        if (i == highlightPos)
            printf("[%d] ", arr[i]);
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
        getchar();

        switch(choice) {
            case 1:
                printf("Enter number of elements (max 10): ");
                scanf("%d", &arrSize);
                if(arrSize > MAX) arrSize = MAX;
                printf("Enter %d elements separated by space:\n", arrSize);
                for(int i=0; i<arrSize; i++) scanf("%d", &arr[i]);
                break;

            case 2:
                printf("Traversing Array (index-based direct access is O(1)): ");
                for(int i=0; i<arrSize; i++) {
                    printf("%d ", arr[i]);
                    Sleep(300);
                }
                printf("\n");
                break;

            case 3:
                printf("\nArray Insertion Explanation:\n");
                printf(" - Arrays store elements in contiguous memory.\n");
                printf(" - Insertion at middle requires shifting elements right from the insert position, O(n) time.\n");
                printf(" - Shifting will be shown below.\n");

                printf("Enter position (0 to %d) to insert: ", arrSize);
                scanf("%d", &pos);
                if(pos < 0 || pos > arrSize) { printf("Invalid position!\n"); break; }
                printf("Enter value: "); scanf("%d", &val);

                printf("\nShifting elements to the right:\n");
                for(int i=arrSize; i>pos; i--){
                    arr[i] = arr[i-1];
                    printArray(i);
                    Sleep(200);
                }

                arr[pos] = val;
                arrSize++;

                printf("\nInserted value %d at position %d:\n", val, pos);
                printArray(-1);
                break;

            case 4:
                printf("\nArray Deletion Explanation:\n");
                printf(" - Deletion requires shifting elements leftward to fill gap.\n");
                printf(" - This operation can be O(n) in worst case.\n");

                printf("Enter position (0 to %d) to delete: ", arrSize - 1);
                scanf("%d", &pos);
                if(pos < 0 || pos >= arrSize) { printf("Invalid position!\n"); break; }

                printf("\nDeleting element %d at position %d:\n", arr[pos], pos);
                for(int i=pos; i<arrSize-1; i++) {
                    arr[i] = arr[i+1];
                    printArray(i);
                    Sleep(200);
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

int stack[MAX], top = -1;

void showStack() {
    if(top == -1) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Current Stack (LIFO - Last In First Out): ");
    for(int i=top; i>=0; i--)
        printf("%d ", stack[i]);
    printf("\n");
}

void push(int val) {
    if(top == MAX - 1) {
        printf("Stack Overflow!\n");
    } else {
        printf("\nStack Push Explanation:\n");
        printf(" - Stack follows LIFO (Last In First Out) principle.\n");
        printf(" - Push adds element to the top.\n");

        printf("Pushing %d...\n", val);
        Sleep(400);
        stack[++top] = val;
        showStack();
        Sleep(200);
    }
}

void pop() {
    if(top == -1) {
        printf("Stack Underflow!\n");
    } else {
        printf("\nStack Pop Explanation:\n");
        printf(" - Pop removes the top element.\n");

        printf("Popping %d...\n", stack[top]);
        Sleep(400);
        top--;
        showStack();
        Sleep(200);
    }
}

void stackModule() {
    int choice, val;
    do {
        printf("\n[STACK MODULE]\n1. Push\n2. Pop\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                push(val);
                break;
            case 2:
                pop();
                break;
            case 3: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 3);
}

int queueArr[MAX], qfront=-1, qrear=-1;

void showQueue() {
    if(qfront == -1 || qfront > qrear) {
        printf("Queue empty.\n");
        return;
    }
    printf("Current Queue (FIFO - First In First Out): ");
    for(int i = qfront; i <= qrear; i++)
        printf("%d ", queueArr[i]);
    printf("\n");
}

void enqueueVal(int val) {
    if(qrear == MAX - 1) {
        printf("Queue Full!\n");
    } else {
        printf("\nQueue Enqueue Explanation:\n");
        printf(" - Queue follows FIFO (First In First Out) principle.\n");
        printf(" - Enqueue adds element at the rear.\n");

        printf("Enqueuing %d...\n", val);
        Sleep(400);
        if(qfront == -1) qfront = 0;
        queueArr[++qrear] = val;
        showQueue();
        Sleep(200);
    }
}

void dequeueVal() {
    if(qfront == -1 || qfront > qrear) {
        printf("Queue Empty!\n");
    } else {
        printf("\nQueue Dequeue Explanation:\n");
        printf(" - Dequeue removes element from the front.\n");

        printf("Dequeuing %d...\n", queueArr[qfront]);
        Sleep(400);
        qfront++;
        showQueue();
        Sleep(200);
    }
}

void queueModule() {
    int choice, val;
    do {
        printf("\n[QUEUE MODULE]\n1. Enqueue\n2. Dequeue\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("Enter value: ");
                scanf("%d", &val);
                enqueueVal(val);
                break;
            case 2:
                dequeueVal();
                break;
            case 3: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 3);
}
// ------------------- LINKED LIST MODULE -------------------
typedef struct LNode {
    int data;
    struct LNode* next;
} LNode;

LNode* listHead = NULL;

void visualizeLinkedList() {

    LNode* temp = listHead;
    while (temp != NULL) {
        printf("[%d] -> ", temp->data);
        Sleep(300);
        temp = temp->next;
    }
    printf("NULL\n");
}

void insertLL(int val) {
    LNode* n = malloc(sizeof(LNode));
    n->data = val;
    n->next = listHead;
    listHead = n;
}

void deleteLL(int val) {
    LNode *t=listHead, *prev=NULL;
    while(t && t->data!=val){
        prev=t;
        t=t->next;
    }
    if(!t){ printf("Value not found.\n"); return; }

    if(prev) prev->next = t->next;
    else listHead = t->next;
    free(t);
}

void linkedListMenu() {
    int ch,val;
    do {
        printf("\nLINKED LIST MENU\n"
               "1. Insert\n"
               "2. Delete\n"
               "3. Visualize\n"
               "4. Back\nChoice: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: printf("Enter value: "); scanf("%d",&val); insertLL(val); break;
            case 2: printf("Enter value: "); scanf("%d",&val); deleteLL(val); break;
            case 3: visualizeLinkedList(); break;
            case 4: break;
            default: printf("Invalid!\n");
        }
    } while(ch!=4);
}

// ------------------- BINARY TREE MODULE -------------------
typedef struct TNode {
    int data;
    struct TNode *left, *right;
} TNode;

TNode* treeRoot = NULL;

TNode* insertBST(TNode* root, int val) {
    if(!root){
        TNode* n = malloc(sizeof(TNode));
        n->data=val; n->left=n->right=NULL;
        return n;
    }
    if(val < root->data) root->left = insertBST(root->left,val);
    else root->right = insertBST(root->right,val);
    return root;
}

void animateTree(TNode* root, int space) {
    if (!root) return;
    Sleep(200);
    space += 5;
    animateTree(root->right, space);
    printf("\n");
    for (int i = 5; i < space; i++) printf(" ");
    printf("%d\n", root->data);
    animateTree(root->left, space);
}

void visualizeTree() {
    animateTree(treeRoot, 0);
}

void treeMenu() {
    int ch,val;
    do {
        printf("\nBINARY TREE MENU\n"
               "1. Insert\n"
               "2. Visualize\n"
               "3. Back\nChoice: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: printf("Enter value: "); scanf("%d",&val);
                    treeRoot = insertBST(treeRoot,val);
                       break;

            case 2: visualizeTree(); break;

            case 3: break;
            default: printf("Invalid!\n");
        }
    } while(ch!=3);
}


// ------------------ QUIZ system ------------------
typedef struct {
    char topic[20];
    int level;
    char question[200];
    char options[4][100];
    char correct;
} Question;

Question quizBank[MAX_QUESTIONS];
int totalQuestions=0;
void initializeUserLevels() {
    currentUser.arrayLevel = 1;
    currentUser.stackLevel = 1;
    currentUser.queueLevel = 1;
    currentUser.linkedListLevel = 1;
    currentUser.treeLevel = 1;
}

void loadQuestionsFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Warning: Cannot open %s (quiz file).\n", filename);
        return;
    }

    char line[256];
    Question q;
    totalQuestions = 0;
    memset(&q, 0, sizeof(Question));

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "topic=", 6) == 0) {
            strcpy(q.topic, line + 6);
        }
        else if (strncmp(line, "level=", 6) == 0) {
            q.level = atoi(line + 6);
        }
        else if (strncmp(line, "Q=", 2) == 0) {
            strcpy(q.question, line + 2);
        }
        else if (strncmp(line, "A=", 2) == 0) {
            strcpy(q.options[0], line + 2);
        }
        else if (strncmp(line, "B=", 2) == 0) {
            strcpy(q.options[1], line + 2);
        }
        else if (strncmp(line, "C=", 2) == 0) {
            strcpy(q.options[2], line + 2);
        }
        else if (strncmp(line, "D=", 2) == 0) {
            strcpy(q.options[3], line + 2);
        }
        else if (strncmp(line, "ANS=", 4) == 0) {
            q.correct = tolower(line[4]);  
        }
        else if (strncmp(line, "---", 3) == 0) {
            quizBank[totalQuestions++] = q;

            memset(&q, 0, sizeof(Question));
        }
    }

    fclose(fp);
    printf("Loaded %d questions from %s\n", totalQuestions, filename);
}

int getUserLevel(char topic[]){
    if(strcmp(topic,"array")==0) return currentUser.arrayLevel;
    else if(strcmp(topic,"stack")==0) return currentUser.stackLevel;
    else if(strcmp(topic,"queue")==0) return currentUser.queueLevel;
    else if(strcmp(topic,"linked list")==0) return currentUser.linkedListLevel;
    else if(strcmp(topic,"tree")==0) return currentUser.treeLevel;
    else return 1;
}


void setUserLevel(char topic[], int newLevel){
    if(strcmp(topic,"array")==0) currentUser.arrayLevel = newLevel;
    else if(strcmp(topic,"stack")==0) currentUser.stackLevel = newLevel;
    else if(strcmp(topic,"queue")==0) currentUser.queueLevel = newLevel;
    else if(strcmp(topic,"linked list")==0) currentUser.linkedListLevel = newLevel;
    else if(strcmp(topic,"tree")==0) currentUser.treeLevel = newLevel;
}


void shuffleQuestions(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void shuffleOptions(Question *q) {
    int pos[4] = {0,1,2,3};

    for(int i=3;i>0;i--){
        int j = rand() % (i+1);
        int temp = pos[i];
        pos[i] = pos[j];
        pos[j] = temp;
    }

    char newOpt[4][200];
    char correctChar = q->correct;
    int correctIndex = correctChar - 'a';

    for(int i=0;i<4;i++){
        strcpy(newOpt[i], q->options[pos[i]]);
        if(pos[i] == correctIndex)
            q->correct = 'a' + i;  
    }

    
    for(int i=0;i<4;i++)
        strcpy(q->options[i], newOpt[i]);
}

void quizModule(char topic[]){
    char ans;
    int correctCount = 0;
    int levelQuestions = 0;
    int tLevel = getUserLevel(topic);

    printf("\n[QUIZ MODULE] Topic: %s | Level %d\n", topic, tLevel);

    int indexes[totalQuestions];
    int cnt = 0;

    for(int i = 0; i < totalQuestions; i++){
        if(strcmp(quizBank[i].topic,topic)==0 && quizBank[i].level == tLevel)
            indexes[cnt++] = i;
    }

    levelQuestions = cnt;

    if(levelQuestions == 0){
        printf("No questions for this level.\n");
        return;
    }

    shuffleQuestions(indexes, cnt);
        int skipAvailable = 1;   

    for(int k = 0; k < cnt; k++){
        int i = indexes[k];
            shuffleOptions(&quizBank[i]);

        printf("\nQ: %s\n", quizBank[i].question);
printf("a) %s\nb) %s\nc) %s\nd) %s\n",
    quizBank[i].options[0], quizBank[i].options[1],
    quizBank[i].options[2], quizBank[i].options[3]);
if(skipAvailable)
    printf("Press **S** to SKIP this question (1 lifeline left)\n");
else
    printf("(Skip used already)\n");

printf("Your Answer: ");
scanf(" %c", &ans);
ans = tolower(ans);

// ------------------ SKIP HANDLER ------------------
if(ans == 's' && skipAvailable){
    printf("⏩ Question Skipped! (No points gained or lost)\n");
    skipAvailable = 0;
    continue;
}

        if(ans == quizBank[i].correct){
            points += 10;
            correctCount++;
            printf("Correct! +10\n");
        } else {
            points -= 5;
            printf("Wrong! -5 (Correct: %c)\n", quizBank[i].correct);
        }
    }

    printf("\nTotal Points: %d\n", points);

    if(correctCount == levelQuestions){

    saveUserProgress();
}
}


// ------------------ LEADERBOARD (Max-Heap + optional sort) ------------------
typedef struct UserNode {
    char username[50];
    char password[50];
    int points;
    int arrayLevel;
    int stackLevel;
    int queueLevel;
    struct UserNode* next;
} UserNode;

UserNode* head = NULL;

void appendUser(UserNode** head_ref, UserNode* new_user) {
    if (*head_ref == NULL) {
        *head_ref = new_user;
        new_user->next = NULL;
    } else {
        UserNode* last = *head_ref;
        while (last->next != NULL) last = last->next;
        last->next = new_user;
        new_user->next = NULL;
    }
}

int loadUsersToList(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening the file %s\n", filename);
        return 0;
    }

    UserNode temp_user;
    int count = 0;
    UserNode* current = head;
    while(current != NULL){
        UserNode* t = current->next;
        free(current);
        current = t;
    }
    head = NULL;

    while (fscanf(fp, "%49s %49s %d %d %d %d", temp_user.username, temp_user.password,
                  &temp_user.points, &temp_user.arrayLevel, &temp_user.stackLevel,
                  &temp_user.queueLevel) == 6) {
        UserNode* new_node = (UserNode*) malloc(sizeof(UserNode));
        if (!new_node) {
            fclose(fp);
            return count; 
        }
        *new_node = temp_user;
        new_node->next = NULL;
        appendUser(&head, new_node);
        count++;
    }
    fclose(fp);
    return count;
}

void sortUsersByPoints() {
    if (!head || !head->next) return;

    int swapped;
    UserNode *ptr1;
    UserNode *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->points < ptr1->next->points) {
                char temp_str[50];
                int temp_int;

                strcpy(temp_str, ptr1->username);
                strcpy(ptr1->username, ptr1->next->username);
                strcpy(ptr1->next->username, temp_str);

                strcpy(temp_str, ptr1->password);
                strcpy(ptr1->password, ptr1->next->password);
                strcpy(ptr1->next->password, temp_str);

                temp_int = ptr1->points;
                ptr1->points = ptr1->next->points;
                ptr1->next->points = temp_int;

                
                temp_int = ptr1->arrayLevel;
                ptr1->arrayLevel = ptr1->next->arrayLevel;
                ptr1->next->arrayLevel = temp_int;

                temp_int = ptr1->stackLevel;
                ptr1->stackLevel = ptr1->next->stackLevel;
                ptr1->next->stackLevel = temp_int;

                temp_int = ptr1->queueLevel;
                ptr1->queueLevel = ptr1->next->queueLevel;
                ptr1->next->queueLevel = temp_int;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void displayLeaderboard() {
    int n = loadUsersToList("users.txt");
    if (n == 0 || head == NULL) {
        printf("No users found.\n");
        return;
    }

    sortUsersByPoints();

    printf("\n===== Leaderboard =====\n");
    UserNode* temp = head;
    int rank = 1;
    while (temp != NULL && rank <= 10) {
    printf("%-15s %5d points\n", temp->username, temp->points);
    temp = temp->next;
    rank++;
}

    printf("=======================\n");
}

void freeList() {
    UserNode* current = head;
    while (current != NULL) {
        UserNode* next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}
// ------------------ FRIEND SYSTEM (Graph adjacency list) ------------------
typedef struct FriendNode {
    char friendName[50];
    struct FriendNode* next;
} FriendNode;

FriendNode* friendGraph[MAX_USERS];
int totalKnownUsers = 0;
char userNames[MAX_USERS][50];

int userExists(const char* name){
    FILE *f = fopen("users.txt","r");
    if(!f) return 0;
    char user[50], pass[50];
    while(fscanf(f,"%s %s",user,pass)==2){
        if(strcmp(user,name)==0){
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int findUserIndex(const char* name){
    for(int i=0;i<totalKnownUsers;i++){
        if(strcmp(userNames[i], name)==0) return i;
    }
    return -1;
}
int getOrCreateUserIndex(const char* name){
    int idx = findUserIndex(name);
    if(idx!=-1) return idx;
    if(totalKnownUsers < MAX_USERS){
        strcpy(userNames[totalKnownUsers], name);
        friendGraph[totalKnownUsers] = NULL;
        return totalKnownUsers++;
    }
    return -1;
}

int friendExists(int idx1, const char* friendname){
    FriendNode* p = friendGraph[idx1];
    while(p){
        if(strcmp(p->friendName, friendname)==0) return 1;
        p = p->next;
    }
    return 0;
}

void addFriendEdge(const char* user1, const char* user2) {

    if (!userExists(user2)) {
        printf("'%s' is NOT a registered user. Cannot add.\n", user2);
        return;
    }

    if (strcmp(user1, user2) == 0) {
        printf("You cannot add yourself as a friend.\n");
        return;
    }

    int idx1 = getOrCreateUserIndex(user1);
    int idx2 = getOrCreateUserIndex(user2);

    if (idx1 == -1 || idx2 == -1) {
        printf("Cannot add friend (maximum users reached).\n");
        return;
    }

    if (friendExists(idx1, user2)) {
        printf("'%s' is already your friend.\n", user2);
        return;
    }

    FriendNode* newNode1 = (FriendNode*)malloc(sizeof(FriendNode));
    strcpy(newNode1->friendName, user2);
    newNode1->next = friendGraph[idx1];
    friendGraph[idx1] = newNode1;

    FriendNode* newNode2 = (FriendNode*)malloc(sizeof(FriendNode));
    strcpy(newNode2->friendName, user1);
    newNode2->next = friendGraph[idx2];
    friendGraph[idx2] = newNode2;

    FILE *f = fopen("friends.txt", "a");
    if (f) {
        fprintf(f, "%s %s\n", user1, user2);
        fclose(f);
    }

    printf("Friend added successfully!\n");
}


void loadFriendsFromFile(){
    FILE *f = fopen("friends.txt","r");
    if(!f) return;
    char a[50], b[50];
    while(fscanf(f,"%s %s", a, b)==2){
        int i1 = getOrCreateUserIndex(a);
        int i2 = getOrCreateUserIndex(b);
        if(!friendExists(i1,b)){
            FriendNode* n1 = (FriendNode*)malloc(sizeof(FriendNode));
            strcpy(n1->friendName,b); n1->next = friendGraph[i1]; friendGraph[i1]=n1;
        }
        if(!friendExists(i2,a)){
            FriendNode* n2 = (FriendNode*)malloc(sizeof(FriendNode));
            strcpy(n2->friendName,a); n2->next = friendGraph[i2]; friendGraph[i2]=n2;
        }
    }
    fclose(f);
}

void viewFriends(const char* username) {
    int idx = findUserIndex(username);
    printf("\nFriends of %s:\n", username);
    if(idx==-1) { printf("No friends added yet.\n"); return; }
    FriendNode* temp = friendGraph[idx];
    if (!temp) {
        printf("No friends added yet.\n");
        return;
    }
    while (temp) {
        printf("• %s\n", temp->friendName);
        temp = temp->next;
    }
}
void suggestFriends(const char* username) {
    int start = findUserIndex(username);
    if (start == -1) {
        printf("User not found.\n");
        return;
    }

    int visited[MAX_USERS] = {0};          
    int depth[MAX_USERS];                
    int suggested[MAX_USERS] = {0};       

    int q[MAX_USERS], front = 0, rear = 0;

    q[rear++] = start;
    visited[start] = 1;
    depth[start] = 0;

    int suggestions = 0;

    while (front < rear) {
        int cur = q[front++];

        if (depth[cur] >= 3)
            continue;

        FriendNode* f = friendGraph[cur];

        while (f) {
            int nx = findUserIndex(f->friendName);

            if (nx != -1) {
                if (!visited[nx]) {
                    visited[nx] = 1;
                    depth[nx] = depth[cur] + 1;
                    q[rear++] = nx;
                }

                if (depth[cur] >= 1 && depth[cur] <= 2) {
                    if (depth[nx] == depth[cur] + 1) {   
                        if (nx != start &&              
                            depth[nx] >= 2 && depth[nx] <= 3 && 
                            !suggested[nx]) {

                            printf("• %s (depth %d)\n", f->friendName, depth[nx]);
                            suggested[nx] = 1;
                            suggestions++;
                        }
                    }
                }
            }
            f = f->next;
        }
    }

    if (suggestions == 0)
        printf("No suggestions.\n");
}

void friendMenu() {
    int choice;
    char fname[50];

    do {
        printf("\n[FRIEND SYSTEM]\n");
        printf("1. Add Friend\n2. View Friends\n3. Suggest Friends\n4. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter friend's username: ");
                scanf("%49s", fname);
                addFriendEdge(currentUser.username, fname);
                break;
            case 2:
                viewFriends(currentUser.username);
                break;
            case 3:
                suggestFriends(currentUser.username);
                break;
            case 4:
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

// ------------------ CHAT system (linked list of messages) ------------------
typedef struct Message {
    char sender[50];
    char receiver[50];
    char text[200];
    struct Message* next;
} Message;

Message* chatHead = NULL;

void loadChatsFromFile(){
    FILE *f = fopen("chats.txt","r");
    if(!f) return;
    char s[50], rcv[50], txt[200];
    while(fgets(txt,200,f)){
        // each line format: sender|receiver|message\n
        char *p = strchr(txt,'\n'); if(p) *p=0;
        char *tok1 = strtok(txt,"|");
        char *tok2 = strtok(NULL,"|");
        char *tok3 = strtok(NULL,"|");
        if(tok1 && tok2 && tok3){
            Message* m = (Message*)malloc(sizeof(Message));
            strcpy(m->sender, tok1); strcpy(m->receiver, tok2); strcpy(m->text, tok3);
            m->next = NULL;
            if(!chatHead) chatHead = m;
            else {
                Message* t = chatHead; while(t->next) t = t->next; t->next = m;
            }
        }
    }
    fclose(f);
}

void persistChatToFile(const char* sender, const char* receiver, const char* text){
    FILE *f = fopen("chats.txt","a");
    if(!f) return;
    fprintf(f, "%s|%s|%s\n", sender, receiver, text);
    fclose(f);
}

int isFriend(const char *friendName) {
    int idx = findUserIndex(currentUser.username);
    if(idx == -1) return 0;

    FriendNode* p = friendGraph[idx];
    while(p) {
        if(strcmp(p->friendName, friendName) == 0) return 1;
        p = p->next;
    }
    return 0;
}

void sendMessageToFriend(){
    char friendnm[50], text[200];
    printf("Enter friend's username to message: ");
    scanf("%49s", friendnm);
    getchar();

    if(!isFriend(friendnm)) {
        printf("Error: %s is not your friend. Cannot send message.\n", friendnm);
        return;
    }

    printf("Write message: ");
    fgets(text, 200, stdin);
    char *p = strchr(text,'\n'); if(p) *p=0;

    
    Message* m = (Message*)malloc(sizeof(Message));
    strcpy(m->sender, currentUser.username);
    strcpy(m->receiver, friendnm);
    strcpy(m->text, text);
    m->next = NULL;

    if(!chatHead) chatHead = m;
    else {
        Message* t = chatHead; while(t->next) t = t->next;
        t->next = m;
    }
    persistChatToFile(currentUser.username, friendnm, text);
    printf("Message sent.\n");
}

void viewChatWithFriend(){
    char friendnm[50];
    printf("Enter friend's username to view chat: ");
    scanf("%s", friendnm);
    printf("\n--- Chat between %s and %s ---\n", currentUser.username, friendnm);
    Message* t = chatHead;
    int shown = 0;
    while(t){
        if((strcmp(t->sender,currentUser.username)==0 && strcmp(t->receiver,friendnm)==0) ||
           (strcmp(t->sender,friendnm)==0 && strcmp(t->receiver,currentUser.username)==0)){
           if(strcmp(t->sender,currentUser.username)==0)
    printf("YOU ➤ %s\n", t->text);
else
    printf("%s ➤ %s\n", t->sender, t->text);

            shown=1;
        }
        t = t->next;
    }
    if(!shown) printf("No messages found.\n");
}

void chatMenu(){
    int c;
    do{
        printf("\n[CHAT]\n1. Send Message\n2. View Chat\n3. Back\nChoice: ");
        scanf("%d",&c); getchar();
        switch(c){
            case 1: sendMessageToFriend(); break;
            case 2: viewChatWithFriend(); break;
            case 3: break;
            default: printf("Invalid\n");
        }
    } while(c!=3);
}

// ------------------ BADGES / ACHIEVEMENTS ------------------
typedef struct {
    char username[50];
    char badgeName[80];
} Badge;

void awardBadge(const char* username, const char* badge){
    FILE *f = fopen("badges.txt","r");
    int exists = 0;
    if(f){
        char un[50], bn[80];
        while(fscanf(f,"%s [^\n]s", un, bn)==2){
            if(strcmp(un,username)==0 && strcmp(bn,badge)==0){ exists=1; break;}
        }
        fclose(f);
    }
    if(!exists){
        FILE *fw = fopen("badges.txt","a");
        if(fw){
            fprintf(fw, "%s %s\n", username, badge);
            fclose(fw);
            printf("🎖 Awarded badge: %s\n", badge);
        }
    }
}

void checkAndAwardBadges(){
    if(points >= 100) awardBadge(currentUser.username, "Centurion (100+ points)");
    if(currentUser.arrayLevel >= 3) awardBadge(currentUser.username, "Array Master (Lv3)");
    if(currentUser.stackLevel >= 3) awardBadge(currentUser.username, "Stack Master (Lv3)");
    if(currentUser.queueLevel >= 3) awardBadge(currentUser.username, "Queue Master (Lv3)");
}

void showMyBadges(){
    FILE *f = fopen("badges.txt","r");
    if(!f){ printf("No badges found.\n"); return; }
    char un[50], bn[80];
    int any=0;
    printf("\n--- Your Badges ---\n");
    while(fscanf(f,"%s [^\n]s", un, bn)==2){
        if(strcmp(un, currentUser.username)==0){
            printf("• %s\n", bn);
            any=1;
        }
    }
    if(!any) printf("No badges yet.\n");
    fclose(f);
}

// ------------------ MINI DSA CHALLENGES ------------------



typedef struct Challenge {
    char question[200];
    char options[4][100];
    char correct;
    int pointsReward;
} Challenge;

Challenge challenges[MAX_QUESTIONS];
int challengesCount = 0;


void loadChallengesFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open %s\n", filename);
        return;
    }

    char line[512];
    Challenge q;
    memset(&q, 0, sizeof(Challenge));
    challengesCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "question=", 9) == 0)
            sscanf(line + 9, "%199[^\n]", q.question);
        else if (strncmp(line, "optionA=", 8) == 0)
            sscanf(line + 8, "%99[^\n]", q.options[0]);
        else if (strncmp(line, "optionB=", 8) == 0)
            sscanf(line + 8, "%99[^\n]", q.options[1]);
        else if (strncmp(line, "optionC=", 8) == 0)
            sscanf(line + 8, "%99[^\n]", q.options[2]);
        else if (strncmp(line, "optionD=", 8) == 0)
            sscanf(line + 8, "%99[^\n]", q.options[3]);
        else if (strncmp(line, "correct=", 8) == 0) {
            char temp;
            sscanf(line + 8, " %c", &temp);
            q.correct = tolower(temp);
        }
        else if (strncmp(line, "points=", 7) == 0)
            sscanf(line + 7, "%d", &q.pointsReward);
        else if (strncmp(line, "---", 3) == 0) {
            if (challengesCount < MAX_QUESTIONS) {
                challenges[challengesCount++] = q;
                memset(&q, 0, sizeof(Challenge));
            }
        }
    }
    if (strlen(q.question) > 0 && challengesCount < MAX_QUESTIONS) {
        challenges[challengesCount++] = q;
    }
    fclose(fp);
    printf("Loaded %d questions from %s\n", challengesCount, filename);
}

void miniChallengesMenu(){
    printf("\n--- Mini DSA Challenges ---\n");
    int earnedPoints = 0;
    for(int i=0; i<challengesCount; i++){
        printf("\nQ%d: %s\n", i+1, challenges[i].question);
        printf("a) %s\nb) %s\nc) %s\nd) %s\nYour Answer: ",
               challenges[i].options[0], challenges[i].options[1],
               challenges[i].options[2], challenges[i].options[3]);
        char ans;
        scanf(" %c", &ans);
        ans = tolower(ans);
        if(ans == challenges[i].correct){
            printf("Correct! +%d points\n", challenges[i].pointsReward);
            points += challenges[i].pointsReward;
            earnedPoints += challenges[i].pointsReward;
        } else {
            printf("Wrong. No points. Correct: %c\n", challenges[i].correct);
        }
    }
    printf("\nTotal earned in this quiz: %d points\n", earnedPoints);
     saveUserProgress();
     checkAndAwardBadges();
}
void generateReport(){
    FILE *f = fopen("reports.txt","a");
    if(!f) { printf("Cannot write report.\n"); return; }
    int friendCount = 0;
    int idx = findUserIndex(currentUser.username);
    if(idx!=-1){
        FriendNode* p = friendGraph[idx];
        while(p){ friendCount++; p=p->next; }
    }
    int badgesCount = 0;
    FILE *bf = fopen("badges.txt","r");
    if(bf){
        char un[50], bn[80];
        while(fscanf(bf, "%s [^\n]s", un, bn)==2){
            if(strcmp(un, currentUser.username)==0) badgesCount++;
        }
        fclose(bf);
    }
    fprintf(f, "Report for %s | Points: %d | ArrayLv: %d | StackLv: %d | QueueLv: %d | Friends: %d | Badges: %d\n",
            currentUser.username, points, currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel, friendCount, badgesCount);
    fclose(f);
    printf("Report generated and saved.\n");
}

// ------------------ Progress Bar ------------------
void displayProgressBar(){
    int levelSum = currentUser.arrayLevel + currentUser.stackLevel + currentUser.queueLevel;
    int badgeBonus = 0;
    FILE *bf = fopen("badges.txt","r");
    if(bf){
        char un[50], bn[80];
        while(fscanf(bf, "%s [^\n]s", un, bn)==2){
            if(strcmp(un, currentUser.username)==0) badgeBonus++;
        }
        fclose(bf);
    }
    int score = levelSum * 10 + badgeBonus * 5 + (points/10);
    if(score>100) score = 100;
    int filled = score/10;
    printf("\nProgress: [");
    for(int i=0;i<10;i++) printf(i<filled?"#":"-");
    printf("] %d%%\n", score);
}

// ------------------ Load initial persistence data ------------------
void loadAllPersistentData(){
    totalKnownUsers = 0;
    for(int i=0;i<MAX_USERS;i++) { friendGraph[i]=NULL; userNames[i][0]=0; }
    loadFriendsFromFile();
    loadChatsFromFile();

}

// ------------------ Main Menu integration ------------------
int main(){

    int choice, loggedIn=0;

    srand(time(NULL));
    loadChatsFromFile();
    loadQuestionsFromFile("quiz.txt");
    loadAllPersistentData();



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

    getOrCreateUserIndex(currentUser.username);


    while(1){
        printf("\n====== Hello, %s! ======\n",currentUser.username);
        printf("Points: %d | Array Level: %d | Stack Level: %d | Queue Level: %d\n",
               points, currentUser.arrayLevel, currentUser.stackLevel, currentUser.queueLevel);
        displayProgressBar();
        printf("\n1. Array Module\n2. Stack Module\n3. Queue Module\n4. Linked List Module\n5. Binary Search Tree Module\n6. Quiz on Array\n7. Quiz on Stack\n8. Quiz on Queue\n9. Quiz on Linked List\n10. Quiz on Binary Search Tree\n11. Leaderboard\n12. Friend System\n13. Chat\n14. Mini Challenges\n15. Badges\n16. Generate Report\n17. Save Progress\n18. Logout\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice){
           case 1: arrayModule(); break;
            case 2: stackModule(); break;
            case 3: queueModule(); break;
            case 4: linkedListMenu(); break;
            case 5: treeMenu(); break;

            case 6: initializeUserLevels();
            quizModule("array"); checkAndAwardBadges(); break;
            case 7: initializeUserLevels();quizModule("stack"); checkAndAwardBadges(); break;
            case 8: initializeUserLevels();quizModule("queue"); checkAndAwardBadges(); break;
            case 9: initializeUserLevels();quizModule("linked list"); checkAndAwardBadges(); break;
            case 10:initializeUserLevels();quizModule("tree"); checkAndAwardBadges(); break;

            case 11: displayLeaderboard();
                     freeList(); break;
            case 12: friendMenu(); break;
            case 13: chatMenu(); break;
            case 14: points = 0;
    loadChallengesFromFile("challenges.txt");
    if(challengesCount == 0){
        printf("No challenges loaded. Please check the challenges.txt file.\n");
        return 1;
    }
    miniChallengesMenu();
    break;
            case 15: showMyBadges(); break;
            case 16: generateReport(); break;
            case 17: saveUserProgress(); printf("Saved.\n"); break;
            case 18: saveUserProgress(); printf("Saved. Logging out.\n"); exit(0); break;
            default: printf("Invalid choice!\n");
        }
        
        Sleep(200);
    }

    return 0;
}


