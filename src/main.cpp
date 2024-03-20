/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-mesh-esp32-esp8266-painlessmesh/

  This is a simple example that uses the painlessMesh library: https://github.com/gmag11/painlessMesh/blob/master/examples/basic/basic.ino
*/

#include "painlessMesh.h"

// #define NODE_TYPE_SWITCH
#define NODE_TYPE_LIGHT

#define MESH_PREFIX "simpopus"
#define MESH_PASSWORD "DoNotMessWithMeGirl"
// #define   MESH_PREFIX     "whateverYouLike"
// #define   MESH_PASSWORD   "somethingSneaky"
#define MESH_PORT 5555

#define ON_MESSAGE "Get to work!!"
#define OFF_MESSAGE "Take a Nap!!"

#define NODE_1_ID "3814000448"
#define NODE_ID_1 NODE_1_ID "_1"
#define NODE_ID_2 NODE_1_ID "_2"
#define NODE_ID_3 NODE_1_ID "_3"

#ifdef NODE_TYPE_SWITCH
#define SWITCH_1_PIN D3
#define SWITCH_2_PIN D2
#define SWITCH_3_PIN D1
#endif

#ifdef NODE_TYPE_LIGHT
#define LIGHT_1_PIN D1
#define LIGHT_2_PIN D2
#define LIGHT_3_PIN D3
#endif

Scheduler userScheduler;  // to control your personal task
painlessMesh mesh;

#ifdef NODE_TYPE_SWITCH
// User stub
void readSwitches();  // Prototype so PlatformIO doesn't complain
// void turnOn() ; // Prototype so PlatformIO doesn't complain
void turnOnNode1();  // Prototype so PlatformIO doesn't complain
void turnOnNode2();  // Prototype so PlatformIO doesn't complain
void turnOnNode3();  // Prototype so PlatformIO doesn't complain
// void turnOff() ; // Prototype so PlatformIO doesn't complain
void turnOffNode1();  // Prototype so PlatformIO doesn't complain
void turnOffNode2();  // Prototype so PlatformIO doesn't complain
void turnOffNode3();  // Prototype so PlatformIO doesn't complain

Task taskReadSwitches(TASK_MILLISECOND * 10, TASK_FOREVER, &readSwitches);
// Task taskTurnOn( TASK_SECOND * 1 , TASK_FOREVER, &turnOn );
Task taskTurnOnNode1(TASK_SECOND * 1, TASK_FOREVER, &turnOnNode1);
Task taskTurnOnNode2(TASK_SECOND * 1, TASK_FOREVER, &turnOnNode2);
Task taskTurnOnNode3(TASK_SECOND * 1, TASK_FOREVER, &turnOnNode3);
// Task taskTurnOff( TASK_SECOND * 1 , TASK_FOREVER, &turnOff );
Task taskTurnOffNode1(TASK_SECOND * 1, TASK_FOREVER, &turnOffNode1);
Task taskTurnOffNode2(TASK_SECOND * 1, TASK_FOREVER, &turnOffNode2);
Task taskTurnOffNode3(TASK_SECOND * 1, TASK_FOREVER, &turnOffNode3);

void readSwitches() {
    if (digitalRead(SWITCH_1_PIN) == LOW) {
        // lightOn = true;
        taskTurnOnNode1.enableIfNot();
        taskTurnOffNode1.disable();
    } else {
        // lightOn = false;
        taskTurnOffNode1.enableIfNot();
        taskTurnOnNode1.disable();
    }
    if (digitalRead(SWITCH_2_PIN) == LOW) {
        // lightOn = true;
        taskTurnOnNode2.enableIfNot();
        taskTurnOffNode2.disable();
    } else {
        // lightOn = false;
        taskTurnOffNode2.enableIfNot();
        taskTurnOnNode2.disable();
    }
    if (digitalRead(SWITCH_3_PIN) == LOW) {
        // lightOn = true;
        taskTurnOnNode3.enableIfNot();
        taskTurnOffNode3.disable();
    } else {
        // lightOn = false;
        taskTurnOffNode3.enableIfNot();
        taskTurnOnNode3.disable();
    }
}

void turnOnNode(String nodeId) {
    String msg = nodeId + " " ON_MESSAGE;
    msg += mesh.getNodeId();
    mesh.sendBroadcast(msg);
    // taskTurnOn.disable();
}

// void turnOn() {
//   turnOnNode("1");
// }

void turnOnNode1() {
    turnOnNode(NODE_ID_1);
}

void turnOnNode2() {
    turnOnNode(NODE_ID_2);
}

void turnOnNode3() {
    turnOnNode(NODE_ID_3);
}

void turnOffNode(String nodeId) {
    String msg = nodeId + " " OFF_MESSAGE;
    msg += mesh.getNodeId();
    mesh.sendBroadcast(msg);
    // taskTurnOff.disable();
}

