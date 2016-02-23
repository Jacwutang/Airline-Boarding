#include <iostream>
#include <fstream>
#include "StackAr.h"
#include "QueueAr.h"
using namespace std;

enum aisleStatus { empty, transit, luggage, swapp };
int boardingCount = 0;
int timer = 0;

class Passenger
{
public:

	Passenger() {}

	Passenger(int x, char y) : targetNumber(x), seatLetter(y) {}

	//Passenger& operator= (const Passenger& p);

	int targetNumber;
	char seatLetter;

}; // Passenger class

class Row
{
public:

	Passenger pass;
	int rowNumber;
	int swapCount;
	aisleStatus status;
	StackAr  <char> left;
	StackAr  <char> right;
	StackAr  <char> temp;

	Row() {};
	

	Row(int i) : left(3), right(3), temp(3), swapCount(0), status(::empty)
	{
		rowNumber = i;
		pass.targetNumber = 0;
		pass.seatLetter = ' ';
	
	} // default constructor
};

void changeEnum(Row& row, Row& prow, ifstream& inf) 
{
	char letter;
	int number;

	
	if (row.status == swapp)
	{
		row.swapCount--;
		if (row.swapCount == 0)
		{
			row.status = :: empty;
		
		}
	}//swap

	if (row.status == luggage)
	{
		row.status = swapp;
		row.swapCount = 0;
		if (row.pass.seatLetter < 'D')
		{
			while ((row.left.isEmpty() != true) && (row.pass.seatLetter < row.left.top()))
			{
				row.temp.push(row.left.topAndPop());
				row.swapCount += 2;
			}
			row.swapCount++;
			while (row.temp.isEmpty() == false)
			{
				row.left.push(row.temp.topAndPop());
			}
			row.left.push(row.pass.seatLetter);
		} // left aisle
		else if (row.pass.seatLetter > 'C')
		{
			while ((row.right.isEmpty() != true) && (row.pass.seatLetter > row.right.top()))
			{
				row.temp.push(row.right.topAndPop());
				row.swapCount += 2;
			}
			row.swapCount++;
			while (row.temp.isEmpty() == false)
			{
				row.right.push(row.temp.topAndPop());
			}
			row.right.push(row.pass.seatLetter);
		} // right aisle
		row.pass.targetNumber = 0;
		row.pass.seatLetter = ' ';
	} // luggage case

	if (row.status == transit)
	{
		if (row.pass.targetNumber == row.rowNumber)
		{
			row.status = luggage;
		}
	}
	if (row.status == :: empty) // regular empty
	{
		if ((prow.status == transit) && (prow.pass.targetNumber > prow.rowNumber))
		{
			row.status = transit;
			row.pass.targetNumber = prow.pass.targetNumber;
			row.pass.seatLetter = prow.pass.seatLetter;
			prow.status = :: empty;
			prow.pass.targetNumber = 0;
			prow.pass.seatLetter = ' ';
		}
	} // regular empty case
	  ////////////////////////////////////////////////////////////////////////////////////
	if (prow.rowNumber == 1)
	{
		if (prow.status == swapp)
		{
			prow.swapCount = prow.swapCount - 1;
			if (prow.swapCount == 0)
			{
				prow.status = :: empty;
			
			}
		} // swapp

		if (prow.status == luggage)
		{
			prow.status = swapp;
			prow.swapCount = 0;
			if (prow.pass.seatLetter < 'D')
			{
				while ((prow.left.isEmpty() == false) && (prow.pass.seatLetter < prow.left.top()))
				{
					prow.temp.push(prow.left.topAndPop());
					prow.swapCount = prow.swapCount + 2;
				}
				prow.swapCount = prow.swapCount + 2;
				while (prow.temp.isEmpty() == false)
				{
					prow.left.push(prow.temp.topAndPop());
				}
				prow.left.push(prow.pass.seatLetter);
			} // left aisle special case 1
			else if (prow.pass.seatLetter > 'C')
			{
				while ((prow.right.isEmpty() == false) && (prow.pass.seatLetter > prow.right.top()))
				{
					prow.temp.push(prow.right.topAndPop());
					prow.swapCount = prow.swapCount + 2;
				}
				prow.swapCount = prow.swapCount + 2;
				while (prow.temp.isEmpty() == false)
				{
					prow.right.push(prow.temp.topAndPop());
				}
				prow.right.push(prow.pass.seatLetter);
			}
			prow.pass.targetNumber = 0;
			prow.pass.seatLetter = ' ';
		} // luggage

		if ((prow.status == transit) && (prow.pass.targetNumber == prow.rowNumber))
		{
			prow.status = luggage;
		}	//Passenger at correct Row.

		if ((prow.status == :: empty) && (boardingCount < 288))
		{
			inf >> prow.pass.targetNumber;
			inf >> prow.pass.seatLetter;
			prow.status = transit;
			boardingCount++;
		}
	} 
}// change Enum function


int main(int argc, char** argv)
{
	int number, emptyCounter, timeCounter;
	char letter;
	Row previous, current;

	emptyCounter = 0;
	timeCounter = 0;

	Queue <Row> planeQueue(3);
	ifstream inf("passengers18.txt");
	
	//ifstream inf(argv[1]);

	for (int j = 0; j<3; j++)
	{
		planeQueue.makeEmpty();
		for (int i = 3; i >= 1; i--)
		{
			Row newrow = Row(i);
			planeQueue.enqueue(newrow);
		} // all rows have been created

		emptyCounter = 0;
		timeCounter = 0;
		boardingCount = 0;
		
		while (emptyCounter != 3)
		{
			
			emptyCounter = 0;
			current = planeQueue.dequeue(); // row 48
			for (int k = 1; k<3; k++) // happens 47 times, row 1 is special
			{
				previous = planeQueue.dequeue();
				changeEnum(current, previous, inf);
				if (current.status == :: empty)
					emptyCounter++;
				//cout << previous.status;
				planeQueue.enqueue(current);
				if (previous.rowNumber == 1)
				{
					if (previous.status == :: empty)
						emptyCounter++;
					planeQueue.enqueue(previous);
				}
				else
				{
					current = previous;
				}
			}
			timeCounter += 5;
		}
		switch (j)
		{
		case 0: cout << "Back to front: "; break;
		case 1: cout << "Random: "; break;
		case 2: cout << "Outside in: "; break;
		}
		cout << timeCounter << endl;
	}
	system("pause");
	return 0;
}

