#include<string.h>
#include<mysql.h>
#include<sstream>
#include<stdlib.h>
#include<cstdio>
#include<string>

MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *connection, *mysql;
string query;
string command;

void SetColor(int Forg);
// add transition to array of linked list
void addToBeginList(List *list, char symbol, int next_state){
    member *tmp;
    tmp = new member;
    tmp -> symbol = symbol;
    tmp -> next_state = next_state;
    if(list -> n == 0){
        tmp -> next = NULL;
        list -> front = tmp;
    }else{
        tmp -> next = list -> front;
        list -> front = tmp;
    }
    list -> n += 1;
}
// queue is used to store new state in convert and minimize and it is used to store possible state in test string function

Queue* createEmptyQueue(){
    Queue *Q;
    Q = new Queue;
    Q -> n = 0;
    Q -> front = NULL;
    Q -> rear = NULL;

    return Q;
}
void enqueue(Queue *Q, int newData){
    Element *e;
    e = new Element;
    e -> data = newData;
    e -> next = NULL;
    if(Q->n == 0){
        Q->front = e;
        Q->rear = e;
    }else{
        Q->rear->next = e;
        Q->rear = e;
    }
    Q->n += 1;
}
void dequeue(Queue *Q){
    Element *tmp;
    tmp = Q->front;
    Q->front = Q->front->next;
    delete tmp;
    Q->n -= 1;
}
// this function is used to test others function if they are correct such as test string
void displayQueue(Queue *Q){
    Element *tmp;
    tmp = Q->front;
    while(tmp!=NULL){
        cout <<" "<<tmp->data;
        tmp = tmp->next;
    }
    cout<<"\n";
}
// this function is used to check a queue if it contains the final state
bool isFinal(Queue *Q, int n_final, int *final){
    Element *tmp;
    tmp = Q -> front;
    while(tmp!=NULL){
        for(int j=0;j<n_final;j++){
            if(tmp->data == final[j]){
                return true;
            }
        }
        tmp = tmp -> next;
    }
    return false;
}
// this function is used to check if a state already exists in queue
bool isNotExistInQueue(Queue *Q, int state){
    Element *tmp;
    tmp = Q -> front;
    while(tmp != NULL){
        if(tmp -> data == state){
            return false;
        }
        tmp = tmp -> next;
    }
    return true;
}
// in edit function we need this to delete transition from state
List *DeleteTranFromState(List **ArrayOfState, char symbol, int next_state, int state){
    member* tmp = ArrayOfState[state] -> front;
    member* prev = NULL;

    while(tmp!=NULL){
        if(tmp->symbol==symbol && tmp->next_state==next_state){
            if(prev == NULL){
                ArrayOfState[state] -> front = tmp -> next;
                delete tmp;
                break;
            }else{
                if(tmp->next == NULL){
                    prev->next = NULL;
                }
                prev -> next = tmp -> next;
                delete tmp;
                break;
            }
        }else{
            prev = tmp;
            tmp = tmp -> next;
        }
    }
    ArrayOfState[state]->n -= 1;
    return ArrayOfState[state];
}
// recursive function used to delete symbol from all states of FA
List *DeleteSymbolFromState(List **ArrayOfState, char symbol, int state){
    member* tmp = ArrayOfState[state] -> front;
    member* prev = NULL;

    while(tmp!=NULL){
        if(tmp->symbol==symbol){
            if(prev == NULL){
                ArrayOfState[state] -> front = tmp -> next;
                delete tmp;
                break;
            }else{
                if(tmp->next == NULL){
                    prev->next = NULL;
                }
                prev -> next = tmp -> next;
                delete tmp;
                break;
            }
        }
        prev = tmp;
        tmp = tmp -> next;
    }
    if(tmp!=NULL){
        ArrayOfState[state] = DeleteSymbolFromState(ArrayOfState, symbol, state);
    }
    ArrayOfState[state]->n -= 1;
    return ArrayOfState[state];
}

// this function used to copyQ in minimize, to code faster
Queue *copyQ(Queue *Q) {
    Element *tmp = Q->front;  //holds the current node
    Queue *QueueCopy = createEmptyQueue();

    //enqueue data into new Queue
    while(tmp != NULL) {
        enqueue(QueueCopy, tmp->data);
        tmp = tmp->next;
    }
    return QueueCopy;
}
Queue *deleteQueue(Queue *Q){
    Element *tmp = Q -> front;
    while(tmp != NULL){
        tmp = tmp->next;
        Q->n -= 1;
        delete Q->front;
        Q->front=tmp;
    }
    return Q;
}
// this function is used to convert NFA to DFA. To define uniqueness of a set of states we need to do sum of square so that we can check later --
// if the set of states already exist
int checkIfExistSqSum(int * sumOfSquare, int p, int data){
    for(int i=0;i<p;i++){
        if(sumOfSquare[i]==data){
            return i;       // it is already exist and i is the state
            break;
        }
    }
    return -1;          //not exist (create new state) increment new state keep looping
}
// display table of transitions of each states in FA
void displayTable(List **ArrayOfState,int n ,char* symbol,int n_symbol,int* final, int nf){

    cout<<("\n\t\t\t\t =======");
    for(int i=0; i<n_symbol+1; i++){
        cout<<("================");
    }
    cout<<endl;
    cout<<("\t\t\t\t|\t|");
    for(int i=0; i<n_symbol+1; i++){
        cout<<"\t"<<symbol[i]<<"\t|";
    }
    cout<<("\n\t\t\t\t =======");
    for(int i=0; i<n_symbol+1; i++){
        cout<<("================");
    }
    cout<<endl;
    for(int i=0;i<n;i++){
        cout<<("\t\t\t\t| ");
//        int space=0;
        for(int k=0; k<nf; k++){
            if(final[k]==i){
                cout<<"*";
            }
//            else{
//                space = 1;
//            }
        }
        if(i==0) cout<<("->");
//        if(i!=0 && space==1){
//                cout<<"  ";
//        }
        cout<<"q"<<i<<"\t|\t";
        for(int j=0; j<n_symbol+1; j++){
            member* temp = ArrayOfState[i]->front;
            member* temp1 = ArrayOfState[i]->front;
            int count=0;
            int count1=0;

            while(temp!=NULL){
                if(temp->symbol==symbol[j]){
                    // printf("q%d ",temp->next_state);
                    cout<<"q"<<temp->next_state;
                    if(temp->symbol=='e') count1++;
                }else if(temp->symbol!='e'&&j==n_symbol){
                    count1++;
                    if(count1==1) cout<<("");
                    else         cout<<("");
                }
                temp=temp->next;
            }
            cout<<("\t|\t");
        }
        cout<<("\n\t\t\t\t =======");
        for(int i=0; i<n_symbol+1; i++){
            cout<<("================");
        }
        cout<<endl;
    }
}
// check if a state has epsilon transition
int hasEpsilonTran(List **ArrayOfState, int currentState){
    member *tmp;
    tmp = ArrayOfState[currentState] -> front;

    while(tmp!= NULL){
        if(tmp->symbol == 'e'){
            return tmp->next_state;    // if a state has epsilon transition return next state
        }
        tmp = tmp -> next;
    }
    return -1;          // if it does not return -1 that means no epsilon transition
}

