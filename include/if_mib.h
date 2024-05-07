#ifndef _if_mib_h_
#define _if_mib_h_

#include <agent_pp/mib.h>
#include <agent_pp/snmp_textual_conventions.h>

#define oidIf           "1.3.6.1.2.1.31"
// Scalars

// Columns
// ifXEntry
#define oidIfName           "1.3.6.1.2.1.31.1.1.1.1"
#define colIfName           "1"
#define oidIfHCInOctets     "1.3.6.1.2.1.31.1.1.1.6"
#define colIfHCInOctets     "6"
#define oidIfHCOutOctets    "1.3.6.1.2.1.31.1.1.1.10"
#define colIfHCOutOctets    "10"
#define oidIfHighSpeed      "1.3.6.1.2.1.31.1.1.1.15"
#define colIfHighSpeed      "15"

// Tables
#define oidIfXTable     "1.3.6.1.2.1.31.1.1"
#define oidIfXEntry     "1.3.6.1.2.1.31.1.1.1"
#define cIfName         1
#define nIfName         0
#define cIfHCInOctets   1 
#define nIfHCInOctets   0 

#ifdef AGENTPP_NAMESPACE
using namespace Agentpp;
#endif

// ifName
class AGENTPP_DECL ifName: public SnmpDisplayString{
public:
    ifName(const Oidx &);
    virtual ~ifName() { }
    virtual MibEntry* clone(){
        MibEntry * other = new ifName(oid);
        ((ifName*)other)->replace_value(value->clone());
        ((ifName*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
};

// ifHCInOctets
class AGENTPP_DECL ifHCInOctets: public MibLeaf{
public:
    ifHCInOctets(const Oidx &);
    virtual ~ifHCInOctets() { }
    virtual MibEntry* clone(){
        MibEntry * other = new ifHCInOctets(oid);
        ((ifHCInOctets*)other)->replace_value(value->clone());
        ((ifHCInOctets*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
    OctetStr itf;
private:
    unsigned long long get_if_hc_in_octets();
};

// ifHCOutOctets
class AGENTPP_DECL ifHCOutOctets: public MibLeaf{
public:
    ifHCOutOctets(const Oidx &);
    virtual ~ifHCOutOctets() { }
    virtual MibEntry* clone(){
        MibEntry * other = new ifHCOutOctets(oid);
        ((ifHCOutOctets*)other)->replace_value(value->clone());
        ((ifHCOutOctets*)other)->set_reference_to_table(my_table);
        return other;
    }
    void get_request(Request *, int);
    OctetStr itf;
private:
    unsigned long long get_if_hc_out_octets();
};

class AGENTPP_DECL ifXEntry: public MibTable{
public:
    ifXEntry();
    virtual ~ifXEntry();
    static ifXEntry* instance; 
    static ifXEntry* get_instance(Mib* mib){
        Oidx oid(oidIfXEntry);
        ifXEntry * entry = (ifXEntry*)mib->get(oid);
        return (entry) ? entry : instance;
    }
};

class AGENTPP_DECL if_mib: public MibGroup{
public:
    if_mib();
    virtual ~if_mib() { };
};

#endif
