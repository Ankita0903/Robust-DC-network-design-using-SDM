/*****************************************************
File:   myProblem.c
Written by: ankita
Date modified: ??????

(1) To compile this program, please type: make
(2) To run this program, please type: ./myProblem
****************************************************/

#include "myProblem.h"
#ifndef CPX_PROTOTYPE_MIN

static void
   free_and_null (char **ptr),
   usage         (char *progname);
#else
static void
   free_and_null (),
   usage         (); 
#endif

#ifndef CPX_PROTOTYPE_MIN
int main (int argc, char *argv[])
#else
int main (argc, argv)
int argc;
char *argv[];
#endif
{	
	int i, j, a ,b, c,d, e, g, l, p, q, r, gapRow = 55, gapCol = 50, gapIndexCol, gapCount, *temp, maxEdge, dnode, edgeNo, indexNo, gapsUsed; 
	int counter=0, counterx = 0, countery = 0,bcounterx = 0, bcountery = 0, regexCount=0, infeasibilityCount =0, totalResources;
	float BP;
	
	int topArr[N][N], topDistArr[N][N], dataCenter[3], disaster[D], inputDisasTopArr[N][N], demand[R][2], replication[6][2], modulation[4][3];
	int primaryInfo[N*2][3], backupInfo[N*2][3], gapTopology[gapRow][gapCol],topBackupArr[N][N], topBackDistArr[N][N];

	FILE *inputFile, *inputDataCenterFile, *inputDisasterFile, *outFile, *lpFile, *inputDisasTopFile, *Fileid, *inputDemandsFile, *inputReplicationFile, *inputGapFile, *gapIndexFile, *gapInputFile, *updateGapFile;
	int modulationInfo[MF][3]={{0, 4, 1200}, {1, 3, 2400}, {2, 2, 4800}}; /*denotes 16-QAM,8-QAM,QPSK;k_0 (spectral efficiency), d_0 (optical distance)*/

 
	
			
	time_t startTime,endTime;
	double solution_time;
	char ch,*tokenPtr,s[30],s1[30];
	char lpFileName[1000],solutionFileName[50], disasterFileName[50], gapIndexFileName[1000], updateGapFileName[1000];
	
	/****************************************** end of initializations *************************************************************************/
	
	/* Declare and allocate space for the variables and arrays where we
        will store the optimization results including the status, objective
        value, and variable values. */


	char *colname=NULL;

	value=NULL;
	env=NULL;
	lp=NULL;

	int solstat;
	double objval;
	int surplus;
	char **cur_colname = NULL;
	char *cur_colnamestore = NULL;
	int cur_colnamespace;
	int num;
	int cur_numcols;

	srand(time(NULL));
	
    /**************************************Reading input files - start ******************************************************/
	
    //read topology with distance of edges in the file
	if((inputFile=fopen("topologyWithDistance.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		printf("\n THESIS :\n");
		printf("\n The Topology file with distance of edges is :\n");

	for(i=0;i<N;i++)
		for (j=0; j<N; j++)
		fscanf(inputFile,"%d",&topDistArr[i][j]);//scanning
	
		 
	for(i=0;i<N;i++){
		for (j=0; j<N; j++)
        printf("%d ", topDistArr[i][j]); //printing topology file
		printf("\n");
    }
    fclose(inputFile);
	
	 //read topology with edge number in the file
	if((inputFile=fopen("topologyWithEdgeNumber.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		printf("\n The Topology file with edge number is :\n");

	for(i=0;i<N;i++)
		for (j=0; j<N; j++)
		fscanf(inputFile,"%d",&topArr[i][j]);//scanning
	
		 
	for(i=0;i<N;i++){
		for (j=0; j<N; j++)
        printf("%d ", topArr[i][j]); //printing topology file
		printf("\n");
    }
	maxEdge = topArr[0][0];
 
   /*for(i=0;i<N;i++)// getting the max edge no of the topology file to create same no of gap edges
   {
      for(j=0;j<N;j++)
      {
         if(topArr[i][j] > maxEdge)
            maxEdge = topArr[i][j];
      }
   }*/
   //printf("Maximum element in topology matrix is %d\n", maxEdge);
   //gapRow= maxEdge+1;*/
	
	
    fclose(inputFile);
  

	
	//read demands file
	if((inputDemandsFile=fopen("demandsFile.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		
	printf("\n The Demands file is :\n");

	for(i=0;i<R;i++)
		for (j=0; j<2; j++)
			fscanf(inputDemandsFile,"%d",&demand[i][j]);//scanning
	
		 
	for(i=0;i<R;i++){
		for (j=0; j<2; j++)
        printf("%d ", demand[i][j]); //printing demands file
		printf("\n");
    }
    fclose(inputDemandsFile);
	
	
	
	//read replication information file
	if((inputReplicationFile=fopen("replicationInfoFile.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		
	printf("\n The Replication Info file is :\n");

	for(i=0;i<6;i++)
		for (j=0; j<2; j++)
			fscanf(inputFile,"%d",&replication[i][j]);// scanning
	
		 
	for(i=0;i<6;i++){
		for (j=0; j<2; j++)
        printf("%d ", replication[i][j]); //printing replication information file
		printf("\n");
    }
    fclose(inputReplicationFile);
	
	
	
    //read data center file
	if((inputDataCenterFile=fopen("dataCenter_file.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		
	printf("\n The data Center file is :\n");

	for(i=0;i<3;i++)
			fscanf(inputDataCenterFile,"%d",&dataCenter[i]);//scanning
	
		 
	for(i=0;i<3;i++)
       printf("%d \n", dataCenter[i]); //printing data center file
    

    fclose(inputDataCenterFile);

	
	//read disaster file
	if((inputDisasterFile=fopen("disaster.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		
	printf("\n The Disaster file is :\n");

	for(i=0;i<D;i++)
			fscanf(inputDisasterFile,"%d",&disaster[i]);//scanning
	
		 
	for(i=0;i<D;i++)
       printf("%d \n", disaster[i]); //printing disaster file
    
    fclose(inputDisasterFile);
	
	
	printf("\n The Modulation Info file is :\n");	 
	for(i=0;i<3;i++){
		for (j=0; j<3; j++)
        printf("%d ", modulationInfo[i][j]); //printing modulation information file as initialized in the array above
		printf("\n");
    }
	
    for	(q=0; q<R; q++)//loop
	{
	//read gap topology file
	if((gapInputFile=fopen("gapTopologyFile.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	else
		printf("\n The Gap Topology file is :\n");

	for(i=0;i<gapRow;i++)
		for (j=0; j<gapCol; j++)
		fscanf(inputFile,"%d",&gapTopology[i][j]);//scanning
	
		 
	for(i=0;i<gapRow;i++){
		for (j=0; j<gapCol; j++)
        printf("%d ", gapTopology[i][j]); //printing gap topology file
		printf("\n");
    }
    fclose(gapInputFile);
	/**************************************Reading input files - end ******************************************************/
	
	/**************************************Writing files - start **********************************************************/

		
	//write lpfile file
	sprintf(lpFileName, "lpFile.txt");

	if((lpFile=fopen(lpFileName,"w"))==NULL)
	{	printf("LP file can not be opened.\n");
		exit(2);
	}


	//write gapIndex file.. just to view if generated links are what we want
	sprintf(gapIndexFileName, "gapIndex.txt");
	
	if((gapIndexFile=fopen(gapIndexFileName,"w"))==NULL)
	{	printf("Gap Index file can not be opened.\n");
		exit(2);
	}
	
	
	//write solution file
	sprintf(solutionFileName, "solution.txt");

	if((outFile=fopen(solutionFileName,"w"))==NULL)
	{	printf("Solution file can not be opened.\n");
		exit(2);
	}
	
	//write solution file
	sprintf(updateGapFileName, "gapTopologyFile.txt");

	if((updateGapFile=fopen(updateGapFileName,"w"))==NULL)
	{	printf("Solution file can not be opened.\n");
		exit(2);
	}
	
	/**************************************Writing files - end **************************************************************/
			
	/*********** Calculating gap numbers and starting/ending gap indexes per edge - start ************************************/

	
	int gapNumber[gapRow] = {0};//array for storing gap numbers per edge
	
	if(gapCol%2 !=0 )
	 { 
        gapIndexCol = (gapCol/2 +1)*2; //gapIndexCol is the dimension for number of cols in the array which stores a_ij and b_ij
     }
	else
	 { 
        gapIndexCol = gapCol/2 *2; 
     } 
    
	
	int gapIndex[gapRow][gapIndexCol];//array for storing starting and ending gap indexes

	
	
	printf("\n gap topology file is : \n");
		for(i=0;i<gapRow;i++)
			{
				for(j=0;j<gapCol;j++)
				{
					fprintf(gapIndexFile,"%d ", gapTopology[i][j]); //printing gap topology file in gapIndex.txt
					//printf("%d ", gapTopology[i][j]);//printing gap topology file
				}
				fprintf(gapIndexFile,"\n");
				//printf("\n");
			}
			fprintf(gapIndexFile,"\n");
	
		
		for(i=0;i<gapRow;i++)
			{
				for(j=0;j<gapIndexCol;j++)
				{
					gapIndex[i][j]=-1;
					//fprintf(gapIndexFile,"%d ",gapIndex[i][j]);
				}
				fprintf(gapIndexFile,"\n");
			}
	
	

		for ( i= 0; i<gapRow; i++)
			{  
				gapCount =0; p=0;
				for ( j= 0; j<gapCol; j++)
					{
						if (gapTopology[i][j]==0) 
						{  
							if(gapCount ==0)
							{
								gapIndex[i][p]=j+1;
								p++;
							}

						gapCount++;

							if(gapCount>0 && gapNumber[i]>0)
							{ 
								if(gapTopology[i][j]!=(gapTopology[i][j-1]))
								{
								gapIndex[i][p]=j+1;
								p++;
								}
							}             

						}
			 
						else if(gapCount>0) 
						{
							if(gapTopology[i][j]!=(gapTopology[i][j-1]))
							{
								gapIndex[i][p]=j;
								p++;
								gapNumber[i]++;
							}					
						} 
					} 
				         
        
				if(gapCount>0 && gapTopology[i][gapCol-1]==0)
				{
					gapIndex[i][p]=j;
                    p++;
					gapNumber[i]++;
				}

		}
    
		fprintf(gapIndexFile,"#edge #gap no\n");
		printf("\n#edge #gap no\n");
		for (a= 0; a<gapRow; a++)
        {
			fprintf(gapIndexFile,"%d  %d \n", a, gapNumber[a]);
			printf(" %d      %d \n", a, gapNumber[a]);
        }
		fprintf(gapIndexFile,"\n\n");
		printf("\n\n");
    
  
		printf("\n Gap Index array :\n");
		for(i=0;i<gapRow;i++)
		{
			for(j=0;j<gapIndexCol;j++)
			{
				fprintf(gapIndexFile, "%d ",gapIndex[i][j]);
				printf("%d ",gapIndex[i][j]);
			}
			fprintf(gapIndexFile,"\n");
			printf("\n");
		}
	
	
	/*********** Calculating gap numbers and starting/ending gap indexes per edge - end *************************************/
	
	/********* generating Backup Topology based on the number of disasters - start ******************************************/

			for ( a=0; a<N; a++)
				{
					for(b=0;b<N;b++)
					{ 
						topBackupArr[a][b]= topArr[a][b];
						topBackDistArr[a][b]= topDistArr[a][b];
					}               
				}
				
			for ( int z=0; z<D; z++)
            {
				dnode = disaster[z];
				printf("disaster occured at node = %d , ",dnode);
		 
			
				
				for ( a=0; a<N; a++)
				{
					for(b=0;b<N;b++)
					{ 
						if(topBackupArr[a][b]!=-1)
						{
							if(a == dnode|| b == dnode)
							topBackupArr[a][b]=-1;
						}					
					}               
				} 
				
				 
				for ( a=0; a<N; a++)
				{
					for(b=0;b<N;b++)
					{ 						
						if(topBackDistArr[a][b]!=-1)
						{
							if(a == dnode|| b == dnode)
							topBackDistArr[a][b]=-1;
						}
					}              
				} 
			}
			
			printf("\n Backup topology file is :\n");
			for ( a=0; a<N; a++)
				{
					for(b=0;b<N;b++)
					{ 
						printf("%d ", topBackupArr[a][b]);
					}
					printf("\n"); 
				}
				printf("\n");
			
			printf("\n Backup Distance topology file is :\n");
			for ( a=0; a<N; a++)
				{
					for(b=0;b<N;b++)
					{ 
						printf("%d ", topBackDistArr[a][b]);
					}
					printf("\n"); 
				}
				printf("\n");
	
	/********* generating backup topology based on the number of disasters - end ******************************************/
	
	
	
	/**********************************  OBJECTIVE FUNCTION (1) **********8********************************/

	fprintf (lpFile, "Minimize\n obj: ");
    
	fprintf(lpFile, "Phi - theta + psi - omega");
 
   for(i=0;i<N;i++)	
			for (j=0;j<N;j++)
				if(topArr[i][j]!=-1)
					fprintf(lpFile, "+ %lf x_%d_%d ", 0.0001, i, j);
				
                       
	 for(i=0;i<N;i++)	
		for (j=0;j<N;j++)
			if(topBackupArr[i][j]!=-1)
				fprintf(lpFile, "+ %lf y_%d_%d ", 0.0001, i, j);
   
	
        
	/************ Constraint 2 *********** Flow balance equation : Primary **************************************/      
 
 
    fprintf (lpFile, "\nSubject to \n");
	
			
			for (i=0;i<N;i++)	
			{	
				fprintf(lpFile, "c%d: ", counter++);
				
				for (j=0;j<N;j++)
					if(topArr[i][j]!=-1)
						fprintf(lpFile, "+ x_%d_%d ", i, j );

				for (j=0;j<N;j++)
					if(topArr[j][i]!=-1)
						fprintf(lpFile, "- x_%d_%d ", j, i );
					
				if(i == source)	  //lightpath source is virtual node 0 		
					fprintf(lpFile, "= 1 \n");
				else if(i == demand[q][1])  //lightpath destination is 5
					fprintf(lpFile, "= -1 \n");
				else
					fprintf(lpFile, "= 0 \n");	
				
			} 
						

	/************* Constraint 3 **** Flow balance equation : Backup **********************************************/
		
		
		for (d=0;d<D;d++)	
		//if(demand[q][1] != disaster[d])
				{
					for (i=0;i<N;i++)	
					{	
						fprintf(lpFile, "c%d: ", counter++);
							
						for (j=0;j<N;j++)
							if(topBackupArr[i][j]!=-1)
							{
									fprintf(lpFile, "+ w_%d_%d_%d ", disaster[d], i, j );
							}

						for (j=0;j<N;j++)
							if(topBackupArr[j][i]!=-1)
							{
									fprintf(lpFile, "- w_%d_%d_%d ", disaster[d], j, i );
							}
								
						if(i == source)	  //lightpath source is virtual node 0 		
							fprintf(lpFile, "- q_%d = 0\n", disaster[d]);
						else if(i == demand[q][1])  
							fprintf(lpFile, "+ q_%d = 0\n", disaster[d]);
						else
							fprintf(lpFile, "= 0 \n");	
							
					} 
				
				}			
		
	
	/*************Backup: set q_d =1 if disaster happens **************************/	
	
	/************** Constraint 4 ***********/	
			
   for (d=0;d<D;d++)
   	//if(demand[q][1] != disaster[d])
  		for(i=0;i<N;i++)	
  			for (j=0;j<N;j++)
  				if(topArr[i][j]!=-1)
					if(i == disaster[d] || j == disaster[d])
    					{
    						fprintf(lpFile, "c%d: ", counter++);
    						fprintf(lpFile, "q_%d - x_%d_%d >= 0 \n", disaster[d], i, j);
    					}

	
	

	/************** Constraint 5 **********/		
				
	for (d=0;d<D;d++)
    //if(demand[q][1] != disaster[d])	
		{		
			fprintf(lpFile, "c%d: ", counter++);
			fprintf(lpFile, "q_%d ", disaster[d]);
			
			for(i=0;i<N;i++)			
				for (j=0;j<N;j++)
					if(topArr[i][j]!=-1)	
						if(i == disaster[d] || j == disaster[d])
						{		
							fprintf(lpFile, "- x_%d_%d ", i, j);
						}

      fprintf(lpFile, "<= 0 \n");
		}
		

	
	/********** Determine backup path when disaster happens ***********************************************/
	
	/************** Constraint 6 **********/
	
	for(d=0;d<D;d++)
    //if(demand[q][1] != disaster[d])
		
			for(i=0;i<N;i++)			
				for (j=0;j<N;j++)
					if(topArr[i][j]!=-1)
					{	
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, "y_%d_%d + %d q_%d - w_%d_%d_%d <= %d \n", i, j, M, disaster[d], disaster[d], i, j, M );
					}

		
	/************** Constraint 7 **********/
	
	
	for(d=0;d<D;d++)
    //if(demand[q][1] != disaster[d])
			for(i=0;i<N;i++)
			  for (j=0;j<N;j++)
					if(topArr[i][j]!=-1)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, "y_%d_%d - w_%d_%d_%d >= 0 \n", i, j, disaster[d], i, j);
					}

	
		
	/************** Additional Constraint  change**********/
		
			for(i=0;i<N;i++)
			{				
				for (j=0;j<N;j++)
				{	
					if(topBackupArr[i][j]!=-1)
					{	fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, "y_%d_%d ", i, j);
						for (d=0;d<D;d++)	
						{
								
							fprintf(lpFile, " - w_%d_%d_%d ",disaster[d], i, j);
						}
							
						fprintf(lpFile, "<=0\n");
					}
				}
				
			}	
	/************** Constraint 8 (a) ****************** length of path : Primary *********************************/
	
	  	
		fprintf(lpFile, "c%d: ", counter++);	
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					if(topArr[i][j]!=-1)
						fprintf(lpFile, "+ %d x_%d_%d ", topDistArr[i][j], i, j);

			fprintf(lpFile," - l_P = 0\n");
			
	
	
	/************** Constraint 8 (b)****************** length of path : Backup *********************************/
	
	  	
		fprintf(lpFile, "c%d: ", counter++);	
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					if(topBackupArr[i][j]!=-1)
						fprintf(lpFile, "+ %d y_%d_%d ", topBackDistArr[i][j], i, j);

			fprintf(lpFile," - l_B = 0\n");


	/******************************* For 16-QAM : Primary *****************************************************************************************************/
	
	/* Constraint 9 (a) */
		
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d F_P_%d + l_P >= %d\n", M, modulationInfo[0][0], modulationInfo[0][2]);    
          
    	
			
		
	/* Constraint 10 (a) */
	
			
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " - %d F_P_%d - l_P >= %d\n", M, modulationInfo[0][0], (-1*modulationInfo[0][2])-M);
				
				
	
	/******************************* For 16-QAM : Backup *****************************************************************************************************/
	
	/* Constraint 9 (b) */
		
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d F_B_%d + l_B >= %d\n", M, modulationInfo[0][0], modulationInfo[0][2]);    
          
    	
			
		
	/* Constraint 10 (b) */
	
			
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " - %d F_B_%d - l_B >= %d\n", M, modulationInfo[0][0], (-1*modulationInfo[0][2])-M);
				
				
	
	/******************************* For 8-QAM : Primary *****************************************************************************************************/
	/* Constraint 11 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d G_P_%d + l_P >= %d\n", M, modulationInfo[1][0], modulationInfo[1][2]);  
				
			
			
	/* Constraint 12 (a) */		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " - %d G_P_%d - l_P >= %d\n", M, modulationInfo[1][0], (-1*modulationInfo[1][2])-M);
				
	
	/* Constraint 13 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d - G_P_%d <= 0\n", modulationInfo[1][0], modulationInfo[1][0]); 
				
				
	
	/* Constraint 14 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d + F_P_%d <= 1\n", modulationInfo[1][0], modulationInfo[0][0]); 	


	/* Constraint 15 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d - G_P_%d + F_P_%d >= 0\n",modulationInfo[1][0], modulationInfo[1][0], modulationInfo[0][0]);		

				
	
	
	/******************************* For 8-QAM : Backup *****************************************************************************************************/
	/* Constraint 11 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d G_B_%d + l_B >= %d\n", M, modulationInfo[1][0], modulationInfo[1][2]);  
				
			
			
	/* Constraint 12 (b) */		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " - %d G_B_%d - l_B >= %d\n", M, modulationInfo[1][0], (-1*modulationInfo[1][2])-M);
				
	
	/* Constraint 13 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d - G_B_%d <= 0\n", modulationInfo[1][0], modulationInfo[1][0]); 
				
				
	
	/* Constraint 14 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d + F_B_%d <= 1\n",modulationInfo[1][0], modulationInfo[0][0]); 	


	/* Constraint 15 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d - G_B_%d + F_B_%d >= 0\n",modulationInfo[1][0], modulationInfo[1][0], modulationInfo[0][0]);		

	
	
	
	/******************************* For QPSK : Primary *****************************************************************************************************/
	/* Constraint 16 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d G_P_%d + l_P >= %d\n", M, modulationInfo[2][0], modulationInfo[2][2]);  
				
			
			
	/* Constraint 17 (a) */		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " - %d G_P_%d - l_P >= %d\n", M, modulationInfo[2][0], (-1*modulationInfo[2][2])-M);
				
	
	/* Constraint 18 (a) */		
	
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d - G_P_%d <= 0\n", modulationInfo[2][0], modulationInfo[2][0]); 
				
				
	
	/* Constraint 19 (a) */	
	
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d + F_P_%d <= 1\n",modulationInfo[2][0], modulationInfo[0][0]); 	
				
				
	/* Constraint 20 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d + F_P_%d <= 1\n",modulationInfo[2][0], modulationInfo[1][0]); 
				

	/* Constraint 21 (a) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_P_%d - G_P_%d + F_P_%d + F_P_%d >= 0\n",modulationInfo[2][0], modulationInfo[2][0], modulationInfo[0][0], modulationInfo[1][0]);		

				
	/******************************* For QPSK : Backup *****************************************************************************************************/
	/* Constraint 16 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d G_B_%d + l_B >= %d\n", M, modulationInfo[2][0], modulationInfo[2][2]);  
				
			
			
	/* Constraint 17 (b) */		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " - %d G_B_%d - l_B >= %d\n", M, modulationInfo[2][0], (-1*modulationInfo[2][2])-M);
				
	
	/* Constraint 18 (b) */		
	
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d - G_B_%d <= 0\n", modulationInfo[2][0], modulationInfo[2][0]); 
				
				
	
	/* Constraint 19 (b) */	
	
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d + F_B_%d <= 1\n",modulationInfo[2][0], modulationInfo[0][0]); 	
				
				
	/* Constraint 20 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d + F_B_%d <= 1\n",modulationInfo[2][0], modulationInfo[1][0]); 
				

	/* Constraint 21 (b) */		
	
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " F_B_%d - G_B_%d + F_B_%d + F_B_%d >= 0\n", modulationInfo[2][0], modulationInfo[2][0], modulationInfo[0][0], modulationInfo[1][0]);		

							
	/*******************************  No of Subcarriers For 16-QAM : Primary **************
				
				
	/** Constraint 22 (a) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_P - %d F_P_%d >= 0 \n", modulationInfo[0][1], B, modulationInfo[0][0]);

			
		
			
	/** Constraint 23 (a) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_P + %d F_P_%d <= %d \n", modulationInfo[0][1], M*modulationInfo[0][1], modulationInfo[0][0], M*modulationInfo[0][1]+ B);
				
	
	
	/*******************************  No of Subcarriers For 16-QAM : Backup **************
				
				
	/** Constraint 22 (b) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_B - %d F_B_%d >= 0 \n", modulationInfo[0][1], B, modulationInfo[0][0]);

			
		
			
	/** Constraint 23 (b) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_B + %d F_B_%d <= %d \n", modulationInfo[0][1], M*modulationInfo[0][1], modulationInfo[0][0], M*modulationInfo[0][1]+ B);
				
	
	
	/******************************* No of Subcarriers For 8-QAM : Primary **************
				
				
	/** Constraint 22 (a) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_P - %d F_P_%d >= 0 \n", modulationInfo[1][1], B, modulationInfo[1][0]);

			
		
			
	/** Constraint 23 (a) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_P + %d F_P_%d <= %d \n", modulationInfo[1][1], M*modulationInfo[1][1], modulationInfo[1][0], M*modulationInfo[1][1]+ B);
				
	
/******************************* No of Subcarriers For 8-QAM : Backup **************
				
				
	/** Constraint 22 (b) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_B - %d F_B_%d >= 0 \n", modulationInfo[1][1], B, modulationInfo[1][0]);

			
		
			
	/** Constraint 23 (b) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_B + %d F_B_%d <= %d \n", modulationInfo[1][1], M*modulationInfo[1][1], modulationInfo[1][0], M*modulationInfo[1][1]+ B);
				
		
	/******************************* No of Subcarriers For QPSK : Primary **************
				
				
	/** Constraint 22 (a) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_P - %d F_P_%d >= 0 \n", modulationInfo[2][1], B, modulationInfo[2][0]);

			
		
			
	/** Constraint 23 (a) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_P + %d F_P_%d <= %d \n", modulationInfo[2][1], M*modulationInfo[2][1], modulationInfo[2][0], M*modulationInfo[2][1]+ B);
				
	
		/******************************* No of Subcarriers For QPSK : Backup **************
				
				
	/** Constraint 22 (b) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_B - %d F_B_%d >= 0 \n", modulationInfo[2][1], B, modulationInfo[2][0]);

			
		
			
	/** Constraint 23 (b) **/		
	
		
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, "%d n_B + %d F_B_%d <= %d \n", modulationInfo[2][1], M*modulationInfo[2][1], modulationInfo[2][0], M*modulationInfo[2][1]+ B);
				
	
	/***** Constraint 24 (a) ********* Exactly one spectrum gap should be used per core per fiber :Primary ********/	
	

				 for (i=0;i<N;i++)
				 {
					for (j=0;j<N;j++)
					{
						if(topArr[i][j]!=-1)
						{	
							fprintf(lpFile, "c%d: ", counter++);
							for(g=0; g<gapNumber[topArr[i][j]]; g++) //array gapNumber stores no of gaps per edge
							{		
								fprintf(lpFile, " + X_%d_%d_%d ", g, i, j);
								fprintf(gapIndexFile, " const 13 --> for edge %d , gap no =%d \n",topArr[i][j],g);
							}
							fprintf(lpFile, "- x_%d_%d = 0 \n", i, j);
						}
					}
				 }	
				fprintf(gapIndexFile, " \n\n");
				
	/***** Constraint 24 (b) ********* Exactly one spectrum gap should be used per core per fiber :Backup *************************/	
	

				 for (i=0;i<N;i++)
				 {
					for (j=0;j<N;j++)
					{
						if(topBackupArr[i][j]!=-1)
						{	
							fprintf(lpFile, "c%d: ", counter++);
							for(g=0; g<gapNumber[topBackupArr[i][j]]; g++) //array gapNumber stores no of gaps per edge
							{		
								fprintf(lpFile, " + Y_%d_%d_%d ", g, i, j);
								fprintf(gapIndexFile, " const 13 --> for edge %d , gap no =%d \n",topBackupArr[i][j],g);
							}
							fprintf(lpFile, "- y_%d_%d = 0 \n", i, j);
						}
					}
				 }	
				fprintf(gapIndexFile, " \n\n");				
	
	/** Constraint 25 (a) **** starting subcarrier of primary communication should be greater or equal to the starting subcarrier of gth gap : Primary*****/
   
		
				for (i=0;i<N;i++)
					{
						for (j=0;j<N;j++)
							{
								if(topArr[i][j]!=-1)
								{	int l=0;
									for(g=0;g<gapCol-1;g+=2)
									{			
										if(gapIndex[topArr[i][j]][g]!=-1) //array gapIndex stores first and last index of te number of gaps in gapNumber
										{
											fprintf(lpFile, "c%d: ", counter++);
											fprintf(gapIndexFile, " const 14 --> for edge %d , no of gaps = %d, gap No %d, a_ij= %d \n",topArr[i][j], gapNumber[topArr[i][j]], l, gapIndex[topArr[i][j]][g]);
											fprintf(lpFile, "theta - %d X_%d_%d_%d >= 0 \n", gapIndex[topArr[i][j]][g], l, i, j);
											l++;
										}
									}
								}
							}
					}
				fprintf(gapIndexFile, " \n\n");	

	
	/** Constraint 25 (b) **** starting subcarrier of backup communication should be greater or equal to the starting subcarrier of gth gap : Backup *****/
   
		
				for (i=0;i<N;i++)
					{
						for (j=0;j<N;j++)
							{
								if(topBackupArr[i][j]!=-1)
								{	int l=0;
									for(g=0;g<gapCol-1;g+=2)
									{			
										if(gapIndex[topBackupArr[i][j]][g]!=-1) //array gapIndex stores first and last index of te number of gaps in gapNumber
										{
											fprintf(lpFile, "c%d: ", counter++);
											fprintf(gapIndexFile, " const 14 --> for edge %d , no of gaps = %d, gap No %d, a_ij= %d \n",topBackupArr[i][j], gapNumber[topArr[i][j]], l, gapIndex[topArr[i][j]][g]);
											fprintf(lpFile, "omega - %d Y_%d_%d_%d >= 0 \n", gapIndex[topBackupArr[i][j]][g], l, i, j);
											l++;
										}
									}
								}
							}
					}
				fprintf(gapIndexFile, " \n\n");	

		
		
	/** Constraint 26 (a) **** ending subcarrier of primary communication should be less  than or equal to the starting subcarrier of gth gap :Primary *********/
	
	 	
				for (i=0;i<N;i++)
					{
						for (j=0;j<N;j++)
						{
							if(topArr[i][j]!=-1)
							{	int l=0;
								for(int g=0;g<gapCol;g+=2)
								{							
									if(gapIndex[topArr[i][j]][g]!=-1)
									{
										fprintf(lpFile, "c%d: ", counter++);
										fprintf(lpFile, "Phi + %d X_%d_%d_%d <= %d  \n", M, l, i, j, M+gapIndex[topArr[i][j]][g+1]);
										fprintf(gapIndexFile, " const 15 --> for edge %d , no of gaps = %d, gap No %d, b_ij= %d \n",topArr[i][j], gapNumber[topArr[i][j]], l, gapIndex[topArr[i][j]][g+1]);										
										l++;
									}
								}
							}
						}
					}
				fprintf(gapIndexFile, " \n\n");

	
	/** Constraint 26 (b) **** ending subcarrier of backup communication should be less  than or equal to the starting subcarrier of gth gap :Backup*********/
	
 	
				for (i=0;i<N;i++)
					{
						for (j=0;j<N;j++)
						{
							if(topBackupArr[i][j]!=-1)
							{	int l=0;
								for(int g=0;g<gapCol;g+=2)
								{							
									if(gapIndex[topBackupArr[i][j]][g]!=-1)
									{
										fprintf(lpFile, "c%d: ", counter++);
										fprintf(lpFile, "psi + %d Y_%d_%d_%d <= %d  \n", M, l, i, j, M+gapIndex[topBackupArr[i][j]][g+1]);
										fprintf(gapIndexFile, " const 15 --> for edge %d , no of gaps = %d, gap No %d, b_ij= %d \n",topBackupArr[i][j], gapNumber[topBackupArr[i][j]], l, gapIndex[topBackupArr[i][j]][g+1]);										
										l++;
									}
								}
							}
						}
					}
				fprintf(gapIndexFile, " \n\n");			
			

	/*** Constraint 27 (a) **** total no of subcarriers on all gaps & cores must be greater than or equal to the reqd bandwidth : Primary **************/
	

					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, " %d Phi - %d theta - n_P >= -%d \n",C, C, C);			
					

					
	/*** Constraint 27 (b) **** total no of subcarriers on all gaps & cores must be greater than or equal to the reqd bandwidth : Backup**************/
	

					//fprintf(lpFile, "c%d: ", counter++);
					//fprintf(lpFile, " %d psi - %d omega - n_B >= -%d \n",C, C, C);

	
	/*** Constraint  **** for positive value of objective function: Primary ****/

	
					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, " Phi - theta>= 0 \n");
					
	
	/*** Constraint  **** for positive value of objective function: Backup ****/

					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, " psi - omega >= 0 \n");
	
	
					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, " %d psi - %d omega - n_B - %d q >= %d \n", C, C, M*C, -C-M*C );
	
					
					for(d=0;d<D;d++)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " q - q_%d >= 0 \n", disaster[d]);	
					}
	
					
					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, " q");	
					for(d=0;d<D;d++)
					{
						fprintf(lpFile, " - q_%d",disaster[d]);	
					}
					fprintf(lpFile, " <= 0\n");	
					
					
					
					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, "psi - %d q <= 0\n", M);
					
					
					
					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, "F_P_%d + F_P_%d + F_P_%d = 1\n", 0,1,2);
					
	/************** bounds *************/

    fprintf(lpFile,"\nBounds"); 
			
    /************ integers ************/

  fprintf(lpFile,"\ngeneral");
	
  fprintf(lpFile, "\nn_P");
  fprintf(lpFile, "\nl_P");
  fprintf(lpFile, "\ntheta");
  fprintf(lpFile, "\nPhi");
  fprintf(lpFile, "\nl_B");//backup
  fprintf(lpFile, "\nn_B");//backup
  fprintf(lpFile, "\nomega");//backup
  fprintf(lpFile, "\npsi");//backup
  
  
	
  
	/************ binaries ************/

	
		fprintf(lpFile,"\nBinaries");


		for (i=0;i<N;i++)
			for (j=0;j<N;j++)
				if(topArr[i][j]!=-1)
					fprintf(lpFile,"\nx_%d_%d ", i, j );
		
		for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topArr[i][j]!=-1)
							for (int g=0; g<gapNumber[topArr[i][j]]; g++)
								fprintf(lpFile, "\nX_%d_%d_%d ", g, i, j);		
		
		
		for(d=0;d<D;d++)
		//if(demand[q][1] != disaster[d])
			{
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topBackupArr[i][j]!=-1)
							fprintf(lpFile,"\nw_%d_%d_%d ", disaster[d], i, j );
			}

		
		for (i=0;i<N;i++)
			for (j=0;j<N;j++)
				if(topBackupArr[i][j]!=-1)
					fprintf(lpFile,"\ny_%d_%d ", i, j );

				
		for (i=0;i<N;i++)
			for (j=0;j<N;j++)
				if(topBackupArr[i][j]!=-1)
					for (int g=0; g<gapNumber[topBackupArr[i][j]]; g++)
						fprintf(lpFile, "\nY_%d_%d_%d ", g, i, j);

		for(d=0;d<D;d++)
		//if(demand[q][1] != disaster[d])
			{
				fprintf(lpFile,"\nq_%d ",disaster[d] );
			}
	
					

		fprintf(lpFile, "\nF_P_%d \nF_P_%d \nF_P_%d", 0, 1, 2);
		fprintf(lpFile, "\nG_P_%d \nG_P_%d ", 1, 2);	
		fprintf(lpFile, "\nF_B_%d \nF_B_%d \nF_B_%d", 0, 1, 2); //backup
		fprintf(lpFile, "\nG_B_%d \nG_B_%d ", 1, 2); //backup

	
  	fprintf(lpFile,"\nEnd\n");
  
  	fclose(lpFile);
	


		/* Initialize the CPLEX environment */

		startTime = clock();

		env = CPXopenCPLEX(&status);

		if(env == NULL)
		{	refreshCPLEXEnvironment();
			printf("env error1.\n");
			exit(3);
		}

		//set time limit
		status=CPXsetdblparam(env,CPX_PARAM_TILIM,7200.0);

		/* Turn on output to the screen */
		status = CPXsetintparam(env,CPX_PARAM_SCRIND,CPX_ON);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("status error2.\n");
			exit(3);
		}

		/* Create the problem, using the filename as the problem name */

		lp = CPXcreateprob(env, &status, lpFileName);

		if(lp == NULL)
		{	refreshCPLEXEnvironment();
			printf("\nerror3:CPXcreateprob()\n");
			exit(3);
		}

		/* Read LP file into an existing CPLEX problem object */

		status = CPXreadcopyprob(env, lp, lpFileName, NULL);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror4: CPXreadcopyprob()\n");
			exit(3);
		}

		/* optimize the problem and obtain solution */

		status = CPXmipopt(env, lp);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror5:CPXmipopt()\n");
			exit(3);
		}

		/* get solution status */

		solstat = CPXgetstat(env, lp);

		/* get objective value */

		status = CPXgetmipobjval(env, lp, &objval);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror6:CPXgetmipobjval()\n");
			infeasibilityCount++;
			printf("\ninfeasibilityCount = %d \n", infeasibilityCount);
			goto LOOP;
			//exit(3);
		}

		endTime = clock();

		solution_time = (double)(endTime - startTime)/(double)CLOCKS_PER_SEC;

		fprintf(outFile, "Objective value = %f\nSolution time = %0.3f s\n", objval,solution_time);
        printf("Objective value = %f\nSolution time = %0.3f s\n", objval,solution_time);
		gapsUsed=(int)objval+1;
		printf("gapsUsed = %d",gapsUsed);


		/* The size of the problem should be obtained by asking CPLEX what the
		   actual size is. cur_numcols stores the current number of columns */

		cur_numcols = CPXgetnumcols(env, lp);

		/* Allocate space for solution */

		value = (double *)malloc(cur_numcols * sizeof(double));

		if(value == NULL)
		{	refreshCPLEXEnvironment();
			printf("\nerror:Allocating space for solution\n");
			exit(3);
		}

		status = CPXgetmipx(env, lp, value, 0, cur_numcols - 1 );

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror7:CPXgetmipx()\n");
			exit(3);
		}

		/* Use CPXgetcolname() to obtain the actual variables' name */

		status = CPXgetcolname(env, lp, NULL, NULL, 0, &surplus, 0, cur_numcols - 1);

		if((status != CPXERR_NEGATIVE_SURPLUS) && (status != 0))
		{	refreshCPLEXEnvironment();
			printf("\nerror8:CPXgetcolname()\n");
			exit(3);
		}

		cur_colnamespace = -surplus;

		if ( cur_colnamespace > 0 )
		{
			cur_colname      = (char **) malloc (sizeof(char *)*cur_numcols);
			cur_colnamestore = (char *)  malloc (cur_colnamespace);

			if( cur_colname == NULL || cur_colnamestore == NULL )
			{	status = -1;
				refreshCPLEXEnvironment();
				printf("\nerror9:cur_colname == NULL || cur_colnamestore == NULL.\n");
				exit(3);
			}

          		status = CPXgetcolname (env, lp, cur_colname, cur_colnamestore, cur_colnamespace, &surplus, 0, cur_numcols-1);

			if(status)
			{	refreshCPLEXEnvironment();
				printf("\nerror10:CPXgetcolname()\n");
				exit(3);
			}
		}
			
		/* Write out the solution */

	    	colname = (char *) (malloc (10));

		for (num = 0; num < cur_numcols; num++)
		{
			if (value[num] != 0)
			{
				strcpy(colname,"");
				colname = cur_colname[num];
				ch = colname[0];
				

				if (ch == 'x' || ch == 'X' || ch == 'w' || ch == 'y' || ch == 'Y' || ch == 'l' || ch == 'n' || ch == 'F' || ch == 't' || ch == 'P' || ch == 'p' || ch == 'o' || ch == 'q')  
        
					fprintf(outFile, "%-16s= %f\n", cur_colname[num],value[num]);
					
			
				
				if( ch == 'X')
				{	       
					//fprintf(outFile, "\n%s: ", cur_colname[num]);
					tokenPtr=strtok(colname,"X");
					strcpy(s,"");
					strcat(s,tokenPtr);
					/*tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					q=atoi(s1);
					tokenPtr=strtok(NULL,"_");*/
					tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					l=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					i=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					j=atoi(s1);
					//fprintf(outFile, "Lightpath uses gap %d for the link %d-->%d\n",l , i, j);	
					//fprintf(outFile, "\n");
				
				
					//for storing X_0_1_2 in an array					
					primaryInfo[counterx][countery] = l;
					primaryInfo[counterx][countery+1] = i;
					primaryInfo[counterx][countery+2] = j;
					//primaryInfo[counterx][countery+3] = j;
					counterx++;	//use this to print max no of rows when run the array for primaryInfoLink array	
					//fprintf(gapIndexFile,"counterx= %d",counterx);					
					
				}
				
				if( ch == 'Y')
				{	       
					//fprintf(outFile, "\n%s: ", cur_colname[num]);
					tokenPtr=strtok(colname,"Y");
					strcpy(s,"");
					strcat(s,tokenPtr);
					/*tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					q=atoi(s1);
					tokenPtr=strtok(NULL,"_");*/
					tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					c=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					d=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					e=atoi(s1);
					//fprintf(outFile, "Lightpath uses gap %d for the link %d-->%d\n",l , i, j);	
					//fprintf(outFile, "\n");
				
				
				
					//for storing Y_0_1_2 in an array					
					backupInfo[bcounterx][bcountery] = c;
					backupInfo[bcounterx][bcountery+1] = d;
					backupInfo[bcounterx][bcountery+2] = e;
					//primaryInfo[counterx][countery+3] = j;
					bcounterx++;	//use this to print max no of rows when run the array for primaryInfoLink array				
					
				}
			} /*end of value==1*/
		}  /*end of for*/
		
				 
		printf("\n\n Primary Light Path Link matrix is :\n");
		fprintf(gapIndexFile,"\n\n Primary Light Path Link matrix is :\n");
		for(i=0;i<counterx;i++)
		{
			for(j=0;j<3;j++)
			{
				if(primaryInfo[i][j]!=-1)
				{
					printf(" %d ",primaryInfo[i][j]);
					fprintf(gapIndexFile," %d ",primaryInfo[i][j]);
				}
			}
			printf("\n");
			fprintf(gapIndexFile,"\n");
		}
		printf("\n\n Backup Light Path Link matrix is :\n");
		fprintf(gapIndexFile,"\n\n Backup Light Path Link matrix is :\n");
		for(i=0;i<bcounterx;i++)
		{
			for(j=0;j<3;j++)
			{
				if(backupInfo[i][j]!=-1)
				{
					printf(" %d ",backupInfo[i][j]);
					fprintf(gapIndexFile," %d ",backupInfo[i][j]);
				}
			}
			printf("\n");
			fprintf(gapIndexFile,"\n");
		}
		
		
		
		//To update the previous gap topology with the no of slices used by the primary light path
		for(i=0;i<counterx;i++)
		{
			for(j=0;j<3;j++)
			{
				if(primaryInfo[i][1]!=-1 && primaryInfo[i][2]!=-1)
				{
					a = primaryInfo[i][1];//start link for LP
					//fprintf(gapIndexFile," a = %d ",a);
					b = primaryInfo[i][2];//end link for LP
					//fprintf(gapIndexFile," b = %d ",b);
					c = primaryInfo[i][0];//the gap no used in the LP
					//fprintf(gapIndexFile," gap no used = %d ",c);
					//fprintf(gapIndexFile," total gaps no used = %d ",gapsUsed);
					
					edgeNo = topArr[a][b];//edge on which gaps are getting allocated
					//fprintf(gapIndexFile," edgeNo = %d \n ",edgeNo);
					
					indexNo= gapIndex[edgeNo][c*2];//index no of the start of the allotment of slices
					//fprintf(gapIndexFile,"indexNo = %d\n", indexNo);
					
					if(i!=0)
					{
						if(primaryInfo[i][1]!=primaryInfo[i-1][1] && primaryInfo[i][2]!=primaryInfo[i-1][2])
						{
							for(int u = 0; u < gapsUsed; u++)//alloting 1's to the no of gaps used on the edge
							{
								gapTopology[edgeNo][indexNo-1]=1;
								indexNo++;
								fprintf(gapIndexFile," %d", gapTopology[i][j]);
							}
							fprintf(gapIndexFile,"\n");
						}
					}
					else
						for(int u = 0; u < gapsUsed; u++)//alloting 1's to the no of gaps used on the edge
							{
								gapTopology[edgeNo][indexNo-1]=1;
								indexNo++;
								fprintf(gapIndexFile," %d", gapTopology[i][j]);
							}
							fprintf(gapIndexFile,"\n");
						
				}
			}
			 fprintf(gapIndexFile,"\n");
		}
		
		// Printing Updated gap topology after assigning primary links
		printf("\n\n Updated gap topology file after assigning primary links : \n");
		for(i=0;i<gapRow;i++)
			{	
				for(j=0;j<gapCol;j++)
				{
					fprintf(gapIndexFile,"%d ", gapTopology[i][j]); //printing gap topology file in gapIndex.txt
					printf("%d ", gapTopology[i][j]);//printing gap topology file
					//fprintf(updateGapFile,"%d ", gapTopology[i][j]);
				}
				fprintf(gapIndexFile,"\n");
				//fprintf(updateGapFile,"\n");
				printf("\n");
			}
			fprintf(gapIndexFile,"\n");
			//fprintf(updateGapFile,"\n");
		
		
		//To update the previous gap topology with the no of slices used by the primary light path
		for(i=0;i<bcounterx;i++)
		{
			for(j=0;j<3;j++)
			{
				if(backupInfo[i][1]!=-1 && backupInfo[i][2]!=-1)
				{
					a = backupInfo[i][1];//start link for LP
					//fprintf(gapIndexFile," a = %d ",a);
					b = backupInfo[i][2];//end link for LP
					//fprintf(gapIndexFile," b = %d ",b);
					c = backupInfo[i][0];//the gap no used in the LP
					//fprintf(gapIndexFile," gap no used = %d ",c);
					//fprintf(gapIndexFile," total gaps no used = %d ",gapsUsed);
					
					edgeNo = topArr[a][b];//edge on which gaps are getting allocated
					//fprintf(gapIndexFile," edgeNo = %d \n ",edgeNo);
					
					indexNo= gapIndex[edgeNo][c*2];//index no of the start of the allotment of slices
					//fprintf(gapIndexFile,"indexNo = %d\n", indexNo);

					if(i!=0)
					{
						if(backupInfo[i][1]!=backupInfo[i-1][1] && backupInfo[i][2]!=backupInfo[i-1][2])
						{
							for(int u = 0; u < gapsUsed; u++)//alloting 1's to the no of gaps used on the edge
							{
								gapTopology[edgeNo][indexNo-1]=1;
								indexNo++;
								fprintf(gapIndexFile," %d", gapTopology[i][j]);
							}
							fprintf(gapIndexFile,"\n");
						}
					}
					else
						for(int u = 0; u < gapsUsed; u++)//alloting 1's to the no of gaps used on the edge
							{
								gapTopology[edgeNo][indexNo-1]=1;
								indexNo++;
								fprintf(gapIndexFile," %d", gapTopology[i][j]);
							}
							fprintf(gapIndexFile,"\n");

				}
			}
			fprintf(gapIndexFile,"\n");
		}
		
		// Printing Updated gap topology after assigning backup links
		printf("\n\n Updated gap topology file after assigning backup links : \n");
		totalResources=0;
		BP=0.0f;
		for(i=0;i<gapRow;i++)
			{	
				for(j=0;j<gapCol;j++)
				{
					fprintf(gapIndexFile,"%d ", gapTopology[i][j]); //printing gap topology file in gapIndex.txt
					printf("%d ", gapTopology[i][j]);//printing gap topology file
					fprintf(updateGapFile,"%d ", gapTopology[i][j]);
					if(gapTopology[i][j] > 0)
					totalResources++;
				}
				fprintf(gapIndexFile,"\n");
				fprintf(updateGapFile,"\n");
				printf("\n");
			}
			fprintf(gapIndexFile,"\n");
			fprintf(updateGapFile,"\n");
			printf("\n totalResources = %d\n", totalResources);
			BP= infeasibilityCount*100/R;
			printf("\n infeasibilityCount = %d \n R= %d \n BP = %f \n", infeasibilityCount, R, BP);
	
	
	LOOP :fclose(updateGapFile);

	
	
	fclose(outFile);

	free_and_null((char **) &value);
	free_and_null((char **) &cur_colname);
	free_and_null((char **) &cur_colnamestore);
	}
	
	
	
	return 0;
}