// the process of test string is to input one by one character in order to find next state
// We found next state by transition we enqueue one by one until the end of the list
// When we finished testing a symbol into a state we dequeue the old state that we have processed the symbol.
// Recursive function: we use it because it is easily do the transition by one index and it is also easy to enqueue and dequeue with correct segementation
void testString(List* ArrayOfState[], Queue *Q , string str, int n, int final[], int n_final ,int index){
    // check if the index of string reaches the end, that means all char has process in functions
    if(index == str.length()){
        if(isFinal(Q, n_final, final)){   // check if the Queue contains final state
            cout << "\n\t\t\t\tString";SetColor(2); // Queue contains final state then string accepted
            cout << " accepted";SetColor(11);
            //displayQueue(Q);
        }else{
            cout << "\n\t\t\t\tString";SetColor(4);
            cout << " rejected";SetColor(11);
            //displayQueue(Q);
        }
    }else{
        int k = Q->n;
        for(int i=0;i<k;i++){
            Element* tmp1 = Q -> front;
            member* tmp = ArrayOfState[tmp1->data] -> front;
            while(tmp != NULL){
                if(str[index] == tmp -> symbol){      // check if there are transitions with symbol that we input
                    enqueue(Q, tmp -> next_state);    // if it contains the symbol we enqueue next state to Queue
                    //cout << "After enqueue: "; displayQueue(Q);
                }
                tmp = tmp -> next;
                int a = hasEpsilonTran(ArrayOfState, tmp1->data);   // check if there are epsilon transitions of the state
                if(a!=-1){
                    enqueue(Q, a); // if it contains epsilon transition then we enqueue next state
                }
                //cout << "After enqueue: "; displayQueue(Q);
            }
            dequeue(Q);       // after enqueue all next state we need to dequeue the state that we have already test symbol
            //cout << "Afer dequeue: "; displayQueue(Q);

        }

        testString(ArrayOfState, Q, str, n, final , n_final, index+1);   // the process is the same until it finishes the last character of the string
    }
}
// to test that it is DFA or NFA:
// check e transition if it contains epsilon transition then it's NFA
// check number of transition in each state if it's different from number of symbol then it's NFA
// check each symbol in state if there are the same symbol in two transition then it's NFA

// We try to check if it is a NFA by characteristics of NFA
bool isDFA(List **ArrayOfState, int n, char *symbol, int n_symbol){
    for(int i=0;i<n;i++){    // loop access to each state
        int count = 0;       // check if each states have equal transition and symbol
        member * tmp = ArrayOfState[i] -> front;
        while(tmp != NULL){
            if(tmp -> symbol == 'e'){
                return false;
            }
            tmp = tmp -> next;
            count += 1;
        }
        if(count != n_symbol){
            return false;
        }
        char sym_in_state[n_symbol];
        int k=0;
        tmp = ArrayOfState[i] -> front;
        // copy symbol from state into 'sym_in_state' in order to compare
        while(tmp != NULL){
            sym_in_state[k] = tmp -> symbol;
            tmp = tmp->next;
            k++;
        }
        //compare all symbol in state to check if it is the same
        // if it's the same it's NFA
        for(int i=0;i<n_symbol;i++){
            for(int j=i+1;j<n_symbol;j++){
                if(sym_in_state[i] == sym_in_state[j]){
                    return false;
                }
            }
        }
    }
    return true;
}



