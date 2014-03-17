//
//  BitMessage.h
//  

#include <memory>
#include <string>
#include <ctime>
#include "Network.h"
#include "XmlRPC.h"
#include "base64.h"


typedef std::string BitMessageAddress;

class BitMessageIdentity {
    
public:
    
    BitMessageIdentity(base64 label, BitMessageAddress address, int stream=1, bool enabled=true, bool chan=false) : m_label(label), m_address(address), m_stream(stream), m_enabled(enabled), m_chan(chan) {};
    
    // Note "getLabel" returns a base64 formatted label, you will need to decode this object via base64::decoded
    base64 getLabel(){return m_label;};
    BitMessageAddress getAddress(){return m_address;};
    int getStream(){return m_stream;};
    bool getEnabled(){return m_enabled;};
    bool getChan(){return m_chan;};
    
private:
    
    base64 m_label;
    BitMessageAddress m_address;
    int m_stream;
    bool m_enabled;
    bool m_chan;
};

typedef std::vector<BitMessageIdentity> BitMessageIdentities;




class BitMessageAddressBookEntry {
    
public:
    
    BitMessageAddressBookEntry(BitMessageAddress address, base64 label) : m_address(address), m_label(label) {};

    BitMessageAddress getAddress(){return m_address;};
    base64 getLabel(){return m_label;};
    
private:
    
    BitMessageAddress m_address;
    base64 m_label;
    
};

typedef std::vector<BitMessageAddressBookEntry> BitMessageAddressBook;




class BitInboxMessage {

public:
    
    BitInboxMessage(std::string msgID, BitMessageAddress toAddress, BitMessageAddress fromAddress, base64 subject, base64 message, int encodingType, std::time_t m_receivedTime, bool m_read) : m_msgID(msgID), m_toAddress(toAddress), m_fromAddress(fromAddress), m_subject(subject), m_message(message), m_encodingType(encodingType), m_receivedTime(m_receivedTime), m_read(m_read) {};
    
    std::string getMsgID(){return m_msgID;};
    BitMessageAddress getToAddress(){return m_toAddress;};
    BitMessageAddress getFromAddress(){return m_fromAddress;};
    base64 getSubject(){return m_subject;};
    base64 getMessage(){return m_message;};
    int getEncodingType(){return m_encodingType;};
    std::time_t getReceivedTime(){return m_receivedTime;};
    bool getRead(){return m_read;};


private:
    
    std::string m_msgID;
    BitMessageAddress m_toAddress;
    BitMessageAddress m_fromAddress;
    base64 m_subject;
    base64 m_message;
    int m_encodingType;
    std::time_t m_receivedTime;
    bool m_read;
    
};

typedef std::vector<BitInboxMessage> BitMessageInbox;


class BitSentMessage {
    
public:
    
    BitSentMessage(std::string msgID, BitMessageAddress toAddress, BitMessageAddress fromAddress, base64 subject, base64 message, int encodingType, std::time_t lastActionTime, std::string status, std::string ackData) : m_msgID(msgID), m_toAddress(toAddress), m_fromAddress(fromAddress), m_subject(subject), m_message(message), m_encodingType(encodingType), m_lastActionTime(lastActionTime), m_status(status), m_ackData(ackData) {};
    
    std::string getMsgID(){return m_msgID;};
    BitMessageAddress getToAddress(){return m_toAddress;};
    BitMessageAddress getFromAddress(){return m_fromAddress;};
    base64 getSubject(){return m_subject;};
    base64 getMessage(){return m_message;};
    int getEncodingType(){return m_encodingType;};
    std::time_t getLastActionTime(){return m_lastActionTime;};
    std::string getStatus(){return m_status;};
    std::string getAckData(){return m_ackData;};

    
private:
    
    std::string m_msgID;
    BitMessageAddress m_toAddress;
    BitMessageAddress m_fromAddress;
    base64 m_subject;
    base64 m_message;
    int m_encodingType;
    std::time_t m_lastActionTime;
    std::string m_status;
    std::string m_ackData;
    
};

typedef std::vector<BitSentMessage> BitMessageOutbox;



class BitMessage : public NetworkModule {

public:
    
    BitMessage(std::string Host, int Port, std::string Username, std::string Pass);
    ~BitMessage();
    
    
    //
    // Core API Functions
    // https://bitmessage.org/wiki/API_Reference
    //

    // Inbox Management
    
    std::vector<BitInboxMessage> getAllInboxMessages();
    
    BitInboxMessage getInboxMessageByID(std::string msgID, bool setRead=true);
    
