#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int master_module();
void report_module();
void transaction_module(int l);
void alert_module(int l);
void discard_module(int l);
void delete_medicine(int l,int index);
int cfileexists(char *filename);
int p_dd,p_mm,p_year;
struct details
{
	char medicine_name[100];
	int quantity,original;
	int ex_mm,ex_year;
	float price;
}medi[100];
struct bill
{
        char medicines[50];
}bill[50];
int cfileexists(char * filename)
{
	FILE *file;
	if (file = fopen(filename, "r"))
	{
	       fclose(file);
	       return 1;
	}
	else
		return 0;
	fclose(file);
}
int master_module()
{
	FILE *med;
        int num,i=0;
	printf("Enter number of medicines:");
	scanf("%d",&num);
       // struct details medi[num];
        med=fopen("details.txt","w");
	printf("Enter present day's date: ");
	scanf("%d%d%d",&p_dd,&p_mm,&p_year);
	for(i=0;i<num;i++)
	{
		printf("Enter medicine name: ");
		scanf("%s",&medi[i].medicine_name);
		fprintf(med,"%s",medi[i].medicine_name);
		printf("Enter the price of one medicine: ");
		scanf("%f",&medi[i].price);
		fprintf(med,"%.2f",medi[i].price);
		printf("\nEnter expiry date of the medicine in month year format: ");
		scanf("%d%d",&medi[i].ex_mm,&medi[i].ex_year);
		fprintf(med,"%d/%d",medi[i].ex_mm,medi[i].ex_year);
		printf("Enter the quantity: ");
		scanf("%d",&medi[i].quantity);
		fprintf(med,"%d",medi[i].quantity);
		medi[i].original=medi[i].quantity;
	}
	discard_module(num);
	fclose(med);
        return num;
}
void report_module()
{
	FILE *report;
        report=fopen("sales.txt","r");
	char ch;
	if (report== NULL)
	{
		printf("Sale is not been done\n");
                exit(0);
        }
        else
	{
	        while ((ch=getc(report))!= EOF)
		{
			printf ("%c", ch);
		}
	}
        fclose(report);
}
void transaction_module(int l)
{
	FILE *sale,*file;
	sale=fopen("sales.txt","w");
	int num,i=0,quantity,j,exists=3,stock,flag=0;
	char *medicine,name[50],*customer;
	float total=0,price;
	printf("Enter your name:");
	scanf("%s",&name);
	customer=name;
	fprintf(sale," Date:%d-%d-%d\n Name:%s  Amount:",p_dd,p_mm,p_year,name);
	exists=cfileexists(customer);
	printf("Enter number of medicines to be ordered:");
        scanf("%d",&num);
	for(i=0;i<num;i++)
	{
		printf("Enter medicine name:");
		scanf("%s",bill[i].medicines);
		printf("Enter the quantity:");
		scanf("%d",&quantity);
		for(j=0;j<l;j++)
		{
		        if(strcmp(bill[i].medicines,medi[j].medicine_name)==0)
			{
				flag=1;
				stock=medi[j].quantity;
				if(quantity<=stock)
				{
				        price=medi[j].price;
				        total=total+(quantity*price);
		        	        medi[j].quantity=stock-quantity;
				}
				else
				{
					printf("We don't have as much medicines as you want\n");
                                }
				break;
			}
			else
			{
				if(j==l-1)
					printf("%s medicine is not there in our store\n",bill[i].medicines);
			}

		}
         }
        if(exists==1)
	{
		total=(total*0.2);
		printf("Your are our regular customer,you get 20 percent discount\n");
		printf("Your bill is %.2f\n",total);
		fprintf(sale,"%f\n",total);
	}
	if(exists==0)
	{
		printf("Your bill is %.2f\n",total);
		fprintf(sale,"%f\n",total);
		file=fopen(customer,"w");
		fclose(file);
        }
	fclose(sale);
	alert_module(l);
}
void alert_module(int l)
{
	int i,stock,quantity;
	for(i=0;i<l;i++)
	{
		quantity=medi[i].quantity;
		stock=medi[i].original/2;
		if(quantity<=stock)
		{
			printf("\n%s medicine stock is less than 50%\n",medi[i].medicine_name);
		}
	}
}
void discard_module(int l)
{
       int i,j;
       for(i=0;i<l;i++)
       {
	       if(p_year>=medi[i].ex_year)
	       {
		       if(p_mm>=medi[i].ex_mm)
		       {
			       j=i+1;
			       printf("\n%s medicine is expired,medicine should be discarded\n",medi[i].medicine_name);
			       delete_medicine(l,i);
                               l=l-1;
		       }
	       }
       }
}
void delete_medicine(int l,int index)
{
	int c;
	for (c = index;c<l; c++)
	{
	       medi[c] = medi[c+1];
	}
}
int main()
{
       FILE *pass;
       int choice,l,i=0;
       char password[50],check[50];
       printf("                     WELCOME TO N&S PHARMACY STORE\n");
       printf("\nEnter your password to open the application:");
       scanf("%s",&password);
       pass=fopen("N&S_Pharmacy_store.txt","r");
       fscanf(pass,"%s",&check);
       if(strcmp(password,check)==0)
       {
            printf("\n**********************************************************************\n");
            printf("Enter 1 if you want to enter the medicine details\nEnter 2 if you want to check the report\nEnter 3 if you want to order\nEnter 0 if you want to exit from the application\n");
            printf("Enter your choice:");
            scanf("%d",&choice);
            printf("***********************************************************************\n");
            while(choice!=0)
            {
                switch(choice)
                {
	                 case 1:l=master_module();
				break;
		         case 2:report_module();
         	                break;
		         case 3:transaction_module(l);
				break;
			 default:exit(0);
	        }
		printf("Enter your choice:\n");
	        scanf("%d",&choice);
            }
	}
       else
	       printf("You enterd wrong password");
       fclose(pass);
       return 0;
}