void convertNFAtoDFA(List **ArrayOfState1 ,int *final, int numFinal, char *symbol, int numSymbol){

    Queue *Q[100];
    List *NewArrayOfState[100];
    int sq_sum[100];


    //Allocate memory for Array of linked list
    for(int i=0; i<100; i++){
        NewArrayOfState[i] = createEmptyList();
    }
    Queue *Qtmp, *Qtmp1;
    int newFinal[100], newNumF = 0;

    // take Q = 0 as a new state;
    Q[0] = createEmptyQueue();
    enqueue(Q[0],0);    // create a new start state
    Qtmp1 = createEmptyQueue();

    int next_state = hasEpsilonTran(ArrayOfState1, 0);  // check if a new start state has epsilon transition
    if(next_state!=-1){
        enqueue(Q[0], next_state);          // if it has epsilon transition to a state then we include the next state and state q0 as a new state
    }
    // initialize new q0 of DFA
    int S = 0;                          // result sum of square in Queue after enqueue
    int CurrentState=0, NewState=1;     // this two variable is used to loop when there is no more new state it will stop the loop


    // find sum of square in the start state
    Element *Tmp = Q[0]->front;
    while(Tmp !=NULL){
        S += pow(2,Tmp->data);
        Tmp = Tmp->next;
    }// Start state;
    sq_sum[0] = S;

    // Check if q0 is the final state.
    if(isFinal(Q[0], numFinal, final)){
        newFinal[newNumF] = 0;
        newNumF++;
    }

    while(CurrentState < NewState){
        // Loop current symbol in FA
        for(int i=0; i<numSymbol;i++){  //  transition through all symbols
            Qtmp = copyQ(Q[CurrentState]);      // we need two queue, one to store states of current new state
            Qtmp1 = deleteQueue(Qtmp1);         // while is Qtmp1 is used to enqueue to create a possible new state
            int count = Qtmp->n;
            for(int j=0; j<count; j++){
                // tmp1 for data in queue
                // tmp for test current data in graph
                Element *tmp1 = Qtmp->front;
                member *tmp = ArrayOfState1[tmp1->data]->front;

                while(tmp!=NULL){
                    if(tmp->symbol == symbol[i]){
                        if(isNotExistInQueue(Qtmp1,tmp->next_state)){
                            enqueue(Qtmp1,tmp->next_state);
                            // displayQueue(Qtmp1);
                        }
                    }
                    tmp = tmp->next;
                }
                dequeue(Qtmp);
            }
            Qtmp = copyQ(Qtmp1);
            // Apply epsilon closure for the epsilon transition.
            count = Qtmp->n;
            for(int k = 0; k < count; k++){
                Element *tmp1 = Qtmp->front;
                member *tmp = ArrayOfState1[tmp1->data]->front;
                while(tmp!=NULL){
                    if(tmp->symbol =='e'){
                        if(isNotExistInQueue(Qtmp1,tmp->next_state)){
                            enqueue(Qtmp1,tmp->next_state);
                        }
                    }
                    tmp = tmp -> next;
                }
                dequeue(Qtmp);
            }
            // finding value of S to test if the set of states already exist
            int S=0;
            Element *tmp4 = Qtmp1->front;
            while(tmp4!=NULL){
                S += pow(2,tmp4->data);
                tmp4 = tmp4->next;
            }
            // if it exists then we add transition of the symbol to the i next state (i is a state that already exist)
            int index = checkIfExistSqSum(sq_sum,NewState,S);
            if(index==-1){// if it does not exist then it is a new state so that we save it as a new state
                sq_sum[NewState] = S;
                Q[NewState] = copyQ(Qtmp1);
                addToBeginList(NewArrayOfState[CurrentState], symbol[i], NewState);
                // check if the current state contains final state in NFA;
                if(isFinal(Qtmp1,numFinal,final)){// If found in transition
                    // take it as final state of DFA
                    newFinal[newNumF] = NewState;
                    newNumF++;
                }
                NewState++;   // when the new state found we increment to keep while loop working until the end of new states found
            }else{
                // if it exists
                addToBeginList(NewArrayOfState[CurrentState], symbol[i], index);
            }
        }
        CurrentState++;
    }
    cout<<endl<<"\t\t\t\tAn equivalent DFA!"<<endl;
    n_final = newNumF;
    n = NewState;
    for(int i=0;i<NewState;i++){
        ArrayOfState1[i] = NewArrayOfState[i];
    }
    for(int i=0;i<newNumF;i++){
        final[i] = newFinal[i];
    }

    //displayTable(ArrayOfState, NewState, symbol, numSymbol, final, n_final);

}

bool checkIfMarked(int a, int b, int k, int **mat){
    if(a>=b){
        if( mat[a][b]==1 ){
            return true;
        }
        else if(mat[a][b]==0){
            return false;
        }
    }
    else if(a<b){
        if( mat[b][a]==1 ){
            return true;
        }
        else if(mat[b][a]==0){
            return false;
        }
    }
    return false;
}

