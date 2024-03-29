/* -*- C++ -*- */
/* Generated by ..\..\bin\opendds_idl version 3.16 (ACE version 6.2a_p19) running on input file Flamingo.idl */
#ifndef OPENDDS_IDL_GENERATED_FLAMINGOTYPESUPPORTIMPL_H_EPL5M4
#define OPENDDS_IDL_GENERATED_FLAMINGOTYPESUPPORTIMPL_H_EPL5M4
#include "FlamingoC.h"
#include "dds/DCPS/Definitions.h"
#include "dds/DdsDcpsC.h"
#include "dds/Version.h"
#if DDS_MAJOR_VERSION != 3 || DDS_MINOR_VERSION != 16 || DDS_MICRO_VERSION != 0
#error This file should be regenerated with opendds_idl
#endif
#include "FlamingoTypeSupportC.h"
#include "dds/DCPS/Serializer.h"
#include "dds/DCPS/TypeSupportImpl.h"
#include "dds/DCPS/ValueWriter.h"
#include "dds/DCPS/XTypes/TypeObject.h"
OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL



/* Begin MODULE: CORBA */


/* End MODULE: CORBA */


/* Begin MODULE: OpenDDS */


/* End MODULE: OpenDDS */


/* Begin MODULE: src */



/* Begin STRUCT: Flamingo */

OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

void set_default(src::Flamingo& stru);

void serialized_size(const Encoding& encoding, size_t& size, const src::Flamingo& stru);

bool operator<<(Serializer& strm, const src::Flamingo& stru);

bool operator>>(Serializer& strm, src::Flamingo& stru);

void serialized_size(const Encoding& encoding, size_t& size, const NestedKeyOnly<const src::Flamingo>& stru);

bool operator<<(Serializer& strm, const NestedKeyOnly<const src::Flamingo>& stru);

bool operator>>(Serializer& strm, const NestedKeyOnly<src::Flamingo>& stru);

void serialized_size(const Encoding& encoding, size_t& size, const KeyOnly<const src::Flamingo>& stru);

bool operator<<(Serializer& strm, const KeyOnly<const src::Flamingo>& stru);

bool operator>>(Serializer& strm, const KeyOnly<src::Flamingo>& stru);

template <>
struct MarshalTraits<src::Flamingo> {
  static void representations_allowed_by_type(DDS::DataRepresentationIdSeq& seq)
  {
    seq.length(3);
    seq[0] = DDS::XCDR_DATA_REPRESENTATION;
    seq[1] = DDS::XCDR2_DATA_REPRESENTATION;
    seq[2] = DDS::XML_DATA_REPRESENTATION;
  }

  static SerializedSizeBound serialized_size_bound(const Encoding& encoding)
  {
    switch (encoding.kind()) {
    case Encoding::KIND_UNALIGNED_CDR:
      return SerializedSizeBound();
    case Encoding::KIND_XCDR1:
      return SerializedSizeBound();
    case Encoding::KIND_XCDR2:
      return SerializedSizeBound();
    default:
      OPENDDS_ASSERT(false);
      return SerializedSizeBound();
    }
  }

  static SerializedSizeBound key_only_serialized_size_bound(const Encoding& encoding)
  {
    switch (encoding.kind()) {
    case Encoding::KIND_UNALIGNED_CDR:
      return SerializedSizeBound(0);
    case Encoding::KIND_XCDR1:
      return SerializedSizeBound();
    case Encoding::KIND_XCDR2:
      return SerializedSizeBound();
    default:
      OPENDDS_ASSERT(false);
      return SerializedSizeBound();
    }
  }

  static bool to_message_block(ACE_Message_Block&, const src::Flamingo&) { return false; }
  static bool from_message_block(src::Flamingo&, const ACE_Message_Block&) { return false; }
  static Extensibility extensibility() { return APPENDABLE; }
  static Extensibility max_extensibility_level() { return APPENDABLE; }
};
} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL


