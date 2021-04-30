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
    // determine and set logging environment
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

    int domainID = 42; // Default
    std::string topicName = "Default";
    std::string userName = "Default";
    std::string userSubject = "Default";

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
    while (std::cin.fail())
    {
        std::cout << "Incorrect input. \nPlease enter an appropriate number: ";
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> domainID;
    }

    std::cout << "\nEnter desired topic name:";
    std::cin >> topicName;

    // DPF initialization, required to register our Flocks
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

    // Setup of our Flamingo message structure
    src::Flamingo flamingo;
    flamingo.name = userName.c_str();
    flamingo.subject = userSubject.c_str();
    flamingo.data = 0;
    flamingo.daysInCurrentMonth = daysInCurrentMonth();

    // Create our flock and set appropriate fields.
    PubFlock flock;
    flock.dpf = dpf;
    flock.domainID = domainID;
    flock.topicName = topicName;
    flock.flamingo = flamingo;

    // Register our Flock with the middleware
    registerPubFlock(flock);

    int attempt;
    // Beginning of program execution loop
    while (true)
    {
        flock.flamingo.subject = userSubject.c_str();
        flock.flamingo.name = userName.c_str();

        printInstructions(domainID, userName, topicName, userSubject, flock.flamingo.data, logging);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 's':
            std::cout << "\nWaiting...\n";
            flock.flamingo.dateAndTime = getTime().c_str();
            attempt = sendFlock(flock);
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
            std::cin >> userName;
            std::cout << "\nUsername set to " + userName;
            std::cout << "\n";
            break;
        case 'e':
            cleanupPubFlock(flock);
            return 0;
        case 'c':
            std::cout << "Enter the Flamingo Subject:";
            std::cin >> userSubject;
            std::cout << "\nEnter Flamingo generic integer:";
            std::cin >> flock.flamingo.data;
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