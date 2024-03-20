/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/
  
  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
*/

#include "painlessMesh.h"

#define   MESH_PREFIX     "simpopus"
#define   MESH_PASSWORD   "DoNotMessWithMeGirl"
// #define   MESH_PREFIX     "whateverYouLike"
// #define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define NODE_TYPE_SWITCH
// #define NODE_TYPE_LIGHT

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

bool lightOn = false;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void readSwitches() ; // Prototype so PlatformIO doesn't complain
void turnOn() ; // Prototype so PlatformIO doesn't complain
void turnOff() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
Task taskReadSwitches( TASK_MILLISECOND * 10 , TASK_FOREVER, &readSwitches );
Task taskTurnOn( TASK_SECOND * 1 , TASK_FOREVER, &turnOn );
Task taskTurnOff( TASK_SECOND * 1 , TASK_FOREVER, &turnOff );

void sendMessage() {
  String msg = "Hi from node1";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

void readSwitches() {
  if(digitalRead(D1) == LOW) {
    // lightOn = true;
    taskTurnOn.enableIfNot();
    taskTurnOff.disable();
  } else {
    // lightOn = false;
    taskTurnOff.enableIfNot();
    taskTurnOn.disable();
  }
}

void turnOnNode(int nodeId) {
  String msg = "Get to work!!";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  // taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  // taskTurnOn.disable();
}

void turnOn() {
  turnOnNode(1);
}

void turnOffNode(int nodeId) {
  String msg = "Take a Nap!!";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  // taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  // taskTurnOff.disable();
}

void turnOff() {
  turnOffNode(1);
}


// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("pineapple: Received from %u msg=%s\n", from, msg.c_str());
}

void receivedCallbackEcho( uint32_t from, String &msg ) {
  Serial.printf("echoNode: Received from %u msg=%s\n", from, msg.c_str());
  String testMsg = "Switch is Now On";
  mesh.sendSingle(from, testMsg);
  mesh.sendSingle(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> pineapple: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

  pinMode(D1, INPUT_PULLUP);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  // userScheduler.addTask( taskSendMessage );
  // taskSendMessage.enable();
  userScheduler.addTask( taskReadSwitches);
  userScheduler.addTask( taskTurnOn );
  userScheduler.addTask( taskTurnOff );
  taskReadSwitches.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  // if (digitalRead(D1) == HIGH) {
  //   lightOn = true;
  // } else {
  //   lightOn = false;
  // }
}