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

#include <The-Flamingos\src\FlamingoTypeSupportImpl.h>

//#include "MessengerTypeSupportImpl.h"

//#include "MessageTypeSupportImpl.h"

int main(int argc, char *argv[])
{
    //INITIALIZING THE PARTICIPANT

    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
    DDS::DomainParticipant_var participant = dpf->create_participant(42, //domain ID
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

    //Messenger::MessageTypeSupport_var mts = new Messenger::MessageTypeSupportImpl();

    if (DDS::RETCODE_OK != mts->register_type(participant, ""))
    {
        std::cerr << "register_type failed." << std::endl;
        return 1;
    }

    CORBA::String_var type_name = mts->get_type_name();
    DDS::Topic_var topic = participant->create_topic("Movie Discussion List",
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

    Messenger::MessageDataWriter_var message_writer = Messenger::MessageDataWriter::_narrow(writer);

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
        DDS::Duration_t timeout = {60, 0};
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
    Messenger::Message message;
    message.subject_id = 99;
    message.from = "Comic Book Guy";
    message.subject = "Review";
    message.text = "Worst. Movie. Ever.";
    message.count = 0;
    for (int i = 0; i < 10; i++)
    {
        DDS::ReturnCode_t error = message_writer->write(message, DDS::HANDLE_NIL);
        message.count++;
        message.subject_id++;
        if (error != DDS::RETCODE_OK)
        {
            // Log or otherwise handle the error condition
            return 1;
        }
    }

    //dev guide ends here I believe
}