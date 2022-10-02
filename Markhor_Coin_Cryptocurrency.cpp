#include <iostream> //For input output stream.
#include <string> //For string data types.
#include <ctime> //For Time Manipulation.
#include <vector> //For Dynamic Array data type.
#include <conio.h> //For console commands e.g _getch().
#include <windows.h> //Windows API for custom colored console output streams.
using namespace std;

//Windows API for chainging Console Output Stream's Colour
void setTextColor(int n)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, n); //Sets colour according to the number
}

//Transaction Data
struct TransactionData
{
	//It holds all the necessary transaction data.
	double amount = 0;
	string senderKey, recieverKey;
	time_t timestamp = 0;
};

//Block Class
class Block
{
	/*Block Class holds all the data and manipulation functions required for a block.*/
private:
	//Block Number
	int index;

	//Stores the hashes of the Block.
	size_t blockHash, previousHash;
	
	//Object to Transaction Data struct.
	TransactionData data; 

	//Hash Generator Function
	size_t generateHash();
public:
	//Constructor
	Block(int idx, TransactionData data, size_t prevHash);

	//Hash Validator
	bool isHashValid()
	{
		return generateHash() == getHash();
	}

	//Getter Functions:-
	//Gets Index
	int getIndex()
	{
		return index;
	}

	//Gets Original Hash
	size_t getHash()
	{
		return blockHash;
	}

	//Gets Previous Hash
	size_t getPreviousHash()
	{
		return previousHash;
	}

	//Get Transaction Data
	double getAmount()
	{
		return data.amount;
	}
	string getSenderKey()
	{
		return data.senderKey;
	}
	string getRecieverKey()
	{
		return data.recieverKey;
	}
	time_t getTimestamp()
	{
		return data.timestamp;
	}

	/*Manipulator Function. Gets an un-authorized access to transaction data.
	Only for Blockchain Demonstration!*/
	void hackBlock(TransactionData d)
	{
		data.amount = d.amount;
		data.recieverKey = d.recieverKey;
		data.senderKey = d.senderKey;
		data.timestamp = d.timestamp;
	}

};

//Block Constructor
Block::Block(int idx, TransactionData d, size_t prevHash)
{
	index = idx;
	data = d;
	previousHash = prevHash;
	blockHash = generateHash();
}

//Private Functions
//Hash Generater
size_t Block::generateHash()
{
	// 2 hashes to combine
	hash<string> tranDataHash; // hashes transaction data string
	hash<string> prevHash; // re-hashes previous hash (for combination)

	// Concatenating string of Transaction Data
	string toHash = to_string(data.amount) + data.recieverKey + data.senderKey + to_string(data.timestamp);

	 /*Hashing Algorithim:-
	 Combines Transaction data hash and left shifted re-hashed previous hash by XOR operator
	 And returns size_t for block hash*/
	return tranDataHash(toHash) ^ (prevHash(to_string(previousHash)) << 1);
}

//Blockchain Class
class Blockchain
{
	/*Blockchain Class holds dynamic chain of Blocks with some manipulation functions*/
private:
	/* Main Chain:-
	 Individual Blocks are stored in this Vector Type Dynamic Array to make a chain of Blocks.
	 Hence a Blockchain.*/
	vector<Block> chain;

	//Genesis (First) Block Generator
	Block createGenesisBlock();

public:
	//Blockchain Constructor
	Blockchain();

	//Public Chain Getter
	vector<Block> getChain()
	{
		return chain;
	}

	//Public Functions
	//Adds a New Block to the Blockchain
	void addBlock(TransactionData);

	//Validates Blockchain
	int isChainValid();

	//Prints every Block from the Blockchain
	void printChain();

	/*Friend function for the demonstration of Blockchain Technology*/
	friend void friendlyExploit(Blockchain&);


};

//Blockchain Constructor
Blockchain::Blockchain()
{
	//Genesis Block
	Block genesis = createGenesisBlock();

	//Genesis Block being pushed to Main Blockchain
	chain.push_back(genesis);
}

