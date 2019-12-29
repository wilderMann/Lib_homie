#ifndef HOMIE_HPP_
#define HOMIE_HPP_
#include <PubSubClient.h>
#include <list>
#include <string>
#include <cctype>
#include <ctype.h>

#define HOMIE_SERIAL false

#define HOMIE_VERSION "4.0.0"

using namespace std;
namespace homie {
enum state {init, ready, disconnected, sleeping, lost, alert};
enum datatype {integer_t, float_t, boolean_t, string_t, enum_t, color_t};
bool homieId(string id);
}

class HomieProperties {
private:
string propName, fName, unit, format;
bool settable;
bool retained;
homie::datatype datatype;

public:
HomieProperties(string propName, string fName = "", bool settable = false,
                bool retained = true, string unit = "",
                homie::datatype datatype = homie::string_t, string format = "");
void init(PubSubClient *client, string prefix);
string getPropName();
string getDTString();
string toString();
string getPubString();
};

class HomieNode {
protected:
std::list<HomieProperties> props;
string nodeName, fName, type;

public:
HomieNode(string nodeName, string fName, string type);
void init(PubSubClient *client, string prefix);
void addProp(HomieProperties prop);
string getNodeName();
string toString();
string getPubString(string propName);
};

class HomieDevice {
private:
std::list<HomieNode> nodes;
string deviceName, name, extensions, implementation;
homie::state state;

public:
HomieDevice(string deviceName, string name, string extensions, string implementation);
HomieDevice();
void init(PubSubClient *client);
void setState(homie::state state);
void addNode(HomieNode node);
string getStateString();
string getDeviceId();
string toString();
string getPubString(string nodeName, string propName);
};

class Homie {
private:
PubSubClient *client;
HomieDevice device;

public:
Homie(PubSubClient *client);
boolean connect(const char *id, const char *user, const char *pass);
void disconnect();
void sleep();
boolean connected();
boolean loop();
void setDevice(HomieDevice device);
HomieDevice getDevice();
string getPubString(string nodeName, string propName);
};

#endif
