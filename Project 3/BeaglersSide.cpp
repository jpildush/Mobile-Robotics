#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <termios.h>

//BB-UART Serial Information
#define PORT "/dev/ttyO4"
#define BAUDRATE B115200

//static std::mutex guard;
enum command {forw = 1,back = 2,left = 3,right = 4,armu = 5, armd = 6,clawo = 7,clawc = 8,agentSleep = 9};
/*
struct TxPacket
{
	unsigned Forward : 1;
	unsigned Reverse: 1;
	unsigned Left : 1;
	unsigned Right : 1;
	unsigned ArmUp : 1;
	unsigned ArmDn : 1;
	unsigned GripOpen : 1;
	unsigned GripClose : 1;
	unsigned Sleep : 1;
};

static TxPacket TxPck;

void resetPckt() {
	TxPck.Forward = 0;
	TxPck.Reverse = 0;
	TxPck.Left = 0;
	TxPck.Right = 0;
	TxPck.ArmUp = 0;
	TxPck.ArmDn = 0;
	TxPck.GripOpen = 0;
	TxPck.GripClose = 0;
	TxPck.Sleep = 0;
}

class UART
{
public:
	UART() {
		resetPckt();
	}
	void readUART(int &uart, void *buffer, size_t maxSize,int sonicSensor) {
		if (uart != -1){
			int temp = 0;
			while (true){
				int rx_length = read(uart, buffer, maxSize);
				if (temp < 2)
					sonicSensor += (int)buffer & 0xFF;
				else if(temp < 4){

				}

				temp++;
				if (rx_length == 0) {
					std::cout << "ERROR:  An error occurred during the UART read" << std::endl;
					break;
				}
			}
		}
	}
	void sendUART(int &uart,int comm) {
		resetPckt();
		char* buffer;
		buffer = new char[1];
		size_t maxSize = 1;
		TxPacket* TxPtr = &TxPck;
		if (uart != -1) {
			switch (comm) {
			case forw:
				buffer = "w";
				TxPtr->Forward = 1;
				break;
			case back:
				buffer = "s";
				TxPtr->Reverse = 1;
				break;
			case left:
				buffer = "a";
				TxPtr->Left = 1;
				break;
			case right:
				buffer = "d";
				TxPtr->Right = 1;
				break;
			case armu:
				buffer = "i";
				TxPtr->ArmUp = 1;
				break;
			case armd:
				buffer = "k";
				TxPtr->ArmDn = 1;
				break;
			case clawo:
				buffer = "o";
				TxPtr->GripOpen = 1;
				std::cout << "bit set" << std::endl;
				break;
			case clawc:
				buffer = "p";
				TxPtr->GripClose = 1;
				break;
			case agentSleep:
				buffer = "z";
				TxPtr->Sleep = 1;
				break;
			default:
				break;
			}
			buffer = (char*)&TxPtr;
			int countSend = write(uart,buffer,maxSize);
			if (countSend < 0)	std::cout << "Failed too write!" << std::endl;
			else std::cout << "success" << std::endl<<std::endl;
		}
	}
};
*/
//Configure the uart for communication 
void configUART(int uart) {
	struct termios options;
	tcgetattr(uart, &options);
	options.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;//sets baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart, TCIFLUSH);
	tcsetattr(uart, TCSANOW, &options);
}
//Display list of commands
void showOptions(int uart_stream) {
	char option;
	int set = -1;
	char* sendOpt;
	sendOpt = new char[1];
	std::cout << "================================" << std::endl;
	std::cout << "||Move Forwards: W" << std::endl;
	std::cout << "||Move Backwards: S" << std::endl;
	std::cout << "||Move Left: A" << std::endl;
	std::cout << "||Swing Left: Q" << std::endl;
	std::cout << "||Move Right: D" << std::endl;
	std::cout << "||Swing Right: E" << std::endl;
	std::cout << "||Arm Up-Pos1: I" << std::endl;
	std::cout << "||Arm Up-Pos2: J" << std::endl;
	std::cout << "||Arm Down-Pos1: K" << std::endl;
	std::cout << "||Arm Down-Pos2: L" << std::endl;
	std::cout << "||Claw Open: O" << std::endl;
	std::cout << "||Claw Close: P" << std::endl;
	std::cout << "||Sleep: Z" << std::endl;
	std::cout << "||------------------------------" << std::endl;
	std::cout << "||Please choose an option: ";
	option = std::cin.get();
	switch (option){
	case 'w':
		sendOpt = (char*)"w";
		std::cout << "chosen w" << std::endl;
		set = 1;
		break;
	case 's':
		sendOpt = (char*)"s";
		std::cout << "chosen s" << std::endl;
		set = 1;
		break;
	case 'a':
		sendOpt = (char*)"a";
		std::cout << "chosen a" << std::endl;
		set = 1;
		break;
	case 'q':
		sendOpt = (char*)"q";
		std::cout << "chosen q" << std::endl;
		set = 1;
		break;
	case 'd':
		sendOpt = (char*)"d";
		std::cout << "chosen d" << std::endl;
		set = 1;
		break;
	case 'e':
		sendOpt = (char*)"e";
		std::cout << "chosen e" << std::endl;
		set = 1;
		break;
	case 'i':
		sendOpt = (char*)"i";
		std::cout << "chosen i" << std::endl;
		set = 1;
		break;
	case 'j':
		sendOpt = (char*)"j";
		std::cout << "chosen j" << std::endl;
		set = 1;
		break;
	case 'k':
		sendOpt = (char*)"k";
		std::cout << "chosen k" << std::endl;
		set = 1;
		break;
	case 'l':
		sendOpt = (char*)"l";
		std::cout << "chosen l" << std::endl;
		set = 1;
		break;
	case 'o':
		sendOpt = (char*)"o";
		std::cout << "chosen o" << std::endl;
		set = 1;
		break;
	case 'p':
		sendOpt = (char*)"p";
		std::cout << "chosen p" << std::endl;
		set = 1;
		break;
	case 'z':
		sendOpt = (char*)"z";
		std::cout << "chosen z" << std::endl;
		set = 1;
		break;
	default:
		break;
	}
	//If one of the options above have been chosen, set will be 1 and the command will be sent to the agent
	if (set == 1) {
		std::cout << std::endl << sendOpt << std::endl << std::endl;
		int countSend = write(uart_stream, sendOpt, sizeof(char));
		if (countSend < 0)	std::cout << "Failed too write!" << std::endl;
		else std::cout << "success" << std::endl << std::endl;
		usleep(1000000);
	}
	else std::cout << "Option not selected!" << std::endl << std::endl;
}
 
int main() {
	//UART uartPtr;
	int uart_stream = -1;
	uart_stream = open(PORT, O_RDWR);
	if (uart_stream == -1){
		std::cout << "Failed to open BB-UART" << std::endl;
		return -1;
	}
	std::cout << "BB-UART Opened" << std::endl;
	//Calls to configure the uart connection
	configUART(uart_stream);
	//Begin displaying the options
	while (true) {
		showOptions(uart_stream);
	}

	return -1;
}