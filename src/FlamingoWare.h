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

void setLogging(bool setting);

// All Generically used functions (both sub & pub)

void _cleanup(DDS::DomainParticipant_var &participant, DDS::DomainParticipantFactory_var &dpf, bool logging);
int _register_type_support(src::FlamingoTypeSupport_var fts, DDS::DomainParticipant_var &participant, CORBA::String_var &type_name, bool logging);
int _create_participant(DDS::DomainParticipantFactory_var &dpf, int domainID, DDS::DomainParticipant_var &participant, bool logging);
int _create_topic(DDS::DomainParticipant_var &participant, std::string topicName, CORBA::String_var type_name, DDS::Topic_var &topic, bool logging);

int _send(DDS::DataWriter_var &writer, src::FlamingoDataWriter_var &flamingo_writer, src::Flamingo flamingo, bool logging);

// All Publisher specific functions

void _registerPub(DDS::DomainParticipantFactory_var &dpf, DDS::DomainParticipant_var &participant,
                  int &domainID, CORBA::String_var &type_name,
                  std::string &topicName, DDS::Topic_var &topic, DDS::Publisher_var &pub, DDS::DataWriter_var &writer,
                  src::FlamingoDataWriter_var &flamingo_writer);
int _create_publisher(DDS::Publisher_var &pub, DDS::DomainParticipant_var &participant, bool logging);
int _create_data_writer(DDS::Publisher_var &pub, DDS::Topic_var &topic, DDS::DataWriter_var &writer, bool logging);

// All Subscriber specific functions

int _create_subscriber(DDS::Subscriber_var &sub, DDS::DomainParticipant_var &participant, bool logging);
int _create_data_reader(DDS::Subscriber_var &sub, DDS::Topic_var &topic, DDS::DataReaderQos &reader_qos,
                        DDS::DataReaderListener_var &listener, DDS::DataReader_var &dr, bool logging);

void _registerSub(DDS::DomainParticipantFactory_var &dpf, DDS::DomainParticipant_var &participant,
                  int &domainID, CORBA::String_var &type_name, std::string &topicName, DDS::Topic_var &topic,
                  DDS::Subscriber_var &sub, src::FlamingoDataReader_var &reader_i, DDS::DataReaderListener_var &listener,
                  DDS::DataReader_var &dr);

// All functions handling a flock via parameter

void cleanupPubFlock(PubFlock &flock);
void cleanupSubFlock(SubFlock &flock);
void registerPubFlock(PubFlock &flock);
void registerSubFlock(SubFlock &flock);
int sendFlock(PubFlock &flock);
