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

void printInstructions(int domainID, string topic);
int receiving(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID, string topicName);
//
int receive(DDS::DataReader_var &dr, int seconds, DDS::DomainParticipant_var participant);
int create_data_reader(DDS::Subscriber_var &sub, DDS::Topic_var &topic, DDS::DataReaderQos &reader_qos,
                       DDS::DataReaderListener_var &listener, DDS::DataReader_var &dr);
int create_subscriber(DDS::Subscriber_var &sub, DDS::DomainParticipant_var &participant);
int create_topic(DDS::DomainParticipant_var &participant, std::string &topicName, CORBA::String_var &type_name, DDS::Topic_var &topic);
int register_type_support(DDS::DomainParticipant_var &participant, src::FlamingoTypeSupport_var &fts, CORBA::String_var &type_name);
int create_participant(DDS::DomainParticipant_var &participant, int domainID, DDS::DomainParticipantFactory_var &dpf);
void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf);

int main(int argc, char *argv[])
{
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    int domainID = 42;
    string username = "Default";
    string topicName = "Default";
    int attempt;
    ////
    DDS::DomainParticipant_var participant;
    src::FlamingoTypeSupport_var fts = new src::FlamingoTypeSupportImpl();
    CORBA::String_var type_name;
    DDS::Topic_var topic;
    DDS::Subscriber_var sub;
    DDS::DataReaderListener_var listener(new DataReaderListenerImpl);
    DDS::DataReader_var dr;

    create_participant(participant, domainID, dpf);

    register_type_support(participant, fts, type_name);

    create_topic(participant, topicName, type_name, topic);

    create_subscriber(sub, participant);

    DDS::DataReaderQos reader_qos;
    sub->get_default_datareader_qos(reader_qos);
    reader_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

    create_data_reader(sub, topic, reader_qos, listener, dr);

    src::FlamingoDataReader_var reader_i = src::FlamingoDataReader::_narrow(dr);

    if (!reader_i)
    {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("ERROR: %N:%l: main() -")
                              ACE_TEXT(" _narrow failed!\n")),
                         1);
    }

    ///

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
        printInstructions(domainID, topicName);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 'r':
            std::cout << "Set timeout time in seconds:";
            int seconds;
            std::cin >> seconds;
            std::cout << "\nWaiting...\n";
            //attempt = receiving(seconds, dpf, domainID, topicName);
            //attempt = receive(dr, seconds, participant);
            if (attempt == 0)
            {
                std::cout << "Attempt successful!\n";
                //return 0;
            }
            else
            {
                //std::cout << "Receive attempt failed: Error code: " << attempt << std::endl;
                std::cout << "Receiving ended.\n";
                //return 1;
            }

            break;
        case 'd':
            std::cout << "Enter desired domain ID: ";
            std::cin >> domainID;
            std::cout << "\n";

            //participant->delete_contained_entities();
            //dpf->delete_participant(participant);

            create_participant(participant, domainID, dpf);

            register_type_support(participant, fts, type_name);

            create_topic(participant, topicName, type_name, topic);

            create_subscriber(sub, participant);

            sub->get_default_datareader_qos(reader_qos);
            reader_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;

            create_data_reader(sub, topic, reader_qos, listener, dr);

            reader_i = src::FlamingoDataReader::_narrow(dr);

            if (!reader_i)
            {
                ACE_ERROR_RETURN((LM_ERROR,
                                  ACE_TEXT("ERROR: %N:%l: main() -")
                                      ACE_TEXT(" _narrow failed!\n")),
                                 1);
            }

            break;
        case 't':
            std::cout << "Enter desired topic name:";
            std::cin >> topicName;
            std::cout << "\n";
            break;
        case 'e':
            cleanup(participant, dpf);
            return 0;
        }
    }
}

void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf)
{
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();
}

int create_participant(DDS::DomainParticipant_var &participant, int domainID, DDS::DomainParticipantFactory_var &dpf)
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

int register_type_support(DDS::DomainParticipant_var &participant, src::FlamingoTypeSupport_var &fts, CORBA::String_var &type_name)
{
    if (DDS::RETCODE_OK != fts->register_type(participant, ""))
    {
        std::cerr << "register_type failed." << std::endl;
        return 1;
    }
    type_name = fts->get_type_name();
    return 0;
}

int create_topic(DDS::DomainParticipant_var &participant, std::string &topicName, CORBA::String_var &type_name, DDS::Topic_var &topic)
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

int create_subscriber(DDS::Subscriber_var &sub, DDS::DomainParticipant_var &participant)
{
    // Create the subscriber
    sub =
        participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
                                       0, // No listener required
                                       OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (!sub)
    {
        std::cerr << "Failed to create_subscriber." << std::endl;
        return 1;
    }
    return 0;
}

int create_data_reader(DDS::Subscriber_var &sub, DDS::Topic_var &topic, DDS::DataReaderQos &reader_qos,
                       DDS::DataReaderListener_var &listener, DDS::DataReader_var &dr)
{
    //Create the Datareader
    dr = sub->create_datareader(topic,
                                reader_qos,
                                listener,
                                OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!dr)
    {
        std::cerr << "create_datareader failed." << std::endl;
        return 1;
    }
    return 0;
}

int receive(DDS::DataReader_var &dr, int seconds, DDS::DomainParticipant_var participant)
{
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
            /**ACE_ERROR_RETURN((LM_ERROR,
                              ACE_TEXT("ERROR: %N:%l: main() -")
                                  ACE_TEXT(" wait failed!\n")),
                             1);*/
            std::cout << "Wai finished.\n";
            return 1;
        }
    }
    ws->detach_condition(condition);

    //std::cout << "Hits Line 249!\n";

    //participant->delete_contained_entities();

    return 0;
}

int receiving(int seconds, DDS::DomainParticipantFactory_var dpf, int domainID, string topicName)
{
    try
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

        src::FlamingoTypeSupport_var mts = new src::FlamingoTypeSupportImpl();

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

        ///////////////////////////////////////////////////////////////////////////////

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
                ACE_ERROR_RETURN((LM_ERROR,
                                  ACE_TEXT("ERROR: %N:%l: main() -")
                                      ACE_TEXT(" wait failed!\n")),
                                 1);
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

void printInstructions(int domainID, string topic)
{
    std::cout << "\n-------------------------.\n";
    std::cout << "domain ID: " << domainID;
    std::cout << " | ";
    std::cout << "topic name: " + topic;
    std::cout << "\n-------------------------.\n";
    std::cout << "r: Open to receive data.\n";
    std::cout << "d: Set desired Domain ID.\n";
    std::cout << "t: Set desired Topic.\n";
    std::cout << "e: Exit.\n";
}