//Genesis (First) Block Generator
Block Blockchain::createGenesisBlock()
{
	//Get Currnet Time
	time_t current;

	//Setup Genesis Block Transaction Data
	TransactionData d;
	d.amount = 9999999999.00;
	d.recieverKey = "Who is Satoshi Nakamoto?";
	d.senderKey = "Inter-Services Intelligence - Classified";
	d.timestamp = time(&current);

	//Return Genesis Block
	Block genesis(0, d, 0);
	return genesis;
}

//New Block Adder
void Blockchain::addBlock(TransactionData d)
{
	int index = (int)chain.size();
	//Sets Previous Hash
	size_t previousHash = (int)chain.size() > 0 ? chain.back().getHash() : 0;

	//Assigns data to New Block
	Block newBlock(index, d, previousHash);

	//Pushes New Block on to the Blockchain
	chain.push_back(newBlock);
}

//Gets Friendly access to private data and then exploits it
//Ony for Demonstration!!!
void friendlyExploit(Blockchain& exploit)
{
	//Object for recording manipulated data
	TransactionData hackData;
Again:
	//Stores Block number to be exploited
	int idx = 0;
	cin >> idx;

	//Condition for checking buffer overflow
	if (idx >= (int)exploit.chain.size())
	{
		cout << "\nMaximum Chain Size: " << (int)exploit.chain.size() << "\nPlease enter valid index number: ";
		goto Again;
	}

	//Records current time
	time_t nonce;

	//Input data from the user
	cout << "\n\nInitializing New Transaction............!"
		<< "\n\nEnter the amount in Markhor Coins: ";
	cin >> hackData.amount;
	cout << "\nEnter the Sender Key: ";
	//Clears Buffer
	cin.ignore(256, '\n');
	getline(cin, hackData.senderKey);
	cout << "\nEnter the Reciever Key: ";
	getline(cin, hackData.recieverKey);
	hackData.timestamp = time(&nonce);

	cout << "\nApplying Patch to the Blockchain.......!";

	//Pushing manipulated data to Main Blockchain
	exploit.chain[idx].hackBlock(hackData);
	cout << "\nData Successfully Patched!\n\n";
}

//Chain Validator
int Blockchain::isChainValid()
{
	//iterator for vectors
	vector<Block>::iterator i;

	//Loops through whole Blockchain
	for (i = chain.begin(); i != chain.end(); ++i)
	{
		//Pointer to current Block
		Block currentBlock = *i;

		//Condition Checks if hash of the block is valid or not
		if (!currentBlock.isHashValid())
		{
			//Invalid!!!!
			return currentBlock.getIndex();
		}

		//Condition prevents Genesis Block.
		if (i != chain.begin())
		{
			//Pointer to previous Block
			Block previousBlock = *(i - 1);

			//Condition checks if hash is same or not
			if (currentBlock.getPreviousHash() != previousBlock.getHash())
			{
				//Invalid!!!!
				return previousBlock.getIndex();
			}
		}
	}
	return false;
}

//Chain Printer
void Blockchain::printChain()
{
	//iterator for vectors
	vector<Block>::iterator i;

	setTextColor(10); //sets colour to Green

	cout << "\n\nTotal Blocks in Blockchain: " << (int)chain.size();

	//Loop Prints every Block in the Blockchain
	for (i = chain.begin(); i != chain.end(); ++i)
	{
		//Pointer to current Block
		Block currentBlock = *i;

		//Changes text colour if the hash is not valid for a specific block
		currentBlock.isHashValid() == 1 ? (setTextColor(9)) : (setTextColor(192));
	
		cout << "\n\n<<====================Block====================>>"
			<< "\nIndex: " << currentBlock.getIndex()
			<< "\nAmount: " << currentBlock.getAmount()
			<< "\nSender Key: " << currentBlock.getSenderKey()
			<< "\nReciever Key: " << currentBlock.getRecieverKey()
			<< "\nTimestamp (Nonce): " << currentBlock.getTimestamp()
			<< "\nHash: " << currentBlock.getHash()
			<< "\nPrevious Hash: " << currentBlock.getPreviousHash()
			<< "\nIs Block Valid?: "
			<< (currentBlock.isHashValid() == 1 ? "Yes\n\n" : "No\n\n");
	}
	setTextColor(9);
}


