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

#pragma once
struct PubFlock
{
    DDS::DomainParticipantFactory_var dpf;
    DDS::DomainParticipant_var participant;
    DDS::Topic_var topic;
    CORBA::String_var type_name;
    int domainID;
    std::string topicName;

    // Publisher Specific
    DDS::Publisher_var pub;
    DDS::DataWriter_var writer;
    src::FlamingoDataWriter_var flamingo_writer;
    src::Flamingo flamingo;
};