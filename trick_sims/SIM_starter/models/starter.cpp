#include <cstdio>
#include "starter.hh"
#include "trick/integrator_c_intf.h"
#include "trick/exec_proto.h"
#include "trick/message_proto.h"

Starter::Starter() {

}

int Starter::default_data() {
  return 0;
}

int Starter::init() {
  return 0;
}


int Starter::scheduled() {
  // message_publish(MSG_NORMAL, "Hello World!\n");
  return 0;
}
int Starter::deriv() {
  return 0;
}

int Starter::integ() {

  int ipass;
  load_state( 
    NULL /* list is NULL terminated */
  );

   /* LOAD THE POSITION AND VELOCITY STATE DERIVATIVES */
  load_deriv(
    NULL /* list is NULL terminated */
  );

   /* Call the Trick integrate service */
   ipass = integrate();

   /* unload new state */
  unload_state(
    NULL /* list is NULL terminated */
  );

  /* returns 0 if integerate() was successful */
  return ipass;
}

int Starter::shutdown() {
  return 0;
}