namespace src {
/// This structure supports use of std::map with one or more keys.
struct  Flamingo_OpenDDS_KeyLessThan {
  bool operator()(const src::Flamingo&, const src::Flamingo&) const
  {
    // With no keys, return false to allow use of
    // map with just one entry
    return false;
  }
};
}

OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

void vwrite(OpenDDS::DCPS::ValueWriter& value_writer, const src::Flamingo& value);

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL


namespace src {
class FlamingoTypeSupportImpl;
}

OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS {
namespace DCPS {
template <>
struct DDSTraits<src::Flamingo> {
  typedef src::Flamingo MessageType;
  typedef src::FlamingoSeq MessageSequenceType;
  typedef src::FlamingoTypeSupport TypeSupportType;
  typedef src::FlamingoTypeSupportImpl TypeSupportTypeImpl;
  typedef src::FlamingoDataWriter DataWriterType;
  typedef src::FlamingoDataReader DataReaderType;
  typedef src::Flamingo_OpenDDS_KeyLessThan LessThanType;
  typedef OpenDDS::DCPS::KeyOnly<const src::Flamingo> KeyOnlyType;

  static const char* type_name() { return "src::Flamingo"; }
  static bool gen_has_key() { return false; }
  static size_t key_count() { return 0; }
};
} // namespace DCPS
} // namespace OpenDDS
OPENDDS_END_VERSIONED_NAMESPACE_DECL


namespace src {
class  FlamingoTypeSupportImpl
  : public virtual OpenDDS::DCPS::LocalObject<FlamingoTypeSupport>
  , public virtual OpenDDS::DCPS::TypeSupportImpl
{
public:
  typedef OpenDDS::DCPS::DDSTraits<Flamingo> TraitsType;
  typedef OpenDDS::DCPS::MarshalTraits<Flamingo> MarshalTraitsType;
  typedef FlamingoTypeSupport TypeSupportType;
  typedef FlamingoTypeSupport::_var_type _var_type;
  typedef FlamingoTypeSupport::_ptr_type _ptr_type;

  FlamingoTypeSupportImpl() {}
  virtual ~FlamingoTypeSupportImpl() {}

  virtual ::DDS::DataWriter_ptr create_datawriter();
  virtual ::DDS::DataReader_ptr create_datareader();
#ifndef OPENDDS_NO_MULTI_TOPIC
  virtual ::DDS::DataReader_ptr create_multitopic_datareader();
#endif /* !OPENDDS_NO_MULTI_TOPIC */
#ifndef OPENDDS_NO_CONTENT_SUBSCRIPTION_PROFILE
  virtual const OpenDDS::DCPS::MetaStruct& getMetaStructForType();
#endif /* !OPENDDS_NO_CONTENT_SUBSCRIPTION_PROFILE */
  virtual bool has_dcps_key();
  const char* default_type_name() const;

  void representations_allowed_by_type(::DDS::DataRepresentationIdSeq& seq);

  virtual const OpenDDS::XTypes::TypeIdentifier& getMinimalTypeIdentifier() const;
  virtual const OpenDDS::XTypes::TypeMap& getMinimalTypeMap() const;

  virtual OpenDDS::DCPS::Extensibility getExtensibility() const;

  static FlamingoTypeSupport::_ptr_type _narrow(CORBA::Object_ptr obj);
};
}

#ifndef OPENDDS_NO_CONTENT_SUBSCRIPTION_PROFILE
OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

class MetaStruct;

template<typename T>
const MetaStruct& getMetaStruct();

template<>
const MetaStruct& getMetaStruct<src::Flamingo>();
bool gen_skip_over(Serializer& ser, src::Flamingo*);

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL

#endif
OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

struct src_Flamingo_xtag {};
template<> const XTypes::TypeIdentifier & getMinimalTypeIdentifier<src_Flamingo_xtag>();

template<> const XTypes::TypeMap& getMinimalTypeMap<src_Flamingo_xtag>();

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL


/* End STRUCT: Flamingo */

/* End MODULE: src */
OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL

#endif /* OPENDDS_IDL_GENERATED_FLAMINGOTYPESUPPORTIMPL_H_EPL5M4 */
