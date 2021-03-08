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

#include "MessengerTypeSupportImpl.h"

int main(int argc, char *argv[])
{
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

    Messenger::MessageTypeSupport_var mts = new Messenger::MessageTypeSupportImpl();
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

    DDS::Publisher_var pub = participant->create_publisher(PUBLISHER_QOS_DEFAULT,
                                                           0,
                                                           OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!pub)
    {
        std::cerr << "create_publisher failed." << std::endl;
        return 1;
    }
}