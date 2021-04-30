#include <ace/Log_Msg.h>

#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include <dds/DCPS/StaticIncludes.h>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/RTPS/RtpsDiscovery.h>
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif

#include <The-Flamingos/src/DataReaderListenerImpl.h>
#include <The-Flamingos/src/SubFlock.h>
#include "FlamingoWare.h"

#include <iostream>
using namespace std;

void printInstructions(int domainID, string topic, bool logging);
int main(int argc, char *argv[])
{
    bool logging;
    if (getenv("FLAMINGO_DEV"))
    {
        logging = true;
        std::cout << "\n////////////LAUNCHED IN DEVMODE////////////\n";
    }
    else
    {
        logging = false;
    }
    setLogging(logging);

    int domainID;
    string topicName;

    std::cout << "------------------------------------\n";
    std::cout << "|  ___ _            _                |\n";
    std::cout << "| | __| |__ _ _ __ (_)_ _  __ _ ___  |\n";
    std::cout << "| | _|| / _` | '  \\| | ' \\/ _` / _ \\ |\n";
    std::cout << "| |_| |_\\__,_|_|_|_|_|_||_\\__, \\___/ |\n";
    std::cout << "|   subscriber            |___/      |\n";
    std::cout << "------------------------------------\n";

    std::cout << "Enter desired Domain ID:";
    std::cin >> domainID;
    while (std::cin.fail())
    {
        std::cout << "Incorrect input. \nPlease enter an appropriate number: ";
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> domainID;
    }

    std::cout << "\nEnter desired topic name:";
    std::cin >> topicName;

    // DDS initialization variables & other random variables
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

    // Create our flock object and set its variables to the ones made above.
    SubFlock flock;
    flock.domainID = domainID;
    flock.dpf = dpf;
    flock.topicName = topicName;

    registerSubFlock(flock);

    while (true)
    {
        printInstructions(domainID, topicName, logging);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 'e':
            cleanupSubFlock(flock);
            return 0;
        }
    }
}

void printInstructions(int domainID, string topic, bool logging)
{
    if (logging)
    {
        std::cout << "\n////////////DEV MODE////////////\n";
    }
    std::cout << "\n-------------------------.\n";
    std::cout << "domain ID: " << domainID;
    std::cout << " | ";
    std::cout << "topic name: " + topic;
    std::cout << "\n-------------------------.\n";
    std::cout << "e: Exit.\n";
    if (logging)
    {
        std::cout << "\n////////////DEV MODE////////////\n";
    }
}