int main()
{
	//Initiates Markhor Coin Blockchain
	Blockchain MarkhorCoin;

	//Windows API for chainging Console Output Stream's Colour
	setTextColor(11); //Sets colour to Cyan

Menu:
	setTextColor(13); //Sets colour to Purple
	cout << "Welcome to Royal Markhor Blockchain Explorer!";
	setTextColor(11); //Sets colour to Cyan
	cout << "\nPress '1' to make a new transaction."
		<< "\nPress '2' to check the Validation of the Blockchain."
		<< "\nPress '3' to exploit any block. (Demonstration of 'Proof of Work' for the Blockchain Technology)"
		<< "\nPress '4' to print the whole Blockchain."
		<< "\nPress any other key to exit.\n";

	//Gets input from the user without the need of pressing return.
	char op = _getch();

	//Main Menu Conditions:-
	if (op == '1')
	{
		//Object for recording user data
		TransactionData data;
		//Records data
		time_t nonce;

		setTextColor(14); //Sets colour to Offwhite

		cout << "\n\nInitializing New Transaction!............"
			<< "\n\nEnter the amount in Markhor Coins: ";
		cin >> data.amount;
		cout << "\nEnter the Sender Key: ";

		//Clears Buffer
		cin.ignore(256, '\n');

		//Gets string input with support for white spaces
		getline(cin, data.senderKey);

		cout << "\nEnter the Reciever Key: ";

		//Gets string input with support for white spaces
		getline(cin, data.recieverKey);

		data.timestamp = time(&nonce);

		setTextColor(10); //Sets colour to Green

		cout << "\n\nMining New Block...........!";

		//Pushes New Block to Main Blockchain
		MarkhorCoin.addBlock(data);

		cout << "\nNew Block has been successfully added to the Chain!\n\n";

		//Clears output stream buffer
		system("pause");
		system("CLS");

		setTextColor(11); //Sets colour to Cyan
	}
	else
		if (op == '2') //Chain Validation Checker
		{
			setTextColor(14); //Sets colour to Offwhite

			cout << "\n\nScanning for Invalid Blocks.......!";
			MarkhorCoin.isChainValid() == 0 ? setTextColor(10) : setTextColor(12);
			cout << "\nChain is " << (MarkhorCoin.isChainValid() == 0 ? "Valid! No Invalid Blocks were found!\n\n" : "Not Valid!");

			if (MarkhorCoin.isChainValid() != 0)
			{
				cout << "\n\nBlock with Index number " << MarkhorCoin.isChainValid() << " is corrupted and no longer valid!\n\n";
			}

			//Clears output stream buffer
			system("pause");
			system("CLS");

			setTextColor(11); //Sets colour to Cyan
		}
		else
			if (op == '3') //Demostration for Blockchain
			{
				/*This option is ony for demonstration purposes and not really a part of Blockchain Technology*/

				setTextColor(4); //Sets colour to Red

				//Outputs Warning Message for Unauthorised Access!
				cout << "\n\n!!******************!! Warning !!******************!!"
					<< "\n\nYou have been detected for requesting a Restricted Access!"
					<< "\nOnly proceed if you are an Authorised person!\n";
				system("pause");
				cout << "\n\nAccess Breached Successfully....! (^U^)"
					<< "\nInitializing Stable Connection...............!!"
					<< "\n\nWhich Block you want to attack?"
					<< "\nEnter index number: ";

				friendlyExploit(MarkhorCoin); //Calls Exploit Function

				//Clears output stream buffer
				system("pause");
				system("CLS");

				setTextColor(11); //Sets colour to Cyan
			}
			else
				if (op == '4') //Chain Printer
				{
					setTextColor(9); //Sets colour to Blue

					//Prints Blockchain Ledger
					MarkhorCoin.printChain();

					//Clears output stream buffer
					system("pause");
					system("CLS");

					setTextColor(11); //Sets colour to Cyan
				}
				else
				{
					//Exits after successful run of program
					return 0;
				}

				//Returns to Main Menu
				goto Menu;
}