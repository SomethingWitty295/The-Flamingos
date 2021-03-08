//============================================================================
// Name        : Subscriber_se.cpp
// Author      : Tavien Nelson, Cameron Banff
// Version     :1.0.0
// Copyright   : Your copyright notice
// Description : Subscriber file for communicating with Publisher
//============================================================================
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsSubscriptionC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include <dds/DCPS/StaticIncludes.h>
#ifdef ACE_AS_STATIC_LIBS
#  include <dds/DCPS/RTPS/RtpsDiscovery.h>
#  include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif

#include "DataReaderListenerImpl.h"
#include "MessengerTypeSupportImpl.h"
#include <iostream>
using namespace std;

int main() {

	return 0;
}
