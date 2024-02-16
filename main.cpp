#include<iostream>
#include <conio.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<time.h>
using namespace std;
#include"DataStructure.h"
int n;
int n_tran;
int n_final;
int n_symbol;
List *ArrayOfState[10];
char *symbol;
int *final;
#include"function.h"

int main(){
    mysql = mysql_init(0);
    connection = mysql_real_connect(mysql,"localhost","root","","Automata",0,NULL,0);

    system("Color 0B");

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD size;

    if (GetFontSize(h, &size)){
        size.X += (SHORT)(size.X * .5);
        size.Y += (SHORT)(size.Y * .5);
        SetFontSize(h, size);
    }

    Ascii_Art();
    loading();
    system("cls");

    int option;

    int FA_id;
    int o=1;

    while(2>1){
        system("cls");
        char mainmenu[3][50]={"(1)\tCreate FA","(2)\tLoad FA","(3)\tExit"};
        char head[50]="  MAIN MENU";
        int n1=sizeof(mainmenu)/sizeof(mainmenu[0]);
        int num=arrowKeyChoice(mainmenu,head,n1);

        if(num==1){
            system("cls");
            SetColor(11);
            header("CREATE FA");
            cout <<"\n\t\t\t";
            cout <<"Input number of states: "; cin >> n;

            for(int i = 0; i<n; i++){
                ArrayOfState[i] = createEmptyList();
            }

            cout <<"\n\t\t\t";
            cout << "Input number of symbol: "; cin >> n_symbol;
            symbol = new char[n_symbol];

            for(int i=0; i<n_symbol; i++){
                cout <<"\t\t\t\t";
                cout << "Symbol #"<<i+1<<": "; cin >> symbol[i];
            }

            symbol[n_symbol] = 'e';

            //-------------------------
            int current_state_DB[100];
            int next_state_DB[100];
            char symbol_tran_DB[100];
            int index_tran = 0;
            //-------------------------

            for(int i=0; i<n; i++){
                cout <<"\n\t\t\t    ";
                cout << "<<State q"<<i<<">>";
                cout <<"\n\t\t\t";
                cout << "---Input transition---";
                cout <<"\n\t\t\t";
                cout << "Input number of transition: "; cin >> n_tran;
                int next_state;
                char symbol1;

                for(int j = 1; j <= n_tran; j++){
                    cout <<"\t\t\t";
                    cout << "---Transition #"<<j<<"---"<<endl;
                    cout <<"\t\t\t";
                    cout << "Symbol: "; cin >> symbol1;
                    cout <<"\t\t\t";
                    cout << "Next State: "; cin >> next_state;

                    //Fetch the value------------------------
                    current_state_DB[index_tran] = i;
                    next_state_DB[index_tran] = next_state;
                    symbol_tran_DB[index_tran] = symbol1;
                    index_tran++;
                    //---------------------------------------

                    addToBeginList(ArrayOfState[i], symbol1, next_state);
                }

                member* temp = ArrayOfState[i] -> front;
                while(temp!=NULL){
                    cout <<"\t\t\t";
                    cout << "Symbol: " << temp -> symbol << "   ->   " << temp->next_state <<endl;
                    temp=temp->next;
                }
            }

            for(int i=0;i<n;i++){
                member *tmp;
                tmp = ArrayOfState[i]-> front;
                while(tmp!=NULL){
                    cout <<"\t\t\t";
                    cout << "Symbol: " << tmp -> symbol << "   ->   " << tmp->next_state <<endl;
                    tmp=tmp->next;
                }
            }

            cout <<"\n\t\t\t";
            cout << "Input number of final state: "; cin >> n_final;
            final = new int[n_final];
            for(int i=0; i<n_final; i++){
                cout <<"\t\t\t";
                cout << "Input final state #"<<i+1<<": "; cin >> final[i];
            }

            system("cls");
            header("    TABLE");
            displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);
            cout<<"\n\t\t\t\t";
            system("pause");

            bool Save = false;
            int CheckConvert = 0;
            int CheckMinimize = 0;
            int option2;

            int current_state_cov_DB[100];
            int next_state_cov_DB[100];
            char symbol_tran_cov_DB[100];
            int index_tran_cov = 0;

            int current_state_mnm_DB[100];
            int next_state_mnm_DB[100];
            char symbol_tran_mnm_DB[100];
            int index_tran_mnm = 0;

            while(1){
                system("cls");
                char submenu[6][50]={"(1)   Test String",
                                    "(2)   Test FA",
                                    "(3)   Convert NFA to DFA",
                                    "(4)   Minimize FA",
                                    "(5)   Save FA",
                                    "(6)   Back"};
                char head1[50]="   MENU";
                int n2=sizeof(submenu)/sizeof(submenu[0]);
                int num1=arrowKeyChoice(submenu,head1,n2);

                if(num1==1){
                    system("cls");
                        while(1){
                            system("cls");
                            SetColor(11);
                            header("TEST STRING");
                            displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);
                            string str;
                            Queue *Q;
                            Q = createEmptyQueue();

                            cout<<"\n\t\t\tTest String: "; SetColor(15); cin >> str;

                            int accept;
                            enqueue(Q, 0);
                            testString(ArrayOfState, Q, str, n, final, n_final, 0);
                            cout<<"\n\n\t\t\t*Press [Esc] go back";
                            cout<<"\n\t\t\t*Press any key to continue";

                            int press;
                            press = getch();
                            if(press == 27){
                                break;
                            }
                        }
                }else if(num1==2){
                    system("cls");
                    header("  TEST FA");
                    displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);
                    if(isDFA(ArrayOfState, n, symbol, n_symbol)){
                        cout<<"\n\t\t\t\tIt is"; SetColor(13);
                        cout<<" DFA"; SetColor(11);
                        cout<<".\n";
                    }else{
                        cout<<"\n\t\t\t\tIt is"; SetColor(13);
                        cout<<" NFA"; SetColor(11);
                        cout<<".\n";
                    }
                    cout<<"\n\t\t\t\t";
                    system("pause");
                }else if(num1==3){
                    system("cls");
                    header("    TABLE");
                    displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);

                    if(!isDFA(ArrayOfState, n, symbol, n_symbol)){
                        convertNFAtoDFA(ArrayOfState, final, n_final, symbol, n_symbol);
                        displayTable(ArrayOfState, n, symbol, n_symbol-1, final, n_final);

                        CheckConvert = 1;

                        for(int i=0; i<n; i++){
                            member *help;
                            help = ArrayOfState[i]->front;
                            while(help!=NULL){
                                current_state_cov_DB[index_tran_cov] = i;
                                next_state_cov_DB[index_tran_cov] = help->next_state;
                                symbol_tran_cov_DB[index_tran_cov] = help->symbol;
                                index_tran_cov++;
                                help = help->next;
                            }
                        }
                    }else{
                        cout<<"\n\t\t\t\tIt is already a DFA!!!\n";
                    }
                    cout<<"\n\t\t\t\t";
                    system("pause");
                }else if(num1 == 4){
                    system("cls");
                    header("    TABLE");
                    displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);
                    if(isDFA(ArrayOfState, n, symbol, n_symbol)){
                        minimizeDFA(ArrayOfState, n, final, n_final, symbol, n_symbol);
                        displayTable(ArrayOfState, n, symbol, n_symbol-1, final, n_final);
                        CheckMinimize = 1;

                        for(int i=0; i<n; i++){
                            member *help;
                            help = ArrayOfState[i]->front;
                            while(help!=NULL){
                                current_state_mnm_DB[index_tran_mnm] = i;
                                next_state_mnm_DB[index_tran_mnm] = help->next_state;
                                symbol_tran_mnm_DB[index_tran_mnm] = help->symbol;
                                index_tran_mnm++;
                                help = help->next;
                            }
                        }
                    }else{
                        cout<<"\n\t\t\t\tIt is not a DFA!!!\n";
                    }
                    cout<<"\n\t\t\t\t";
                    system("pause");
                }else if(num1 == 5){
                    system("cls");
                    header("    SAVE");
                    if(Save){
                        cout<<"\t\t\t\tThis FA has already been saved!"<<endl;
                        cout<<"\n\t\t\t\t";
                        system("pause");
                    }else{
                        if(CheckConvert == 0 && CheckMinimize == 0){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran, current_state_DB, symbol_tran_DB, next_state_DB);
                        }else if(CheckConvert == 1 && CheckMinimize == 0){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_cov, current_state_cov_DB, symbol_tran_cov_DB, next_state_cov_DB);
                        }else if(CheckMinimize == 1){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_mnm, current_state_mnm_DB, symbol_tran_mnm_DB, next_state_mnm_DB);
                        }
                        Save = true;
                        cout<<"\n\t\t\t\tSaving is done.";
                        cout<<"\n\t\t\t\t";
                        system("pause");
                        break;
                    }
                }else if(num1 == 6){
                    if(Save){
                        cout<<"\n\t\t\t\tProgram exited!!!\n";
                        break;
                    }else{
                        char Confirm;
                        cout<<"\n\t\t\t\tDo you want to save? (Y/N): "; cin>>Confirm;
                        fflush(stdin);
                        if(toupper(Confirm) == 'Y'){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran, current_state_DB, symbol_tran_DB, next_state_DB);
                        }else if(toupper(Confirm) == 'N'){
                            cout<<"\n\t\t\t\tProgram exited!!!\n";
                            break;
                        }
                    }
                }
            }
        }else if(num == 2){
            // load from database
            system("cls");
            header("    LOAD");
            LoadDataFromDB();
            cout<<"\n\t\tInput ID: "; cin>>FA_id;
            n = findnumberstate(FA_id);
            n_symbol = findnumbersymbol(FA_id);
            n_final = findnumberfinalstate(FA_id);
            final = new int[n_final];
            symbol = new char[n_symbol];
            LoadFinalState(FA_id,final);
            LoadSymbol(FA_id,symbol);
            for(int i=0; i<n; i++){
                ArrayOfState[i] = createEmptyList();
            }
            for(int i=0; i<n; i++){
                n_tran = findnumbertransition(FA_id,i);
                Loadtransition(FA_id,i,ArrayOfState[i],n_tran);
            }
            displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
            cout<<"\n\t\t";
            system("pause");
            // after loading display this menu

            //-------------------------
            int current_state_Load_DB[100];
            int next_state_Load_DB[100];
            char symbol_tran_Load_DB[100];
            int index_tran_Load = 0;
            //-------------------------
            for(int i=0; i<n; i++){
                member *help;
                help = ArrayOfState[i]->front;
                while(help!=NULL){
                    current_state_Load_DB[index_tran_Load] = i;
                    next_state_Load_DB[index_tran_Load] = help->next_state;
                    symbol_tran_Load_DB[index_tran_Load] = help->symbol;
                    index_tran_Load++;
                    help = help->next;
                }
            }

            bool Save = true;
            int CheckConvert = 0;
            int CheckMinimize = 0;

            int current_state_cov_DB[100];
            int next_state_cov_DB[100];
            char symbol_tran_cov_DB[100];
            int index_tran_cov = 0;

            int current_state_mnm_DB[100];
            int next_state_mnm_DB[100];
            char symbol_tran_mnm_DB[100];
            int index_tran_mnm = 0;

            while(1){
                system("cls");
                char submenu[9][50]={"(1)   Test String",
                                    "(2)   Test FA",
                                    "(3)   Convert NFA to DFA",
                                    "(4)   Minimize FA",
                                    "(5)   Edit FA",
                                    "(6)   Save FA",
                                    "(7)   Save as New FA",
                                    "(8)   Delete FA",
                                    "(9)   Back"};
                char head1[50]="   MENU";
                int n2=sizeof(submenu)/sizeof(submenu[0]);
                int num2=arrowKeyChoice(submenu,head1,n2);
                if(num2 == 1){
                    while(1){
                        system("cls");
                        SetColor(11);
                        header("TEST STRING");
                        displayTable(ArrayOfState, n, symbol, n_symbol-1, final, n_final);
                        string str;
                        Queue *Q;
                        Q = createEmptyQueue();

                        cout<<"\n\t\t\tTest String: "; SetColor(15); cin >> str;

                        int accept;
                        enqueue(Q, 0);
                        testString(ArrayOfState, Q, str, n, final, n_final, 0);
                        cout<<"\n\n\t\t\t*Press [Esc] go back";
                        cout<<"\n\t\t\t*Press any key to continue";

                        int press;
                        press = getch();
                        if(press == 27){
                            break;
                        }
                    }
                }else if(num2 == 2){
                    system("cls");
                    header("  TEST FA");
                    displayTable(ArrayOfState, n, symbol, n_symbol-1, final, n_final);
                    if(isDFA(ArrayOfState, n, symbol, n_symbol)){
                        cout<<"\n\t\t\t\tIt is"; SetColor(13);
                        cout<<" DFA"; SetColor(11);
                        cout<<".\n";
                    }else{
                        cout<<"\n\t\t\t\tIt is"; SetColor(13);
                        cout<<" NFA"; SetColor(11);
                        cout<<".\n";
                    }
                    cout<<"\n\t\t\t\t";
                    system("pause");
                }else if(num2 == 3){
                    system("cls");
                    header("    TABLE");
                    displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);

                    if(!isDFA(ArrayOfState, n, symbol, n_symbol)){
                        int SymE = 0;
                        for(int i=0; i<=n_symbol; i++){
                            if(symbol[i]=='e'){
                                SymE = 1;
                            }
                        }
                        if(SymE == 1){
                            n_symbol--;
                            convertNFAtoDFA(ArrayOfState, final, n_final, symbol, n_symbol);
                        }else{
                            convertNFAtoDFA(ArrayOfState, final, n_final, symbol, n_symbol);
                        }
                        displayTable(ArrayOfState, n, symbol, n_symbol-1, final, n_final);

                        CheckConvert = 1;
                        for(int i=0; i<n; i++){
                            member *help;
                            help = ArrayOfState[i]->front;
                            while(help!=NULL){
                                current_state_cov_DB[index_tran_cov] = i;
                                next_state_cov_DB[index_tran_cov] = help->next_state;
                                symbol_tran_cov_DB[index_tran_cov] = help->symbol;
                                index_tran_cov++;
                                help = help->next;
                            }
                        }

                        Save = false;
                    }else{
                        cout<<"\n\t\t\t\tIt is already a DFA!!!\n";
                    }
                    cout<<"\n\t\t\t\t";
                    system("pause");
                }else if(num2 == 4){
                    system("cls");
                    header("    TABLE");
                    displayTable(ArrayOfState, n, symbol, n_symbol, final, n_final);

                    if(isDFA(ArrayOfState, n, symbol, n_symbol)){
                        minimizeDFA(ArrayOfState, n, final, n_final, symbol, n_symbol);
                        displayTable(ArrayOfState, n, symbol, n_symbol-1, final, n_final);
                        CheckMinimize = 1;
                        for(int i=0; i<n; i++){
                            member *help;
                            help = ArrayOfState[i]->front;
                            while(help!=NULL){
                                current_state_mnm_DB[index_tran_mnm] = i;
                                next_state_mnm_DB[index_tran_mnm] = help->next_state;
                                symbol_tran_mnm_DB[index_tran_mnm] = help->symbol;
                                index_tran_mnm++;
                                help = help->next;
                            }
                        }
                        Save = false;
                    }else{
                        cout<<"\n\t\t\t\tIt is not a DFA!!!\n";
                    }
                    cout<<"\n\t\t\t\t";
                    system("pause");
                }else if(num2 == 5){
                        //system("cls");
                        displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                        int option1;
                        int CheckAdd = 0;
                        int CheckRemove = 0;

                        char AddSymbol[10];
                        int index_newsymbol = 0;
                        int CheckAddSymbol = 0;

                        DBstate AddState[10];
                        int index_newstate = 0;
                        int CheckAddState = 0;

                        DBtransition AddTransition[10];
                        int index_newtransition = 0;
                        int CheckAddTransition = 0;

                        char DeleteSymbol[10];
                        int index_deletesymbol = 0;
                        int CheckDeleteSymbol = 0;

                        int DeleteState[10];
                        int index_deletestate = 0;
                        int CheckDeleteState = 0;

                        DBtransition DeleteTransition[10];
                        int index_deletetransition = 0;
                        int CheckDeleteTransition = 0;

                        while(1>0){
                            system("cls");
                            char submenu[9][50]={"(1)   Change description",
                                                "(2)   Add symbol",
                                                "(3)   Add transition",
                                                "(4)   Add state",
                                                "(5)   Remove transition",
                                                "(6)   Remove symbol",
                                                "(7)   Remove state",
                                                "(8)   Save",
                                                "(9)   Back"};
                            char head1[50]="   MENU";
                            int n2=sizeof(submenu)/sizeof(submenu[0]);
                            int num3=arrowKeyChoice(submenu,head1,n2);
                            //displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                            if(num3 == 2){
                                system("cls");
                                header("ADD SYMBOL");
                                cout<<"\n\t\t\t\tInput symbol: "; cin >> symbol[n_symbol];

                                //Get value to input intoData to save later
                                AddSymbol[index_newsymbol] = symbol[n_symbol];
                                index_newsymbol++;
                                CheckAdd++;
                                CheckAddSymbol = 1;
                                //-------------------------------------------

                                n_symbol = n_symbol + 1;
                                symbol[n_symbol] = 'e';

                                system("cls");
                                header("   TABLE");
                                displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                                cout<<"\n\t\t\t\t";
                                system("pause");
                            }else if(num3 == 3){
                                system("cls");
                                int state1;
                                char symbol1;
                                int next_state1;
                                header("ADD NEW TRANSITION");
                                cout <<"\n\t\t\t\tInput State: "; cin >> state1;
                                cout <<"\n\t\t\t\tSymbol: "; cin >> symbol1;
                                cout <<"\n\t\t\t\tNext state: "; cin >> next_state1;

                                //---------------------------------------------------------
                                AddTransition[index_newtransition].current_state = state1;
                                AddTransition[index_newtransition].symbol = symbol1;
                                AddTransition[index_newtransition].next_state = next_state1;
                                index_newtransition++;
                                CheckAdd++;
                                CheckAddTransition = 1;
                                //----------------------------------------------------------

                                addToBeginList(ArrayOfState[state1], symbol1, next_state1);

                                system("cls");
                                header("   TABLE");
                                displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                                cout<<"\n\t\t\t\t";
                                system("pause");
                           }else if(num3 == 4){
                                system("cls");
                                int n_tran_new_state;
                                int next_state12;
                                char symbol12;
                                header("  ADD NEW STATE  ");
                                cout<<"\n\t\t\t\t     <<State q"<<n<<">>";
                                cout<<"\n\t\t\t\t---Input transition---\n";
                                cout<<"\t\t\t\tInput number of transition: "; cin >> n_tran_new_state;
                                ArrayOfState[n] = createEmptyList();

                                for(int j = 0; j < n_tran_new_state; j++){
                                    cout << "\n\t\t\t\t---Transition #"<<j+1<<"---"<<endl;
                                    cout << "\t\t\t\tSymbol: "; cin >> symbol12;
                                    cout << "\t\t\t\tNext State: "; cin >> next_state12;

                                    //---------------------------------------------------------
                                    AddTransition[index_newtransition].current_state = n;
                                    AddTransition[index_newtransition].symbol = symbol12;
                                    AddTransition[index_newtransition].next_state = next_state12;
                                    index_newtransition++;
                                    CheckAdd++;
                                    CheckAddTransition = 1;
                                    //----------------------------------------------------------

                                    addToBeginList(ArrayOfState[n], symbol12, next_state12);
                                }

                                char FinalState;
                                cout << "\n\t\t\t\tIs it a final state? (Y/N): "; cin >> FinalState;

                                if(toupper(FinalState) == 'Y'){
                                    final[n_final] = n;
                                    n_final += 1;

                                    //------------------------------------------
                                    AddState[index_newstate].finalstate = true;
                                    //------------------------------------------
                                }

                                //---------------------------------
                                AddState[index_newstate].state = n;
                                index_newstate++;
                                CheckAdd++;
                                CheckAddState = 1;
                                //---------------------------------
                                n = n + 1;
                                system("cls");
                                header("   TABLE");
                                displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                                cout<<"\n\t\t\t\t";
                                system("pause");
                            }else if(num3 == 5){
                                system("cls");
                                int next_state=0;
                                int stateToRemoveTran=0;
                                char symbol1;
                                header("REMOVE TRANSITION");
                                cout << "\n\t\t\t\tInput State: "; cin >> stateToRemoveTran;
                                cout << "\n\t\t\t\tSymbol: "; cin >> symbol1;
                                cout << "\n\t\t\t\tNext State: "; cin >> next_state;

                                //-------------------------------------------------------------------------
                                DeleteTransition[index_deletetransition].current_state = stateToRemoveTran;
                                DeleteTransition[index_deletetransition].symbol = symbol1;
                                DeleteTransition[index_deletetransition].next_state = next_state;
                                index_deletetransition++;
                                CheckRemove++;
                                CheckDeleteTransition = 1;
                                //-------------------------------------------------------------------------

                                DeleteTranFromState(ArrayOfState, symbol1, next_state, stateToRemoveTran);
                                system("cls");
                                header("   TABLE");
                                displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                                cout<<"\n\t\t\t\t";
                                system("pause");
                            }else if(num3 == 6){
                                system("cls");
                                char symbolToRemove;
                                header("  REMOVE SYMBOL  ");
                                cout<<"\n\t\t\t\tWarning: All transitions with input symbol will be deleted!!!"<<endl;
                                cout<<"\n\t\t\t\tInput Symbol: "; cin >> symbolToRemove;

                                //-------------------------------------------------
                                DeleteSymbol[index_deletesymbol] = symbolToRemove;
                                index_deletesymbol++;
                                CheckRemove++;
                                CheckDeleteSymbol = 1;
                                //-------------------------------------------------

                                for(int i=0;i<n;i++){
                                    DeleteSymbolFromState(ArrayOfState, symbolToRemove, i);
                                }
                                int m;
                                for(int i=0;i<n_symbol;i++){
                                    if(symbol[i] == symbolToRemove){
                                        m = i;
                                        break;
                                    }
                                }

                                memmove(symbol + m, symbol + m + 1, (n_symbol - m - 1)*sizeof(int));
                                n_symbol--;
                                symbol[n_symbol] = 'e';
                                // display FA
                                system("cls");
                                header("   TABLE");
                                displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                                cout<<"\n\t\t\t\t";
                                system("pause");
                            }else if(num3 == 7){
                                system("cls");
                                // might add more feature next time if we still have time
                                // error
                                int stateToDelete;
                                header("   REMOVE STATE   ");
                                cout<<"\n\t\t\t\tWarning: All transitions with this input state will be deleted!!!"<<endl;
                                cout<<"\n\t\t\t\tInput State: q"; cin >> stateToDelete;

                                //---------------------------------------------
                                DeleteState[index_deletestate] = stateToDelete;
                                index_deletestate++;
                                CheckRemove++;
                                CheckDeleteState = 1;
                                //---------------------------------------------

                                //ArrayOfState[stateToDelete] = createEmptyList();
                                int Count = 0;
                                // delete an index from ArrayOfState

                                memmove(ArrayOfState + stateToDelete, ArrayOfState + stateToDelete+1, (n - stateToDelete - 1) * sizeof(int));
                                n--;
                                int isFinal=-1;
                                for(int i=0;i<n_final;i++){
                                    if(stateToDelete == final[i]){
                                        isFinal = i;
                                    }
                                }
                                if(isFinal!=-1){
                                    memmove(final + isFinal, final + isFinal +1, (n_final - isFinal - 1) * sizeof(int));
                                    n_final--;
                                }
                                system("cls");
                                header("   TABLE");
                                displayTable(ArrayOfState,n,symbol,n_symbol-1,final,n_final);
                                cout<<"\n\t\t\t\t";
                                system("pause");
                            }else if(num3 == 1){
                                system("cls");
                                SetColor(11);
                                header("CHANGE DESCRIPTION");
                                string newDes;
                                cout<<"\n\t\t\t\tNew Description: "; getline(cin,newDes);
                                ChangeDescription(FA_id,newDes);
                                cout<<"\n\t\t\t\tDescription is changed!";
                                cout<<"\n\t\t\t\t";
                                system("pause");
                            }else if(num3 == 8){
                                system("cls");
                                // save edited FA to database
                                header("SAVE EDITED");
                                char ConfirmSaveEdit;
                                cout<<"\n\t\t\t\tDo you want to save?(Y/N): "; cin>>ConfirmSaveEdit;
                                if(toupper(ConfirmSaveEdit) == 'Y'){
                                    if(CheckAdd != 0){
                                        if(CheckAddSymbol == 1){
                                            for(int i=0; i<index_newsymbol; i++){
                                                InsertSymbol(FA_id, AddSymbol[i]);
                                            }
                                        }
                                        if(CheckAddState == 1){
                                            for(int i=0; i<index_newstate; i++){
                                                InsertState(FA_id, AddState[i].state, AddState[i].startstate, AddState[i].finalstate);
                                            }
                                        }
                                        if(CheckAddTransition == 1){
                                            for(int i=0; i<index_newtransition; i++){
                                                InsertDataToTransition(FA_id, AddTransition[i].current_state, AddTransition[i].symbol, AddTransition[i].next_state);
                                            }
                                        }
                                    }
                                    if(CheckRemove != 0){
                                        if(CheckDeleteTransition == 1){
                                            for(int i=0; i<index_deletetransition; i++){
                                                Removetransition(FA_id, DeleteTransition[i].current_state, DeleteTransition[i].symbol, DeleteTransition[i].next_state);
                                            }
                                        }
                                        if(CheckDeleteSymbol == 1){
                                            for(int i=0; i<index_deletesymbol; i++){
                                                RemoveSymbol(FA_id, DeleteSymbol[i]);
                                            }
                                        }
                                        if(CheckDeleteState == 1){
                                            for(int i=0; i<index_deletestate; i++){
                                                RemoveState(FA_id, DeleteState[i]);
                                            }
                                        }
                                    }
                                    cout<<"\n\t\t\t\tSaving is done."<<endl;
                                    cout<<"\n\t\t\t\t";
                                    system("pause");
                                }
                            }else if(num3 == 9){
                                break;
                            }
                        }
                }else if(num2 == 6){
                        system("cls");
                        header("    SAVE");
                        if(Save){
                            cout<<"\t\t\t\tThis FA has already been saved!"<<endl;
                            cout<<"\n\t\t\t\t";
                            system("pause");
                        }else{
                            if(CheckConvert == 1 && CheckMinimize == 0){
                                SaveData(FA_id,ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_cov, current_state_cov_DB, symbol_tran_cov_DB, next_state_cov_DB);
                            }else if(CheckMinimize == 1){
                                SaveData(FA_id,ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_mnm, current_state_mnm_DB, symbol_tran_mnm_DB, next_state_mnm_DB);
                            }
                            Save = true;
                            cout<<"\n\t\t\t\tSaving is done.";
                            cout<<"\n\t\t\t\t";
                            system("pause");
                            break;
                        }
                        system("pause");
                }else if(num2 == 7){
                        system("cls");
                        header(" SAVE AS");
                        if(CheckConvert == 0 && CheckMinimize == 0){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_Load, current_state_Load_DB, symbol_tran_Load_DB, next_state_Load_DB);
                        }else if(CheckConvert == 1 && CheckMinimize == 0){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_cov, current_state_cov_DB, symbol_tran_cov_DB, next_state_cov_DB);
                            CheckConvert == 0;
                        }else if(CheckMinimize == 1){
                            FA_id = InsertDataToDatabase(ArrayOfState, n, n_symbol, symbol, final, n_final, index_tran_mnm, current_state_mnm_DB, symbol_tran_mnm_DB, next_state_mnm_DB);
                            CheckMinimize = 0;
                        }
                        cout<<"\n\t\t\t\tSaving is done.";
                        cout<<"\n\t\t\t\t";
                        system("pause");
                }else if(num2 == 8){
                        system("cls");
                        header(" DELETE FA");
                        char ConfirmDeleteFa;
                        cout<<"\n\t\t\t\tDo you want to delete this FA? (Y/N): "; cin>>ConfirmDeleteFa;
                        if(toupper(ConfirmDeleteFa) == 'Y'){
                            DeleteFaFromDatabase(FA_id);
                            o = 2;
                            cout<<"\n\t\t\t\tDelete Successfully!";
                            cout<<"\n\t\t\t\t";
                            system("pause");
                            break;
                        }else if(toupper(ConfirmDeleteFa) == 'N'){
                            //cout<<"Program exited!!!";
                            break;
                        }
                        system("pause");
                }else if(num2 == 9){
                    cout << "Program exited!!!";
                    break;
                }
            }
        }else if(num == 3){
            system("cls");
            cout<<"Exited the program!!!";
            break;
        }
    }
    mysql_close(connection);
}
