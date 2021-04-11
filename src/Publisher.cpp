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

//#include "MessengerTypeSupportImpl.h"

//#include "MessageTypeSupportImpl.h"
int sending(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID, std::string topicName, std::string user_name, std::string user_subject, int num_of_messages, src::Flamingo flamingo);
void printInstructions(int domainID, std::string username, std::string topic, std::string subject, int data);
std::string getTime();
int daysInCurrentMonth();
//
void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf);
int create_data_writer(DDS::Publisher_var &pub, DDS::Topic_var &topic, DDS::DataWriter_var &writer);
int register_type_support(src::FlamingoTypeSupport_var fts, DDS::DomainParticipant_var &participant, CORBA::String_var &type_name);
int create_publisher(DDS::Publisher_var &pub, DDS::DomainParticipant_var &participant);
int create_participant(DDS::DomainParticipantFactory_var &dpf, int domainID, DDS::DomainParticipant_var &participant);
int create_topic(DDS::DomainParticipant_var &participant, std::string topicName, CORBA::String_var type_name, DDS::Topic_var &topic);
int send(DDS::DataWriter_var &writer, int seconds, int num_of_messages, src::FlamingoDataWriter_var &flamingo_writer, src::Flamingo flamingo);

int main(int argc, char *argv[])
{
    //Variables for sending message
    //char user_name[20];    //Might need to change this to get program working error free
    //char user_subject[20]; //Same with this
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    int num_of_messages = 1;
    std::string user_name = "Default";
    std::string user_subject = "Default";
    std::string topicName = "Default";
    int domainID = 42;
    int attempt;
    int seconds = 10;

    src::Flamingo flamingo;
    flamingo.dateAndTime = getTime().c_str();

    flamingo.name = user_name.c_str();
    flamingo.subject = user_subject.c_str();
    flamingo.data = 0;
    flamingo.daysInCurrentMonth = daysInCurrentMonth();
    ////////////////////////////////////////////////////

    src::FlamingoTypeSupport_var fts = new src::FlamingoTypeSupportImpl();
    CORBA::String_var type_name;

    DDS::Topic_var topic;
    DDS::DataWriter_var writer;
    DDS::Publisher_var pub;

    // Create domain participant
    DDS::DomainParticipant_var participant;
    create_participant(dpf, domainID, participant);

    register_type_support(fts, participant, type_name);

    // Create topic with participant and our topic variable
    create_topic(participant, topicName, type_name, topic);

    create_publisher(pub, participant);

    create_data_writer(pub, topic, writer);

    src::FlamingoDataWriter_var flamingo_writer = src::FlamingoDataWriter::_narrow(writer);

    ////////////////////////////////////////////////////
    //Welcome Message
    std::cout << " " << std::endl;
    std::cout << "------------------------------------\n";
    std::cout << "|  ___ _            _                |\n";
    std::cout << "| | __| |__ _ _ __ (_)_ _  __ _ ___  |\n";
    std::cout << "| | _|| / _` | '  \\| | ' \\/ _` / _ \\ |\n";
    std::cout << "| |_| |_\\__,_|_|_|_|_|_||_\\__, \\___/ |\n";
    std::cout << "|   publisher             |___/      |\n";
    std::cout << "------------------------------------\n";
    std::cout << " " << std::endl;

    while (true)
    {
        flamingo.subject = user_subject.c_str();
        flamingo.name = user_name.c_str();

        printInstructions(domainID, user_name, topicName, user_subject, flamingo.data);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 's':
            //std::cout << "Set timeout time in seconds:";
            //int seconds;
            //std::cin >> seconds;
            std::cout << "\nWaiting...\n";
            //attempt = sending(seconds, dpf, domainID, topicName, user_name, user_subject, num_of_messages, flamingo);
            flamingo.dateAndTime = getTime().c_str();
            attempt = send(writer, seconds, num_of_messages, flamingo_writer, flamingo);
            if (attempt == 0)
            {
                std::cout << "Message was successfully sent!\n";
            }
            else
            {
                std::cout << "Send attempt failed: Error code: " << attempt << std::endl;
            }
            break;
        case 'd':
            std::cout << "Enter desired domain ID: ";
            std::cin >> domainID;
            std::cout << "\n";
            break;
        case 'u':
            std::cout << "Please set username: ";
            std::cin >> user_name;
            std::cout << "\nUsername set to " + user_name;
            std::cout << "\n";
            break;
        case 'e':
            return 0;
        case 't':
            std::cout << "Enter desired topic name:";
            std::cin >> topicName;
            std::cout << "\n";
            break;
        case 'c':
            std::cout << "Enter the Flamingo Subject:";
            std::cin >> user_subject;
            std::cout << "\nEnter Flamingo generic integer:";
            std::cin >> flamingo.data;
            break;
        }
    }
}

void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf)
{
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();
}

