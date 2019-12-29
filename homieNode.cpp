#include "homie.hpp"


//-------------------------------HOMIE NODE------------------------------------
HomieNode::HomieNode(string nodeName, string fName, string type){
        if(homie::homieId(nodeName)) this->nodeName = nodeName;
        this->fName = fName;
        this->type = type;
        if(HOMIE_SERIAL) Serial.println("HomieNode Const");
}

void HomieNode::init(PubSubClient *client, string prefix){
        string topicPrefix = prefix + "/" + this->nodeName;
        string topic = topicPrefix + "/$name";
        client->publish(topic.c_str(),this->fName.c_str(), true);
        topic = topicPrefix + "/$type";
        client->publish(topic.c_str(),this->type.c_str(), true);

        std::list<HomieProperties>::iterator it;
        string properties = "";
        for(it = this->props.begin(); it != this->props.end(); ++it) {
                properties += it->getPropName() + ",";
        }
        properties.pop_back();
        topic = topicPrefix + "/$properties";
        client->publish(topic.c_str(),properties.c_str(), true);

        for(it = this->props.begin(); it != this->props.end(); ++it) {
                it->init(client, topicPrefix);
        }
}

void HomieNode::addProp(HomieProperties prop){
        this->props.push_back(prop);
}

string HomieNode::getNodeName(){
        return this->nodeName;
}

string HomieNode::toString(){
        return "Node Name: " + nodeName + " fName: " + fName + " type: " + type;
}

string HomieNode::getPubString(string propName){
        string nodeString = this->nodeName + "/";
        string propString = "";
        std::list<HomieProperties>::iterator it;
        for(it = this->props.begin(); it != this->props.end(); ++it) {
                if(it->getPropName() == propName) {
                        propString = it->getPubString();
                }
        }
        if(propString != "") {
                return nodeString + propString;
        }else{
                return "";
        }
}