int refreshCPLEXEnvironment()
{	/* Free up the basis and solution */
	free_and_null ((char **) &value);

	/* Free up the problem as allocated by CPXcreateprob, if necessary */
	if ( lp != NULL )
	{
		status = CPXfreeprob (env, &lp);
		if ( status )
			fprintf (stderr, "CPXfreeprob failed, error code %d.\n", status);
 	 }

	/* Free up the CPLEX environment, if necessary */
	if ( env != NULL )
	{
		status = CPXcloseCPLEX (&env);
		/* Note that CPXcloseCPLEX produces no output,
		so the only way to see the cause of the error is to use
		CPXgeterrorstring.  For other CPLEX routines, the errors will
		be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON. */

		if ( status )
		{
			char  errmsg[1024];
			fprintf (stderr, "Could not close CPLEX environment.\n");
			CPXgeterrorstring (env, status, errmsg);
			fprintf (stderr, "%s", errmsg);
		}
	}

	return 0;
}

/* This simple routine frees up the pointer *ptr, and sets *ptr to NULL */

#ifndef CPX_PROTOTYPE_MIN
static void free_and_null (char **ptr)
#else
static void free_and_null (ptr)
char **ptr;
#endif
{
   if ( *ptr != NULL ) {
      free (*ptr);
      *ptr = NULL;
   }
} /* END free_and_null */


#ifndef CPX_PROTOTYPE_MIN
static void usage (char *progname)
#else
static void usage (progname)
char *progname;
#endif
{
   fprintf (stderr,"Usage: %s inputfile\n", progname);
   fprintf (stderr,"This program uses the CPLEX MIP optimizer.\n");
   fprintf (stderr,"Exiting...\n");
} /* END usage */