bool matrixChanged( int k, int** mat1, int** mat2){

    for(int i=0;i<k-1;i++){
        for(int j=i+1;j<k;j++){
            if( mat1[j][i] != mat2[j][i]){
                return true;
                break;
            }
        }
    }
    return false;
}
//function for minimize a DFA
void minimizeDFA(List** ArrayOfState, int n_state, int* final, int nf ,char* symbol, int n_symbol){
    //copy array of linked list
    List* NewArrayOfState[n_state];
    for(int i=0;i<n_state;i++){
        NewArrayOfState[i] = ArrayOfState[i];
    }
    Queue *Q1 = createEmptyQueue();
    Queue *Q2 = createEmptyQueue();

    enqueue(Q1,0);  // create start state
    enqueue(Q2,0);
    // step one: we need to find accessible states and delete non accessible states
    //find all accessible state:
    while(Q2->n!=0){
        Element *tmp = Q2 -> front;
        member* tmp1 = NewArrayOfState[tmp->data] -> front;  //temp for adjacency list of graph
        while (tmp1 != NULL){
            if(isNotExistInQueue(Q1,tmp1->next_state)){
                enqueue(Q1,tmp1->next_state);   // save all next state that means the state is accessible
                enqueue(Q2,tmp1->next_state);
            }
            tmp1=tmp1->next;
        }
        dequeue(Q2);
    }
    //check non accessible state
    int as[n_state];        //array for store accessible states
    int nas[n_state];       //array for store non accessible states
    int n_nas=0;            //number of non accessible state
    int n_as=0;             //number of accessible state

    for(int i=0; i<n_state; i++){
        if(isNotExistInQueue(Q1,i)){
            nas[n_nas] = i;
            n_nas++;
        }
        else{
            as[n_as]=i;
            n_as++;
        }
    }
    //displayQueue(Q1);
    //delete non accessible state
    for (int i=0;i<n_nas;i++){          //delete graph of non accessible state
        NewArrayOfState[nas[i]]= createEmptyList();
    }
    // create 2D array to do first second third iteration
    int **mat = new int*[n_as];
    for(int i=0;i<n_as;i++){
        mat[i] = new int[n_as];
    }
    int **matcopy = new int*[n_as];
    for(int i=0;i<n_as;i++){
        matcopy[i] = new int[n_as];
    }

    //initail zero to lower triangle of matrix
    for(int i=0;i<n_as;i++){                // where i:column    j:row
        for(int j=0;j<n_as;j++){
            mat[j][i]=0;
            matcopy[j][i]=0;
        }
    }
    // for(int i=0;i<n_as;i++){
    //     for(int j=0;j<n_as;j++){
    //         cout << mat[i][j]<<" ";
    //     }
    //     cout<<"\n";
    // }
    //displayQueue(Q1);
    //first iteration, mark all pairs that contain final state, but not include pair of 2 final states
    for(int i=0;i<n_as-1;i++){
        for(int j=i+1;j<n_as;j++){
            for(int k=0;k<nf;k++){
                if( as[i]==final[k]){
                    mat[j][i]=1;       // mark it if it contains final state
                }
            }
        }
    }

    for(int i=0;i<n_as-1;i++){
        for(int j=i+1;j<n_as;j++){
            for(int k=0;k<nf;k++){
                if( as[j]==final[k]){
                    if(mat[j][i]==0){
                       mat[j][i]=1;
                    }
                    else if(mat[j][i]==1){  //exclude pair of 2 final states;
                       mat[j][i]=0;
                    }
                }
            }
        }
    }
    //displayQueue(Q1);
    while (matrixChanged(n_as, mat , matcopy)){
     //copy matrix to compare

        for(int i=0;i<n_as-1;i++){
            for(int j=i+1;j<n_as;j++){
                matcopy[j][i]=mat[j][i];
            }
        }
        // for(int i=0;i<n_as;i++){
        //     for(int j=0;j<n_as;j++){
        //         cout << matcopy[i][j]<<" ";
        //     }
        //     cout<<"\n";
        // }
        int x1,x2,index_x1,index_x2;
        for(int i=0;i<n_as-1;i++){
            for(int j=i+1;j<n_as;j++){
                if(mat[j][i]==0){           //for pair that is not marked yet
                    for(int t=0;t<n_symbol;t++){            //note that: each pair of state is ( as[i],as[j]  )  i,j index of matrix
                        member* temp = NewArrayOfState[as[i]] -> front;          //find as[i] transition to what?
                        while (temp != NULL){
                            if ( symbol[t] == temp->symbol){
                                x1=temp->next_state;
                                break;
                            }
                            temp=temp->next;
                        }
                        member* temp1 = NewArrayOfState[as[j]] -> front;        //find as[j] transition to what?
                        while (temp1 != NULL){
                            if (symbol[t] == temp1->symbol){
                                x2=temp1->next_state;
                                break;
                            }
                            temp1=temp1->next;
                        }
                        //get index
                        for(int r=0;r<n_as;r++){
                            if(as[r]==x1){
                                index_x1=r;
                                break;
                            }
                        }
                        for(int r=0;r<n_as;r++){
                            if(as[r]==x2){
                                index_x2=r;
                                break;
                            }
                        }

                        //check if the pair of after-transition of ( as[i],as[j] ) is already marked?
                        if(checkIfMarked(index_x1, index_x2 ,n_as, mat)){
                            mat[j][i]=1;
                            break;
                        }
                    }
                }
            }
        }
    }
    // displayQueue(Q1);

    int deletestate[n_as];
    int n_delstate=0;
    for(int i=0;i<n_as-1;i++){
        for(int j=i+1;j<n_as;j++){
            if(mat[j][i]==0){
                // display
                for(int i=0;i<n_as;i++){
                    for(int j=0;j<n_as;j++){
                        // cout << matcopy[i][j]<<" ";
                    }
                    // cout<<"\n";
                }
                // display
                deletestate[n_delstate]=as[j];
                n_delstate++;

                //delete one of the equivalent state : delete its graph
                // here we delete big-number state which is as[j]
                NewArrayOfState[as[j]]= createEmptyList();

                // change all as[j] to as[i]
                for(int k=0;k<n_as;k++){
                        member* temp = NewArrayOfState[as[k]] -> front;
                        while (temp != NULL){
                            if ( as[j] == temp->next_state){
                                temp->next_state = as[i];
                            }
                            temp=temp->next;
                        }
                }

            }
        }
    }

    //error
    int n_newstate=0; //number of accessible state - number of deleted state
    int newstate[n_as];
    int n_newfinal=0;
    int newfinal[nf];

    //new state:
    for(int k=0;k<n_as;k++){
        int r=0;
        for(int j=0;j<n_delstate;j++){
            if( as[k] == deletestate[j] ){
                r=1;        //meaning it is the deleted state
                break;
            }

        }
        if(r==0){
            newstate[n_newstate]=as[k];
            n_newstate++;
        }
    }

    //find new final state after delete equivalent state;
    for(int k=0;k<nf;k++){
        int re=0;
        for(int j=0;j<n_delstate;j++){
            if( final[k] == deletestate[j] ){
                re=1;        //meaning it is the deleted state
                break;
            }
        }
        if(re==0){
            newfinal[n_newfinal]=final[k];
            n_newfinal++;
        }
    }

    //change to new states to order index (0,1,2,3,....)
    for(int i=0;i<n_as;i++){
            member* temp = NewArrayOfState[i] -> front;
            while (temp != NULL){
                for(int k=0;k<n_newstate;k++){
                    if ( newstate[k] == temp->next_state){
                        temp->next_state = k;
                        break;
                    }
                }
                temp=temp->next;
            }
    }


    ////change index of new final state
    for (int i=0;i<n_newfinal;i++){
        for(int j=0;j<n_newstate;j++){
            if(newfinal[i]==newstate[j]){
                newfinal[i]=j;
                break;
            }
        }
    }


    ////
    List* NewArrayOfState1[n_newstate];
    for(int k=0;k<n_newstate;k++){
        NewArrayOfState1[k] = createEmptyList();
    }
    // error here
    for(int k=0;k<n_newstate;k++){
        NewArrayOfState1[k]=NewArrayOfState[newstate[k]];
    }
    cout << "\n\n\t\t\t\tTransition of the minimize DFA:\n";

    // for(int i=0;i<n_newstate;i++){
    //     member *tmpp = NewArrayOfState1[i] -> front;
    //     while(tmpp!=NULL){
    //         cout << "Symbol: " << tmpp -> symbol << "   ->   " << tmpp->next_state <<endl;
    //         tmpp=tmpp->next;
    //     }
    //     cout << "\n\n";

    // }
    //displayTable(NewArrayOfState1, n_newstate, symbol, n_symbol, newfinal, n_newfinal);

    n_final = n_newfinal;
    n = n_newstate;
    for(int i=0;i<n_newstate;i++){
        ArrayOfState[i] = NewArrayOfState1[i];
    }
    for(int i=0;i<n_newfinal;i++){
        final[i] = newfinal[i];
    }

}