// void turnOff() {
//   turnOffNode("1");
// }

void turnOffNode1() {
    turnOffNode(NODE_ID_1);
}

void turnOffNode2() {
    turnOffNode(NODE_ID_2);
}

void turnOffNode3() {
    turnOffNode(NODE_ID_3);
}

// Needed for painless library
void receivedCallback(uint32_t from, String &msg) {
    Serial.printf("pineapple: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> pineapple: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
#endif

#ifdef NODE_TYPE_LIGHT
void receivedCallbackEcho(uint32_t from, String &msg) {
    Serial.printf("echoNode: Received from %u msg=%s\n", from, msg.c_str());
    // String testMsg = "Switch is Now On";
    // mesh.sendSingle(from, testMsg);
    mesh.sendSingle(from, msg);
}

void receivedCallbackHandleLight(uint32_t from, String &msg) {
    Serial.printf("echoNode: Received from %u msg=%s\n", from, msg.c_str());
    String NodeIDStr = String(mesh.getNodeId());
    if (msg.indexOf(NodeIDStr) >= 0) {
      String LampID = msg.substring(NodeIDStr.length() + 1, NodeIDStr.length() + 2);
      Serial.printf("LampID; %s\n", LampID.c_str());
        if (msg.indexOf(ON_MESSAGE) >= 0) {
            if (LampID == "1") {
                digitalWrite(LIGHT_1_PIN, HIGH);
            } else if (LampID == "2") {
                digitalWrite(LIGHT_2_PIN, HIGH);
            } else if (LampID == "3") {
                digitalWrite(LIGHT_3_PIN, HIGH);
            }
        } else if (msg.indexOf(OFF_MESSAGE) >= 0) {
            if (LampID == "1") {
                digitalWrite(LIGHT_1_PIN, LOW);
            } else if (LampID == "2") {
                digitalWrite(LIGHT_2_PIN, LOW);
            } else if (LampID == "3") {
                digitalWrite(LIGHT_3_PIN, LOW);
            }
        } else {
            Serial.println("Unknown message");
        }
    } else {
      Serial.printf("Message for: %s. I am: %s", msg.substring(0, msg.indexOf("_")).c_str(), NodeIDStr.c_str());
        Serial.println("Message not for me");
    }
    mesh.sendSingle(from, msg);
}
#endif

void setup() {
    Serial.begin(115200);

#ifdef NODE_TYPE_SWITCH
    pinMode(SWITCH_1_PIN, INPUT_PULLUP);
    pinMode(SWITCH_2_PIN, INPUT_PULLUP);
    pinMode(SWITCH_3_PIN, INPUT_PULLUP);

    // mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes(ERROR | STARTUP);  // set before init() so that you can see startup messages

    mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    userScheduler.addTask(taskReadSwitches);
    // userScheduler.addTask( taskTurnOn );
    userScheduler.addTask(taskTurnOnNode1);
    userScheduler.addTask(taskTurnOnNode2);
    userScheduler.addTask(taskTurnOnNode3);
    // userScheduler.addTask( taskTurnOff );
    userScheduler.addTask(taskTurnOffNode1);
    userScheduler.addTask(taskTurnOffNode2);
    userScheduler.addTask(taskTurnOffNode3);
    taskReadSwitches.enable();
#endif

#ifdef NODE_TYPE_LIGHT
    pinMode(LIGHT_1_PIN, OUTPUT);
    digitalWrite(LIGHT_1_PIN, LOW);
    pinMode(LIGHT_2_PIN, OUTPUT);
    digitalWrite(LIGHT_2_PIN, LOW);
    pinMode(LIGHT_3_PIN, OUTPUT);
    digitalWrite(LIGHT_3_PIN, LOW);

    // delay(500);
    // digitalWrite(LIGHT_1_PIN, HIGH);
    // digitalWrite(LIGHT_2_PIN, HIGH);
    // digitalWrite(LIGHT_3_PIN, HIGH);
    // delay(500);
    // digitalWrite(LIGHT_1_PIN, LOW);
    // digitalWrite(LIGHT_2_PIN, LOW);
    // digitalWrite(LIGHT_3_PIN, LOW);
    // delay(500);
    // digitalWrite(LIGHT_1_PIN, HIGH);
    // digitalWrite(LIGHT_2_PIN, HIGH);
    // digitalWrite(LIGHT_3_PIN, HIGH);
    // delay(500);
    // digitalWrite(LIGHT_1_PIN, LOW);
    // digitalWrite(LIGHT_2_PIN, LOW);
    // digitalWrite(LIGHT_3_PIN, LOW);

    mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);  // set before init() so that you can see startup messages

    mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
    mesh.onReceive(&receivedCallbackHandleLight);
#endif
}

void loop() {
    // it will run the user scheduler as well
    mesh.update();
}