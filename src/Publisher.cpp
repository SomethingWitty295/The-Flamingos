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
/*
void printInstructions(int domainID, std::string username, std::string topic, std::string subject, int data, bool logging);
std::string getTime();
int daysInCurrentMonth();
void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf, bool logging);
int create_data_writer(DDS::Publisher_var &pub, DDS::Topic_var &topic, DDS::DataWriter_var &writer, bool logging);
int register_type_support(src::FlamingoTypeSupport_var fts, DDS::DomainParticipant_var &participant, CORBA::String_var &type_name, bool logging);
int create_publisher(DDS::Publisher_var &pub, DDS::DomainParticipant_var &participant, bool logging);
int create_participant(DDS::DomainParticipantFactory_var &dpf, int domainID, DDS::DomainParticipant_var &participant, bool logging);
//int create_topic(DDS::DomainParticipant_var &participant, std::string topicName, CORBA::String_var type_name, DDS::Topic_var &topic, bool logging);
int send(DDS::DataWriter_var &writer, int seconds, int num_of_messages, src::FlamingoDataWriter_var &flamingo_writer, src::Flamingo flamingo, bool logging);
**/
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
            cleanup(flock.participant, flock.dpf, logging);
            return 0;
        case 'c':
            std::cout << "Enter the Flamingo Subject:";
            std::cin >> user_subject;
            std::cout << "\nEnter Flamingo generic integer:";
            std::cin >> flamingo.data;
            break;
        case 'l':
            if (!logging)
            {
                std::cout << "Logging enabled.\n";
            }
            else
            {
                std::cout << "Logging disabled.\n";
            }
            logging = !logging;
        }
    }
}
/**

void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf, bool logging)
{
    participant->delete_contained_entities();
    if (logging)
    {
        std::cerr << "LOG: All participant entities have been deleted.\n";
    }

    dpf->delete_participant(participant);
    if (logging)
    {
        std::cerr << "LOG: Participant has been deleted via the current DPF.\n";
    }

    TheServiceParticipant->shutdown();
    if (logging)
    {
        std::cerr << "LOG: DPF shutdown() is complete.\n";
    }
}

int create_data_writer(DDS::Publisher_var &pub, DDS::Topic_var &topic, DDS::DataWriter_var &writer, bool logging)
{
    //Create the datawriter
    writer = pub->create_datawriter(topic,
                                    DATAWRITER_QOS_DEFAULT,
                                    0, // No listener required
                                    OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!writer)
    {
        std::cerr << "create_datawriter: failed." << std::endl;
        return 1;
    }
    else
    {
        if (logging)
        {
            std::cerr << "LOG: create_data_writer: datawriter has been set to writer variable. (&pub, &topic, &writer)\n";
        }
    }

    return 0;
}

int register_type_support(src::FlamingoTypeSupport_var fts, DDS::DomainParticipant_var &participant, CORBA::String_var &type_name, bool logging)
{
    // REGISTERING THE DATA TYPE AND CREATING A TOPIC
    // Trying to get my IDE to recognize the type support object type.
    if (DDS::RETCODE_OK != fts->register_type(participant, ""))
    {
        std::cerr << "register_type failed." << std::endl;
        return 1;
    }
    else
    {
        if (logging)
        {
            std::cerr << "LOG: register_type_support: FlamingoTypeSupport has correctly registered type with given participant.\n";
        }
    }
    type_name = fts->get_type_name();

    return 0;
}

int create_publisher(DDS::Publisher_var &pub, DDS::DomainParticipant_var &participant, bool logging)
{
    // Create publisher from participant
    pub = participant->create_publisher(PUBLISHER_QOS_DEFAULT,
                                        0,
                                        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!pub)
    {
        std::cerr << "create_publisher failed." << std::endl;
        return 1;
    }
    else
    {
        if (logging)
        {
            std::cerr << "LOG: create_publisher: publisher successfully created with current participant.";
        }
    }
    return 0;
}

int create_participant(DDS::DomainParticipantFactory_var &dpf, int domainID, DDS::DomainParticipant_var &participant, bool logging)
{
    participant = dpf->create_participant(domainID, //domain ID
                                          PARTICIPANT_QOS_DEFAULT,
                                          0, //No listener required
                                          OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (!participant)
    {
        std::cerr << "create_participant failed." << std::endl;
        return 1;
    }
    else
    {
        if (logging)
        {
            std::cerr << "LOG: create_participant: participant object successfully created within";
            std::cerr << domainID << " domain.\n";
        }
    }
    return 0;
}

int create_topic(DDS::DomainParticipant_var &participant, std::string topicName, CORBA::String_var type_name, DDS::Topic_var &topic, bool logging)
{
    topic = participant->create_topic(topicName.c_str(),
                                      type_name,
                                      TOPIC_QOS_DEFAULT,
                                      0, // No listener required
                                      OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic)
    {
        std::cerr << "create_topic: failed." << std::endl;
        return 1;
    }
    else
    {
        if (logging)
        {
            std::cerr << "LOG: create_topic: topic has been created with topicname ";
            std::cerr << topicName << "\n";
        }
    }
    return 0;
}

int send(DDS::DataWriter_var &writer, int seconds, int num_of_messages, src::FlamingoDataWriter_var &flamingo_writer, src::Flamingo flamingo, bool logging)
{
    //Block until Subscriber is available
    DDS::StatusCondition_var condition = writer->get_statuscondition();
    condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);

    DDS::WaitSet_var ws = new DDS::WaitSet;
    ws->attach_condition(condition);

    while (true)
    {
        DDS::PublicationMatchedStatus matches;
        if (writer->get_publication_matched_status(matches) != DDS::RETCODE_OK)
        {
            if (logging)
            {
                std::cerr << "LOG: send: get_publication_matched_status failed!" << std::endl;
            }
            return 1;
        }

        if (matches.current_count >= 1)
        {
            break;
        }

        DDS::ConditionSeq conditions;
        DDS::Duration_t timeout = {seconds, 0};
        if (ws->wait(conditions, timeout) != DDS::RETCODE_OK)
        {
            std::cerr << "wait failed!" << std::endl;
            if (logging)
            {
                std::cerr << "LOG: send: ws->wait(conditions, timeout) != DDS::RETCODE_OK.\n";
            }
            return 1;
        }
    }

    ws->detach_condition(condition);

    //For more details about status, conditions, and wait sets, see Ch. 4.
    // SAMPLE PUBLICATION
    // Write samples

    for (int i = 0; i < num_of_messages; i++)
    {
        DDS::ReturnCode_t error = flamingo_writer->write(flamingo, DDS::HANDLE_NIL);
        flamingo.data++;
        if (error != DDS::RETCODE_OK)
        {
            // Log or otherwise handle the error condition
            if (logging)
            {
                std::cerr << "LOG: send: flamingo has failed to send from pub side. (error != DDS::RETCODE_OK)\n";
            }
            return 1;
        }
    }

    if (logging)
    {
        std::cerr << "LOG: send: flamingo has been successfully sent from publisher side.\n";
    }

    return 0;
}
*/
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
    }
}