//Database
string ConvertIntToString(int id){
    string ID;
    stringstream ss;

    ss<<id;
    ss>>ID;
    return ID;
}

string ConvertCharToString(char symbol){
    string ID;
    stringstream ss;

    ss<<symbol;
    ss>>ID;
    return ID;
}

//Load

void LoadDataFromDB(){

    mysql_query (connection, "Select * from fa;");
    result = mysql_store_result(connection);
    printf("\n\t\t =======================================================================================\n");
    printf("\t\t | %-2s | %-60s                   |", "ID", "Description");
    printf("\n\t\t =======================================================================================\n");
    while(row = mysql_fetch_row(result)){
        printf("\t\t | %-2s | %-60s                   | \n", row[0], row[1]);
    }
    printf("\t\t =======================================================================================\n");
    mysql_free_result(result);
}

int findnumberstate(int id){
    mysql_query (connection, "Select * from states;");
    result = mysql_store_result(connection);
    int i=0;
    while ((row = mysql_fetch_row(result))){
        if(stoi(row[0]) == id){
            i++;
        }
    }
    return i;
    mysql_free_result(result);
}

int findnumbersymbol(int id){
    mysql_query (connection, "Select * from alphabet;");
    result = mysql_store_result(connection);
    int i=0;
    while ((row = mysql_fetch_row(result))){
        if(stoi(row[0]) == id){
            i++;
        }
    }
    return i;
    mysql_free_result(result);
}

void LoadSymbol(int id,char *symbol){
    mysql_query (connection, "Select * from alphabet;");
    result = mysql_store_result(connection);
    int i=0;
    while ((row = mysql_fetch_row(result))){
        if(stoi(row[0]) == id){
            string s1;
            s1 = row[1];
            char n[2];
            stringstream ss;
            ss<<s1;
            ss>>n;
            symbol[i] = n[0];
            i++;
        }
    }
    mysql_free_result(result);
}

int findnumberfinalstate(int id){
    int i=0;
    mysql_query (connection, "Select * from states;");
    result = mysql_store_result(connection);
    while ((row = mysql_fetch_row(result))){
        if(stoi(row[0]) == id){
            if(stoi(row[3]) == 1){
                i++;
            }
        }
    }
    return i;
    mysql_free_result(result);
}

void LoadFinalState(int id, int *final){
    mysql_query (connection, "Select * from states;");
    result = mysql_store_result(connection);
    int i=0;
    while((row = mysql_fetch_row(result))){
        if(stoi(row[0]) == id){
            if(stoi(row[3]) == 1){
                final[i] = stoi(row[1]);
                i++;
            }
        }
    }
    mysql_free_result(result);
}

int findnumbertransition(int id, int state){
    int i=0;
    string ID = ConvertIntToString(id);
    command = "SELECT * FROM transitions WHERE fa_id = "+ID+";";
    mysql_query(connection, command.c_str());
    result = mysql_store_result(connection);
    while ((row = mysql_fetch_row(result))){
        if(stoi(row[1]) == state){
            i++;
        }
    }
    return i;
    mysql_free_result(result);
}

void LoadTransition(int id, int state, char *symboltran, int *nextstate){
    int i=0;
    string ID = ConvertIntToString(id);
    command = "SELECT * FROM transitions WHERE fa_id = "+ID+";";
    mysql_query (connection, command.c_str());
    result = mysql_store_result(connection);
    while ((row = mysql_fetch_row(result))){
        if(stoi(row[1]) == state){
            string s1;
            s1 = row[2];
            char n[2];
            stringstream ss;
            ss<<s1;
            ss>>n;
            symboltran[i] = n[0];
            nextstate[i] = stoi(row[3]);
            i++;
        }
    }
    mysql_free_result(result);
}

