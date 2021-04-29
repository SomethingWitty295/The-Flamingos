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

#pragma once
struct PubFlock
{

    //Flamingo struct specific for this publisher
    src::Flamingo flamingo;
    //Domain participant factory required for creating DDS relevant connectivity.
    DDS::DomainParticipantFactory_var dpf;
    //Domain ID corresponding to this publisher
    int domainID;
    //Topic name corresponding to this publisher
    std::string topicName;

    DDS::DomainParticipant_var _participant;
    DDS::Topic_var _topic;
    CORBA::String_var _typeName;

    // Publisher Specific variables

    DDS::Publisher_var _pub;
    DDS::DataWriter_var _writer;
    src::FlamingoDataWriter_var _flamingoWriter;
};