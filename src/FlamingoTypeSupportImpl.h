/* -*- C++ -*- */
/* Generated by C:\Users\Corey\Desktop\OpenDDS-3.16\bin\opendds_idl version 3.16 (ACE version 6.2a_p19) running on input file Flamingo.idl */
#ifndef OPENDDS_IDL_GENERATED_FLAMINGOTYPESUPPORTIMPL_H_J7IUXO
#define OPENDDS_IDL_GENERATED_FLAMINGOTYPESUPPORTIMPL_H_J7IUXO
#include "FlamingoC.h"
#include "dds/DCPS/Definitions.h"
#include "dds/DdsDcpsC.h"
#include "dds/Version.h"
#if DDS_MAJOR_VERSION != 3 || DDS_MINOR_VERSION != 16 || DDS_MICRO_VERSION != 0
#error This file should be regenerated with opendds_idl
#endif
#include "dds/DCPS/Serializer.h"
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

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL

OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL
namespace OpenDDS { namespace DCPS {

void vwrite(OpenDDS::DCPS::ValueWriter& value_writer, const src::Flamingo& value);

} }
OPENDDS_END_VERSIONED_NAMESPACE_DECL

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

#endif /* OPENDDS_IDL_GENERATED_FLAMINGOTYPESUPPORTIMPL_H_J7IUXO */
