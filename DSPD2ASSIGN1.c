#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<limits.h>


typedef enum {true=1,false=0} bool;
typedef enum {SUCCESS,FAILURE} status_code;

typedef struct mm_nodetag       //MAIN MEMORY node
{
    int add;                    //mm address
    int data;
    struct mm_nodetag* next;
}mm_node;

typedef struct cc_nodetag       //CACHE node
{
    int index;                  //cc index
    int add;                    //mm address
    int data;
    int lat;                    //last access time
    int freq;                   //access frequency
    bool valid;                 //empty check
    struct cc_nodetag* next;
}cc_node;

//FUNC DEF



mm_node* InitializeMM(int clk)                             //populate MM
{
    void PrintMM(mm_node* top, int clk);
    mm_node* CreateMMNode(mm_node* top, int d, int ad);
    FILE *myFile;
    mm_node *top=NULL;
    int ad=1;

    myFile=fopen("hello.txt","r");
    int i;
    int d;
    if(!myFile)
    {
        printf("cannot open file");
        return;
    }
    while(!feof(myFile)&&ad<=100)
    {
        fscanf(myFile,"%d,", &d);
        top=CreateMMNode(top,d,ad);
        ad++;
    }

    PrintMM(top,clk);
    fclose(myFile);

    return top;
}


mm_node* CreateMMNode(mm_node* top, int d, int ad)
{
    mm_node* nptr, *ptr;
    nptr=(mm_node*)malloc(sizeof(mm_node));
    nptr->data=d;
    nptr->add=ad;
    nptr->next=NULL;
    ptr=top;
    if(top==NULL)
    {
        top=nptr;
    }
    else
    {
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=nptr;
    }

    return top;
}


void PrintMM(mm_node* top, int clk)
{
    mm_node* ptr;
    ptr=top;
    if(ptr!=NULL)
    {
        printf("\n\n     SYS_CLOCK: %d\n\n",clk-1);
        printf("_________________\n");
        printf("|**MAIN_MEMORY**|\n");
        printf("_________________\n");
        printf("  ADD  |  DATA  |\n");
        printf("_________________\n");
        while(ptr!=NULL)
        {
            if(ptr->add/10<1)
            {
                printf("   %d   |   %d   |\n",ptr->add, ptr->data);
                printf("_________________\n");
            }
            else if(ptr->add/10<10)
            {
                printf("  %d   |  %d   |\n",ptr->add, ptr->data);
                printf("_________________\n");
            }
            else
            {
                printf(" %d   | %d   |\n",ptr->add, ptr->data);
                printf("_________________\n");
            }

            ptr=ptr->next;
        }
    }
    else
    {
        printf("empty main memory");
    }
}

cc_node* InitializeCC(int n)                                    //create empty cache
{
    cc_node* top, *nptr, *ptr;
    int ind=1;
    top=NULL;
    ptr=top;

    while(n>0)
    {
        nptr=(cc_node*)malloc(sizeof(cc_node));
        nptr->valid=0;
        nptr->index=ind;
        nptr->lat=0;
        nptr->freq=0;
        nptr->next=NULL;
        if(top==NULL)
        {
            top=nptr;
        }
        else
        {
            ptr->next=nptr;
        }

        ptr=nptr;
        n--;
        ind++;

    }

    return top;
}

cc_node* SearchCC(cc_node* cc_top, mm_node* mm_top, int ad, int clk,int cc_n)
{
    cc_node* InsertCC(cc_node* top,int ad, int d, int clk,int cc_n);
    status_code SearchMM(mm_node* mm_top, int ad, int *dptr);
    cc_node* cc_ptr;
    int d;
    status_code sc=FAILURE;
    status_code sc1;
    if(cc_top!=NULL)
    {
        cc_ptr=cc_top;

        while(cc_ptr!=NULL && sc!=SUCCESS)
        {
            if(cc_ptr->add==ad)
            {
                sc=SUCCESS;
                cc_ptr->lat=clk;
                cc_ptr->freq++;
            }
            else
            {
                cc_ptr=cc_ptr->next;
            }
        }
        if(sc==FAILURE)
        {

            sc1=SearchMM(mm_top, ad, &d);
            if(sc1==SUCCESS)
            {
                cc_top=InsertCC(cc_top,ad,d,clk,cc_n);
            }
        }
    }
    else
    {
        printf("\nEmpty Cache\n");
    }

    return cc_top;
}


status_code SearchMM(mm_node* mm_top, int ad, int *dptr)
{
    mm_node* ptr;
    status_code sc=FAILURE;                         //for add found in MM
    status_code sc1=SUCCESS;                        //for add doesnt exist in MM
    ptr=mm_top;
    //int ad2=1;
    int d;
    if(mm_top==NULL)
    {
        printf("\nEmpty Main Memory\n");
        return;
    }
    else
    {
        if(ptr->add==ad)                               //for found in 1st mm block
            {
                sc=SUCCESS;
            }
        while(ptr!=NULL && sc==FAILURE)
        {
            ptr=ptr->next;
            //ad++;
            if(ptr->add==ad)
            {
                sc=SUCCESS;
            }

            //printf("\nhi\n");
        }


        if(sc==SUCCESS)
        {
            *dptr=ptr->data;                            //read mm's data
        }
        else
        {
            printf("\nAddress: %d doesn't exist in Main Memory\n", ad);
            sc1=FAILURE;
        }

    }
    return sc1;

}

