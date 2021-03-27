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

#include <iostream>
using namespace std;

void printInstructions(int domainID, string username, string topic);
int receiving(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID, string topicName);

int main(int argc, char *argv[])
{
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    int domainID = 42;
    string username = "Default";
    string topicName = "Flamingo List";

    std::cout << "------------------------------------\n";
    std::cout << "|  ___ _            _                |\n";
    std::cout << "| | __| |__ _ _ __ (_)_ _  __ _ ___  |\n";
    std::cout << "| | _|| / _` | '  \\| | ' \\/ _` / _ \\ |\n";
    std::cout << "| |_| |_\\__,_|_|_|_|_|_||_\\__, \\___/ |\n";
    std::cout << "|   subscriber            |___/      |\n";
    std::cout << "------------------------------------\n";

    //std::cout << "Username, Topic, and Domain ID are set to default.\n";

    while (true)
    {
        printInstructions(domainID, username, topicName);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 'r':
            std::cout << "Set timeout time in seconds:";
            int seconds;
            std::cin >> seconds;
            std::cout << "\nWaiting...\n";
            receiving(seconds, dpf, domainID, topicName);
            break;
        case 't':
            std::cout << "Enter desired domain ID: ";
            std::cin >> domainID;
            std::cout << "\n";
            break;
        case 'u':
            std::cout << "Please set username: ";
            std::cin >> username;
            std::cout << "\nUsername set to " + username;
            std::cout << "\n";
            break;
        case 'e':
            return 0;
        case 'n':
            std::cout << "Enter desired topic name:";
            std::cin >> topicName;
            std::cout << "\n";
            break;
        }
    }
}

int receiving(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID, string topicName)
{
    try
    {
        DDS::DomainParticipant_var participant = dpf->create_participant(42, //domain ID
                                                                         PARTICIPANT_QOS_DEFAULT,
                                                                         0, //No listener required
                                                                         OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!participant)
        {
            std::cerr << "create_participant failed." << std::endl;
            return 1;
        }

        src::FlamingoTypeSupport_var mts = new src::FlamingoTypeSupportImpl();

        if (DDS::RETCODE_OK != mts->register_type(participant, ""))
        {
            std::cerr << "register_type failed." << std::endl;
            return 1;
        }
        CORBA::String_var type_name = mts->get_type_name();

        DDS::Topic_var topic = participant->create_topic("Flamingo List",
                                                         type_name,
                                                         TOPIC_QOS_DEFAULT,
                                                         0, // No listener required
                                                         OpenDDS::DCPS::DEFAULT_STATUS_MASK);

        if (!topic)
        {
            std::cerr << "create_topic failed." << std::endl;
            return 1;
        }

        // Create the subscriber
        DDS::Subscriber_var sub =
            participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
                                           0, // No listener required
                                           OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        if (!sub)
        {
            std::cerr << "Failed to create_subscriber." << std::endl;
            return 1;
        }

        DDS::DataReaderListener_var listener(new DataReaderListenerImpl);

        DDS::DataReaderQos reader_qos;
        sub->get_default_datareader_qos(reader_qos);
        reader_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

        //Create the Datareader
        DDS::DataReader_var dr = sub->create_datareader(topic,
                                                        reader_qos,
                                                        listener,
                                                        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

        if (!dr)
        {
            std::cerr << "create_datareader failed." << std::endl;
            return 1;
        }

        src::FlamingoDataReader_var reader_i = src::FlamingoDataReader::_narrow(dr);

        if (!reader_i)
        {
            ACE_ERROR_RETURN((LM_ERROR,
                              ACE_TEXT("ERROR: %N:%l: main() -")
                                  ACE_TEXT(" _narrow failed!\n")),
                             1);
        }

        DDS::StatusCondition_var condition = dr->get_statuscondition();
        condition->set_enabled_statuses(DDS::SUBSCRIPTION_MATCHED_STATUS);

        DDS::WaitSet_var ws = new DDS::WaitSet;
        ws->attach_condition(condition);

        while (true)
        {
            DDS::SubscriptionMatchedStatus matches;
            if (dr->get_subscription_matched_status(matches) != DDS::RETCODE_OK)
            {
                ACE_ERROR_RETURN((LM_ERROR,
                                  ACE_TEXT("ERROR: %N:%l: main() -")
                                      ACE_TEXT(" get_subscription_matched_status failed!\n")),
                                 1);
            }

            if (matches.current_count == 0 && matches.total_count > 0)
            {
                break;
            }

            DDS::ConditionSeq conditions;
            DDS::Duration_t timeout = {seconds, 0};
            if (ws->wait(conditions, timeout) != DDS::RETCODE_OK)
            {
                std::cout << "\nData could not be found at ";
                std::cout << "DomainID=" + domainID;
                std::cout << ", Topic name=" + topicName + "" << std::endl;
                /*ACE_ERROR_RETURN((LM_ERROR,
                                  ACE_TEXT("ERROR: %N:%l: main() -")
                                      ACE_TEXT(" wait failed!\n")),
                                 1);*/
            }
        }
        ws->detach_condition(condition);

        // Clean-up!
        participant->delete_contained_entities();
        dpf->delete_participant(participant);

        TheServiceParticipant->shutdown();
    }
    catch (const CORBA::Exception &e)
    {
        e._tao_print_exception("Exception caught in main():");
        return 1;
    }
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
    std::cout << "r: Open to receive data.\n";
    std::cout << "t: Set desired Domain ID.\n";
    std::cout << "u: Set username.\n";
    std::cout << "n: Set desired Topic.\n";
    std::cout << "e: exit subscriber.\n";
}