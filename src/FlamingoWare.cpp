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
#include <The-Flamingos/src/SubFlock.h>
#include <The-Flamingos/src/PubFlock.h>
#include "FlamingoWare.h"

src::FlamingoTypeSupport_var fts = new src::FlamingoTypeSupportImpl();

bool logging;

void setLogging(bool setting)
{
    logging = setting;
}

////////////////////////////////////////////////////////////////////////////
/** Section 0: Flock Functions */
////////////////////////////////////////////////////////////////////////////

/** Register a Publisher Flock for publishing samples with given domain ID and topic.*/
void registerPubFlock(PubFlock &flock)
{
    registerPub(flock.dpf, flock._participant, flock.domainID, flock._typeName,
                flock.topicName, flock._topic, flock._pub, flock._writer, flock._flamingoWriter);
}
/** Register a Subscriber Flock for receiving samples with given domain ID and topic.*/
void registerSubFlock(SubFlock &flock)
{
    registerSub(flock.dpf, flock._participant, flock.domainID, flock._typeName, flock.topicName,
                flock._topic, flock._sub, flock._freader, flock.listener, flock._dr);
}

/** Cleanup Publisher Flock nested processes, usually used before terminating program.*/
void cleanupPubFlock(PubFlock &flock)
{
    cleanup(flock._participant, flock.dpf, logging);
}
/** Cleanup Subscriber Flock nested processes, usually used before terminating program.*/
void cleanupSubFlock(SubFlock &flock)
{
    cleanup(flock._participant, flock.dpf, logging);
}

/** Send Flamingo samples to the appropriate domain and topic given the Flock.*/
int sendFlock(PubFlock &flock)
{
    return send(flock._writer, 10, 1, flock._flamingoWriter, flock.flamingo, logging);
}

////////////////////////////////////////////////////////////////////////////
/** Section 1: All generic OpenDDS functions used by BOTH Pub & Sub. */
////////////////////////////////////////////////////////////////////////////
void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf, bool logging)
{
    participant->delete_contained_entities();
    if (logging)
    {
        std::cout << "LOG: All participant entities have been deleted.\n";
    }

    dpf->delete_participant(participant);
    if (logging)
    {
        std::cout << "LOG: Participant has been deleted via the current DPF.\n";
    }

    TheServiceParticipant->shutdown();
    if (logging)
    {
        std::cout << "LOG: DPF shutdown() is complete.\n";
    }
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

////////////////////////////////////////////////////////////////////////////
/** Section 2: All Publisher specific functions. */
////////////////////////////////////////////////////////////////////////////

void registerPub(DDS::DomainParticipantFactory_var &dpf, DDS::DomainParticipant_var &participant,
                 int &domainID, CORBA::String_var &type_name,
                 std::string &topicName, DDS::Topic_var &topic, DDS::Publisher_var &pub, DDS::DataWriter_var &writer,
                 src::FlamingoDataWriter_var &flamingo_writer)
{
    create_participant(dpf, domainID, participant, logging);
    register_type_support(fts, participant, type_name, logging);
    create_topic(participant, topicName, type_name, topic, logging);
    create_publisher(pub, participant, logging);
    create_data_writer(pub, topic, writer, logging);
    flamingo_writer = src::FlamingoDataWriter::_narrow(writer);
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
            std::cerr << "LOG: create_publisher: publisher successfully created with current participant.\n";
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

////////////////////////////////////////////////////////////////////////////
/** Section 3: All Subscriber specific functions. */
////////////////////////////////////////////////////////////////////////////

void registerSub(DDS::DomainParticipantFactory_var &dpf, DDS::DomainParticipant_var &participant,
                 int &domainID, CORBA::String_var &type_name, std::string &topicName, DDS::Topic_var &topic,
                 DDS::Subscriber_var &sub, src::FlamingoDataReader_var &reader_i, DDS::DataReaderListener_var &listener,
                 DDS::DataReader_var &dr)
{
    create_participant(dpf, domainID, participant, logging);
    register_type_support(fts, participant, type_name, logging);
    create_topic(participant, topicName, type_name, topic, logging);
    create_subscriber(sub, participant, logging);
    DDS::DataReaderQos reader_qos;
    sub->get_default_datareader_qos(reader_qos);
    reader_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
    create_data_reader(sub, topic, reader_qos, listener, dr, logging);
    reader_i = src::FlamingoDataReader::_narrow(dr);
}

int create_subscriber(DDS::Subscriber_var &sub, DDS::DomainParticipant_var &participant, bool logging)
{
    // Create the subscriber
    sub =
        participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
                                       0, // No listener required
                                       OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    if (!sub)
    {
        std::cerr << "create_subscriber failed." << std::endl;
        return 1;
    }
    else
    {
        if (logging)
        {
            std::cerr << "LOG: create_subscriber: subscriber successfully created with current participant.";
        }
    }
    return 0;
}

int create_data_reader(DDS::Subscriber_var &sub, DDS::Topic_var &topic, DDS::DataReaderQos &reader_qos,
                       DDS::DataReaderListener_var &listener, DDS::DataReader_var &dr, bool logging)
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
    else
    {
        if (logging)
        {
            std::cerr << "LOG: create_data_reader: successfully created with topic.";
        }
    }
    return 0;
}
