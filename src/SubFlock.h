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

#pragma once
struct SubFlock
{
    //Domain participant factory required for creating DDS relevant connectivity.
    DDS::DomainParticipantFactory_var dpf;
    //Domain ID corresponding to this subscriber
    int domainID;
    //Topic name corresponding to this subscriber
    std::string topicName;

    DDS::DataReaderListener_var _listener = new DataReaderListenerImpl;
    DDS::DomainParticipant_var _participant;
    DDS::Topic_var _topic;
    CORBA::String_var _typeName;

    // Subscriber Specific

    DDS::Subscriber_var _sub;
    src::FlamingoDataReader_var _freader;
    DDS::DataReader_var _dr;
};