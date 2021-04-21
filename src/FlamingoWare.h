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
//#include <The-Flamingos/src/SubFlock.h>
//#include <The-Flamingos/src/PubFlock.h>

void cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf, bool logging);
int create_data_writer(DDS::Publisher_var &pub, DDS::Topic_var &topic, DDS::DataWriter_var &writer, bool logging);
int register_type_support(src::FlamingoTypeSupport_var fts, DDS::DomainParticipant_var &participant, CORBA::String_var &type_name, bool logging);
int create_publisher(DDS::Publisher_var &pub, DDS::DomainParticipant_var &participant, bool logging);
int create_participant(DDS::DomainParticipantFactory_var &dpf, int domainID, DDS::DomainParticipant_var &participant, bool logging);
int create_topic(DDS::DomainParticipant_var &participant, std::string topicName, CORBA::String_var type_name, DDS::Topic_var &topic, bool logging);
int create_subscriber(DDS::Subscriber_var &sub, DDS::DomainParticipant_var &participant, bool logging);
int create_data_reader(DDS::Subscriber_var &sub, DDS::Topic_var &topic, DDS::DataReaderQos &reader_qos,
                       DDS::DataReaderListener_var &listener, DDS::DataReader_var &dr, bool logging);
int send(DDS::DataWriter_var &writer, int seconds, int num_of_messages, src::FlamingoDataWriter_var &flamingo_writer, src::Flamingo flamingo, bool logging);
/*
int send(PubFlock flock);
int register_flock_as_sub(SubFlock flock);
int register_flock_as_pub(PubFlock flock);
void cleanup(SubFlock flock);
void cleanup(PubFlock flock);
*/
