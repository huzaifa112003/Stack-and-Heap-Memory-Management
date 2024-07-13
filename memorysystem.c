#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MEMSIZE 500
#define STACK_INITIAL_SIZE 105
#define STACK_MAX_SIZE 199  // 499-300 = 199
#define STACKFRAMES_STARTING_INDEX 394 // 499-105 = 394
#define FRAME_STATUS_SIZE 21
#define MAX_NO_FRAMES 5
#define FRAME_MIN_SIZE 10
#define FRAME_MAX_SIZE 80
#define HEAP_STARTING_INDEX 0
#define HEAP_MAX_SIZE 299
#define RANDOM_CHAR_SET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

struct framestatus {
	int number;               // frame number
    char name[8];             // function funcname representing the frame
	int functionaddress;      // address of function in code section (will be randomly generated in this case)
	int frameaddress;         // starting address of frame belonging to this header in Stack
	char used;                 // a boolean value indicating wheter the frame status entry is in use or not
};

struct freelist {
	int start;         // start address of free region
	int size;                 // size of free region
	struct freelist * next;   // pointer to next free region
};

struct allocated {
      char name[8];
      int startaddress;

};

void create_frame(struct framestatus framestatuslist[], int * stackPointer, int * heapPointer, char * funcname, int address) {
	
	if(strlen(funcname) > 8 || strlen(funcname) < 1){ //condition for checking the length of the function name
		printf("Incorrect funcname.\n");
		return;
	}

	if(*stackPointer - FRAME_MIN_SIZE < STACK_MAX_SIZE || *stackPointer - FRAME_MIN_SIZE <= *heapPointer){ //conditon for checking stack and heap pointer values
		printf("Stack Overflow, Not enough memory availaible for new function.\n");
		return;
	}

	int frame_found = 0; //int value to check if a frame is found or not

	for(int i = 0; i < MAX_NO_FRAMES; i++){ //loop to check if a frame with the same name already exists
		if(strcmp(framestatuslist[i].name, funcname) == 0){
			printf("Function with given name already exists!\n");
			return;
		}

		if(framestatuslist[i].used == '0'){ //if a frame is not in use, it is used to create a new frame
			frame_found = 1;
			framestatuslist[i].number = i+1;
			framestatuslist[i].used = '1';
			framestatuslist[i].frameaddress = *stackPointer;
			strcpy(framestatuslist[i].name, funcname);
			break;
		}
	}

	if(frame_found == 0){ //if no frame is found, it means that the maximum number of frames have been created
		printf("Cannot create another frame, maximum number of frames have reached \n");
		return;
	}
	
	*stackPointer -= FRAME_MIN_SIZE; // stack pointer is decremented by 10 bytes because a new frame is created and stack grows downwards
}


void delete_function(struct framestatus framestatuslist[], int * stackPointer) {
	
	if(*stackPointer == STACKFRAMES_STARTING_INDEX){ //condition to check if the stack is empty
		printf("Stack is empty. No function to delete.\n");
		return;
	}

    int i = -1; //i = - 1 as this will ensure it does not go out of range

	while(i < MAX_NO_FRAMES-1){ //loop to find the topmost frame in the stack
		if (framestatuslist[i+1].used == '1'){
			i += 1;
		}
		else{
			break;
		}
	}

	if(i == -1){ //if i = -1, it means that no frame is in use
		printf("The stack is empty!\n");
		return;
		}
		
	else{
		framestatuslist[i].used = '0'; 
		*stackPointer = framestatuslist[i].frameaddress; //stack pointer is set to the address of the frame that is deleted
		}
}