cc_node* InsertCC(cc_node* top,int ad, int d, int clk, int cc_n)
{
    int an;                                          //algorithm number : algo select
    int c=1;                                         //count: cache_full check
    cc_node* ptr=top;
    int cc_lat=INT_MAX, cc_add, cc_freq=INT_MAX;

    while(ptr!=NULL && ptr->valid!=0)
    {
        ptr=ptr->next;
        c++;

    }
    if(c>cc_n)
    {
        printf("Which algorith you preffer:\n 0-LRU\n 1-MRU \n 2-LFU");
        scanf("%d", &an);
        ptr=top;

        if(an==0)                           //LRU
        {

            while(ptr!=NULL)
            {
                if(ptr->lat < cc_lat)
                    {
                        cc_lat=ptr->lat;
                        cc_add=ptr->add;
                    }
                ptr=ptr->next;

            }

        }
        else if(an==1)                      //MRU
        {
            cc_lat=0;
            while(ptr!=NULL)
            {
                if(ptr->lat > cc_lat)
                {
                    cc_lat=ptr->lat;
                    cc_add=ptr->add;
                }
                ptr=ptr->next;

            }

        }
        else                                //LFU
        {
            while(ptr!=NULL)
            {
                if(ptr->freq < cc_freq)
                {
                    cc_freq=ptr->freq;
                    cc_add=ptr->add;
                }
                ptr=ptr->next;

            }
        }
        ptr=top;
        while(ptr->add!=cc_add)
        {
            ptr=ptr->next;
        }

    }

    ptr->data=d;
    ptr->valid=1;
    ptr->add=ad;
    ptr->lat=clk;
    ptr->freq=1;

    return top;
}


void PrintCC(cc_node* top, int cc_n, int clk, int ad)
{
    cc_node* ptr;
    ptr=top;
    int r;                                                                              //random variable for printing empty cache rows
    printf("\n\n                                       SYSTEM_CLOCK : %d",clk);
    printf("\n                                       CACHE_SIZE   : %d",cc_n);
    printf("\n                                       ACC_ADDRESS  : %d\n",ad);

    if(top==NULL)
    {
        printf("\nNo Cache available");
    }
    else
    {
        printf("\n_______________________________________________________");
        printf("\n|*********************_C_A_C_H_E_*********************|");
        printf("\n_______________________________________________________");
        printf("\n|  CC_IND  |  MM_AD   |   DATA   |   LAT   |   FREQ   |  ");
        printf("\n_______________________________________________________");

        while(ptr!=NULL && ptr->valid!=0)
        {
            if(ptr->add/10==0)
            {
                printf("\n|   %d      |    %d     |    %d    |   %d     |    %d     |", ptr->index, ptr->add, ptr->data, ptr->lat, ptr->freq);
                printf("\n_______________________________________________________");
            }
            else
            {
                printf("\n|   %d      |    %d    |    %d   |   %d     |    %d     |", ptr->index, ptr->add, ptr->data, ptr->lat, ptr->freq);  //just for uniformity in printing
                printf("\n_______________________________________________________");
            }
            ptr=ptr->next;
            cc_n--;
        }


        while(cc_n!=0)
        {

            //r=ptr->index;

            printf("\n|   %d      |   EMPTY  |   EMPTY  |  EMPTY  |  EMPTY   |", ptr->index);                                                           //printing empty CACHE
            printf("\n_______________________________________________________");
            cc_n--;
            //r++;
            ptr=ptr->next;
        }

    }
}


void main()
{




    mm_node* InitializeMM(int clk);
    cc_node* InitializeCC(int n);
    cc_node* SearchCC(cc_node* cc_top, mm_node* mm_top, int ad, int clk, int cc_n);


    void PrintCC(cc_node* top, int cc_n, int clk, int ad);
    mm_node* mm_top;
    cc_node* cc_top;

    bool yn=1;                                        //yes or no
    int cc_n;                                        //cache size
    int ad;
    int clk=1;                                       //GLOBAL CLOCK
    bool r=1;                                         //retry cache size input check


    while(r!=0)
    {
        printf("Enter the size of CACHE you want:");
        scanf("%d",&cc_n);

        if(cc_n>0)
        {
            mm_top=InitializeMM(clk);
            cc_top=InitializeCC(cc_n);

            while(yn!=0)
            {
                printf("\nEnter the address you want to access:");
                scanf("%d",&ad);
                cc_top=SearchCC(cc_top,mm_top,ad,clk,cc_n);
                PrintCC(cc_top,cc_n,clk,ad);

                printf("\n\n\nDo you want to search again? \n1-Yes \n0-No");
                scanf("%d",&yn);
                if(yn==0)
                {
                    r=0;
                }
                clk++;
            }
        }
        else
        {
            printf("\nSize of CACHE should be > 0 \nDo you want to continue? \n1-Yes \n0-No");
            scanf("%d",&r);
        }
    }
}
