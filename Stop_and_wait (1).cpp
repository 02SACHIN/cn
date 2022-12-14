#include <iostream>
#include <cstdlib>

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

class Sender  //Class to represent the sender.
{
    public:


    //Data members
    Frame* frame1;
    Frame* frame2;
    int frame_expected = 1;
    int frame_ack_expected = 1;
    int frame_to_send = 0;
    int ack_to_send = 0;

    //Functions
    void create_frame();
    void send_frame(Frame*);
    int recieve_frame(Frame*);
};

class Reciever   //Class to represent the reciever.
{
    public:

    //Data Members
    Frame* frame1;  //This Frame acts as the buffer for the message to be sent.
    Frame* frame2;  //This Frame acts as the buffer for the message to be recieved.
    int frame_expected = 0;  //This represents the number of the Frame that is expected to arrive.
    int frame_ack_expected = 0; //This contains the number of the frame that was sent in the previous cycle.
    int frame_to_send = 0;      //This contains the number of the frame that will be sent.
    int ack_to_send = 1;        //This contains the acknowledgment to be sent with this frame.

    //Functions
    void create_frame();   //A function that creates the Frame.
    void send_frame(Frame*);      //A function that sends the frame to other side.
    int recieve_frame(Frame*); //A function that recieves the frame.
};

void Sender::create_frame()
{
    Frame* f = new Frame();
    f->ack = this->ack_to_send;  //Setting the acknowledgement of the frame equal to the acknowledgement to be sent.

    this->frame1 = f;  //Setting the frame created just now equal to frame1.
}

void Reciever::create_frame()   // Similar function for the reciever side.
{
    Frame* f = new Frame();
    f->ack = this->ack_to_send;

    this -> frame1 = f;
}

int Sender::recieve_frame(Frame* f)  //Function to recieve the frame.
{
    if(f->frame_number == this->frame_expected)  //If the frame number of the frame that arrived is equal to frame that was
    {                                            //Expected.
        this->frame2 = f; //The we set the frame2 equal to the frame that arrived.

        if(f->frame_number==0)  //If the frame number that is recieved is equal to zero.
        {
            this->frame_expected = 1; //Then we set the next frame that is expected equal to 1.
        }

        else
        {
            this->frame_expected = 0; //Otherwise we set it equal to zero.
        }

        ack_to_send = f->frame_number; //Also the acknowledgement to be sent is set to the frame number that arrived now.
        
        return 1;  //This code means that the message was recieved successfully.
    }

    else
    {
        return -1;  //This code is means the frame was not recieved.
    }
}

int Reciever::recieve_frame(Frame* f) //The same function for the reciever side.
{
    if(f->frame_number == this->frame_expected)
    {
        this->frame2 = f;

        if(f->frame_number==0)
        {
            this->frame_expected = 1;
        }

        else
        {
            this->frame_expected = 0;
        }

        ack_to_send = f->frame_number;
        
        return 1;
    }

    else
    {
        return -1;
    }
}

void Sender::send_frame(Frame* f)  //This function sends the frame to other side.
{
    this->frame_ack_expected = f->frame_number; //We set the frame ack expected to the frame number being sent.
    
    if(f->frame_number == 0)  //and update the value of the frame to be sent equal to the complement of the frame number.
    {
        this->frame_to_send = 1;
    }
    
    else
    {
        this->frame_to_send = 0;
    }
}

void Reciever::send_frame(Frame*  f) //Same code for reciever side.
{
    this->frame_ack_expected = f->frame_number; //We set the frame ack expected to the frame number being sent.
    
    if(f->frame_number == 0)  //and update the value of the frame to be sent equal to the complement of the frame number.
    {
        this->frame_to_send = 1;
    }
    
    else
    {
        this->frame_to_send = 0;
    }
}

int main()
{
    Sender s;
    Reciever r;

    std::cout<<"The sender creates a frame and sends it to the reciever."<<std::endl;
    s.create_frame();
    s.send_frame(s.frame1);
    std::cout<<"The message that is to be sent:"<<std::endl;
    s.frame1->display();
    std::cout<<"The frame number is:"<<s.frame1->frame_number<<std::endl;
    std::cout<<"The next frame that will be sent is:"<<s.frame_to_send<<std::endl;
    std::cout<<"The frame for which acknowledgement is expected with the next message is:"<<s.frame_ack_expected<<std::endl;

    int i = r.recieve_frame(s.frame1);

    if(i = 1)
    {
        if(r.frame_expected == 1)
        {
            r.frame_expected = 0;
        }

        else
        {
            r.frame_expected = 1;
        }
    }


    std::cout<<"\n\nThe values at the reciever side are:"<<std::endl;
    std::cout<<"The message that will be sent to the network layer is:";
    r.frame2->display();
    std::cout<<"The frame for which acknowledgement will be sent is:"<<r.ack_to_send<<std::endl;
    std::cout<<std::endl;

    std::cout<<"\nNow the sender starts the timer. If acknowledgement arrives then the timer is stopped; otherwise the frame is resent."<<std::endl;
    std::cout<<"The message was recieved successfully.\n"<<std::endl;


    std::cout<<"Now the receiver creates a frame and send it to the sender:"<<std::endl;
    r.create_frame();
    r.send_frame(r.frame1);
    std::cout<<"The message that is to be sent:"<<std::endl;
    r.frame1->display();
    std::cout<<"The acknowledgement sent with the frame is:"<<r.ack_to_send<<std::endl;

    int j = s.recieve_frame(r.frame1);
    if(j = 1)
    {
        if(s.frame_expected == 1)
        {
            std::cout<<"The frame was recieved successfully."<<std::endl;
            s.frame_expected = 0;
        }

        else
        {
            s.frame_expected = 1;
        }
    }

    std::cout<<"Now the sender and the reciever flips the side and the cycle continues."<<std::endl;
    return 0;
}