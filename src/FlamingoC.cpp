// -*- C++ -*-
// $Id$

/**
 * Code generated by the The ACE ORB (TAO) IDL Compiler v2.2a_p19
 * TAO and the TAO IDL Compiler have been developed by:
 *       Center for Distributed Object Computing
 *       Washington University
 *       St. Louis, MO
 *       USA
 *       http://www.cs.wustl.edu/~schmidt/doc-center.html
 * and
 *       Distributed Object Computing Laboratory
 *       University of California at Irvine
 *       Irvine, CA
 *       USA
 * and
 *       Institute for Software Integrated Systems
 *       Vanderbilt University
 *       Nashville, TN
 *       USA
 *       http://www.isis.vanderbilt.edu/
 *
 * Information about TAO is available at:
 *     http://www.cs.wustl.edu/~schmidt/TAO.html
 **/

// TAO_IDL - Generated from
// C:\Users\Tavien\Desktop\OpenDDS-3.16\ace_wrappers\tao\TAO_IDL\be\be_codegen.cpp:376


#include "FlamingoC.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
#include "FlamingoC.inl"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// C:\Users\Tavien\Desktop\OpenDDS-3.16\ace_wrappers\tao\TAO_IDL\be\be_visitor_structure\cdr_op_cs.cpp:52

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

::CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const src::Flamingo &_tao_aggregate)
{
  return
    (strm << _tao_aggregate.dateAndTime.in ()) &&
    (strm << _tao_aggregate.name.in ()) &&
    (strm << _tao_aggregate.subject.in ()) &&
    (strm << _tao_aggregate.data) &&
    (strm << _tao_aggregate.daysInCurrentMonth);
}

::CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    src::Flamingo &_tao_aggregate)
{
  return
    (strm >> _tao_aggregate.dateAndTime.out ()) &&
    (strm >> _tao_aggregate.name.out ()) &&
    (strm >> _tao_aggregate.subject.out ()) &&
    (strm >> _tao_aggregate.data) &&
    (strm >> _tao_aggregate.daysInCurrentMonth);
}

TAO_END_VERSIONED_NAMESPACE_DECL



