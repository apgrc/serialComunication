#include <libserial/SerialStream.h>
#include <string>
#include <sstream>
#include <iostream>

/*
 * Delay Function
 *
 * include <chrono>
 * include <thread>
 * std::this_thread::sleep_for(std::chrono::milliseconds(50));
 */
using namespace std;

enum com:uint8_t {
    Hello = 0, Start, Continue, Stop
};

#pragma pack(2)
struct order{
    com command;
    int16_t out_m0;
    int16_t out_m1;
  };

#pragma pack(2)
struct recive{
    float time;
    int_fast32_t position1;
    int_fast32_t position2;
    int_fast16_t distance[5];
};

int main(int argc, char* argv[])
{
    LibSerial::SerialStream serial_stream;

    serial_stream.Open("/dev/ttyACM3");
    serial_stream.SetBaudRate(LibSerial::BaudRate::BAUD_9600);

    int num = 0;
    order my_data{};
    string output;
    string input;

    while(input[0] != 'z'){
        input.clear();
        output.clear();
        cout << "0: Hello" << endl;
        cout << "1: Start" << endl;
        cout << "2: Continue" << endl;
        cout << "3: Stop" << endl;
        cout << "Input command: ";
        getline(cin,input);
        if(input[0] == 'z')
            break;
        stringstream(input) >> num;
        my_data.command = static_cast<com>(num);
        if(my_data.command == 1 || my_data.command == 2){
            cout << "Motor 0 (max 7199): ";
            getline(cin,input);
            stringstream(input) >> num;
            my_data.out_m0 = static_cast<int16_t>(num);

            cout << "Motor 1 (max 7199): ";
            getline(cin,input);
            stringstream(input) >> num;
            my_data.out_m1 = static_cast<int16_t>(num);
        }
        else{
            my_data.out_m0 = 0;
            my_data.out_m1 = 0;
        }

        serial_stream.write(reinterpret_cast<const char *>(&my_data), sizeof(my_data));
        //getline(serial_stream,output);
        //cout << output << endl;
    }
    serial_stream.Close();
    return 0;
}