void Loadtransition(int id, int state, List *ArrayOfState, int n_tran){
    char symboltran[n_tran];
    int nextstate[n_tran];
    LoadTransition(id,state,symboltran,nextstate);
    for(int i=0; i<n_tran; i++){
        addToBeginList(ArrayOfState,symboltran[i],nextstate[i]);
    }
}

//Insert

int InsertDataToFA(){
    string des;
    cout<<"\n\t\t\t\tEnter FA description: ";
    getline(cin, des); fflush(stdin);
    query = "Insert into fa (description) values ('"+des+"');";
    mysql_query(connection, query.c_str());

    mysql_query(connection, "Select MAX(id) from fa;");
    result = mysql_store_result(connection);
    int fa_id;
    while(row = mysql_fetch_row(result)){
        fa_id = stoi(row[0]);
    }
    return fa_id;
    mysql_free_result(result);
}

void InsertSymbol(int id, char symbol){
    string ID = ConvertIntToString(id);
    string Sym = ConvertCharToString(symbol);

    query = "Insert into alphabet (fa_id, symbol) values ('"+ID+"','"+Sym+"');";
    mysql_query(connection, query.c_str());
}

void InsertState(int id, int state, bool start_state, bool final_state){
    string ID = ConvertIntToString(id);
    string StateString = ConvertIntToString(state);

    if(start_state && final_state){
        query = "Insert into states (fa_id,state,start_state,final_state) values ('"+ID+"','"+StateString+"','1','1');";
    }
    else if(start_state || final_state){
        if(start_state){
        query = "Insert into states (fa_id,state,start_state,final_state) values ('"+ID+"','"+StateString+"','1','0');";
        }
        else if(final_state){
            query = "Insert into states (fa_id,state,start_state,final_state) values ('"+ID+"','"+StateString+"','0','1');";
        }
    }
    else{
        query = "Insert into states (fa_id,state,start_state,final_state) values ('"+ID+"','"+StateString+"','0','0');";
    }
    mysql_query(connection,query.c_str());
}

void InsertDataToTransition(int id, int current_state, char symbol, int next_State){
    string ID = ConvertIntToString(id);
    string firstState = ConvertIntToString(current_state);
    string Sym = ConvertCharToString(symbol);
    string lastState = ConvertIntToString(next_State);

    query = "Insert into transitions (fa_id, state, symbol, next_state) values ('"+ID+"','"+firstState+"','"+Sym+"','"+lastState+"');";
    mysql_query(connection, query.c_str());
}
int InsertDataToDatabase(List **ArrayOfState, int n, int n_symbol, char *symbol, int *final, int n_final, int m, int current_state_DB[], char symbol_DB[], int next_state_DB[]){
    int fa_id = InsertDataToFA();

    int check1;
    for(int i=0; i<m; i++){
        if(symbol_DB[i] == 'e'){
            check1 = 1;
            break;
        }
        else{
            check1 = 0;
        }
    }
    if(check1 == 1){
        for(int i=0; i<n_symbol+1; i++){
            InsertSymbol(fa_id, symbol[i]); //Return primary key of each symbol
        }
    }
    else{
        for(int i=0; i<n_symbol; i++){
            InsertSymbol(fa_id, symbol[i]);
        }
    }

     // Insert Data to states table in database
    // Check for final state

    int check;
    for(int i=0; i<n; i++){
        for(int j=0; j<n_final; j++){
            if(final[j] == i){ // Final state
                check = 1;
                break; // Confirm only one final state has one value
            }
            else if(final[j] != i && i != 0){ // Normal state
                check = 2;
            }
            else{ // Start state
                check = 0;
            }
        }
        // End checking

        if(check == 1 && i == 0){ // Check if the state is both start state and final state
            InsertState(fa_id, i, true, true);
        }
        else if(check == 1){ // Check if exist only final state
            InsertState(fa_id, i, false, true);
        }
        else if(check == 0){ // Check if exist only start state
            InsertState(fa_id, i, true, false);
        }
        else if(check == 2){ // Check if not exist both start state and final state
            InsertState(fa_id, i, false, false);
        }
        // Insert Successful
        // Return primary key of each state
    }
//    for(int i=0; i<m; i++){
//        cout<<"Transition #"<<i+1<<": "<<current_state_DB[i]<<" "<<symbol_DB[i]<<" "<<next_state_DB[i]<<endl;
//    }

    for(int i=0; i<m; i++){
        InsertDataToTransition(fa_id, current_state_DB[i], symbol_DB[i], next_state_DB[i]);
    }
    return fa_id;
}

void DeleteFaFromDatabase(int id){
    string ID = ConvertIntToString(id);

    query = "Delete from transitions where fa_id = '"+ID+"';";
    mysql_query(connection, query.c_str());
    query = "Delete from states where fa_id = '"+ID+"';";
    mysql_query(connection, query.c_str());
    query = "Delete from alphabet where fa_id = '"+ID+"';";
    mysql_query(connection, query.c_str());
    query = "Delete from fa where id = '"+ID+"';";
    mysql_query(connection, query.c_str());
}

void RemoveSymbol(int id, char symbol){
    string ID = ConvertIntToString(id);
    string Sym = ConvertCharToString(symbol);

    query = "Delete from transitions where fa_id = '"+ID+"' and symbol = '"+Sym+"';";
    mysql_query(connection, query.c_str());
    query = "Delete from alphabet where fa_id = '"+ID+"' and symbol = '"+Sym+"';";
    mysql_query(connection, query.c_str());
}

