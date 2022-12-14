#include <iostream>

class Frame
{
    public:

    //Data members
    int message[16];   //Array that will store the message to be sent.
    int ack;           //Acknowledgement that will be sent with the frame.
    int frame_number;  //The number of the frame being sent.

    //Functions and Constructors
    Frame()
    {
        for(int i = 0; i < 16; i++)
        {
            message[i] = 0 + (rand()%2);  //Here we randomly genereate a bit pattern.
        }
    }

    void display();  //Funtion to display the message.

    friend class Sender;
    friend class Reciever;
};

void Frame::display()
{
    for(int i = 0; i< 16; i++)
    {
        std::cout<<message[i]<<" ";
    }

    std::cout<<std::endl;
}


class Sender
{
    public:

    Frame* f1;  //The frame that will be sent.
    int* window; //Array that reprsents the sequence number of the frame that are in window.
    int window_size; //Maximum size of the window that is allowed.
    int current_window_size = 0; //The current window size that is being occupid.
    int Frame_count = 0;   //Variable to show the next sequence number for the frame arriving.

    Sender();
    Frame* send_frame(); 
    void get_ack();
};

Sender::Sender()
{
    std::cout<<"Enter the size of window for the reciever:";
    std::cin>>window_size;

    window = new int[window_size];
}

Frame* Sender::send_frame()
{
    if(current_window_size == window_size)
    {
        std::cout<<"Maximum window size allowed reached"<<std::endl;
    }

    else
    {
        f1 = new Frame();   //create a new frame

        f1->frame_number = Frame_count;  //Put the frame seq number.

        if(Frame_count == window_size - 1)  //If the frame count has reached the window size make it equal to zero.
        {
            Frame_count = 0;
        }

        current_window_size++; //Increase the current window size.

        return f1;
    }
    
}

void Sender::get_ack()
{
    current_window_size--;  //Getting the ack decreases the size of the current window by 1.
}

class Reciever
{
    public:

    Frame* f1;
    int accept_frame = 1;  //This variable show if the reciever is ready to recieve a frame.


    void recieve_frame(Frame*);  //This function copies the data from the coming frame to the buffer.
    void send_packet_to_network_layer(); //This makes the accept frame = 0 making it read another frame.
};

void Reciever::recieve_frame(Frame* f)
{
    if(accept_frame == 0)
    {
        std::cout<<"Frame buffer is full."<<std::endl;
        return;
    }

    f1 = f;

    accept_frame = 0;
}

void Reciever::send_packet_to_network_layer()
{
    accept_frame = 1;
}

int main()
{
    Sender s1;
    Reciever r1;
    Frame* f;

    Frame* frame_array[s1.window_size];

    std::cout<<"First the sender will create "<<s1.window_size<<" frames and then send them all at once."<<std::endl;

    for(int i = 0; i < s1.window_size; i++)
    {
        frame_array[i] = s1.send_frame();

        std::cout<<"Frame number "<<i+1<<" is:"<<std::endl;
        frame_array[i]->display();
    }

    std::cout<<"The window size of the sender is at:"<<s1.current_window_size<<std::endl;

    std::cout<<"Now the reciever starts receiving the frames and start sending the acknowledgement."<<std::endl;

    for(int i = 0; i < s1.window_size; i++)
    {
        std::cout<<"The frame "<<i+1<<" is:"<<std::endl;
        r1.recieve_frame(frame_array[i]);
        r1.f1->display();
        r1.send_packet_to_network_layer();
        s1.get_ack();

        std::cout<<"The window size of the sender is at:"<<s1.current_window_size<<std::endl;
    }


    std::cout<<"---------------------------------------------------------------------------------------------\n"<<std::endl;

    int lost_frame;
    std::cout<<"Now we start showing the processing in case an ack is lost or a frame is not recieved.\n"<<std::endl;

    std::cout<<"Please enter the number of the frame for which is lost or ack not recieved:";
    std::cin>>lost_frame;

    if(lost_frame > s1.window_size)
    {
        std::cout<<"Invalid count for lost frame."<<std::endl;
        return 0;
    }

    std::cout<<"First the sender sends all the frame:"<<std::endl;
    for(int i = 0; i < s1.window_size; i++)
    {
        frame_array[i] = s1.send_frame();

        std::cout<<"Frame number "<<i+1<<" is:"<<std::endl;
        frame_array[i]->display();
    }

    std::cout<<"The window size of the sender is at:"<<s1.current_window_size<<std::endl;

    std::cout<<"Now the reciever recieves "<<lost_frame - 1<<" frames."<<std::endl;

    for(int i = 0; i < lost_frame - 1; i++)
    {
        std::cout<<"The frame "<<i+1<<" is:"<<std::endl;
        r1.recieve_frame(frame_array[i]);
        r1.f1->display();
        r1.send_packet_to_network_layer();
        s1.get_ack();

        std::cout<<"The window size of the sender is at:"<<s1.current_window_size<<std::endl;
    }

    std::cout<<"Now the sender will send another "<<s1.window_size - s1.current_window_size <<" frames to make the window full again."<<std::endl;

    for(int i = 0; i < s1.window_size - s1.current_window_size +1; i++)
    {
        f = s1.send_frame();
        f -> display();
    }

    std::cout<<"Now the window size of the sender is at:"<<s1.current_window_size<<std::endl;

    std::cout<<"Now since the ack for the first packet in the sender window doesn't get acknowleged it's timer will expire."<<std::endl;

    std::cout<<"The sender will send all "<<s1.window_size<<" frames again in such a case."<<std::endl;
}