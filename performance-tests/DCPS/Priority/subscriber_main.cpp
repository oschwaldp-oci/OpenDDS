// -*- C++ -*-
// $Id$

#include "Test.h"
#include "Options.h"
#include "Commas.h"
#include "Subscriber.h"
#include "dds/DCPS/Service_Participant.h"

#include <iomanip>
#include <sstream>

int
main( int argc, char *argv[])
{
  int result = -1;

  try {
    // Initialize DDS.
    TheParticipantFactoryWithArgs( argc, argv);

    // Initialize the test.
    const Test::Options options( argc, argv);

    // Create the subscriber thingie.
    Test::Subscriber subscriber( options);

    if( options.verbose()) {
      std::stringstream buffer;
      buffer << options.transportType();
      ACE_DEBUG((LM_DEBUG,
        ACE_TEXT("(%P|%t) subscriber_main() - ")
        ACE_TEXT("started with transport %s(%d).\n"),
        buffer.str().c_str(),
        options.transportKey()
      ));
    }

    // Execute the test.
    subscriber.run();

    std::stringstream buffer;
    buffer.fill( '0');
    for( std::map< long, long>::const_iterator current = subscriber.counts().begin();
         current != subscriber.counts().end();
         ++current
       ) {
      // Utility iterator into data maps.
      std::map< long, long>::const_iterator where;

      // Find the priority for this writer.
      long priority = -1;
      where = subscriber.priorities().find( current->first);
      if( where != subscriber.priorities().end()) {
        priority = where->second;
      }

      // Find the byte count for this writer.
      long bytes = -1;
      where = subscriber.bytes().find( current->first);
      if( where != subscriber.bytes().end()) {
        bytes = where->second;
      }

      // Form the message for this writer.
      buffer << "   writer[ 0x" << std::hex << current->first << "] == "
             << Commas( current->second) << " samples / "
             << Commas( bytes) << " bytes recieved at priority "
             << std::dec << priority << "." << std::endl;

    }
    buffer << "Total messages received: " << std::dec << subscriber.total_messages() << std::endl;
    buffer << "Valid messages received: " << std::dec << subscriber.valid_messages() << std::endl;
    buffer << subscriber << std::endl;
    ACE_DEBUG((LM_DEBUG,
      ACE_TEXT("(%P|%t) subscriber_main() - ")
      ACE_TEXT("test over:\n%s"),
      buffer.str().c_str()
    ));

    // Test passes if there were no exceptions.
    result = 0;

  } catch( CORBA::Exception& /* e */) {
    ACE_ERROR((LM_ERROR,
      ACE_TEXT("(%P|%t) subscriber_main() - ")
      ACE_TEXT("CORBA exception caught during processing.\n")
    ));

  } catch( Test::Exception e)  {
    ACE_ERROR((LM_ERROR,
      ACE_TEXT("(%P|%t) subscriber_main() - ")
      ACE_TEXT("Test exception caught during processing: %s.\n"),
      e.what()
    ));

  }

  return result;
}