void RemoveState(int id, int state){
    string ID = ConvertIntToString(id);
    string State = ConvertIntToString(state);

    query = "DELETE FROM transitions WHERE fa_id = '"+ID+"' AND state = '"+State+"';";
    mysql_query(connection, query.c_str());
    query = "DELETE FROM transitions WHERE fa_id = '"+ID+"' AND next_state = '"+State+"';";
    mysql_query(connection, query.c_str());
    query = "DELETE FROM states WHERE fa_id = '"+ID+"' AND state = '"+State+"';";
    mysql_query(connection, query.c_str());
}

void Removetransition(int id, int current_state, char symbol, int next_state){
    string ID = ConvertIntToString(id);
    string State1 = ConvertIntToString(current_state);
    string State2 = ConvertIntToString(next_state);
    string Sym = ConvertCharToString(symbol);


    query = "DELETE FROM transitions WHERE fa_id = '"+ID+"' AND state = '"+State1+" AND next_state = '"+State2+"' AND symbol = '"+Sym+"';";
    mysql_query(connection, query.c_str());
}

void RemoveBeforeSave(int id){
    string ID = ConvertIntToString(id);

    query = "DELETE FROM transitions WHERE fa_id = '"+ID+"';";
    mysql_query(connection, query.c_str());
    query = "DELETE FROM alphabet WHERE fa_id = '"+ID+"';";
    mysql_query(connection, query.c_str());
    query = "DELETE FROM states WHERE fa_id = '"+ID+"';";
    mysql_query(connection, query.c_str());
}

void ChangeDescription(int id, string newDes){
    string ID = ConvertIntToString(id);

    query = "UPDATE fa SET DESCRIPTION = '"+newDes+"' WHERE id= '"+ID+"';";
    mysql_query(connection, query.c_str());
}

void SaveData(int fa_id, List **ArrayOfState, int n, int n_symbol, char *symbol, int *final, int n_final, int m, int current_state_DB[], char symbol_DB[], int next_state_DB[]){


    char ChangeDes;
    cout<<"\n\t\t\t\tDo you want to change description of this FA? (Y/N): "; cin>>ChangeDes; fflush(stdin);

    if(toupper(ChangeDes) == 'Y'){
        string newDes;
        cout<<"\n\t\t\t\tNew Description: "; getline(cin,newDes);
        ChangeDescription(fa_id,newDes);
    }else if(toupper(ChangeDes == 'N')){
        cout<<"\n\t\t\t\tDescription is not changed."<<endl;
    }

    if(toupper(ChangeDes) == 'Y' || toupper(ChangeDes) == 'N'){
        RemoveBeforeSave(fa_id);
        //symbol
        int check1;
        for(int i=0; i<m; i++){
            if(symbol_DB[i] == 'e'){
                check1 = 1;
                break;
            }
            else{
                check1 = 0;
            }
        }
        if(check1 == 1){
            for(int i=0; i<n_symbol+1; i++){
                InsertSymbol(fa_id, symbol[i]); //Return primary key of each symbol
            }
        }
        else{
            for(int i=0; i<n_symbol; i++){
                InsertSymbol(fa_id, symbol[i]);
            }
        }
        //state
        int check;
        for(int i=0; i<n; i++){
            for(int j=0; j<n_final; j++){
                if(final[j] == i){ // Final state
                    check = 1;
                    break; // Confirm only one final state has one value
                }
                else if(final[j] != i && i != 0){ // Normal state
                    check = 2;
                }
                else{ // Start state
                    check = 0;
                }
            }
            if(check == 1 && i == 0){ // Check if the state is both start state and final state
                InsertState(fa_id, i, true, true);
            }
            else if(check == 1){ // Check if exist only final state
                InsertState(fa_id, i, false, true);
            }
            else if(check == 0){ // Check if exist only start state
                InsertState(fa_id, i, true, false);
            }
            else if(check == 2){ // Check if not exist both start state and final state
                InsertState(fa_id, i, false, false);
            }
        }
        //transition
        for(int i=0; i<m; i++){
            InsertDataToTransition(fa_id, current_state_DB[i], symbol_DB[i], next_state_DB[i]);
        }
    }
}

struct DBtransition{
    int current_state;
    int next_state;
    char symbol;
};

struct DBstate{
    int state;
    bool startstate = false;
    bool finalstate = false;
};

void Ascii_Art(){

    //Loop for print first layer
    //SetColor(3);
    cout<<"\t\t\t"<<"    ___________   __________________   ___   __  ____________  __  ______  _________"<<endl;
    cout<<"\t\t\t"<<"   / ____/  _/ | / /  _/_  __/ ____/  /   | / / / /_  __/ __ \\/  |/  /   |/_  __/   |"<<endl;
    cout<<"\t\t\t"<<"  / /_   / //  |/ // /  / / / __/    / /| |/ / / / / / / / / / /|_/ / /| | / / / /| |"<<endl;
    cout<<"\t\t\t"<<" / __/ _/ // /|  // /  / / / /___   / ___ / /_/ / / / / /_/ / /  / / ___ |/ / / ___ |"<<endl;
    cout<<"\t\t\t"<<"/_/   /___/_/ |_/___/ /_/ /_____/  /_/  |_\\____/ /_/  \\____/_/  /_/_/  |_/_/ /_/  |_|"<<endl;
}
void loading(){

    printf("\n\n\n\t\t\t\t\t\t\tLoading...\n");
    printf("\t\t\t\t\t\t  ");

    for(int i=0; i<=20; i++){

        Sleep(50);
        printf("%c",219);

    }
    printf("\n\n\n\t\t\t\t\t       ");
    system("pause");
}
void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;


}
int GetFontSize(HANDLE windowHandle, COORD *size)
    {
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

    if (!GetCurrentConsoleFontEx(windowHandle, 0, &font))
        {
        return 0;
        }

    *size = font.dwFontSize;

    return 1;
    }

