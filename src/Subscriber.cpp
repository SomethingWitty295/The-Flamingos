// Author: Tavien Nelson, Cameron Banff
// Version 1.0.0
// Description: Subscriber file to communicate with the Publisher

#include <ace/Log_Msg.h>

#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsSubscriptionC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include <dds/DCPS/StaticIncludes.h>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/RTPS/RtpsDiscovery.h>
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif

#include "DataReaderListenerImpl.h"
#include "MessengerTypeSupportImpl.h"

int main(int argc, char* argv[])
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
	if (DDS::RETCODE_OK != mts->register_type(participant, "")) {
	std::cerr << "Failed to register the MessageTypeSupport." << std::endl;
		return 1;
	}

	COBRA::String_var type_name = mts->get_type_name();

	DDS::Topic_var topic =
		participant->create_topic("Movie Discussion List",
			type_name,
			TOPIC_QOS_DEFAULT
			0,	// No listener required
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if (!topic) {
		std::cerr << "Failed to create_topic." << std::endl;
		return 1;
	}

	// Create the subscriber
	DDS::Subscriber_var sub =
		participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
									   0,	// No listener required
									   OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if (!sub) {
	std:cerr << "Failed to create_subscriber." << std::endl;
		return 1;
	}

	DDS::DataReaderListener_var listener(new DataReaderListenerImpl);

	// Create the Datareader
	DDS::DataReader var dr = sub->create_datareader(topic,
													DATAREADER_QOS_DEFAULT,
													listener
													OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if (!dr) {
		std::cerr << "create_datareader failed." << std::endl;
		return 1;
	}




}
