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
#include <The-Flamingos/src/PubFlock.h>
#include "FlamingoWare.h"

int daysInCurrentMonth();
std::string getTime();
void printInstructions(int domainID, std::string username, std::string topic, std::string subject, int data, bool logging);

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

    int domainID = 42;
    std::string topicName = "Default";

    //////////////////////////////////////////////////////
    //Welcome Message (& asking user for dID and topicName)
    //////////////////////////////////////////////////////
    std::cout << " " << std::endl;
    std::cout << "------------------------------------\n";
    std::cout << "|  ___ _            _                |\n";
    std::cout << "| | __| |__ _ _ __ (_)_ _  __ _ ___  |\n";
    std::cout << "| | _|| / _` | '  \\| | ' \\/ _` / _ \\ |\n";
    std::cout << "| |_| |_\\__,_|_|_|_|_|_||_\\__, \\___/ |\n";
    std::cout << "|   publisher             |___/      |\n";
    std::cout << "------------------------------------\n";
    std::cout << " " << std::endl;
    std::cout << "Enter desired DomainID:";
    std::cin >> domainID;
    std::cout << "\nEnter desired topic name:";
    std::cin >> topicName;

    /*
    PubFlock flock;
    flock.domainID = domainID;
    flock.topicName = topicName;*/

    // DDS initialization variables & other random variables
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    int num_of_messages = 1;
    std::string user_name = "Default";
    std::string user_subject = "Default";
    int attempt;
    int seconds = 10;

    // Flamingo struct of the user //
    src::Flamingo flamingo;
    flamingo.name = user_name.c_str();
    flamingo.subject = user_subject.c_str();
    flamingo.data = 0;
    flamingo.daysInCurrentMonth = daysInCurrentMonth();

    //int register_attempt = register_flock_as_pub(flock);

    // These are never changed, so establish them here.
    src::FlamingoTypeSupport_var fts = new src::FlamingoTypeSupportImpl();
    CORBA::String_var type_name;

    // All our various DDS variables for participant, etc.

    DDS::Topic_var topic;
    DDS::DataWriter_var writer;
    DDS::Publisher_var pub;
    DDS::DomainParticipant_var participant;
    src::FlamingoDataWriter_var flamingo_writer;

    PubFlock flock;
    flock.topic = topic;
    flock.writer = writer;
    flock.pub = pub;
    flock.participant = participant;
    flock.flamingo_writer = flamingo_writer;
    flock.type_name = type_name;
    flock.domainID = domainID;
    flock.topicName = topicName;
    flock.flamingo = flamingo;
    flock.dpf = dpf;

    registerPubFlock(flock);

    // Beginning of program execution loop
    while (true)
    {
        flamingo.subject = user_subject.c_str();
        flamingo.name = user_name.c_str();

        printInstructions(domainID, user_name, topicName, user_subject, flamingo.data, logging);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 's':
            std::cout << "\nWaiting...\n";
            flamingo.dateAndTime = getTime().c_str();
            attempt = send(flock.writer, seconds, num_of_messages, flock.flamingo_writer, flamingo, logging);
            if (attempt == 0)
            {
                std::cout << "Message was successfully sent!\n";
            }
            else
            {
                std::cout << "Send attempt failed: Error code: " << attempt << std::endl;
            }
            break;
        case 'u':
            std::cout << "Please set username: ";
            std::cin >> user_name;
            std::cout << "\nUsername set to " + user_name;
            std::cout << "\n";
            break;
        case 'e':
            //cleanup(flock.participant, flock.dpf, logging);
            cleanupPubFlock(flock);
            return 0;
        case 'c':
            std::cout << "Enter the Flamingo Subject:";
            std::cin >> user_subject;
            std::cout << "\nEnter Flamingo generic integer:";
            std::cin >> flamingo.data;
            break;
        }
    }
    return 0;
}

void printInstructions(int domainID, std::string username, std::string topic, std::string subject, int data, bool logging)
{
    if (logging)
    {
        std::cout << "\n////////////DEV MODE////////////\n";
    }
    std::cout << "\n-------------------------\n";
    std::cout << "username: " + username + " | ";
    std::cout << "dID: " << domainID;
    std::cout << " | ";
    std::cout << "topic: " << topic << std::endl;
    std::cout << "Flamingo - Subject: " << subject;
    std::cout << " | Generic Data: " << data;
    std::cout << "\n-------------------------\n";
    std::cout << "s: Try to send your data.\n";
    std::cout << "u: Set username.\n";
    std::cout << "c: Change your Flamingo to send.\n";
    if (logging)
    {
        std::cout << "l: Toggle logging.\n";
    }
    std::cout << "e: Exit.\n";
    if (logging)
    {
        std::cout << "\n////////////DEV MODE////////////\n";
    }
}

std::string getTime()
{
    time_t now = time(0);
    char *curTime = ctime(&now);
    return std::string(curTime);
}

int daysInCurrentMonth()
{
    time_t now = time(0);
    tm *lt = localtime(&now);
    switch (lt->tm_mon + 1)
    {
    case 1:
        return 31;
    case 2:
        if (((1900 + lt->tm_year) % 4) == 0)
        {
            return 29;
        }
        else
        {
            return 28;
        }
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    default:
        return -1;
    }
}