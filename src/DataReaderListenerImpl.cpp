/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include <ace/Log_Msg.h>
#include <ace/OS_NS_stdlib.h>

// Original imports from Messenger example.
//#include "DataReaderListenerImpl.h"
//#include "MessengerTypeSupportC.h"
//#include "MessengerTypeSupportImpl.h"

#include "DataReaderListenerImpl.h"
#include "FlamingoTypeSupportC.h"
#include "FlamingoTypeSupportImpl.h"

#include <iostream>

void DataReaderListenerImpl::on_requested_deadline_missed(
    DDS::DataReader_ptr /*reader*/,
    const DDS::RequestedDeadlineMissedStatus & /*status*/)
{
}

void DataReaderListenerImpl::on_requested_incompatible_qos(
    DDS::DataReader_ptr /*reader*/,
    const DDS::RequestedIncompatibleQosStatus & /*status*/)
{
}

void DataReaderListenerImpl::on_sample_rejected(
    DDS::DataReader_ptr /*reader*/,
    const DDS::SampleRejectedStatus & /*status*/)
{
}

void DataReaderListenerImpl::on_liveliness_changed(
    DDS::DataReader_ptr /*reader*/,
    const DDS::LivelinessChangedStatus & /*status*/)
{
}

void DataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
  src::FlamingoDataReader_var reader_i = src::FlamingoDataReader::_narrow(reader);

  if (!reader_i)
  {
    // print error here!
    exit(1);
  }

  src::Flamingo flamingo;
  DDS::SampleInfo info;

  DDS::ReturnCode_t error = reader_i->take_next_sample(flamingo, info);

  if (error == DDS::RETCODE_OK)
  {
    std::cout << "SampleInfo.sample_rank = " << info.sample_rank << std::endl;
    std::cout << "SampleInfo.instance_state = " << info.instance_state << std::endl;

    if (info.valid_data)
    {
      std::cout << "date/time: " << flamingo.dateAndTime.in() << std::endl
                << "name: " << flamingo.name.in() << std::endl
                << "subject: " << flamingo.subject.in() << std::endl
                << "generic data: " << flamingo.data << std::endl
                << "number for days in month: " << flamingo.daysInCurrentMonth << std::endl;
    }
  }
  else
  {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
                   ACE_TEXT(" take_next_sample failed!\n")));
  }
}

void DataReaderListenerImpl::on_subscription_matched(
    DDS::DataReader_ptr /*reader*/,
    const DDS::SubscriptionMatchedStatus & /*status*/)
{
}

void DataReaderListenerImpl::on_sample_lost(
    DDS::DataReader_ptr /*reader*/,
    const DDS::SampleLostStatus & /*status*/)
{
}
