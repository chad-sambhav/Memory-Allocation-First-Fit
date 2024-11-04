#include <stdio.h>
#include <stdlib.h>
int total_memory_size;

struct node
{
    int program_id;
    int size_in_kb;
    int start_add;
    struct node *next;
};
struct node *create_node()
{
    struct node *n = (struct node *)malloc(sizeof(struct node));
    if (n == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }
    printf("Enter program id, size_in_kb: ");
    scanf("%d %d", &(n->program_id), &(n->size_in_kb));

    return n;
}
void traverse_linked_list(struct node *head)
{   struct node* ptr = head;
    struct node* q;
    struct node* p = NULL;
    if(ptr == NULL){
        printf("[free,%d,0,%d] ->NULL\n",total_memory_size,total_memory_size);
        return;
    }
    while(ptr != NULL){
        p = ptr;
        q = ptr->next;
       //Check and show free space before first element.
        if(ptr == head){
            if(ptr->start_add != 0){
                printf("[free,%d,0,%d] ->",ptr->start_add,ptr->start_add);
            }
        }
        //Printing current element
        printf("[%d,%d,%d,%d] ->", ptr->program_id, ptr->size_in_kb, ptr->start_add, ptr->start_add+ptr->size_in_kb);
        //Check and show free spaces in between
        if(q!=NULL && q->start_add - (ptr->start_add + ptr->size_in_kb) != 0 ){
            printf("[free,%d,%d,%d] ->",q->start_add - (ptr->start_add + ptr->size_in_kb),ptr->start_add + ptr->size_in_kb, q->start_add );
        }
        
        ptr = q;
    }
    //Check and show free space at end
    if(total_memory_size - (p->start_add + p->size_in_kb) != 0){
        printf("[free,%d,%d,%d] ->",total_memory_size - (p->start_add + p->size_in_kb), p->start_add + p->size_in_kb,total_memory_size);
    }
    
    printf("NULL\n");
}

//Allocation
struct node* allocate(struct node* head){
    struct node *n = create_node();
    //For the first node
    if(head == NULL){
        if(n->size_in_kb <=total_memory_size){
            n->start_add = 0;
            n->next = NULL;
            head = n;
            traverse_linked_list(head);
            return head;
        }
        else{
            printf("Error: Memory allocation of this size is not possible for given total memory.\n");
        }
    }
    //If there is sufficient space before first node.
    else if(n->size_in_kb <= head->start_add){

        n->start_add = 0;
        n->next = head;
        head = n;
        traverse_linked_list(head);
        return head;
    }
    //Finding sufficient gap between nodes
    else{
        struct node* p = head;
        struct node* q = head->next;
        while(q!=NULL){
            if(n->size_in_kb <= (q->start_add - (p->start_add + p->size_in_kb))){
                //fit in the gap
                n->start_add = p->start_add + p->size_in_kb;
                p->next = n;
                n->next = q;
                traverse_linked_list(head);
                return head;
            }
            else{
                p = p->next;
                q = q->next;
            }
        }
        if(q == NULL){
            //no gap was found in between
            //insert at end of LL, if sufficicent space is available.
            if(n->size_in_kb <= (total_memory_size - (p->start_add + p->size_in_kb)) ){
                //sufficient space is there after last node.
                n->start_add = p->start_add + p->size_in_kb;
                p->next = n;
                n->next = NULL;
                traverse_linked_list(head);
                return head;
            }
            else{
                printf("Error: Memory allocation by first fit is not possible for this size.\n");
            }
        }

    }
    
    return head;
    
}
//Deallocation
struct node* deallocate(struct node* head){
    int id = -1;
    printf("Enter program_id to be deallocated: ");
    scanf("%d", &id);
    struct node* p = head;
    struct node* q = head->next;
    //If first node to be deallocated
    if(p->program_id == id){
        free(p);
        head = q;
        traverse_linked_list(head);
        return head;
    }
    //If node other than first to be deallocated
    else{
        while(q!=NULL){
            if(q->program_id == id){
                p->next = q->next;
                free(q);
                traverse_linked_list(head);
                return head;
            }
            p = p->next;
            q = q->next;
        }
        if(q==NULL){
            printf("Error: Invalid program_id\n");
        }
    }
    return head;

}
//Compaction
void compaction(struct node* head){
    struct node* p = head;
    struct node* q = head->next;
    if(head == NULL){
        printf("Already compacted.\n");
    }
    else{
        //Looking for free spaces and adjusting start addresses.

        //looking for free space before first node
        if(p->start_add != 0){
            p->start_add = 0;
        }
        //looking for free spaces in between
        while(q!=NULL){
            q->start_add = p->start_add + p->size_in_kb;
            p = p->next;
            q = q->next;
        }
        printf("After compaction\n");
        traverse_linked_list(head);
    }

    

}
int main(){
    struct node* head = NULL;
    printf("Enter total memory size\n");
    scanf("%d", &total_memory_size);
    int choice;
    do{
        printf("Choose the process:\n");
        printf("1. Allocation.\n");
        printf("2. Deallocation.\n");
        printf("3. Compaction.\n");
        printf("4. Exit the program.\n");
        scanf("%d", &choice);

        switch(choice){
            case 1:{
                head = allocate(head);
                break;
            }
            case 2:{
                head = deallocate(head);
                break;
            }
            case 3:{
                compaction(head);
                break;
            }
            case 4:{
                printf("Program successfully exited.\n");
                break;
            }
            default:{
                printf("Error: Invalid choice.\n");
            }
        
        }
    }while(choice != 4);
    

    return 0;
}