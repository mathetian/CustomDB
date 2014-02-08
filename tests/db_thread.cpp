#include <iostream>
using namespace std;

#include "CustomDB.h"
#include "Option.h"
#include "TimeStamp.h"

#include <assert.h>


/**
	Create 5 four thread, each put 250000 items.
	For each thread, each time, it compute 5000 items.
**/
#define SIZE 1000000
#define BATCHSIZE 250000
#define SUBSIZE   5000
#define THRNUM    4

Options option;
CustomDB * db;

void* thrFunc(void * data)
{
	int flag = *(int*)data;
	const int beg  = flag*BATCHSIZE;
	int round = (BATCHSIZE + SUBSIZE - 1)/SUBSIZE;
	TimeStamp thrtime; char buf[50];

	printf("thread %d begin\n", flag);

	thrtime.StartTime();
	for(int i = 0;i < round;i++)
	{
		WriteBatch batch(SUBSIZE);

		for(int j = 0;j < SUBSIZE;j++)
		{
			int k = beg + i*SUBSIZE + j;

    		BufferPacket packet(sizeof(int));
	        packet << k;
	        Slice key(packet.getData(),sizeof(int));
	        Slice value(packet.getData(),sizeof(int));
    		batch.put(key, value);	
		}
		printf("%d %d\n", flag, i);
		
		db -> tWrite(&batch);
	}

	sprintf(buf, "Thread %d has been completed, spend time :", flag);
	thrtime.StopTime(buf);

	return NULL;
}

int main()
{
    db = new CustomDB;
    db -> open(option);
    printf("open successful\n");

    int ids[THRNUM];
    ThreadPosix thrs[THRNUM];

    TimeStamp total;
    
    total.StartTime();

    for(int i = 0;i < THRNUM;i++)
    {
    	ids[i] = i;
    	thrs[i] = ThreadPosix(thrFunc, &ids[i]);
    	thrs[i].run();
    }  

    for(int i = 0;i < THRNUM;i++) thrs[i].join();

    total.StopTime("Total PutTime(Thread Version: ");
    
    printf("Begin Check\n");
    
    /*total.StartTime();
    for(int i=SIZE-1;i>=0;i--)
    {
        BufferPacket packet(sizeof(int)); packet << i;
        
        Slice key(packet.getData(), sizeof(int));

        Slice value = db -> get(key);

        BufferPacket packet2(sizeof(int));
        
        packet2 << value; packet2.setBeg();
        
        int num = -1; packet2 >> num;
        if(i!=num)
            cout << i <<" "<<num <<" ) ";
    }
    total.StopTime("GetTime(Without Cache): ");*/
    
    delete db;
}