void create_integer(struct framestatus framestatuslist[], int * stackPointer, int * heapPointer, char memory[], char * intname, int intvalue) {
	
	if(strlen(intname) < 1){ //condition for checking the length of the integer name
		printf("Incorrect name.\n");
		return;
	}
	
	if(*stackPointer - sizeof(int) < STACK_MAX_SIZE || *stackPointer - sizeof(int) <= *heapPointer){ //conditon for checking stack and heap pointer values
		printf("Stack Overflow, Not enough memory availaible for new integer.\n");
		return;
	}

	int i = -1; //i = - 1 as this will ensure it does not go out of range

	while(i < MAX_NO_FRAMES-1){	//loop to find the topmost frame in the stack
		if (framestatuslist[i+1].used == '1'){
			i += 1;
		}
		else{
			break;
		}
	}

	if (i != -1) { // framestatuslist[i] points to the topmost used framestatus
    // Perform operations on this framestatus
		int frame_size = (framestatuslist[i].frameaddress - *stackPointer) + sizeof(int);
		if(frame_size > FRAME_MAX_SIZE){
			printf("The frame is full cannot create more data on it.\n");
			return;
		}
		//All checks made, no we can create an integer
		memcpy(&memory[*stackPointer], &intvalue, sizeof(int)); //copying the integer value to the memory
		*stackPointer -= sizeof(int);// stack pointer is decremented by 4 bytes because a new integer is created and stack grows downwards
	}

	else { // No frame is currently in use
		printf("The stack is empty!\n");
	}
}

void create_double(struct framestatus framestatuslist[], int * stackPointer, int * heapPointer, char memory[], char * doublename, double doublevalue) {
	
	if(strlen(doublename) < 1){ //condition for checking the length of the double name
		printf("Incorrect name.\n");
		return;
	}
	
	if(*stackPointer - sizeof(double) < STACK_MAX_SIZE || *stackPointer - sizeof(double) <= *heapPointer){ //conditon for checking stack and heap pointer values
		printf("Stack Overflow, Not enough memory availaible for new integer.\n");
		return;
	}

	int i = -1; //i = - 1 as this will ensure it does not go out of range

	while(i < MAX_NO_FRAMES-1){ //loop to find the topmost frame in the stack
		if (framestatuslist[i+1].used == '1'){
			i += 1;
		}
		else{
			break;
		}
	}

	if (i != -1) { // framestatuslist[i] points to the topmost used framestatus
    // Perform operations on this framestatus
		int frame_size = (framestatuslist[i].frameaddress - *stackPointer) + sizeof(double);
		if(frame_size > FRAME_MAX_SIZE){
			printf("The frame is full cannot create more data on it.\n");
			return;
		}
		//All checks have been made, no we can create a double
		memcpy(&memory[*stackPointer], &doublevalue, sizeof(double)); //copying the double value to the memory
		*stackPointer -= sizeof(double); // stack pointer is decremented by 8 bytes because a new double is created and stack grows downwards
	}

	else { // No frame is currently in use
		printf("The stack is empty!\n");
	}
}

void create_char(struct framestatus framestatuslist[], int * stackPointer, int * heapPointer, char memory[], char * charname, char charvalue) {
	
	if(strlen(charname) < 1){ //condition for checking the length of the char name
		printf("Incorrect name.\n");
		return;
	}
	
	if(*stackPointer - sizeof(char) < STACK_MAX_SIZE || *stackPointer - sizeof(char) <= *heapPointer){ //conditon for checking stack and heap pointer values
		printf("Stack Overflow, Not enough memory availaible for new integer.\n");
		return;
	}

	int i = -1; //i = - 1 as this will ensure it does not go out of range

	while(i < MAX_NO_FRAMES-1){ //loop to find the topmost frame in the stack
		if (framestatuslist[i+1].used == '1'){
			i += 1;
		}
		else{
			break;
		}
	}

	if (i != -1) { // framestatuslist[i] points to the topmost used framestatus
    // Perform operations on this framestatus
		int frame_size = (framestatuslist[i].frameaddress - *stackPointer) + sizeof(char);
		if(frame_size > FRAME_MAX_SIZE){
			printf("The frame is full cannot create more data on it.\n");
			return;
		}
		//All checks done, no we can create a double
		memcpy(&memory[*stackPointer], &charvalue, sizeof(char));//copying the char value to the memory
		*stackPointer -= sizeof(char);//stack pointer is decremented by 1 byte because a new char is created and stack grows downwards
	}

	else { // No frame is currently in use
		printf("The stack is empty!\n");
	}
}

