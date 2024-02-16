struct member{
    char symbol;
    int next_state;
    member *next;
};
struct List{
    int n;
    member *front;
};
List *createEmptyList(){
    List *list;
    list = new List;
    list -> n = 0;
    list -> front = NULL;

    return list;
}
struct Element{
    int data;
    Element *next;
};

struct Queue{
    int n;
    Element *front;
    Element *rear;
};
