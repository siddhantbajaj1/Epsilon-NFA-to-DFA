#include<stdio.h>
#include<stdlib.h>

//------NUMBER OF STATES/SYMBOLS------
int numsym,numsta;
//-------------------START STATE---------
int st;
//---------FUNCTION DECLARATION FOR FINDING NEW STATES----------
int* compute();
int* silon();
int main()
{
	int temp;
	//---------INITIAL INPUT-----------
	printf("Enter number of states ::::: ");
	scanf("%d",&numsta);
	printf("Enter number of symbols :::::");
	scanf("%d",&numsym);
	//--------TAKING EPSILON CASE--------
	numsym += 1;
	//---------3-D ARRAY FOR STORING E-NFA----------
	int arr[numsta][numsym][numsta];
	int i,j,k;
	//---------------INITIALIZING THE 3-D ARRAY-------
	for(i=0;i<numsta;i++)
	{
		for(j=0;j<numsym;j++)
		{
			for(k=0;k<numsta;k++)
			{
				arr[i][j][k] = 0;
			}
		}
	}
	//--------------TAKING E-NFA AS INPUT------------------
	for(i=0;i<numsta;i++)
	{
		for(j=0;j<numsym;j++)
		{
			for(k=0;k<numsta;k++)
			{
				if(j == 0)
				{
					//TAKING CASE FOR EPSILON TRANSITIONS
					printf("Epsilon transition for Q%d -> Q%d ::::: ",i,k);
					scanf("%d",&temp);
					arr[i][j][k] = temp;
				}
				else{
					printf("Symbol %d transition for Q%d -> Q%d ::::: ",j,i,k);
					scanf("%d",&temp);
					arr[i][j][k] = temp;
				}
			}
		}	
	}
	/*
	//---------PRINTING THE 3-D ARRAY--------------------
	for(i=0;i<numsta;i++)
	{
		for(j=0;j<numsym;j++)
		{
			for(k=0;k<numsta;k++)
			{
				printf("%d",arr[i][j][k]);
			}
			printf("\t");
		}
		printf("\n");
	}
	//-----------------------------------------------------
	*/
	//------STORING EPSILON CLOSURE OF ORIGINAL STATES
	int eclose[numsta][numsta];
	//-------------------INITIALIZING EPSILON CLOSURE SET----------------
	for(i=0;i<numsta;i++)
	{
		for(j=0;j<numsta;j++)
			eclose[i][j] = 0;
	}
	printf("\n\nWhich is the starting state ::::: ");
	scanf("%d",&st);
	//-----------COMPUTING THE EPSILON CLOSURE OF ORIGINAL STATES
	for(i=0;i<numsta;i++)
	{
		int *toni;
		toni = silon(eclose,i,arr);
		/*for(j=0;j<numsta;j++){
			if(arr[i][0][j] == 1)
				eclose[i][j] = 1;
		}*/
	}
	/*
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//PRINTING EPSILON CLOSURE OF ORIGINAL STATES
	for(i=0;i<numsta;i++)
	{
		for(j=0;j<numsta;j++)
			printf("%d",eclose[i][j]);
		printf("\n");
	}
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	//-------------------------------------------------------------------------------------------------------
	*/
	int foot = 0;//NEXT NEW STATE TO BE SENT
	int past[10][numsta];//STATES TO TRAVEL IN THE FUTURE
	int future[10][numsta];//STATES ALREADY TRAVELLED
	for(i=0;i<10;i++)
	{
		for(j=0;j<numsta;j++)
		{
			future[i][j] = -2;
		}
	}
	int pas = 0 , futu = 0;//POINTERS TO THE NEXT AVAILABLE SPACE IN THE PAST AND FUTURE MATRIX
	int send[numsta];//TO BE SENT TO THE COMPUTE FUNCTION
	//-------------INITIALLY SEND THE EPSILON CLOSURE OF THE START STATE
	for(j=0;j<numsta;j++)
		send[j] = eclose[st][j];
	while(1)
	{
		//for(i=0;i<numsta;i++)
			//printf("%d",send[i]);
		int *sid;//POINTER RECIEVED FROM COMPUTE FUNCTION
		int cols[numsym][numsta];//TO STORE THE FINAL RESULT
		for(i=0;i<numsta;i++)
		{
			cols[0][i] = send[i];
		}
		sid = compute(arr,eclose,send);
		//---------APPEND THE ALREADY TRAVERCED STATE TO PAST ARRAY-----
		for(i=0;i<numsta;i++)
			past[pas][i] = send[i];
		pas += 1;
		//------------STORE FINAL RESULT IN COLS----------------
		for(i=1;i<numsym;i++)
		{
			for(j=0;j<numsta;j++)
			{
				cols[i][j] = *sid;
				sid++;
			}
		}
		//=========DISPLAY COLS===========
		for(i=0;i<numsym;i++)
		{
			printf("{ ");
			for(j=0;j<numsta;j++)
			{
				if(cols[i][j] == 1)
					printf("Q%d, ",j);
			}
			printf("}\t\t");
		}
		//printf("\n");

		//----------Check if new element arrived---------
		int ii,jj,apo[numsym - 1];
		int lark;
		for(lark = 0 ; lark < (numsym - 1) ; lark++)
			apo[lark] = 0;
		for(i=1;i<numsym;i++)
		{
				for(ii=0;ii<pas;ii++)
				{
					int flag = 0;
					for(jj=0;jj<numsta;jj++)
					{
						if(cols[i][jj] == past[ii][jj])
							flag++;
					}
					if(flag == numsta)
						apo[i-1] = 1;
				}
		}
		//-------if APO[I] IS 0 THEN NEW ELEMENT ARRIVED----------
		for(i = 0; i< (numsym - 1);i++)
		{
			if(apo[i] == 0)
			{
				for(j=0;j<numsta;j++)
				{
					//=========ADD IT TO THE FUTURE ARRAY=================
					future[futu][j] = cols[i+1][j];
				}
				futu++;
			}
		}
		//=========CHECK IF NEXT VALUE TO BE SENT TO COMPUTE HAS ALREADY BEEN TRAVERCED OR NOT===========
		int cazz = 1;
			for(i=0;i<pas;i++)
			{
				int tuna = 0;
				for(j=0;j<numsta;j++)
				{
					if(past[i][j] == future[foot][j])
						tuna+=1;
				}
				if(tuna == numsta){
					foot += 1;
					i = 0;
				}
			}
		//=========SET THE SEND ARRAY TO THE NEXT VALUE IN THE FUTURE===========
		for(j=0;j<numsta;j++)
		{
			send[j] = future[foot][j];
		}
		foot += 1;
		//printf("--------%d%d%d--------",future[foot][0],future[foot][1],future[foot][2]);
		if(send[0] == -2)
			break;
	}
	/*
	printf("\n");
	for(i=0;i<=foot;i++)
	{
		for(j=0;j<numsta;j++)
		{
			printf("%d",future[i][j]);
		}
		printf("\n");
	}
	for(i=0;i<=pas;i++)
	{
		for(j=0;j<numsta;j++)
		{
			printf("%d",past[i][j]);
		}
		printf("\n");
	}*/
	printf("\n\n");
	//----------------------------------------------------------------------------------------------------------------------
}

