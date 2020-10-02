#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node
{
    struct node * prev;
    int info;
    struct node * next;
}*new_node,*start,*last,*temp;
void create()
{
    int value,choice;
    do
    {
        new_node=(struct node*)malloc(sizeof(struct node));
        printf("enter element : ");
        scanf("%d",&value);
        new_node->prev=NULL;
        new_node->info=value;
        new_node->next=NULL;
        if (start==NULL)
        {
            start = new_node;
            last = new_node;
           // start->next=start;
            start->prev=start;
        }
        else
        {
            last->next=new_node;
            new_node->prev=last;
           // new_node->next=start;
            start->prev=new_node;
            last=new_node;
        }
        printf("aaje jaane ke liye 1 dabaye ya 0 dabaker nikle : ");
        scanf("%d",&choice); 
    } while (choice==1);
}
void display()
{       temp = start;
   // printf("linklist in right direction : NULL<->");
    while(temp->next!=last) 
  {
      printf("%d<->",temp->info);
        temp=temp->next;
  }  
    printf("%d<->",temp->info);
  //  printf("NULL\n");
   // printf("linklist in left direction : NULL<->");
    temp=last;
    while(temp->prev!=start)
    {
        printf("%d<->",temp->info);
        temp=temp->prev;
    }
     printf("%d<->",temp->info);
   // printf("null\n");
}
int main()
{
    create();
    display();
    return 0;
}
