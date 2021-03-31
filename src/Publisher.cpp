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

#include <iostream>
using namespace std;
//#include "MessengerTypeSupportImpl.h"

//#include "MessageTypeSupportImpl.h"
void printInstructions(int domainID, string username, string topic);
int send_message(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID,
                 string topicName, string username, string subject, int data,
                 int daysInCurrentMonth, string dateAndTime);
int send_message(src::Flamingo flamingoMessage, int seconds,
                 DDS::DomainParticipantFactory_var dpf, int domainID,
                 string topicName);
int send_message(src::Flamingo flamingoMessage, int seconds,
                 DDS::DomainParticipantFactory_var dpf, int domainID,
                 string topicName, int num_of_messages);

int main(int argc, char *argv[])
{
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    //Variables for sending message
    //char user_name[20];    //Might need to change this to get program working error free
    //char user_subject[20]; //Same with this
    int num_of_messages;
    string user_name = "Default Username";
    string user_subject = "Default Subject";

    int domainID = 42;
    //string username = "Default";
    string topicName = "Flamingo List";
    int generic_data = 0;
    int number_of_messages = 1;

    std::cout << "------------------------------------\n";
    std::cout << "|  ___ _            _                |\n";
    std::cout << "| | __| |__ _ _ __ (_)_ _  __ _ ___  |\n";
    std::cout << "| | _|| / _` | '  \\| | ' \\/ _` / _ \\ |\n";
    std::cout << "| |_| |_\\__,_|_|_|_|_|_||_\\__, \\___/ |\n";
    std::cout << "|   publisher             |___/      |\n";
    std::cout << "------------------------------------\n";

    src::Flamingo flamingo;
    flamingo.dateAndTime = "now";
    flamingo.name = user_name.c_str();
    flamingo.subject = user_subject.c_str();
    flamingo.data = generic_data;
    flamingo.daysInCurrentMonth = 0;

    while (true)
    {
        printInstructions(domainID, user_name, topicName);

        char input;

        cin >> input;

        switch (input)
        {
        case 'r':
            std::cout << "How many messages do you want to send?: ";
            std::cin >> num_of_messages;
            std::cout << "\n";
            std::cout << "Set timeout time in seconds: ";
            int seconds;
            std::cin >> seconds;
            std::cout << "\nWaiting...\n";
            send_message(flamingo, seconds, dpf,
                         domainID, topicName, num_of_messages);
            break;
        case 't':
            std::cout << "Enter desired domain ID: ";
            std::cin >> domainID;
            std::cout << "\n";
            break;
        case 'u':
            std::cout << "Please set username: ";
            std::cin >> user_name;
            flamingo.name = user_name.c_str();
            std::cout << "\n";
            break;
        case 'e':
            return 0;
        case 'n':
            std::cout << "Enter desired topic name: ";
            std::cin >> topicName;
            std::cout << "\n";
            break;
        case 'p': // edit message to send out
            char pinput;
            string subject_change;
            int data_change;
            cout << "Which field in your message do you want to change?\n";
            cin >> pinput;
            switch (pinput)
            {
            case 's':
                cout << "Set subject to: ";
                cin >> subject_change;
                flamingo.subject = subject_change.c_str();
                break;
            case 'd':
                cout << "Set data to: ";
                cin >> data_change;
                flamingo.data = data_change;
                break;
            }
        }
    }
}

int send_message(src::Flamingo flamingoMessage, int seconds,
                 DDS::DomainParticipantFactory_var dpf, int domainID,
                 string topicName, int num_of_messages)
{
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

    // while started here

    DDS::PublicationMatchedStatus matches;
    if (writer->get_publication_matched_status(matches) != DDS::RETCODE_OK)
    {
        std::cerr << "get_publication_matched_status failed!" << std::endl;
        return 1;
    }

    /*
        if (matches.current_count >= 1)
        {
            break;
        }*/

    DDS::ConditionSeq conditions;
    DDS::Duration_t timeout = {seconds, 0};
    if (ws->wait(conditions, timeout) != DDS::RETCODE_OK)
    {
        std::cerr << "wait failed!"
                  << std::endl;
        return 1;
    }
    else
    {

        ws->detach_condition(condition);

        for (int i = 0; i < num_of_messages; i++)
        {
            DDS::ReturnCode_t error = flamingo_writer->write(flamingoMessage, DDS::HANDLE_NIL);
            flamingoMessage.data++;
            if (error != DDS::RETCODE_OK)
            {
                // Log or otherwise handle the error condition
                std::cerr << "flamingo_writer returned ! DDS::RETCODE_OK\n";
                return 1;
            }
        }
        std::cerr << "Message correctly sent!\n";
    }

    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();

    return 0;
}

void printInstructions(int domainID, string username, string topic)
{
    std::cout << "\n-------------------------.\n";
    std::cout << "username: " + username + " | ";
    std::cout << "dID: " << domainID;
    std::cout << " | ";
    std::cout << "topic: " + topic;
    std::cout << "\n-------------------------.\n";
    std::cout << "r: Send data.\n";
    std::cout << "t: Set desired Domain ID.\n";
    std::cout << "u: Set username.\n";
    std::cout << "n: Set desired Topic.\n";
    cout << "p: change flamingo struct to send.\n";
    std::cout << "e: exit publisher.\n";
}

void printFlamingoStruct()
{
}