int* compute(int arr[numsta][numsym][numsta], int eclose[numsta][numsta],int eeclose[numsta])
{
		int symbol,i,j;
		symbol = numsym - 1;
		//------------VARIABLE TO STORE FINAL RESULT------------
		int temp1[symbol][numsta];
		//======INITIALIZE IT==========
		for(i=0;i<symbol;i++)
			for(j=0;j<numsta;j++)
				temp1[i][j] = 0;
		//----------RUN LOOP FOR EACH SYMBOL---------
		while(symbol)
		{
			//--------INITIALIZE TEMPORARY ARRAY FOR TEMPORARY STORAGE OF SOLUTION------------
			int temp[numsta];
			for(j=0;j<numsta;j++)
				temp[j] = 0;
			for(i=0;i<numsta;i++)
			{
				if(eeclose[i] == 1)
				{
					for(j=0;j<numsta;j++)
					{
						if (arr[i][symbol][j] == 1)
							temp[j] = 1;
					}
				}
			}
			/*
			for(i=0;i<numsta;i++)
				printf("%d",temp[i]);
			*/
			//============================
			//-------CROSS CHECK TEMPORARY SOLUTION WITH THE EPSILON CLOSURE OF ORIGINAL STATES AND APPEND TO FINAL SOLUTION--------------
			printf("\n");
			symbol -= 1;
			for(j=0;j<numsta;j++)
				if (temp[j] == 1)
					for(i=0;i<numsta;i++)
						if(temp1[symbol][i] != 1) 
							temp1[symbol][i] = eclose[j][i];
		}
		/*
		symbol = numsym - 1;
		for(j=0;j<symbol;j++){
			for(i=0;i<numsta;i++)
				printf("%d",temp1[j][i]);
			printf("\n");
		}*/
		return temp1;	
}

int* silon(int eclose[numsta][numsta], int i, int arr[numsta][numsym][numsta])
{
	//printf("cazz");
	int j;
	int *toni;
	eclose[i][i] = 1;
	for(j=0;j<numsta;j++){
			if (i == j)
				continue;
			if(arr[i][0][j] == 1){
				eclose[i][j] = 1;
				toni = silon(eclose,j,arr);
				int temp;
				for(temp = 0 ; temp < numsta ; temp++)
				{
					if(toni[temp] == 1)
						eclose[i][temp] = 1;
				}
			}
	}
	return eclose[i];
}
