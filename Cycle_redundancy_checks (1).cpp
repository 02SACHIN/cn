#include <iostream>

void XOR(int* arr1, int* arr2, int size) //This function performs XOR operation on the element of two arrays.
{
    for(int i = 0; i < size; i++)
    {
        arr1[i] = arr1[i] ^ arr2[i];
    }
}

int shift_remainder(int* message, int* remainder, int remainder_size, int pointer)
{
    for(int i = 0; i < remainder_size - 1; i++) //shift the array to the left
    {
        remainder[i] = remainder[i+1]; 
    }

    remainder[remainder_size - 1] = message[pointer]; // The remainder lsb will be next bit pointed by the pointer.
    pointer++; //return the new pointer.
    return pointer;
}

int* CRC(int* message, int* generator, int message_size, int generator_size) //Function to calculate reaminder of CRC
{
    int* remainder = new int[generator_size];  //The number of bits can't be more then the generator size.
    int zero[generator_size] = {0,};           // Zero array to subtract in case the remainder after a step is smaller then generator.
    int pointer = generator_size;              // pointer stores the bit upto which we have performed the division (index) 

    for(int i = 0; i < generator_size; i++)    //The first bits upto index less then the generator size will always be left as remainder.
    {
        remainder[i] = message[i];
    }

    for(int i = 0; i < message_size - generator_size; i++) //For the rest we perform divison.
    {
        if(remainder[0] != 1)   //If the msb of the remaider is not 1 then we will have to shift untill we get 1 as msb.
        {
            XOR(remainder , zero, generator_size); //We first XOR the remainder with zero array
            pointer = shift_remainder(message, remainder, generator_size, pointer); // Shift the remainder and calculate the
        }                                                                           // next pointer.

        else
        {
            XOR(remainder, generator, generator_size); //Otherwise we XOR the remainder with the generator.
            pointer = shift_remainder(message, remainder, generator_size, pointer);
        }
    }

    return remainder;   //return the pointer to the remainder array.
}

int read_data(int* arr,int size)  //This function an array from the user of the given size.
{
    for(int i = 0; i < size; i++)
    {
        std::cout<<"Bit number "<<i+1<<":";
        std::cin>>arr[i];

        if(arr[i] != 0)
        {
            if(arr[i] != 1) //The only two possible bits are 0 and 1 other are invalid.
            {
                std::cout<<"Error: Invalid Entry"<<std::endl;
                return -1;
            }
        }
    }

    return 1;
}

void display(int* arr,int size)  //This function displays the array of given size.
{
    for(int i = 0; i < size; i++)
    {
        std::cout<<arr[i]<<" ";
    }
    std::cout<<std::endl;
}

int* Subtract(int* message, int* remainder, int message_size, int remainder_size)
{   
    int* new_message = new int[message_size];
    int generator_counter = 0;

    for(int i = 0; i < message_size - remainder_size ; i++)
    {
        new_message[i] = message[i];
    }

    for(int j = message_size-remainder_size  ; j <  message_size ; j++)
    {
       new_message[j] = message[j] ^ remainder[generator_counter];
       generator_counter++;
    }

    return new_message;
}

int main()
{
    int generator_size, message_size;  //Integers to store the size of the message and the generator.

    std::cout<<"Please enter the number of bits in the generator:";
    std::cin>>generator_size;

    if(generator_size < 2)  //The generator must have at least two bits.
    {
        std::cout<<"Error: Size of generator < 2"<<std::endl;
        return 0;
    }

    int* generator = new int[generator_size]; //Dynamically alocating the array for the generator.

    std::cout<<"Please enter the bits in the generator:"<<std::endl;
    int a = read_data(generator,generator_size);  //Reading the bits of the generator.

    if(a == -1)   //Error in reading the data results in termination.
    {
        return 0;
    }

    if(generator[0] == 0 || generator[generator_size - 1] == 0) //Also the highest bit and the lowest bit must be 1.
    {
        std::cout<<"Error: Bit at end = 0"<<std::endl;
        return 0;
    }

    std::cout<<"The generator is:"<<std::endl;
    display(generator,generator_size);

    std::cout<<"Please enter the number of bits in the message:";
    std::cin >> message_size;

    if(message_size < generator_size) // The messge can't be smaller then the generator.
    {
        std::cout<<"Error: Message size < Generator size"<<std::endl;
        return 0;
    }

    int* message = new int[message_size + generator_size - 1]; //Here we allocate extra memory at the end as zeros will be
                                                               // appended at the end of the message.
    std::cout<<"Please enter the bits in the messagge:"<<std::endl;
    int b = read_data(message, message_size);

    if(b == -1)
    {
        return 0;
    }

    std::cout<<"The message is:"<<std::endl;
    display(message,message_size);

    for(int i = message_size; i < message_size + generator_size -1; i++)  // We append the r zeros at the end of the message
    {                                                                     // where r is the degeree of the generator polynomial.
        message[i] = 0;
    }

    message_size = message_size+generator_size-1;  //Calculating the new size of the message array.

    int* remainder = CRC(message, generator, message_size, generator_size);  //Here the reaminder of CRC is stored in the
                                                                            // reaminder array.
    std::cout<<"The reamainder is:"<<std::endl;
    display(remainder, generator_size);

    int* new_message = Subtract(message, remainder, message_size, generator_size); //The new_message array store the message
    std::cout<<"The message that will be transmitted is:";                         //after subtracting the remainder from the
    display(new_message, message_size);                                            // message.
}