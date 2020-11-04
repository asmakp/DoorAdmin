#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
# include<string.h> 
#include<stdbool.h> 
#include<ctype.h> 
#include<time.h> 
#include<stdio.h> 
#include "serial.h"
#include "safeinput.h"

typedef struct
{
	char name[50];
	bool access;
	time_t DateTime;
} CARD;

typedef struct
{
	CARD* cardlist;
	int NumberOfCards;
	SERIALPORT port;
}SYSTEM_STATE;

void AddCard(SYSTEM_STATE* state,char* CardNo)
{
	printf("NEW CARD ENTRY\n");
	int indexForTheNewOne;
	if (state->NumberOfCards == 0)
	{
		state->cardlist = malloc(sizeof(CARD));
		state->NumberOfCards = 1;
		indexForTheNewOne = 0; //state ->cardlist[0] will have first entry
	}
	else
	{
		state->cardlist = realloc(state->cardlist,
			sizeof(CARD) * (state->NumberOfCards + 1));
		indexForTheNewOne = state->NumberOfCards;
		state->NumberOfCards++;
	}
	//GetInput("ENTER CARD NUMBER: ", state->cardlist[indexForTheNewOne].name,
	//	sizeof(state->cardlist[indexForTheNewOne].name));
	sprintf_s(state->cardlist[indexForTheNewOne].name, sizeof(state->cardlist[indexForTheNewOne].name), CardNo);
	int Gaccess;
	GetInputInt("Enter 1 For access.  0 For no access: ", &Gaccess);
	if (Gaccess)
	{
		state->cardlist[indexForTheNewOne].access = true;
	}
	else
	{
		state->cardlist[indexForTheNewOne].access = false;
	}
	
	state->cardlist[indexForTheNewOne].DateTime = time(NULL);
	printf("Press key to continue");
	getch(); //getch() method pauses the Output Console untill a key is pressed.
}

void AddtoArduino(SYSTEM_STATE* state)
{
	char str[50];
	char list[50];
	strncpy(list, "ADD", 10);

	if (state->NumberOfCards == 0)
	{
		printf("\nLIST IS EMPTY\n");
	}
	else
	{
		
		for (int i = 0; i < state->NumberOfCards; i++)
		{
			CARD c = state->cardlist[i];
			if (c.access == true)
			{
				strncpy(str, c.name, 10);
				strncat(list, ",", 10);
				strncat(list, str, 10);

			}

		}
		
		//printf(list);
		SerialWritePort(state->port, list, strlen(list));
		printf("\nList Added to Arduino\n");

	}


}

void AddRemoveAccess(SYSTEM_STATE* state)
{
	char CardNo[10];
	GetInput("Enter your card number:>", CardNo, 10);
	if (state->NumberOfCards == 0)						// To check if the cardlist is empty
	{
		AddCard(state, CardNo);				// If cardlist is empty create an account for the entered card number.
	}
	else
	{

		for (int i = 0; i < state->NumberOfCards; i++)
		{
			CARD c = state->cardlist[i];

			if (strcmp(c.name, CardNo) == 0)  
			{
				if (c.access == true)
				{
					printf("This Card has access to system\n ");
				}
				else
				{
					printf("This Card has no access to system\n");
				}

				int choice;
				GetInputInt("Enter 1 for access \t 2 for no access: ", &choice);
				if (choice == 1)
				{
					
					state->cardlist[i].access = true;  //Granting/changing access to the already existing card.
					printf("The access to this card is now granted\n");
				}
				else
				{
					
					state->cardlist[i].access = false;  // Removing the card access
					printf("\nThis cards access has been removed\n");
					
				}
				AddtoArduino(state); // once card's access is changed send updated  card list to arduino.
				return 0;
			}

			
		}// if card not in list
		AddCard(state,CardNo);


	}
}

void ListOfCards(SYSTEM_STATE* state)
{
	
	if (state->NumberOfCards == 0)
	{
		printf("\nLIST IS EMPTY\n");
	}
	else
	{
		char str[50];
		printf("\nLIST ALL CARDS\n");
		for (int i = 0; i < state->NumberOfCards; i++)
		{
			CARD c = state->cardlist[i];
			if (c.access == true)
			{
				struct tm* now_t = localtime(&(c.DateTime));
				strftime(str, 50, "%d-%m-%Y %H:%M:%S", now_t);
				printf("%s\tACCESS TO SYSTEM\t%s\n", c.name, str);

			}
			else
			{
				struct tm* now_t = localtime(&(c.DateTime));
				strftime(str, 50, "%d-%m-%Y %H:%M:%S", now_t);
				printf("%s\tNO ACCESS TO SYSTEM\t%s\n", c.name, str);
			}
		}
	}
	printf("\nPress key to continue");
	getch();
}


void Fake_test(SYSTEM_STATE* state)
{
	char CardNo[20];
	char str[] = "SCAN";
	char list[25];
	
	printf("\nPlease scan card to enter\n");
	printf("Press x to go back to admin mode\n");
	GetInput("\nEnter your card number:", CardNo, 10);
	strncpy(list, str, 25);
	strncat(list, ",", 10);
	strncat(list, CardNo, 30);
	Sleep(10);
	
	SerialWritePort(state ->port, list, strlen(list));
	
	if ((strcmp(CardNo, "X") == 0) || (strcmp(CardNo, "x") == 0))
	{
		return 0;
	}
	else
	{
		
		for (int i = 0; i < state->NumberOfCards; i++)
		{
			CARD c = state->cardlist[i];
			if (strcmp(c.name, CardNo) == 0 && (c.access==true))
			{
				printf("\nCURRENTLY THE LAMP IS GREEN\n");
				return 0;
			}

		}
		printf("\nCURRENTLY THE LAMP IS RED\n");
		
		
	}
	
	printf("\nPress key to continue");
	getch();
	
}

void RemoteOpenDoor(SYSTEM_STATE* state)
{
	char str[] = "REMOTE";
	
	SerialWritePort(state->port, str, strlen(str));
	
	printf("CURRENTLY LAMP IS: GREEN\n");
	printf("Press key to continue\n");
	
	getch();
	
}
void Huvudmeny(SYSTEM_STATE* state)
{
	while (true)
	{	
		printf("\n***********************\n");
		printf(" ***ADMIN MENU***\n");
		printf("1. Remote open door\n2. List all cards in system\n3. Add/Remove access\n4. Add List to Arduino\n5. Exit\n9. FAKE TEST SCAN CARD\n");
		int choice;
		if (GetInputInt("Enter your choice:>", &choice) == false)
			continue;
		switch (choice)
		{
		case 1:
			RemoteOpenDoor(state);
			break;
		case 2:
			ListOfCards(state);
			break;
		case 3:
			AddRemoveAccess(state);
			break;
		case 4:
			AddtoArduino(state);
			break;
		case 5:
			return 0;    // if break is used it just comes out of switch case and prints the menu again.
		case 9:
			Fake_test(state);
			break;
		}

	}

}
void main()
{
	
	SYSTEM_STATE state;
	state.cardlist = NULL;
	state.NumberOfCards = 0;
	state.port = SerialInit("\\\\.\\COM3");
	
	//time_t now = time(NULL);
	//struct tm* now_t = localtime(&now);
	if (!SerialIsConnected(state.port))
	{
		printf("port is not connected");
		return;
	}


	Huvudmeny(&state);

	
	SerialClose(state.port);
	return 0;
	
}


