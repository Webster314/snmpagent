#ifndef _host_resources_mib_h_
#define _host_resources_mib_h_

#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include <agent_pp/mib.h>
#include <agent_pp/snmp_textual_conventions.h>

#define oidHostResources    "1.3.6.1.2.1.25"
// Scalars
#define oidHrSystemUptime   "1.3.6.1.2.1.25.1.1.0"

// Columns
// hrProcessorEntry
#define oidHrProcessorFrwID	"1.3.6.1.2.1.25.3.3.1.1"
#define colHrProcessorFrwID "1"
#define oidHrProcessorLoad  "1.3.6.1.2.1.25.3.3.1.2"
#define colHrProcessorLoad  "2"
// hrStorageEntry
#define oidHrStorageDescr   "1.3.6.1.2.1.25.2.3.1.3"
#define colHrStorageDescr   "3"
#define oidHrStorageSize    "1.3.6.1.2.1.25.2.3.1.5"
#define colHrStorageSize    "5"
#define oidHrStorageUsed    "1.3.6.1.2.1.25.2.3.1.6"
#define colHrStorageUsed    "6"

// Tables
// hrProcessorTable
#define oidHrProcessorTable "1.3.6.1.2.1.25.3.3"
#define oidHrProcessorEntry "1.3.6.1.2.1.25.3.3.1"
#define nHrProcessorFrwID   0
#define cHrProcessorFrwID   2
#define nHrProcessorLoad    1
#define cHrProcessorLoad    3
// hrStorageTable
#define oidHrStorageTable   "1.3.6.1.2.1.25.2.3"
#define oidHrStorageEntry   "1.3.6.1.2.1.25.2.3.1"
#define nHrStorageDescr     0
#define cHrStorageDescr     3
#define nHrStorageSize      1
#define cHrStorageSize      5
#define nHrStorageUsed      2
#define cHrStorageUsed      6


#ifdef AGENTPP_NAMESPACE    
namespace Agentpp {
    using namespace Snmp_pp;
#endif

// Scalars
// hrSystemUptime
class AGENTPP_DECL hrSystemUptime: public MibLeaf{
public:    
    hrSystemUptime();
    virtual ~hrSystemUptime() { }
    void get_request(Request *, int);
private:
    long get_hr_system_uptime();
};

// hrProcessorTable
// hrProcessrorFrwID
class AGENTPP_DECL hrProcessorFrwID: public MibLeaf{
public:
    hrProcessorFrwID(const Oidx &);
    virtual ~hrProcessorFrwID() { };
    virtual MibEntry * clone(){
        MibEntry * other = new hrProcessorFrwID(oid);
        ((hrProcessorFrwID*)other)->replace_value(value->clone());
        ((hrProcessorFrwID*)other)->set_reference_to_table(my_table);
        return other;
    };
};

// hrProcessorLoad
class AGENTPP_DECL hrProcessorLoad: public MibLeaf {
public:
    hrProcessorLoad(const Oidx &);
    virtual ~hrProcessorLoad() { };
    virtual MibEntry * clone(){
        MibEntry * other = new hrProcessorLoad(oid);
        ((hrProcessorLoad*)other)->replace_value(value->clone());
        ((hrProcessorLoad*)other)->set_reference_to_table(my_table);
        return other;
    }    
};

// hrProcessorEntry
class AGENTPP_DECL hrProcessorEntry: public MibTable {
public: 
    hrProcessorEntry();
    virtual ~hrProcessorEntry();
    static hrProcessorEntry * instance;
    static hrProcessorEntry * get_instance(Mib * mib){
        Oidx oid(oidHrProcessorEntry);
        hrProcessorEntry * entry = (hrProcessorEntry*)mib->get(oid);
        return (entry) ? entry : instance;
    }
    void set_row(MibTableRow * r, const Oidx & frwid, const SnmpInt32 & load);
    MibTableRow * add_entry(const OctetStr & name, const Oidx & frwid, const SnmpInt32 & load);
};

// hrStorageTable
class AGENTPP_DECL hrStorageDescr: public SnmpDisplayString {
public:
    hrStorageDescr(const Oidx & o);
    virtual ~hrStorageDescr() { }
    virtual MibEntry * clone(){
        MibEntry * other = new hrStorageDescr(oid);
        ((hrStorageDescr*)other)->replace_value(value->clone());
        ((hrStorageDescr*)other)->set_reference_to_table(my_table);
        return other;
    };
    void get_request(Request * req, int ind);
};

// hrStorageSize
class AGENTPP_DECL hrStorageSize: public MibLeaf {
public:
    hrStorageSize(const Oidx & o);
    hrStorageSize(const Oidx & o, const OctetStr & m);
    virtual ~hrStorageSize() { }
    virtual MibEntry * clone(){
        MibEntry * other = new hrStorageSize(oid);
        ((hrStorageSize*)other)->replace_value(value->clone());
        ((hrStorageSize*)other)->set_reference_to_table(my_table);
        return other;
    };
    void get_request(Request * req, int ind);
    OctetStr mnt;
private:
    long get_hr_storage_size();
};

// hrStorageUsed
class AGENTPP_DECL hrStorageUsed : public MibLeaf{
public:
    hrStorageUsed(const Oidx & o);
    hrStorageUsed(const Oidx & o, const OctetStr & m);
    virtual ~hrStorageUsed() { };
    virtual MibEntry * clone(){
        MibEntry * other = new hrStorageUsed(oid);
        ((hrStorageUsed*)other)->replace_value(value->clone());
        ((hrStorageUsed*)other)->set_reference_to_table(my_table);
        return other;
    };
    void get_request(Request * req, int ind);
    OctetStr mnt;
private:
    long get_hr_storage_used();
};

// hrStorageEntry
class AGENTPP_DECL hrStorageEntry : public MibTable{
public:
    static hrStorageEntry * instance;
    hrStorageEntry();
    virtual ~hrStorageEntry();
    static hrStorageEntry* get_instance(Mib* mib){
        Oidx oid(oidHrStorageEntry);
        hrStorageEntry * entry = (hrStorageEntry*)mib->get(oid);
        return (entry) ? entry : instance;
    }
    void set_row(MibTableRow * r, const OctetStr &, const SnmpInt32 &, const SnmpInt32 &);
    MibTableRow * add_entry(const OctetStr &, const SnmpInt32 &, const SnmpInt32 &);
};

/**********************************************************************
 *
 *  class host_resources_mib
 *
 **********************************************************************/

class AGENTPP_DECL host_resources_mib: public MibGroup {
public:
    host_resources_mib();
    virtual ~host_resources_mib() { }
    virtual hrProcessorEntry* get_hr_processor_table(){
        ListCursor<MibEntry> content = get_content();
        if (content.next() && content.next()) {
            return (hrProcessorEntry*)content.get();
        }
        return 0;
    }
};

#ifdef AGENTPP_NAMESPACE
};
#endif

#endif