void create_buffer(char memory[], char *name, char *bufferSizeStr, int *heapPointer, int *stackPointer) {
    int bufferSize = atoi(bufferSizeStr); // Convert buffer size string to integer

	if(bufferSize <= 0){ //condition for checking the length of the buffer size
		printf("Invalid buffer size.\n");
		return;
	}

    if (bufferSize > HEAP_MAX_SIZE - *heapPointer) {//condition for checking if the buffer size is greater than the heap size
        printf("The heap is full, cannot create more data.\n");
        return;
    }

    // Check if buffer allocation overlaps with the stack
    if (*heapPointer + bufferSize + 8 > *stackPointer) {
        printf("Buffer allocation would overlap with stack. Allocation failed.\n");
        return;
    }

    // Filling the buffer with random characters
    for (int idx = *heapPointer; idx < *heapPointer + bufferSize; ++idx) {
        int randIndex = rand() % (sizeof(RANDOM_CHAR_SET) - 1); // -1 for null-terminator
        memory[idx] = RANDOM_CHAR_SET[randIndex];
    }

    // Generating a random number and converting it to string
    int randomNum = 1000 + (rand() % 9000);
    char randomNumberStr[5];
    sprintf(randomNumberStr, "%04d", randomNum); // Ensures 4 digits

    // Storing the random number and buffer size in memory
    memcpy(memory + *heapPointer + bufferSize, randomNumberStr, 4);
    memcpy(memory + *heapPointer + bufferSize + 4, bufferSizeStr, 4);

    // Update heap size
    *heapPointer += (bufferSize + 8);

	// Update the stack pointer
    *stackPointer -= sizeof(char*);

    // Store the starting address of the buffer on the stack
    char **stackAddress = (char**)(memory + *stackPointer);
    *stackAddress = memory + *heapPointer - bufferSize;
}


void deal_buffer(char memory[], char *name, int *heapPointer, int *stackPointer) {
    // Check if any buffers exist that can be deallocated
    if (*stackPointer == STACK_MAX_SIZE) {
        printf("There are no buffers to deallocate\n");
        return;
    }

    // From the stack retrieve the address of the last allocated buffer
    char **stackAddress = (char**)(memory + *stackPointer);
    char *bufferStart = *stackAddress;// Retrieve the starting address of the buffer

    // Check if the pointer is invalid or the buffer was already de-allocated
    if (bufferStart == NULL || bufferStart < memory || bufferStart >= memory + *heapPointer) {
        printf("The pointer is NULL or already de-allocated.\n");
        return;
    }

    // From the information stored after the buffer calculate the buffer size
    char sizeStr[5];
    memcpy(sizeStr, bufferStart + (bufferStart - memory - *heapPointer), 4);
    sizeStr[4] = '\0'; // Null-terminate the string
    int bufferSize = atoi(sizeStr);

    // Replace all the data in the deallocated region with zeros / optional
    for (int i = 0; i < bufferSize + 8; ++i) { 
        bufferStart[i] = '\0';
    }

    // Update heap pointer to remove the deallocated buffer
    *heapPointer -= (bufferSize + 8);

    // Update stack pointer to remove the address of the deallocated buffer
    *stackPointer += sizeof(char*);

    // Clear the stack address for safety to avoid dangling pointers / optional
    *stackAddress = NULL;
}

