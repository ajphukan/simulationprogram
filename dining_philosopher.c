#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#define num 5

char state[][10]={"Thinking",
                  "Eating",
                  "Hungry"
                  };
struct philosopher
{pthread_t thread;
 int st; //0 for thinking,2 hungry, 1 eating
 int waiting,thinking,eating;
}phil[num];

void initialize(struct philosopher aj[])
{int i;
  for(i=0;i<num;i++)
   {aj[i].st=0;
    aj[i].waiting=0;
    aj[i].thinking=i;
    aj[i].eating=0;
   }
}
pthread_mutex_t spoon[num];
int print_ctrl=0;
void *pick_drop(void *arg);
int main()
{int i;
 initialize(phil);
 for(i=0;i<num;i++)
  pthread_mutex_init(&spoon[i],NULL);

 for(i=0;i<num;i++)
pthread_create(&phil[i].thread,NULL,pick_drop,(void *)i);
printf("\n\t\t\tDinning philosopher simulation : \nid state\tid state\tid state\tid state\tid state\t");
   while(1)
   {sleep(2);
    print_ctrl=1;
    printf("\n\n");
    sleep(1);
    for(i=0;i<num;i++)
    printf("%2d %s\t",i,state[phil[i].st]);
    print_ctrl=0;
   }
 for(i=0;i<num;i++)
    pthread_join(phil[i].thread,NULL);
 for(i=0;i<num;i++)
    pthread_mutex_destroy(&spoon[i]);
return 0;
}

void *pick_drop(void *arg)
{int tid;
 tid=((int *) arg);
   while(1)
    {if(!print_ctrl)
        {int j,a;
         sleep(1);
           switch(phil[tid].st)
            {case 0: if(phil[tid].thinking==0)
                        phil[tid].st=2;
                     else
                        phil[tid].thinking--;
                     break;
             case 1: if(phil[tid].eating==0)
                    {phil[tid].thinking=rand()%num+1;
                     phil[tid].st=0;
              pthread_mutex_unlock(&spoon[(tid+1)%num]);
              pthread_mutex_unlock(&spoon[tid%num]);
                     }
                     else
                     phil[tid].eating--;
                     break;
              case 2:
           if(pthread_mutex_lock(&spoon[tid%num])==0)
              {
       if(pthread_mutex_lock(&spoon[(tid+1)%num])==0)
                {phil[tid].eating=rand()%num+2;
                 phil[tid].st=1;    
                 }
                else
               pthread_mutex_unlock(&spoon[tid%num]);  
               }
              break;
             }
         }

     }
}