    void getAllSentMessages();
    
    void getSentMessageByID(std::string msgID);

    void getSentMessageByAckData(std::string ackData);

    void getSentMessagesBySender(std::string address);
    
    bool trashMessage(std::string msgID);
    
    void trashSentMessageByAckData(std::string ackData);

    
    // Message Management
    void sendMessage(std::string fromAddress, std::string toAddress, base64 subject, base64 message, int encodingType=2);
    void sendMessage(std::string fromAddress, std::string toAddress, std::string subject, std::string message, int encodingType=2){sendMessage(fromAddress, toAddress, base64(subject), base64(message), encodingType);};

    void sendBroadcast(std::string fromAddress, base64 subject, base64 message, int encodingType=2);
    void sendBroadcast(std::string fromAddress, std::string subject, std::string message, int encodingType=2){sendBroadcast(fromAddress, base64(subject), base64(message), encodingType);};

    
    // Subscription Management
    
    void listSubscriptions();
    
    void addSubscription(std::string address, base64 label);
    void addSubscription(std::string address, std::string label){addSubscription(address, base64(label));};

    void deleteSubscription(std::string address);
    
    
    // Channel Management
    
    BitMessageAddress createChan(base64 password);
    void createChan(std::string password){createChan(base64(password));};
    
    bool joinChan(base64 password, std::string address);
    bool joinChan(std::string password, std::string address){return joinChan(base64(password), address);};
    
    bool leaveChan(std::string address);
    
    
    // Address Management
    
    BitMessageIdentities listAddresses(); // This is technically "listAddresses2" in the API reference
    
    void createRandomAddress(base64 label, bool eighteenByteRipe=false, int totalDifficulty=1, int smallMessageDifficulty=1);
    void createRandomAddress(std::string label, bool eighteenByteRipe=false, int totalDifficulty=1, int smallMessageDifficulty=1){createRandomAddress(label, eighteenByteRipe, totalDifficulty, smallMessageDifficulty);};

    void createDeterministicAddresses(base64 password, int numberOfAddresses=1, int addressVersionNumber=0, int streamNumber=0, bool eighteenByteRipe=false, int totalDifficulty=1, int smallMessageDifficulty=1);
    void createDeterministicAddresses(std::string password, int numberOfAddresses=1, int addressVersionNumber=0, int streamNumber=0, bool eighteenByteRipe=false, int totalDifficulty=1, int smallMessageDifficulty=1){createDeterministicAddresses(base64(password), numberOfAddresses, addressVersionNumber, streamNumber, eighteenByteRipe, totalDifficulty, smallMessageDifficulty);};
    
    void getDeterministicAddress(base64 password, int addressVersionNumber=4, int streamNumber=1);
    void getDeterministicAddress(std::string password, int addressVersionNumber=4, int streamNumber=1){getDeterministicAddress(base64(password), addressVersionNumber, streamNumber);};
    
    void listAddressBookEntries();
    
    bool addAddressBookEntry(std::string address, base64 label);
    bool addAddressBookEntry(std::string address, std::string label){return addAddressBookEntry(address, base64(label));};

    bool deleteAddressBookEntry(std::string address);
    
    bool deleteAddress(BitMessageAddress address);
    
    void decodeAddress(BitMessageAddress address);
    
    
    // Other API Commands
    
    bool testApi();
    
    int add(int x, int y);
    
    void getStatus();
    
    
    // Extra BitMessage Options (some of these are pass-through functions not related to the API)

    void setTimeout(int timeout);
    
    
private:
    
    typedef std::vector<xmlrpc_c::value> Parameters;
    typedef xmlrpc_c::value_int ValueInt;
    typedef xmlrpc_c::value_i8 ValueI8;
    typedef xmlrpc_c::value_boolean ValueBool;
    typedef xmlrpc_c::value_double ValueDouble;
    typedef xmlrpc_c::value_string ValueString;
    typedef xmlrpc_c::value_datetime ValueDateTime;
    typedef xmlrpc_c::value_bytestring ValueByteString;
    typedef xmlrpc_c::value_nil ValueNil;
    typedef xmlrpc_c::value_array ValueArray;
    typedef xmlrpc_c::value_struct ValueStruct;
    typedef std::time_t ValueTime;
    
    // Private member variables
    std::string m_host;
    int m_port;
    std::string m_pass;
    std::string m_username;
    
    
    // Communication Library, XmlRPC in this case
    XmlRPC *m_xmllib;
    
    
    
};