int SetFontSize(HANDLE windowHandle, COORD size)
    {
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

    if (!GetCurrentConsoleFontEx(windowHandle, 0, &font))
        {
        return 0;
        }

    font.dwFontSize = size;

    if (!SetCurrentConsoleFontEx(windowHandle, 0, &font))
        {
        return 0;
        }

    return 1;
    }
void header(char header[50]){
//     cout<<"\t\t\t\t\t==========================================================\n";
//     cout<<"\t\t\t\t\t||\t\t\t"<<header<<"\t\t\t\t||\n";
//     cout<<"\t\t\t\t\t==========================================================\n";
    printf("\n\n\t\t\t\t==========================================================");
    if(strlen(header)>=16){
        cout<<"\n\t\t\t\t||\t\t      "<<header<<"      \t\t||";
    }
    else{
        cout<<"\n\t\t\t\t||\t\t      "<<header<<"      \t\t\t||";
    }
    printf("\n\t\t\t\t==========================================================\n");
}
int arrowKeyChoice(char option[10][50], char head[50], int m){
    int n=1;
    int ch1, ch2,j;
    SetColor(11);
    printf("\n\n\t\t\t\t==========================================================");
    if(strlen(head)>16){
        printf("\n\t\t\t\t||\t      %s      \t\t||",head);
    }
    else{
        printf("\n\t\t\t\t||\t\t      %s      \t\t\t||",head);
    }
    SetColor(11);
    printf("\n\t\t\t\t==========================================================\n");SetColor(11);

    SetColor(15);
    printf("\n\t\t\t\t   >>\t\t%s",option[0]);SetColor(11);

    for( j=1;j<m ;j++ ){
        printf("\n\t\t\t\t\t\t%s",option[j]);
    }
    SetColor(14);
    printf("\n\n\n\t\t\t\tNOTE:");
    SetColor(14);
    printf(" USE  \"UP\" AND \"DOWN\" TO SCROLL.\n\t\t\t\t      USE  \"ENTER\"  TO CHOOSE THE OPTION");
    while(1){

        ch1 = getch();fflush(stdin);
        ch2 = 0;
        if( ch1 == 13){
            return n;
            break;
        }
        else if (ch1 == 0xE0) {    // a scroll key was pressed
            ch2 = getch();fflush(stdin);          // determine what it was
            system("cls");

            switch(ch2){
            case 80:     //DOWN
                if( n==m ){
                    SetColor(11);
                    printf("\n\n\t\t\t\t==========================================================");
                    if(strlen(head)>16){
                        printf("\n\t\t\t\t||\t      %s      \t\t||",head);
                    }
                    else{
                        printf("\n\t\t\t\t||\t\t      %s      \t\t\t||",head);
                    }
                    printf("\n\t\t\t\t==========================================================\n");SetColor(11);

                    for( j=0;j<m-1;j++ ){
                        printf("\n\t\t\t\t\t\t%s",option[j]);
                    }
                    SetColor(15);
                    printf("\n\t\t\t\t   >>\t\t%s",option[m-1]);SetColor(11);//SetColor(0);
                    break;
                }
                else{
                    n+=1;
                    SetColor(11);
                    printf("\n\n\t\t\t\t==========================================================");
                    if(strlen(head)>16){
                        printf("\n\t\t\t\t||\t      %s      \t\t||",head);
                    }
                    else{
                        printf("\n\t\t\t\t||\t\t      %s      \t\t\t||",head);
                    }
                    printf("\n\t\t\t\t==========================================================\n");SetColor(11);

                    for(j=0;j<m;j++){
                        if( j!= n-1 ){
                            printf("\n\t\t\t\t\t\t%s",option[j]);
                        }
                        else{
                            SetColor(15);
                            printf("\n\t\t\t\t   >>\t\t%s",option[j]);SetColor(11);//SetColor(0);
                        }
                    }
                    break;
                }
            case 72:    //UP
                if(n==1){
                    SetColor(11);
                    printf("\n\n\t\t\t\t==========================================================");
                    if(strlen(head)>16){
                        printf("\n\t\t\t\t||\t      %s      \t\t||",head);
                    }
                    else{
                        printf("\n\t\t\t\t||\t\t      %s      \t\t\t||",head);
                    }
                    SetColor(11);
                    printf("\n\t\t\t\t==========================================================\n");SetColor(11);

                    SetColor(15);
                    printf("\n\t\t\t\t   >>\t\t%s",option[0]);SetColor(11);
                    for( j=1;j<m;j++ ){
                        printf("\n\t\t\t\t\t\t%s",option[j]);
                    }
                    break;
                }
                else{
                    n-=1;
                    SetColor(11);
                    printf("\n\n\t\t\t\t==========================================================");
                    if(strlen(head)>16){
                        printf("\n\t\t\t\t||\t      %s      \t\t||",head);
                    }
                    else{
                        printf("\n\t\t\t\t||\t\t      %s      \t\t\t||",head);
                    }
                    SetColor(11);
                    printf("\n\t\t\t\t==========================================================\n");SetColor(11);

                    for(j=0;j<m;j++){
                        if( j!= n-1 ){
                            printf("\n\t\t\t\t\t\t%s",option[j]);
                        }
                        else{
                            SetColor(15);
                            printf("\n\t\t\t\t   >>\t\t%s",option[j]);SetColor(11);
                        }
                    }
                    break;
                }
            }
        }
    }
}
