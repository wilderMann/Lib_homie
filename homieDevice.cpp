#include "homie.hpp"

HomieDevice::HomieDevice(string deviceName, string name, string extensions, string implementation){
        if(homie::homieId(deviceName)) this->deviceName = deviceName;
        this->name = name;
        this->extensions = extensions;
        this->implementation = implementation;
}
HomieDevice::HomieDevice(){
        if(homie::homieId(deviceName)) this->deviceName = "";
        this->name = "";
        this->extensions = "";
        this->implementation = "";
}

void HomieDevice::init(PubSubClient *client){
        if(HOMIE_SERIAL) Serial.println(this->toString().c_str());
        this->state = homie::init;
        string topicPrefix = "homie/" + this->deviceName;
        string topic = topicPrefix + "/$state";
        client->publish(topic.c_str(),this->getStateString().c_str(), true);
        topic = topicPrefix + "/$homie";
        client->publish(topic.c_str(),HOMIE_VERSION, true);
        topic = topicPrefix + "/$name";
        client->publish(topic.c_str(),this->name.c_str(), true);
        topic = topicPrefix + "/$extensions";
        client->publish(topic.c_str(),this->extensions.c_str(), true);
        topic = topicPrefix + "/$implementation";
        client->publish(topic.c_str(),this->implementation.c_str(), true);
        std::list<HomieNode>::iterator it;
        string nodes = "";
        for(it = this->nodes.begin(); it != this->nodes.end(); ++it) {
                nodes += it->getNodeName() + ",";
        }
        nodes.pop_back();
        topic = topicPrefix + "/$nodes";
        client->publish(topic.c_str(),nodes.c_str(), true);

        for(it = this->nodes.begin(); it != this->nodes.end(); ++it) {
                it->init(client, topicPrefix);
        }
        this->state = homie::ready;
        topic = topicPrefix + "/$state";
        client->publish(topic.c_str(),this->getStateString().c_str(), true);
}

void HomieDevice::addNode(HomieNode node){
        this->nodes.push_back(node);
}

string HomieDevice::getStateString(){
        switch (this->state) {
        case homie::init:
                return "init";
                break;
        case homie::ready:
                return "ready";
                break;
        case homie::disconnected:
                return "disconnected";
                break;
        case homie::sleeping:
                return "sleeping";
                break;
        case homie::lost:
                return "lost";
                break;
        case homie::alert:
                return "alert";
                break;
        default:
                return "";
        }
}


string HomieDevice::getDeviceId(){
        return this->deviceName;
}

string HomieDevice::toString(){
        string output = "Device Name: " + deviceName + " Name: " + name +
                        " extensions: " + extensions + " implement: " +
                        implementation;
        return output;
}

string HomieDevice::getPubString(string nodeName, string propName){
        string deviceString = "homie/" + this->getDeviceId() + "/";
        string nodeString = "";
        std::list<HomieNode>::iterator it;
        for(it = this->nodes.begin(); it != this->nodes.end(); ++it) {
                if(it->getNodeName() == nodeName) {
                        nodeString = it->getPubString(propName);
                }
        }
        if(nodeString != "") {
                return deviceString + nodeString;
        }else{
                return "";
        }
}

void HomieDevice::setState(homie::state state){
        this->state = state;
}
