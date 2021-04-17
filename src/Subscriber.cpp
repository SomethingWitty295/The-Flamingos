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
//
int create_data_reader(DDS::Subscriber_var &sub, DDS::Topic_var &topic, DDS::DataReaderQos &reader_qos,
                       DDS::DataReaderListener_var &listener, DDS::DataReader_var &dr);
int create_subscriber(DDS::Subscriber_var &sub, DDS::DomainParticipant_var &participant);
int create_topic(DDS::DomainParticipant_var &participant, std::string &topicName, CORBA::String_var &type_name, DDS::Topic_var &topic);
int register_type_support(DDS::DomainParticipant_var &participant, src::FlamingoTypeSupport_var &fts, CORBA::String_var &type_name);
int create_participant(DDS::DomainParticipant_var &participant, int domainID, DDS::DomainParticipantFactory_var &dpf);
void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf);

int main(int argc, char *argv[])
{
    int domainID;
    bool logging = false;

    string username = "Default";
    string topicName = "Default";
    ////

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

    // These are never changed, so establish them here.
    src::FlamingoTypeSupport_var fts = new src::FlamingoTypeSupportImpl();
    CORBA::String_var type_name;

    // All our various DDS variables for participant, etc.
    DDS::DomainParticipant_var participant;
    DDS::Topic_var topic;
    DDS::Subscriber_var sub;
    DDS::DataReaderListener_var listener(new DataReaderListenerImpl);
    DDS::DataReader_var dr;

    // Through multiple function calls that change the variable you give it,
    // we setup our domain participant.
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

    while (true)
    {
        printInstructions(domainID, topicName);

        char input;

        std::cin >> input;

        switch (input)
        {
        case 'e':
            cleanup(participant, dpf);
            return 0;
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

void printInstructions(int domainID, string topic)
{
    std::cout << "\n-------------------------.\n";
    std::cout << "domain ID: " << domainID;
    std::cout << " | ";
    std::cout << "topic name: " + topic;
    std::cout << "\n-------------------------.\n";
    //std::cout << "r: Open to receive data.\n";
    //std::cout << "d: Set desired Domain ID.\n";
    //std::cout << "t: Set desired Topic.\n";
    std::cout << "l: Toggle logging.\n";
    std::cout << "e: Exit.\n";
}