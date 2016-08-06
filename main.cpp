#include "udp_client_server.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <thread>
#include <chrono>
#include <unistd.h>

int main()
{
    const int LOOPS_PER_SEC = 10; 
    double loopTime = 1.0 / LOOPS_PER_SEC;
    std::string addr = "10.1.15.6";
    int port = 5810;

    udp_client_server::udp_client client(addr, port);
    udp_client_server::udp_server server("10.1.15.8", port);

    clock_t init, end;	    

    int i = 0;
    std::string msg[] = {"5.5", "-3.70", "-2.36", "-1.14", "-1.01", "-3.14", "4.23", "-2.12", "1.13", "5.04"};
    while (true)
    {
            init = clock();
	    
    	    //std::string msg = "Hello World";
	    //std::stringstream sstm;
	    //sstm << i;
	    //msg += sstm.str();

            if (i == (sizeof(msg) / sizeof(double))) i = 0;
            printf("Send Data:  %s\n", msg[i].c_str());
	    client.send(msg[i].c_str(), strlen(msg[i].c_str()));
            i++;

	    printf("Port (%d) at %s\n", port, addr.c_str());
	    printf("Size of msg: %d\n", static_cast<int>(strlen(msg[i].c_str())));

	    char buff [6400];
	    server.recv(buff, sizeof(buff) / sizeof(char));

	    printf("Buffer: %s\n", buff);
	    end = clock();
	    // Clock at 1 Hz
	    double timeElapsed = (double)(end - init) / (double)CLOCKS_PER_SEC;
	    
            // Pause until loop ends
            if (timeElapsed < loopTime)
            {
            	unsigned int microseconds = static_cast<int>((loopTime - timeElapsed) * 1000000);
            	//printf("Loop took (%f) seconds and time to stop = %d\n", timeElapsed, microseconds); 
            	printf ("Stalling for %d seconds\n", microseconds);
            	usleep(microseconds);
            }
            // Not on time
            else 
            {
            	printf("Error: Loop took (%f) seconds and new FPS = %f\n", timeElapsed, (1 / timeElapsed)); 
            }
    }
    return 0;
}