void show_memory(char memory[], int *stackPointer, int *heapPointer) {
    printf("Heap Snapshot (from index %d to %d):\n", HEAP_STARTING_INDEX, *heapPointer);
    for (int i = HEAP_STARTING_INDEX; i < *heapPointer; i++) {//loop to print the heap snapshot
        printf("%c", memory[i]);
        if ((i + 1) % 50 == 0 || i == *heapPointer - 1) {
            printf("\n"); // New line for better readability
        }
    }

    printf("\nStack Snapshot (from index %d to %d):\n", *stackPointer, STACKFRAMES_STARTING_INDEX);
    for (int i = *stackPointer; i < STACKFRAMES_STARTING_INDEX; i++) {//loop to print the stack snapshot
        printf("%02x ", memory[i] & 0xFF); // Print each byte in hex format
        if ((i + 1) % 10 == 0 || i == STACKFRAMES_STARTING_INDEX - 1) {
            printf("\n"); // New line after every 10 bytes for readability
        }
    }
}


int main () {

	char memory[MEMSIZE]; 
	int stackPointer = STACKFRAMES_STARTING_INDEX; // stack pointer Initializes to the top of the stack where frames start
	int heapPointer = HEAP_STARTING_INDEX; // heap pointer Initializes to the bottom of the heap

    struct framestatus framestatuslist[MAX_NO_FRAMES]; // Array of framestatus structs

    // Initialize framestatus array
    for (int i = 0; i < MAX_NO_FRAMES; i++) {
        framestatuslist[i].number = -1; 
        strcpy(framestatuslist[i].name, "N/A");
        framestatuslist[i].functionaddress = 0;
        framestatuslist[i].frameaddress = 0;
        framestatuslist[i].used = '0'; // Indicates not in use
    }

	while (1) { // Loop to keep asking for commands until the user enters "exit"
        char input[100];
        printf("Enter a command: ");
        fgets(input, sizeof(input), stdin);

        // Remove trailing newline character from fgets
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Tokenize the input to get the command and arguments
        char *token = strtok(input, " ");
        if (token == NULL) {
            continue;  // Empty input, ask for a new command
        }

        // Execute the command based on the input
        if (strcmp(token, "CF") == 0) { // Create frame
            char *funct_name = strtok(NULL, " ");
            int funct_address = strtol(strtok(NULL, " "), NULL, 16);
			create_frame(framestatuslist, &stackPointer, &heapPointer, funct_name, funct_address);
        } else if (strcmp(token, "DF") == 0) { // Delete frame
            delete_function(framestatuslist, &stackPointer);
        } else if (strcmp(token, "CI") == 0) {// Create integer
            char *int_name = strtok(NULL, " ");
            int int_value = atoi(strtok(NULL, " "));
            create_integer(framestatuslist, &stackPointer, &heapPointer, memory, int_name, int_value);
        } else if (strcmp(token, "CD") == 0) { // Create double
            char *double_name = strtok(NULL, " ");
            double double_value = atof(strtok(NULL, " "));
            create_double(framestatuslist, &stackPointer, &heapPointer, memory, double_name, double_value);
        } else if (strcmp(token, "CC") == 0) { // Create char
            char *char_name = strtok(NULL, " ");
            char char_value = strtok(NULL, " ")[0];
            create_char(framestatuslist, &stackPointer, &heapPointer, memory, char_name, char_value);
        } else if (strcmp(token, "CH") == 0) { 	// Create buffer
            char *buffer_name = strtok(NULL, " ");
            char *sizeofbuffer = strtok(NULL, " ");
            create_buffer(memory, buffer_name, sizeofbuffer, &heapPointer, &stackPointer);
        } else if (strcmp(token, "DH") == 0) { // Delete buffer
            char *buffername = strtok(NULL, " ");
            deal_buffer(memory, buffername, &heapPointer,&stackPointer);
        } else if (strcmp(token, "SM") == 0) { //	Show memory
    		show_memory(memory, &stackPointer, &heapPointer);
        } else if (strcmp(token, "exit") == 0) {
            break;  // Exit the loop and end the program
        } else {
            printf("Unknown command: %s\n", token);
        }
    }
	return 0;
}
