#include <iostream>

class Frame
{
    public:

    //Data members
    int message[16];   //Array that will store the message to be sent.
    int ack;           //Acknowledgement that will be sent with the frame.
    int frame_number = 0;  //The number of the frame being sent.

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
    int Frame_count = 0;   //Variable to show the next sequence number for the frame arriving from network layer.

    Sender();
    Frame* send_frame(); //Function that sends a frame to the reciever.
    void get_ack(int);   //Function that simulates the phenomenon of receving an acknowledgement.
    void display_window();  //Funtion to display the window of the sender.
};

Sender::Sender()  //constructor for the sender.
{
    std::cout<<"Enter the size of window for the sender:";
    std::cin>>window_size;

    window = new int[window_size];
}

void Sender::display_window()
{
    for(int i = 0; i < window_size; i++)
    {
        std::cout<<window[i]<<" ";
    }

    std::cout<<std::endl;
}

Frame* Sender::send_frame()
{
    if(current_window_size == window_size)  //If the window is full we disable the network layer for a while.
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

        else
        {
            Frame_count++;  //Increase the frame count
        }

        window[current_window_size] = 1;

        current_window_size++; //Increase the current window size.

        return f1;
    }
    
}

void Sender::get_ack(int x)
{
    if(x < 0)  //If the recieved ack is negative then this piece of code comes in effect.
    {
        std::cout<<"Negative ACK for frame"<<-x<<std::endl;
    }

    else  //Otherwise we make the window value at index x equal to zero.
    {
        window[x] = 0;          // X === sequence number of the frame.
        current_window_size--;  //Decrease the window size.
    }
}

class Reciever
{
    public:

    Frame* f;  //A frame to accept the incoming data
    int* frame_buffer;  //An array to buffer the frame sequence
    int buffer_size;
    int current_buffer_size = 0;

    Reciever(int);
    int send_ack();
    int send_nack();
    void recieve_frame(Frame*);
    void display_buffer();
};

Reciever::Reciever(int x)
{
    buffer_size = x;
    frame_buffer = new int[buffer_size];

    for(int i = 0; i < buffer_size; i++)
    {
        frame_buffer[i] = 0;
    }
}

int Reciever::send_ack()
{
    return f->frame_number;
}

int Reciever::send_nack()
{
    return -(f->frame_number);
}

void Reciever::recieve_frame(Frame* f1)
{
    f = f1;   //we make the frame f in class reciver equal to f1, the recieved frame.

    frame_buffer[f1->frame_number] = 1;  // and make the frame buffer for the index of the frame sequence number equal to 1.
}

void Reciever::display_buffer()
{
    for(int i = 0; i < buffer_size; i++)
    {
        std::cout<<frame_buffer[i]<<" ";
    }

    std::cout<<std::endl;
}

int main()
{
    Sender s1;
    Reciever r1(s1.window_size);
    Frame* f;

    Frame* frame_array[s1.window_size];

    std::cout<<"First the sender will create "<<s1.window_size<<" frames and then send them all at once."<<std::endl;

    for(int i = 0; i < s1.window_size; i++)
    {
        frame_array[i] = s1.send_frame();

        std::cout<<"Frame number "<<i+1<<" is:"<<std::endl;
        frame_array[i]->display();
    }

    std::cout<<"The window buffer of the sender is:";
    s1.display_window();
    std::cout<<std::endl;

    std::cout<<"Now the reciever starts receiving the frames and start sending the acknowledgement."<<std::endl;

    for(int i = 0; i < r1.buffer_size; i++)
    {
        std::cout<<"The frame "<<i+1<<" is:"<<std::endl;
        r1.recieve_frame(frame_array[i]);
        r1.f->display();
        s1.get_ack(r1.f->frame_number);

        std::cout<<"The window buffer of the sender is:";
        s1.display_window();

        std::cout<<"The buffer of the reciever is:";
        r1.display_buffer();
        std::cout<<std::endl;
    }


    std::cout<<"---------------------------------------------------------------------------------------------\n"<<std::endl;

    int lost_frame;
    std::cout<<"Now we start showing the processing in case a frame is not recieved.\n"<<std::endl;

    std::cout<<"Please enter the number of the frame for which is lost:";
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

    std::cout<<std::endl;

    std::cout<<"The window buffer of the sender is:";
    s1.display_window();

    std::cout<<"Now the reciever recieves "<<lost_frame - 1<<" frames."<<std::endl;

    for(int i = 0; i < lost_frame - 1; i++)
    {
        std::cout<<"The frame "<<i+1<<" is:"<<std::endl;
        r1.recieve_frame(frame_array[i]);
        r1.f->display();
        s1.get_ack(r1.f->frame_number);

        std::cout<<"The window buffer of the sender is:";
        s1.display_window();
    }

    std::cout<<std::endl;

    std::cout<<"Now as soon as the reciever recieves "<<lost_frame + 1<<" frame, it will buffer it and send a NACK for frame ";
    std::cout<<lost_frame <<std::endl;

    std::cout<<"This will cause the sender to send this frame back to the reciever."<<std::endl;

}