int create_data_writer(DDS::Publisher_var &pub, DDS::Topic_var &topic, DDS::DataWriter_var &writer)
{
    //Create the datawriter
    writer = pub->create_datawriter(topic,
                                    DATAWRITER_QOS_DEFAULT,
                                    0, // No listener required
                                    OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!writer)
    {
        std::cerr << "create_datawriter failed." << std::endl;
    }

    return 0;
}

int register_type_support(src::FlamingoTypeSupport_var fts, DDS::DomainParticipant_var &participant, CORBA::String_var &type_name)
{
    // REGISTERING THE DATA TYPE AND CREATING A TOPIC
    // Trying to get my IDE to recognize the type support object type.
    if (DDS::RETCODE_OK != fts->register_type(participant, ""))
    {
        std::cerr << "register_type failed." << std::endl;
        return 1;
    }
    type_name = fts->get_type_name();

    return 0;
}

int create_publisher(DDS::Publisher_var &pub, DDS::DomainParticipant_var &participant)
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
    return 0;
}

int create_participant(DDS::DomainParticipantFactory_var &dpf, int domainID, DDS::DomainParticipant_var &participant)
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
    return 0;
}

int create_topic(DDS::DomainParticipant_var &participant, std::string topicName, CORBA::String_var type_name, DDS::Topic_var &topic)
{
    topic = participant->create_topic(topicName.c_str(),
                                      type_name,
                                      TOPIC_QOS_DEFAULT,
                                      0, // No listener required
                                      OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic)
    {
        std::cerr << "create_topic failed." << std::endl;
        return 1;
    }
    return 0;
}

int send(DDS::DataWriter_var &writer, int seconds, int num_of_messages, src::FlamingoDataWriter_var &flamingo_writer, src::Flamingo flamingo)
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
            std::cerr << "get_publication_matched_status failed!" << std::endl;
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
            return 1;
        }
    }
    return 0;
}

int sending(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID, std::string topicName, std::string user_name, std::string user_subject, int num_of_messages, src::Flamingo flamingo)
{
    //DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    DDS::DomainParticipant_var participant = dpf->create_participant(domainID, //domain ID
                                                                     PARTICIPANT_QOS_DEFAULT,
                                                                     0, //No listener required
                                                                     OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (!participant)
    {
        std::cerr << "create_participant failed." << std::endl;
        return 1;
    }

    // REGISTERING THE DATA TYPE AND CREATING A TOPIC
    // Trying to get my IDE to recognize the type support object type.
    src::FlamingoTypeSupport_var mts = new src::FlamingoTypeSupportImpl(); // it works!!!!!!!
    //Messenger::MessageTypeSupport_var mts = new Messenger::MessageTypeSupportImpl();
    if (DDS::RETCODE_OK != mts->register_type(participant, ""))
    {
        std::cerr << "register_type failed." << std::endl;
        return 1;
    }

    CORBA::String_var type_name = mts->get_type_name();
    DDS::Topic_var topic = participant->create_topic(topicName.c_str(),
                                                     type_name,
                                                     TOPIC_QOS_DEFAULT,
                                                     0, // No listener required
                                                     OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic)
    {
        std::cerr << "create_topic failed." << std::endl;
        return 1;
    }

    // CREATING A PUBLISHER

    DDS::Publisher_var pub = participant->create_publisher(PUBLISHER_QOS_DEFAULT,
                                                           0,
                                                           OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!pub)
    {
        std::cerr << "create_publisher failed." << std::endl;
        return 1;
    }

    // CREATING A DATAWRITER AND WAITING FOR THE SUBSCRIBER

    //Create the datawriter
    DDS::DataWriter_var writer = pub->create_datawriter(topic,
                                                        DATAWRITER_QOS_DEFAULT,
                                                        0, // No listener required
                                                        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!writer)
    {
        std::cerr << "create_datawriter failed." << std::endl;
    }

    //Messenger::MessageDataWriter_var message_writer = Messenger::MessageDataWriter::_narrow(writer);
    src::FlamingoDataWriter_var flamingo_writer = src::FlamingoDataWriter::_narrow(writer);

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
            std::cerr << "get_publication_matched_status failed!" << std::endl;
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
            return 1;
        }
    }

    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();

    return 0;

    //dev guide ends here I believe
}

void printInstructions(int domainID, std::string username, std::string topic, std::string subject, int data)
{
    std::cout << "\n-------------------------\n";
    std::cout << "username: " + username + " | ";
    std::cout << "dID: " << domainID;
    std::cout << " | ";
    std::cout << "topic: " << topic << std::endl;
    std::cout << "Flamingo - Subject: " << subject;
    std::cout << " | Generic Data: " << data;
    std::cout << "\n-------------------------\n";
    std::cout << "s: Try to send your data.\n";
    std::cout << "d: Set desired Domain ID.\n";
    std::cout << "u: Set username.\n";
    std::cout << "t: Set desired Topic.\n";
    std::cout << "c: Change your Flamingo to send.\n";
    std::cout << "e: Exit.\n";
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