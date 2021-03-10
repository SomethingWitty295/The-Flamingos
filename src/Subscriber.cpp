// Author: Tavien Nelson, Cameron Banff
// Version 1.0.0
// Description: Subsriber file to communicate with the Publisher

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
}
