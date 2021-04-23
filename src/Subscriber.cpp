// Author: Tavien Nelson, Cameron Banff
// Version 1.0.0
// Description: Subscriber file to communicate with the Publisher
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

#include <The-Flamingos/src/FlamingoTypeSupportImpl.h>
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
    std::cout << "Enter desired DomainID:";
    std::cin >> domainID;
    std::cout << "\nEnter desired topic name:";
    std::cin >> topicName;

    // DDS initialization variables & other random variables
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    CORBA::String_var type_name;
    // All our various DDS variables for participant, etc.
    DDS::DomainParticipant_var participant;
    DDS::Topic_var topic;
    DDS::Subscriber_var sub;
    DDS::DataReaderListener_var listener(new DataReaderListenerImpl);
    DDS::DataReader_var dr;

    // Create our flock object and set its variables to the ones made above.
    SubFlock flock;
    flock.topic = topic;
    flock.sub = sub;
    flock.participant = participant;
    flock.type_name = type_name;
    flock.domainID = domainID;
    flock.topicName = topicName;
    flock.dpf = dpf;
    flock.listener = listener;
    flock.dr = dr;

    registerSubFlock(flock);

    while (true)
    {
        printInstructions(domainID, topicName, logging);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 'e':
            //cleanup(flock.participant, flock.dpf, logging);
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